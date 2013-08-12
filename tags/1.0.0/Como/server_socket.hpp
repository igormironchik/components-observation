
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2012 Igor Mironchik

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

#ifndef COMO__SERVER_SOCKET_HPP__INCLUDED
#define COMO__SERVER_SOCKET_HPP__INCLUDED

// Qt include.
#include <QtNetwork/QTcpServer>
#include <QtCore/QScopedPointer>


namespace Como {

class Source;
class ClientSocket;


//
// ServerSocket
//

/*!
	Server socket. This type of socket designed for the dissemination
	of monitoring information outside the application. It is passed
	to the constructor of the class Source, which is when
	the state changed initiates a message to be sent out.

	This class is thread-safe.
*/
class ServerSocket
	:	public QTcpServer
{
	Q_OBJECT

signals:
	//! New client has connected;
	void clientConnected( Como::ClientSocket* );
	//! Client has disconnected.
	void clientDisconnected( Como::ClientSocket* );

public:
	ServerSocket( QObject * parent = 0 );
	~ServerSocket();

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

protected:
	//!	Process new incoming connection.
	void incomingConnection( int socketDescriptor );
	//! Custom event.
	void customEvent( QEvent * e );

private slots:
	//! Client was disconnected.
	void slotClientDisconnected();
	//! Received GetListOfSourcesMessage message.
	void slotGetListOfSourcesMessageReceived();

private:
	//! Notify all clients about changes in value of the source.
	void notifyAllClientsAboutValueChange( const Source & source );
	//! Notify all clients about deinitialization of the source.
	void notifyAllClientsAboutDeinitSource( const Source & source );

private:
	struct ServerSocketPrivate;
	QScopedPointer< ServerSocketPrivate > d;
}; /* class ServerSocket */

} /* namespace Como */

#endif // COMO__SERVER_SOCKET_HPP__INCLUDED
