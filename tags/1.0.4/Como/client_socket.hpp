
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

#ifndef COMO__CLIENT_SOCKET_HPP__INCLUDED
#define COMO__CLIENT_SOCKET_HPP__INCLUDED

// Qt include.
#include <QTcpSocket>
#include <QScopedPointer>


namespace Como {

class Source;


//
// ClientSocket
//

/*!
	Client socket. This type of socket designed to provide
	information about Source changes on the server
	side from ServerSocket.

	Don't forget register Source metatype information in Qt with:

	\code
	qRegisterMetaType< Como::Source > ( "Como::Source" );
	\endcode
*/
class ClientSocket
	:	public QTcpSocket
{
	Q_OBJECT

signals:
	//! Source has updated his value.
	void sourceHasUpdatedValue( const Como::Source & );
	//! GetListOfSourcesMessage request received
	void getListOfSourcesMessageReceived();
	//! Deintitialization of the source.
	void sourceDeinitialized( const Como::Source & );

public:
	ClientSocket( QObject * parent = 0 );
	~ClientSocket();

	//! Send information about source.
	void sendSourceMessage( const Source & source );

	//! Send request to receive all available sources.
	void sendGetListOfSourcesMessage();

	//! Send information about deinitialization of the source.
	void sendDeinitSourceMessage( const Source & source );

private:
	//! Handle errors in read message.
	void handleErrorInReadMessage();

private slots:
	//! New data available.
	void slotReadyRead();

private:
	struct ClientSocketPrivate;
	QScopedPointer< ClientSocketPrivate > d;
}; // class ClientSocket

} /* namespace Como */

#endif // COMO__CLIENT_SOCKET_HPP__INCLUDED
