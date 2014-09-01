
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
#include <atomic>

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
	bool parseHeaderAndNotifyServer( const char * header,
		std::shared_ptr< ClientSocket > socket )
	{
		if( std::strncmp( c_magicNumber1, header, 8 ) == 0 )
		{
			const std::uint16_t type = parseDigit( header + 8 );
			const std::uint16_t length = parseDigit( header + 10 );

			if( c_getListOfSourcesMessageType == type )
				m_server->sendListOfSources( socket );

			if( length > 0 )
				readMessage( length, socket );

			return true;
		}
		else
		{
			m_server->disconnection( socket );

			return false;
		}
	}

	//! Read message.
	void readMessage( std::size_t length,
		std::shared_ptr< ClientSocket > socket )
	{
		std::vector< char > message( length );

		boost::asio::async_read( m_socket,
			boost::asio::buffer( message.data(), length ),
			[ this, socket ] ( boost::system::error_code error, std::size_t )
				{
					if( error )
						m_server->disconnection( socket );
				}
		);
	}

	//! Socket.
	tcp::socket m_socket;
	//! Server socket.
	ServerSocket * m_server;
	//! Stopped flag
	std::atomic_flag m_isStopped;
	//! Bufer.
	char m_header[ c_headerSize ];
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
}

tcp::socket &
ClientSocket::socket()
{
	return d->m_socket;
}

void
ClientSocket::start( std::shared_ptr< ClientSocket > socket )
{
	if( d->m_socket.is_open() )
	{
		boost::asio::async_read( d->m_socket,
			boost::asio::buffer( d->m_header, c_headerSize ),
			[ this, socket ] ( boost::system::error_code error, std::size_t )
				{
					if( error )
						d->m_server->disconnection( socket );
					else
					{
						if( d->parseHeaderAndNotifyServer( d->m_header, socket ) &&
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
	if( !d->m_isStopped.test_and_set( std::memory_order_acquire ) )
		socket->socket().close();
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
	char low = number & 0xFF;
	char hight = ( number >> 8 ) & 0xFF;

	stream << hight << low;
}

static inline void writeMessage( const Source & source,
	std::uint16_t type,
	ServerSocket * server,
	std::shared_ptr< ClientSocket > socket )
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

	boost::asio::async_write( socket->socket(),
		boost::asio::buffer( data.data(), data.length() ),
		[ server, socket ] ( boost::system::error_code error, std::size_t )
			{
				if( error )
					server->disconnection( socket );
			}
	);
}

void
ClientSocket::sendSourceMessage( const Source & source,
	std::shared_ptr< ClientSocket > socket )
{
	if( d->m_socket.is_open() )
		writeMessage( source, c_sourceMessageType,
			d->m_server, socket );
}

void
ClientSocket::sendDeinitSourceMessage( const Source & source,
	std::shared_ptr< ClientSocket > socket )
{
	if( d->m_socket.is_open() )
		writeMessage( source, c_deinitSourceMessageType,
			d->m_server, socket );
}

} /* namespace BoostProtobuf */

} /* namespace Como */

#endif // COMO_BOOST_PROTOBUF
