
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

#ifndef COMO__SAMPLES__SERVER__SECOND_THREAD_HPP__INCLUDED
#define COMO__SAMPLES__SERVER__SECOND_THREAD_HPP__INCLUDED

// Qt include.
#include <QThread>
#include <QScopedPointer>
#include <QObject>


namespace Como {
	class ServerSocket;
} /* namespace Como */


//
// SecondObject
//

/*!
	This is our working object that will operate
	with Como::Source
*/
class SecondObject
	:	public QObject
{
	Q_OBJECT

public:
	explicit SecondObject( Como::ServerSocket * socket,
		QObject * parent = 0 );
	~SecondObject();

private slots:
	//! Switch to state #1.
	void slotSwitchToStateOne();
	//! Switch to state #2.
	void slotSwitchToStateTwo();
	//! Switch to state #3.
	void slotSwitchToStateThree();

private:
	struct SecondObjectPrivate;
	QScopedPointer< SecondObjectPrivate > d;
}; // class SecondObject


//
// SecondThread
//

//! First thread in the server.
class SecondThread
	:	public QThread
{
public:
	explicit SecondThread( Como::ServerSocket * socket,
		QObject * parent = 0 );
	~SecondThread();

protected:
	//! Run the thread.
	void run();

private:
	struct SecondThreadPrivate;
	QScopedPointer< SecondThreadPrivate > d;
}; // class SecondThread

#endif // COMO__SAMPLES__SERVER__SECOND_THREAD_HPP__INCLUDED
