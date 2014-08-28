
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
#include <Como/boost_protobuf/bp_server_socket.hpp>
#include <Como/boost_protobuf/bp_source.hpp>
#include <Como/boost_protobuf/bp_client_socket.hpp>

// boost include.
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>

// C++ include.
#include <list>
#include <mutex>
#include <algorithm>


namespace Como {

namespace BoostProtobuf {

using namespace boost::asio::ip;


//
// ServerSocketPrivate
//

class ServerSocketPrivate {
public:
	ServerSocketPrivate( ServerSocket * parent,
		boost::asio::io_service & io,
		unsigned short portNum )
		:	m_parent( parent )
		,	m_acceptor( io, tcp::endpoint( tcp::v4(), portNum ) )
	{
	}

	//! Notify all clients about changes in value of the source.
	void notifyAllClientsAboutValueChange( const Source & source )
	{
		std::lock_guard< std::mutex > lock( m_mutex );

		std::for_each( m_clients.begin(), m_clients.end(),
			[ &source ] ( const std::shared_ptr< ClientSocket > & socket )
				{
					socket->sendSourceMessage( source );
				}
		);
	}

	//! Notify all clients about deinitialization of the source.
	void notifyAllClientsAboutDeinitSource( const Source & source )
	{
		std::lock_guard< std::mutex > lock( m_mutex );

		std::for_each( m_clients.begin(), m_clients.end(),
			[ &source ] ( const std::shared_ptr< ClientSocket > & socket )
				{
					socket->sendDeinitSourceMessage( source );
				}
		);
	}

	//! Send list of sources to the client.
	void sendListOfSources( ClientSocket * socket )
	{
		std::lock_guard< std::mutex > lock( m_mutex );

		std::for_each( m_sources.begin(), m_sources.end(),
			[ &socket ] ( const Source & source )
				{
					socket->sendSourceMessage( source );
				}
		);
	}

	//! Start accepting new conection.
	void startAcceptNewConnection()
	{
		auto socket = std::shared_ptr< ClientSocket >
			( new ClientSocket( m_acceptor.get_io_service(), m_parent ) );

		m_acceptor.async_accept( socket->socket(),
			boost::bind( &ServerSocketPrivate::handleAccept, this, socket,
				boost::asio::placeholders::error ) );
	}

	//! Handle accept.
	void handleAccept( const std::shared_ptr< ClientSocket > & socket,
		const boost::system::error_code & error )
	{
		if( !error )
		{
			socket->start();

			std::lock_guard< std::mutex > lock( m_mutex );

			m_clients.emplace_back( socket );
		}

		startAcceptNewConnection();
	}

	typedef std::list< std::shared_ptr< ClientSocket > > ListOfSockets;

	//! \return Iterator to the client socket.
	ListOfSockets::const_iterator findSocket( ClientSocket * socket )
	{
		for( auto it = m_clients.begin(), last = m_clients.end();
			 it != last; ++it )
		{
			if( it->get() == socket )
				return it;
		}

		return m_clients.end();
	}

	//! Parent.
	ServerSocket * m_parent;
	//! Acceptor.
	tcp::acceptor m_acceptor;
	//! Clients.
	ListOfSockets m_clients;
	//! Mutex.
	std::mutex m_mutex;
	//! List of sources.
	std::list< Source > m_sources;
}; // class ServerSocketPrivate


//
// ServerSocket
//

ServerSocket::ServerSocket( boost::asio::io_service & io,
	unsigned short portNum )
	:	d( new ServerSocketPrivate( this, io, portNum ) )
{
	d->startAcceptNewConnection();
}

ServerSocket::~ServerSocket()
{
}

void
ServerSocket::disconnection( ClientSocket * socket )
{
	socket->socket().close();

	std::lock_guard< std::mutex > lock( d->m_mutex );

	auto it = d->findSocket( socket );

	if( it != d->m_clients.end() )
		d->m_clients.erase( it );
}

void
ServerSocket::sendListOfSources( ClientSocket * socket )
{
	d->sendListOfSources( socket );
}

void
ServerSocket::initSource( const Source & source )
{
	{
		std::lock_guard< std::mutex > lock( d->m_mutex );

		d->m_sources.push_back( source );
	}

	d->notifyAllClientsAboutValueChange( source );
}

void
ServerSocket::updateSource( const Source & source )
{
	{
		std::lock_guard< std::mutex > lock( d->m_mutex );

		auto i = std::find( d->m_sources.begin(), d->m_sources.end(),
			source );

		if( i != d->m_sources.end() )
			*i = source;
	}

	d->notifyAllClientsAboutValueChange( source );
}

void
ServerSocket::deinitSource( const Source & source )
{
	{
		std::lock_guard< std::mutex > lock( d->m_mutex );

		d->m_sources.remove( source );
	}

	d->notifyAllClientsAboutDeinitSource( source );
}

} /* namespace BoostProtobuf */

} /* namespace Como */

#endif // COMO_BOOST_PROTOBUF