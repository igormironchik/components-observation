
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

#ifndef COMO__BOOSTPROTOBUF__BP_CLIENT_SOCKET_HPP__INCLUDED
#define COMO__BOOSTPROTOBUF__BP_CLIENT_SOCKET_HPP__INCLUDED

// C++ include.
#include <memory>

// boost include.
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>


namespace Como {

namespace BoostProtobuf {

using namespace boost::asio::ip;

class Source;
class ServerSocket;


//
// ClientSocket
//

class ClientSocketPrivate;

/*!
	Client socket. This type of socket designed to provide
	information about Source changes on the server
	side from ServerSocket.
*/
class ClientSocket
{
public:
	ClientSocket( boost::asio::io_service & io,
		ServerSocket * server );
	~ClientSocket();

	ClientSocket( const ClientSocket & ) = delete;
	ClientSocket & operator = ( const ClientSocket & ) = delete;

	//! \return Socket.
	tcp::socket & socket() const;

	//! Start reading messages from the socket.
	void start();

	//! Send information about source.
	void sendSourceMessage( const Source & source );

	//! Send information about deinitialization of the source.
	void sendDeinitSourceMessage( const Source & source );

private:
	std::unique_ptr< ClientSocketPrivate > d;
}; // class ClientSocket

} /* namespace BoostProtobuf */

} /* namespace Como */

#endif // COMO__BOOSTPROTOBUF__BP_CLIENT_SOCKET_HPP__INCLUDED

#endif // COMO_BOOST_PROTOBUF
