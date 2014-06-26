
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
#include <Como/ServerSocket>
#include <Como/ClientSocket>
#include <Como/Source>

// Qt include.
#include <QMutexLocker>
#include <QList>
#include <QMutex>
#include <QEvent>
#include <QCoreApplication>


namespace Como {

//
// SourceHasUpdatedValueEvent
//

static const int SourceHasUpdatedValueEventType =
	QEvent::registerEventType();

class SourceHasUpdatedValueEvent
	:	public QEvent
{
public:
	SourceHasUpdatedValueEvent( const Source & source )
		:	QEvent( static_cast< QEvent::Type > ( SourceHasUpdatedValueEventType ) )
		,	m_source( source )
	{
	}

	const Source & source() const
	{
		return m_source;
	}

private:
	Source m_source;
}; // class SourceHasUpdatedValueEvent


//
// SourceHasDeinitializedEvent
//

static const int SourceHasDeinitializedEventType =
	QEvent::registerEventType();

class SourceHasDeinitializedEvent
	:	public QEvent
{
public:
	SourceHasDeinitializedEvent( const Source & source )
		:	QEvent( static_cast< QEvent::Type > ( SourceHasDeinitializedEventType ) )
		,	m_source( source )
	{
	}

	const Source & source() const
	{
		return m_source;
	}

private:
	Source m_source;
}; // class SourceHasDeinitializedEvent


//
// ServerSocket::ServerSocketPrivate
//

struct ServerSocket::ServerSocketPrivate {
	//! List of client sockets.
	QList< ClientSocket* > m_clientSockets;
	//! List of all available sources.
	QList< Source > m_sources;
	//! Mutex.
	QMutex m_mutex;
}; // struct ServerSocket::ServerSocketPrivate


//
// ServerSocket
//

ServerSocket::ServerSocket( QObject * parent )
	:	QTcpServer( parent )
	,	d( new ServerSocketPrivate )
{
}

ServerSocket::~ServerSocket()
{
}

void
ServerSocket::initSource( const Source & source )
{
	QMutexLocker lock( &d->m_mutex );

	d->m_sources.append( source );

	QCoreApplication::postEvent( this,
		new SourceHasUpdatedValueEvent( source ) );
}

void
ServerSocket::updateSource( const Source & source )
{
	QMutexLocker lock( &d->m_mutex );

	const int index = d->m_sources.indexOf( source );

	if( index )
		d->m_sources[ index ] = source;

	QCoreApplication::postEvent( this,
		new SourceHasUpdatedValueEvent( source ) );
}

void
ServerSocket::deinitSource( const Source & source )
{
	QMutexLocker lock( &d->m_mutex );

	const int index = d->m_sources.indexOf( source );

	if( index )
		d->m_sources.removeAt( index );

	QCoreApplication::postEvent( this,
		new SourceHasDeinitializedEvent( source ) );
}

void
ServerSocket::incomingConnection( int socketDescriptor )
{
	ClientSocket * socket = new ClientSocket( this );

	if( socket->setSocketDescriptor( socketDescriptor ) )
	{
		connect( socket, &ClientSocket::disconnected,
			this, &ServerSocket::slotClientDisconnected,
			Qt::QueuedConnection );

		connect( socket, &ClientSocket::getListOfSourcesMessageReceived,
			this, &ServerSocket::slotGetListOfSourcesMessageReceived,
			Qt::QueuedConnection );

		{
			QMutexLocker lock( &d->m_mutex );

			d->m_clientSockets.append( socket );
		}

		emit clientConnected( socket );
	}
	else
		delete socket;
}

void
ServerSocket::customEvent( QEvent * e )
{
	if( e->type() == SourceHasUpdatedValueEventType )
	{
		SourceHasUpdatedValueEvent * updateEvent =
			dynamic_cast< SourceHasUpdatedValueEvent* > ( e );

		notifyAllClientsAboutValueChange( updateEvent->source() );

		e->accept();
	}
	else if( e->type() == SourceHasDeinitializedEventType )
	{
		SourceHasDeinitializedEvent * deinitEvent =
			dynamic_cast< SourceHasDeinitializedEvent* > ( e );

		notifyAllClientsAboutDeinitSource( deinitEvent->source() );

		e->accept();
	}
	else
		e->ignore();
}

void
ServerSocket::slotClientDisconnected()
{
	ClientSocket * socket = qobject_cast< ClientSocket* > ( sender() );

	{
		QMutexLocker lock( &d->m_mutex );

		d->m_clientSockets.removeOne( socket );
	}

	emit clientDisconnected( socket );

	socket->deleteLater();
}

void
ServerSocket::slotGetListOfSourcesMessageReceived()
{
	ClientSocket * socket = qobject_cast< ClientSocket* > ( sender() );

	QMutexLocker lock( &d->m_mutex );

	foreach( const Source & source, d->m_sources )
		socket->sendSourceMessage( source );
}

void
ServerSocket::notifyAllClientsAboutValueChange( const Source & source )
{
	QMutexLocker lock( &d->m_mutex );

	foreach( ClientSocket * socket, d->m_clientSockets )
		socket->sendSourceMessage( source );
}

void
ServerSocket::notifyAllClientsAboutDeinitSource( const Source & source )
{
	QMutexLocker lock( &d->m_mutex );

	foreach( ClientSocket * socket, d->m_clientSockets )
		socket->sendDeinitSourceMessage( source );
}

} /* namespace Como */
