
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2014 Igor Mironchik

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

#ifdef COMO_BOOST_PROTOBUF

// Como include.
#include <Como/boost_protobuf/bp_client_socket.hpp>
#include <Como/boost_protobuf/bp_source.hpp>
#include <Como/boost_protobuf/bp_server_socket.hpp>
#include <Como/boost_protobuf/messages.pb.h>

// C++ include.
#include <cstdint>
#include <cstring>
#include <vector>
#include <sstream>
#include <thread>
#include <atomic>
#include <iostream>

// boost include.
#include <boost/asio.hpp>
#include <boost/date_time.hpp>


namespace Como {

namespace BoostProtobuf {

//! Magic number for the Como protocol #1.
static const char c_magicNumber1[] =
	{ 0x43u, 0x4Fu, 0x4Du, 0x4Fu, 0x50u, 0x52u, 0x4Fu, 0x31u };

//! Magic number for the Como protocol #2.
static const char c_magicNumber2[] =
	{ 0x43u, 0x4Fu, 0x4Du, 0x4Fu, 0x50u, 0x52u, 0x4Fu, 0x32u };

/*!
	Message's header size. 12 bytes.

	8 bytes of magic number +
	2 bytes of message type +
	2 bytes of message length.
*/
static const std::uint8_t c_headerSize = 12;

//! Type of the GetListOfSourcesMessage message.
static const std::uint16_t c_getListOfSourcesMessageType = 0x0001;

//! Type of the SourceMessage message.
static const std::uint16_t c_sourceMessageType = 0x0002;

//! Type of the DeinitSourceMessage message.
static const std::uint16_t c_deinitSourceMessageType = 0x0003;


//
// ClientSocketPrivate
//

class ClientSocketPrivate {
public:
	ClientSocketPrivate( boost::asio::io_service & io,
		ServerSocket * server )
		:	m_socket( io )
		,	m_server( server )
	{
	}

	//! \return Parsed digit.
	inline std::uint16_t parseDigit( const char * str )
	{
		return ( std::uint16_t ) str[ 0 ] * 256 + ( std::uint16_t ) str[ 1 ];
	}

	//! Parse message's header and notify server.
	bool parseHeaderAndNotifyServer( const char * header )
	{
		std::cout << "parseHeaderAndNotifyServer" << std::endl;
		std::cout.write( header, 8 );
		std::cout << std::endl;

		if( std::strncmp( c_magicNumber1, header, 8 ) == 0 )
		{
			std::cout << "send list of sources" << std::endl;

			const std::uint16_t type = parseDigit( header + 8 );
			const std::uint16_t length = parseDigit( header + 10 );

			if( c_getListOfSourcesMessageType == type )
				m_server->sendListOfSources( m_pointer );

			if( length > 0 )
				readMessage( length );

			return true;
		}
		else
		{
			m_server->disconnection( m_pointer );

			return false;
		}
	}

	//! Read message.
	void readMessage( std::size_t length )
	{
		std::vector< char > message( length );

		std::weak_ptr< ClientSocket > sp = m_pointer;

		boost::asio::async_read( m_socket,
			boost::asio::buffer( message.data(), length ),
			[ this, &sp ] ( boost::system::error_code error, std::size_t /*length*/ )
				{
					if( sp.lock() )
					{
						if( error )
							m_server->disconnection( m_pointer );
					}
				}
		);
	}

	//! Parent.
	std::weak_ptr< ClientSocket > m_pointer;
	//! Socket.
	tcp::socket m_socket;
	//! Server socket.
	ServerSocket * m_server;
	//! Stopped flag
	std::atomic_flag m_isStopped;
}; // class ClientSocketPrivate;


//
// ClientSocket
//

ClientSocket::ClientSocket( boost::asio::io_service & io,
	ServerSocket * server )
	:	d( new ClientSocketPrivate( io, server ) )
{
}

ClientSocket::~ClientSocket()
{
	std::cout << "!!! ClientSocket destructured..." << std::endl;
}

tcp::socket &
ClientSocket::socket() const
{
	return d->m_socket;
}

void
ClientSocket::start( std::shared_ptr< ClientSocket > socket )
{
	std::cout << "start reading from socket" << std::endl;

	if( d->m_socket.is_open() )
	{
		char header[ c_headerSize ];

		boost::asio::async_read( d->m_socket,
			boost::asio::buffer( header, c_headerSize ),
			[ this, &header, socket ] ( boost::system::error_code error, std::size_t /*length*/ )
				{
					if( error )
						d->m_server->disconnection( d->m_pointer );
					else
					{
						if( d->parseHeaderAndNotifyServer( header ) &&
							d->m_socket.is_open() )
								start( socket );
					}
				}
		);
	}
}

void
ClientSocket::stop( std::shared_ptr< ClientSocket > socket )
{
	std::cout << "stop" << std::endl;

	if( !d->m_isStopped.test_and_set( std::memory_order_acquire ) )
	{
		socket->socket().shutdown( tcp::socket::shutdown_both );
		socket->socket().close();

		while( socket->socket().is_open() )
			std::this_thread::yield();
	}
}

static inline std::string anyToString( const boost::any & value,
	Source::Type type )
{
	try {
		switch( type )
		{
			case Source::String :
				return boost::any_cast< std::string > ( value );

			case Source::Int :
			{
				std::ostringstream stream;
				stream << boost::any_cast< int > ( value );

				return stream.str();
			}

			case Source::UInt :
			{
				std::ostringstream stream;
				stream << boost::any_cast< unsigned int > ( value );

				return stream.str();
			}

			case Source::LongLong :
			{
				std::ostringstream stream;
				stream << boost::any_cast< long long > ( value );

				return stream.str();
			}

			case Source::ULongLong :
			{
				std::ostringstream stream;
				stream << boost::any_cast< unsigned long long > ( value );

				return stream.str();
			}

			case Source::Double :
			{
				std::ostringstream stream;
				stream << boost::any_cast< double > ( value );

				return stream.str();
			}

			case Source::DateTime :
			case Source::Time :
				return boost::posix_time::to_iso_extended_string(
					boost::any_cast< boost::posix_time::ptime > ( value ) );

			default :
				return std::string();
		}
	}
	catch( const boost::bad_any_cast & )
	{
		return std::string();
	}
}

static inline void write16BitNumber( std::ostringstream & stream,
	std::uint16_t number )
{	
	stream << std::right << std::setfill( '0' ) << std::setw( 4 ) << std::hex
		<< number;

	std::cout << "write16BitNumber " << number << " " << stream.str() << std::endl;
}

static inline void writeMessage( const Source & source,
	std::uint16_t type,
	tcp::socket & socket,
	ServerSocket * server,
	const std::weak_ptr< ClientSocket > & pointer )
{
	ComoMessage msg;
	msg.set_type( source.type() );
	msg.set_name( source.name() );
	msg.set_typename_( source.typeName() );
	msg.set_datetime( boost::posix_time::to_iso_extended_string(
		source.dateTime() ) );
	msg.set_description( source.description() );
	msg.set_value( anyToString( source.value(), source.type() ) );

	const std::string strMessage = msg.SerializeAsString();

	std::ostringstream stream;
	stream.write( c_magicNumber2, 8 );
	write16BitNumber( stream, type );
	write16BitNumber( stream, ( std::uint16_t ) strMessage.length() );

	const std::string data = stream.str() + strMessage;

	boost::asio::async_write( socket,
		boost::asio::buffer( data.data(), data.length() ),
		[ server, &pointer ] ( boost::system::error_code error, std::size_t /*length*/ )
			{
				if( error && !pointer.expired() )
					server->disconnection( pointer );
			}
	);
}

void
ClientSocket::sendSourceMessage( const Source & source )
{
	std::cout << "sendSourceMessage " << source.name() << std::endl;

	if( d->m_socket.is_open() )
		writeMessage( source, c_sourceMessageType,
			d->m_socket, d->m_server, d->m_pointer );
}

void
ClientSocket::sendDeinitSourceMessage( const Source & source )
{
	if( d->m_socket.is_open() )
		writeMessage( source, c_deinitSourceMessageType,
			d->m_socket, d->m_server, d->m_pointer );
}

void
ClientSocket::setPtr( std::shared_ptr< ClientSocket > p )
{
	d->m_pointer = p;
}

} /* namespace BoostProtobuf */

} /* namespace Como */

#endif // COMO_BOOST_PROTOBUF
