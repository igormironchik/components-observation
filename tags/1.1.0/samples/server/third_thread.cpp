
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

// Server include.
#include "third_thread.hpp"

// Como include.
#include <Como/Source>

// Qt include.
#include <QTimer>


//
// ThirdObject::ThirdObjectPrivate
//

struct ThirdObject::ThirdObjectPrivate {
	ThirdObjectPrivate( Como::ServerSocket * socket )
		:	m_source( new Como::Source( Como::Source::String,
				QString( "ThirdObject" ),
				QString( "DynamicSource" ),
				QVariant( QString( "I'm alive" ) ),
				QString( "This is example of usage\n"
					"Como::Source::String as dynamic source." ),
				socket ) )
		,	m_socket( socket )
	{}

	~ThirdObjectPrivate()
	{
		m_source->deleteLater();
	}

	Como::Source * m_source;
	Como::ServerSocket * m_socket;
}; // struct ThirdObject::ThirdObjectPrivate


static const int c_delay = 1500;

//
// ThirdObject
//

ThirdObject::ThirdObject( Como::ServerSocket * socket,
	QObject * parent )
	:	QObject( parent )
	,	d( new ThirdObjectPrivate( socket ) )
{
	QTimer::singleShot( c_delay, this, SLOT( slotSwitchToStateTwo() ) );
}

ThirdObject::~ThirdObject()
{
}

void
ThirdObject::slotSwitchToStateOne()
{
	d->m_source = new Como::Source( Como::Source::String,
		QString( "ThirdObject" ),
		QString( "DynamicSource" ),
		QVariant( QString( "I'm alive" ) ),
		QString( "This is example of usage\n"
			"Como::Source::String as dynamic source." ),
		d->m_socket );

	QTimer::singleShot( c_delay, this, SLOT( slotSwitchToStateTwo() ) );
}

void
ThirdObject::slotSwitchToStateTwo()
{
	d->m_source->setValue( QString( "I will die soon" ) );

	QTimer::singleShot( c_delay, this, SLOT( slotSwitchToStateThree() ) );
}

void
ThirdObject::slotSwitchToStateThree()
{
	d->m_source->deleteLater();
	d->m_source = 0;

	QTimer::singleShot( c_delay, this, SLOT( slotSwitchToStateOne() ) );
}


//
// ThirdThread::ThirdThreadPrivate
//

struct ThirdThread::ThirdThreadPrivate {
	explicit ThirdThreadPrivate( Como::ServerSocket * socket )
		:	m_socket( socket )
	{
	}

	Como::ServerSocket * m_socket;
}; // struct ThirdThread::ThirdThreadPrivate


//
// ThirdThread
//

ThirdThread::ThirdThread( Como::ServerSocket * socket,
	QObject * parent )
	:	QThread( parent )
	,	d( new ThirdThreadPrivate( socket ) )
{
}

ThirdThread::~ThirdThread()
{
}

void
ThirdThread::run()
{
	ThirdObject object( d->m_socket );

	exec();
}
