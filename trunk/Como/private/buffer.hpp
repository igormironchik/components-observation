
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

#ifndef COMO__BUFFER_HPP__INCLUDED
#define COMO__BUFFER_HPP__INCLUDED

// Qt include.
#include <QByteArray>


namespace Como {

//
// Buffer
//

//! Buffer of the ClientSocket.
class Buffer {
public:
	Buffer();

	//! \return Data of the buffer.
	const QByteArray & data() const;

	//! Write data to the end of the buffer.
	void write( const QByteArray & data );

	//! Remove first \arg bytes bytes from the beginning of the buffer.
	void remove( int bytes );

	//! Clear the buffer.
	void clear();

	//! Is buffer empty?
	bool isEmpty() const;

	//! \return Size of the buffer.
	int size() const;

private:
	//! Data.
	QByteArray m_data;
}; // class Buffer

} /* namespace Como */

#endif // COMO__BUFFER_HPP__INCLUDED
