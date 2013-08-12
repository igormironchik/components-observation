
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

#ifndef COMO__SAMPLES__SERVER__FIRST_THREAD_HPP__INCLUDED
#define COMO__SAMPLES__SERVER__FIRST_THREAD_HPP__INCLUDED

// Qt include.
#include <QtCore/QThread>
#include <QtCore/QScopedPointer>
#include <QtCore/QObject>


namespace Como {
	class ServerSocket;
} /* namespace Como */

//
// FirstObject
//

/*!
	This is our working object that will operate
	with Como::Source
*/
class FirstObject
	:	public QObject
{
	Q_OBJECT

public:
	explicit FirstObject( Como::ServerSocket * socket,
		QObject * parent = 0 );
	~FirstObject();

private slots:
	//! Switch to state #1.
	void slotSwitchToStateOne();
	//! Switch to state #2.
	void slotSwitchToStateTwo();
	//! Switch to state #3.
	void slotSwitchToStateThree();

private:
	struct FirstObjectPrivate;
	QScopedPointer< FirstObjectPrivate > d;
}; // class FirstObject


//
// FirstThread
//

//! First thread in the server.
class FirstThread
	:	public QThread
{
public:
	explicit FirstThread( Como::ServerSocket * socket,
		QObject * parent = 0 );
	~FirstThread();

protected:
	//! Run the thread.
	void run();

private:
	struct FirstThreadPrivate;
	QScopedPointer< FirstThreadPrivate > d;
}; // class FirstThread

#endif // COMO__SAMPLES__SERVER__FIRST_THREAD_HPP__INCLUDED
