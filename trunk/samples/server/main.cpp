
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
#include "second_thread.hpp"
#include "third_thread.hpp"

// Como include.
#include <Como/ServerSocket>
#include <Como/Source>

// Qt include.
#include <QCoreApplication>
#include <QMetaType>


int main( int argc, char ** argv )
{
	QCoreApplication app( argc, argv );

	qRegisterMetaType< Como::Source > ( "Como::Source" );

	Como::ServerSocket socket;

	FirstThread firstThread( &socket );
	SecondThread secondThread( &socket );
	ThirdThread thirdThread( &socket );

	firstThread.start();
	secondThread.start();
	thirdThread.start();

	socket.listen( QHostAddress::Any, 4545 );

	return app.exec();
}
