
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

#ifndef COMO__BOOSTPROTOBUF__BP_SERVER_SOCKET_HPP__INCLUDED
#define COMO__BOOSTPROTOBUF__BP_SERVER_SOCKET_HPP__INCLUDED

// C++ include.
#include <memory>

// boost include.
#include <boost/asio/io_service.hpp>


namespace Como {

namespace BoostProtobuf {

class Source;
class ClientSocket;


//
// ServerSocket
//

class ServerSocketPrivate;

/*!
	Server socket. This type of socket designed for the dissemination
	of monitoring information outside the application. It is passed
	to the constructor of the class Source, which is when
	the state changed initiates a message to be sent out.

	This class is thread-safe.
*/
class ServerSocket
{
public:
	ServerSocket( boost::asio::io_service & io,
		unsigned short portNum );

	~ServerSocket();

	ServerSocket( const ServerSocket & ) = delete;
	ServerSocket & operator = ( const ServerSocket & ) = delete;

	//! Disconnection occured.
	void disconnection( ClientSocket * socket );

	//! Send list of sources.
	void sendListOfSources( ClientSocket * socket );

	/*!
		Initialize sources list with new source.
		This method is called in constructor of the Source.

		ServerSocket must have list of all sources to
		send out info about them.
	*/
	void initSource( const Source & source );

	/*!
		This method is called when source's value is changed.
		And at this moment all clients notifed about this change.

		This method is called in setValue method of the Source.
	*/
	void updateSource( const Source & source );

	/*!
		Deinit source.
	*/
	void deinitSource( const Source & source );

private:
	std::unique_ptr< ServerSocketPrivate > d;
}; /* class ServerSocket */

} /* namespace BoostProtobuf */

} /* namespace Como */

#endif // COMO__BOOSTPROTOBUF__BP_SERVER_SOCKET_HPP__INCLUDED

#endif // COMO_BOOST_PROTOBUF
