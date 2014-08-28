
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

// C++ include.
#include <cstdint>
#include <cstring>
#include <vector>

// boost include.
#include <boost/asio.hpp>


namespace Como {

namespace BoostProtobuf {

//! Magic number for the Como protocol #1.
static const char c_magicNumber1[] =
	{ 0x43u, 0x4Fu, 0x4Du, 0x4Fu, 0x50u, 0x52u, 0x4Fu, 0x31u };

/*!
	Message's header size. 12 bytes.

	8 bytes of magic number +
	2 bytes of message type +
	2 bytes of message length.
*/
static const std::uint8_t c_headerSize = 12;

//! Type of the GetListOfSourcesMessage message.
static const std::uint16_t c_getListOfSourcesMessageType = 0x0001;


//
// ClientSocketPrivate
//

class ClientSocketPrivate {
public:
	ClientSocketPrivate( ClientSocket * parent,
		boost::asio::io_service & io,
		ServerSocket * server )
		:	m_parent( parent )
		,	m_socket( io )
		,	m_server( server )
	{
	}

	//! \return Parsed digit.
	inline std::uint16_t parseDigit( const char * str )
	{
		return ( std::uint16_t ) str[ 0 ] * 256 + ( std::uint16_t ) str[ 1 ];
	}

	//! Parse message's header and notify server.
	void parseHeaderAndNotifyServer( const char * header )
	{
		if( std::strncmp( c_magicNumber1, header, 8 ) == 0 )
		{
			const std::uint16_t type = parseDigit( header + 8 );
			const std::uint16_t length = parseDigit( header + 10 );

			if( c_getListOfSourcesMessageType == type )
				m_server->sendListOfSources( m_parent );

			if( length > 0 )
				readMessage( length );
		}
		else
			m_server->disconnection( m_parent );
	}

	//! Read message.
	void readMessage( std::size_t length )
	{
		std::vector< char > message( length );

		boost::asio::async_read( m_socket,
			boost::asio::buffer( message.data(), length ),
			[ this ] ( boost::system::error_code error, std::size_t /*length*/ )
				{
					if( error )
						m_server->disconnection( m_parent );
				} );
	}

	//! Parent.
	ClientSocket * m_parent;
	//! Socket.
	tcp::socket m_socket;
	//! Server socket.
	ServerSocket * m_server;
}; // class ClientSocketPrivate;


//
// ClientSocket
//

ClientSocket::ClientSocket( boost::asio::io_service & io,
	ServerSocket * server )
	:	d( new ClientSocketPrivate( this, io, server ) )
{
}

ClientSocket::~ClientSocket()
{
}

tcp::socket &
ClientSocket::socket() const
{
	return d->m_socket;
}

void
ClientSocket::start()
{
	char header[ c_headerSize ];

	boost::asio::async_read( d->m_socket,
		boost::asio::buffer( header, c_headerSize ),
		[ this, &header ] ( boost::system::error_code error, std::size_t /*length*/ )
			{
				if( error )
					d->m_server->disconnection( this );
				else
				{
					d->parseHeaderAndNotifyServer( header );

					start();
				}
			} );
}

void
ClientSocket::sendSourceMessage( const Source & source )
{

}

void
ClientSocket::sendDeinitSourceMessage( const Source & source )
{

}

} /* namespace BoostProtobuf */

} /* namespace Como */

#endif // COMO_BOOST_PROTOBUF
