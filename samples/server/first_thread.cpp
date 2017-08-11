
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
#include "first_thread.hpp"

// Como include.
#include <Como/Source>

// Qt include.
#include <QTimer>


//
// FirstObject::FirstObjectPrivate
//

struct FirstObject::FirstObjectPrivate {
	explicit FirstObjectPrivate( Como::ServerSocket * socket )
		:	m_source( Como::Source::String,
				QString( "FirstObject" ),
				QString( "StringSource" ),
				QVariant( QString( "I" ) ),
				QString( "This is example of usage\n"
					"Como::Source::String source." ),
				socket )
	{}

	Como::Source m_source;
}; // struct FirstObject::FirstObjectPrivate


static const int c_delay = 750;

//
// FirstObject
//

FirstObject::FirstObject( Como::ServerSocket * socket,
	QObject * parent )
	:	QObject( parent )
	,	d( new FirstObjectPrivate( socket ) )
{
	QTimer::singleShot( c_delay, this, SLOT( slotSwitchToStateTwo() ) );
}

FirstObject::~FirstObject()
{
}

void
FirstObject::slotSwitchToStateOne()
{
	d->m_source.setValue( QString( "I" ) );

	QTimer::singleShot( c_delay, this, SLOT( slotSwitchToStateTwo() ) );
}

void
FirstObject::slotSwitchToStateTwo()
{
	d->m_source.setValue( QString( "love" ) );

	QTimer::singleShot( c_delay, this, SLOT( slotSwitchToStateThree() ) );
}

void
FirstObject::slotSwitchToStateThree()
{
	d->m_source.setValue( QString( "Como" ) );

	QTimer::singleShot( c_delay, this, SLOT( slotSwitchToStateOne() ) );
}


//
// FirstThread::FirstThreadPrivate
//

struct FirstThread::FirstThreadPrivate {
	explicit FirstThreadPrivate( Como::ServerSocket * socket )
		:	m_socket( socket )
	{
	}

	Como::ServerSocket * m_socket;
}; // struct FirstThread::FirstThreadPrivate


//
// FirstThread
//

FirstThread::FirstThread( Como::ServerSocket * socket,
	QObject * parent )
	:	QThread( parent )
	,	d( new FirstThreadPrivate( socket ) )
{
}

FirstThread::~FirstThread()
{
}

void
FirstThread::run()
{
	FirstObject object( d->m_socket );

	exec();
}
