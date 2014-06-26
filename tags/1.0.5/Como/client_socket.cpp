
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

// Como include.
#include <Como/ClientSocket>
#include <Como/private/Buffer>
#include <Como/private/Protocol>
#include <Como/private/Messages>


namespace Como {

//
// ClientSocket::ClientSocketPrivate
//

struct ClientSocket::ClientSocketPrivate {
	//! Buffer.
	Buffer m_buf;
}; // struct ClientSocket::ClientSocketPrivate


//
// ClientSocket
//

ClientSocket::ClientSocket( QObject * parent )
	:	QTcpSocket( parent )
	,	d( new ClientSocketPrivate )
{
	connect( this, &ClientSocket::readyRead,
		this, &ClientSocket::slotReadyRead );
}

ClientSocket::~ClientSocket()
{
}

void
ClientSocket::sendSourceMessage( const Source & source )
{
	SourceMessage msg( source );

	QSharedPointer< QByteArray > data = Protocol::writeMessage( msg );

	write( *data );

	flush();
}

void
ClientSocket::sendGetListOfSourcesMessage()
{
	GetListOfSourcesMessage msg;

	QSharedPointer< QByteArray > data = Protocol::writeMessage( msg );

	write( *data );

	flush();
}

void
ClientSocket::sendDeinitSourceMessage( const Source & source )
{
	DeinitSourceMessage msg( source );

	QSharedPointer< QByteArray > data = Protocol::writeMessage( msg );

	write( *data );

	flush();
}

void
ClientSocket::slotReadyRead()
{
	d->m_buf.write( readAll() );

	try {
		int bytesRead = 0;

		while( !d->m_buf.isEmpty() )
		{
			bytesRead = 0;

			QSharedPointer< Message > msg =
				Protocol::readMessage( d->m_buf.data(), bytesRead );

			d->m_buf.remove( bytesRead );

			switch( msg->type() )
			{
				case GetListOfSourcesMessage::messageType :
				{
					emit getListOfSourcesMessageReceived();
				} break;

				case SourceMessage::messageType :
				{
					SourceMessage * sourceMsg =
						dynamic_cast< SourceMessage* > ( msg.data() );

					emit sourceHasUpdatedValue( sourceMsg->source() );
				} break;

				case DeinitSourceMessage::messageType :
				{
					DeinitSourceMessage * deinitMsg =
						dynamic_cast< DeinitSourceMessage* > ( msg.data() );

					emit sourceDeinitialized( deinitMsg->source() );
				} break;
			}
		}
	}
	catch( const NotEnoughDataReceivedException & )
	{
	}
	catch( const GarbageReceivedException & )
	{
		handleErrorInReadMessage();
	}
	catch( const ProtocolException & )
	{
		handleErrorInReadMessage();
	}
}

void
ClientSocket::handleErrorInReadMessage()
{
	d->m_buf.clear();
	abort();
}

} /* namespace Como */
