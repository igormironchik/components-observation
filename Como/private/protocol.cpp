
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
#include <Como/private/Protocol>
#include <Como/private/Messages>

// Qt include.
#include <QDataStream>
#include <QIODevice>


namespace Como {

class Message;

//
// ProtocolException
//

ProtocolException::ProtocolException( const QString & what )
	:	std::runtime_error( what.toLocal8Bit().data() )
	,	m_what( what )
{
}

ProtocolException::~ProtocolException() throw()
{
}

const QString &
ProtocolException::whatAsQString() const
{
	return m_what;
}


//
// GarbageReceivedException
//

GarbageReceivedException::GarbageReceivedException()
	:	ProtocolException( QLatin1String( "Garbage received." ) )
{
}

GarbageReceivedException::~GarbageReceivedException() throw()
{
}


//
// NotEnoughDataReceivedException
//

NotEnoughDataReceivedException::NotEnoughDataReceivedException()
	:	ProtocolException( QLatin1String( "Not enough data." ) )
{
}

NotEnoughDataReceivedException::~NotEnoughDataReceivedException() throw()
{
}


//
// Protocol
//

//! Magic number for the Como protocol #1.
static const quint64 c_magicNumber = 0x434F4D4F50524F31;

/*!
	Message's header size. 12 bytes.

	8 bytes of magic number +
	2 bytes of message type +
	2 bytes of message length.
*/
static const quint8 c_headerSize = 12;

QSharedPointer< QByteArray >
Protocol::writeMessage( const Message & msg )
{
	QSharedPointer< QByteArray > data =
		QSharedPointer< QByteArray > ( new QByteArray );

	QSharedPointer< QByteArray > msgData = msg.serialize();

	QDataStream dataStream( data.data(), QIODevice::WriteOnly );
	dataStream.setVersion( QDataStream::Qt_4_0 );

	const quint16 msgDataSize = msgData->size();

	dataStream << c_magicNumber << msg.type() << msgDataSize;
	dataStream.writeRawData( msgData->constData(), msgDataSize );

	return data;
}

QSharedPointer< Message >
Protocol::readMessage( const QByteArray & data, int & bytesRead )
{
	if( data.size() < c_headerSize )
		throw NotEnoughDataReceivedException();

	QDataStream dataStream( data );
	dataStream.setVersion( QDataStream::Qt_4_0 );

	quint64 magicNumber = 0;
	quint16 messageType = 0;
	quint16 messageLength = 0;

	dataStream >> magicNumber >> messageType >> messageLength;

	switch( messageType )
	{
		case GetListOfSourcesMessage::messageType :
		case SourceMessage::messageType :
		case DeinitSourceMessage::messageType :
			break;

		default :
			throw GarbageReceivedException();
	}

	if( magicNumber != c_magicNumber )
		throw GarbageReceivedException();

	if( data.size() < c_headerSize + messageLength )
		throw NotEnoughDataReceivedException();

	QByteArray msgData( messageLength, 0x00 );

	dataStream.readRawData( msgData.data(), messageLength );

	bytesRead = c_headerSize + messageLength;

	QSharedPointer < Message > msg;

	switch( messageType )
	{
		case GetListOfSourcesMessage::messageType :
		{
			msg = QSharedPointer< Message > ( new GetListOfSourcesMessage );
		} break;
		case SourceMessage::messageType :
		{
			msg = QSharedPointer< Message > ( new SourceMessage );
		} break;
		case DeinitSourceMessage::messageType :
		{
			msg = QSharedPointer< Message > ( new DeinitSourceMessage );
		} break;

		default :
			return QSharedPointer < Message > ();
	}

	if( !msg->deserialize( msgData ) )
		throw GarbageReceivedException();

	return msg;
}

} /* namespace Como */
