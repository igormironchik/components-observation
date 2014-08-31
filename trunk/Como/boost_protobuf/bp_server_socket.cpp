
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
#include <iostream>


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
	void sendListOfSources( const std::weak_ptr< ClientSocket > & socket )
	{
		auto sp = socket.lock();

		if( sp )
		{
			std::lock_guard< std::mutex > lock( m_mutex );

			std::for_each( m_sources.begin(), m_sources.end(),
				[ &sp ] ( const Source & source )
					{
						sp->sendSourceMessage( source );
					}
			);
		}
	}

	//! Start accepting new conection.
	void startAcceptNewConnection()
	{
		std::cout << "startAcceptNewConnection" << std::endl;

		auto socket = std::shared_ptr< ClientSocket >
			( new ClientSocket( m_acceptor.get_io_service(), m_parent ) );

		m_acceptor.async_accept( socket->socket(),
			boost::bind( &ServerSocketPrivate::handleAccept, this, socket,
				boost::asio::placeholders::error ) );
	}

	//! Handle accept.
	void handleAccept( std::shared_ptr< ClientSocket > socket,
		const boost::system::error_code & error )
	{
		std::cout << "handleAccept" << std::endl;

		if( !error )
		{
			std::lock_guard< std::mutex > lock( m_mutex );

			std::cout << "handleAccept 1"<< std::endl;

			socket->setPtr( socket );

			std::cout << "handleAccept 2"<< std::endl;

			socket->start( socket );

			std::cout << "handleAccept 3"<< std::endl;

			m_clients.push_back( socket );

			std::cout << "handleAccept 4"<< std::endl;
		}

		startAcceptNewConnection();
	}

	typedef std::list< std::shared_ptr< ClientSocket > > ListOfSockets;

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
ServerSocket::disconnection( const std::weak_ptr< ClientSocket > & socket )
{
	std::cout << "disconnection" << std::endl;

	std::shared_ptr< ClientSocket > sp = socket.lock();

	if( sp )
	{
		std::lock_guard< std::mutex > lock( d->m_mutex );

		auto it = std::find( d->m_clients.begin(), d->m_clients.end(),
			sp );

		if( it != d->m_clients.end() )
			d->m_clients.erase( it );

		sp->stop( sp );
	}
}

void
ServerSocket::sendListOfSources( const std::weak_ptr< ClientSocket > & socket )
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
