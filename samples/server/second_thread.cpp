
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
#include "second_thread.hpp"

// Como include.
#include <Como/Source>

// Qt include.
#include <QtCore/QTimer>


//
// SecondObject::SecondObjectPrivate
//

struct SecondObject::SecondObjectPrivate {
	SecondObjectPrivate( Como::ServerSocket * socket )
		:	m_source( Como::Source::Int,
				QString( "SecondObject" ),
				QString( "IntSource" ),
				QVariant( 0 ),
				QString( "This is example of usage\n"
					"Como::Source::Int source." ),
				socket )
	{}

	Como::Source m_source;
}; // struct SecondObject::SecondObjectPrivate


static const int c_delay = 750;

//
// FirstObject
//

SecondObject::SecondObject( Como::ServerSocket * socket,
	QObject * parent )
	:	QObject( parent )
	,	d( new SecondObjectPrivate( socket ) )
{
	QTimer::singleShot( c_delay, this, SLOT( slotSwitchToStateTwo() ) );
}

SecondObject::~SecondObject()
{
}

void
SecondObject::slotSwitchToStateOne()
{
	d->m_source.setValue( 0 );

	QTimer::singleShot( c_delay, this, SLOT( slotSwitchToStateTwo() ) );
}

void
SecondObject::slotSwitchToStateTwo()
{
	d->m_source.setValue( 50 );

	QTimer::singleShot( c_delay, this, SLOT( slotSwitchToStateThree() ) );
}

void
SecondObject::slotSwitchToStateThree()
{
	d->m_source.setValue( 100 );

	QTimer::singleShot( c_delay, this, SLOT( slotSwitchToStateOne() ) );
}


//
// SecondThread::SecondThreadPrivate
//

struct SecondThread::SecondThreadPrivate {
	explicit SecondThreadPrivate( Como::ServerSocket * socket )
		:	m_socket( socket )
	{
	}

	Como::ServerSocket * m_socket;
}; // struct SecondThread::SecondThreadPrivate


//
// SecondThread
//

SecondThread::SecondThread( Como::ServerSocket * socket,
	QObject * parent )
	:	QThread( parent )
	,	d( new SecondThreadPrivate( socket ) )
{
}

SecondThread::~SecondThread()
{
}

void
SecondThread::run()
{
	SecondObject object( d->m_socket );

	exec();
}
