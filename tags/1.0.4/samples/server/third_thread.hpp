
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

#ifndef COMO__SAMPLES__SERVER__THIRD_THREAD_HPP__INCLUDED
#define COMO__SAMPLES__SERVER__THIRD_THREAD_HPP__INCLUDED

// Qt include.
#include <QThread>
#include <QScopedPointer>
#include <QObject>


namespace Como {
	class ServerSocket;
} /* namespace Como */

//
// ThirdObject
//

/*!
	This is our working object that will operate
	with Como::Source
*/
class ThirdObject
	:	public QObject
{
	Q_OBJECT

public:
	explicit ThirdObject( Como::ServerSocket * socket,
		QObject * parent = 0 );
	~ThirdObject();

private slots:
	//! Switch to state #1.
	void slotSwitchToStateOne();
	//! Switch to state #2.
	void slotSwitchToStateTwo();
	//! Switch to state #3.
	void slotSwitchToStateThree();

private:
	struct ThirdObjectPrivate;
	QScopedPointer< ThirdObjectPrivate > d;
}; // class ThirdObject


//
// ThirdThread
//

//! First thread in the server.
class ThirdThread
	:	public QThread
{
public:
	explicit ThirdThread( Como::ServerSocket * socket,
		QObject * parent = 0 );
	~ThirdThread();

protected:
	//! Run the thread.
	void run();

private:
	struct ThirdThreadPrivate;
	QScopedPointer< ThirdThreadPrivate > d;
}; // class ThirdThread

#endif // COMO__SAMPLES__SERVER__THIRD_THREAD_HPP__INCLUDED
