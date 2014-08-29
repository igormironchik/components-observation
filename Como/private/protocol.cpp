
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

#ifdef COMO_BOOST_PROTOBUF
#include <Como/boost_protobuf/messages.pb.h>

// C++ include.
#include <string>
#include <regex>
#include <cstdlib>
#endif

// Qt include.
#include <QDataStream>


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

ProtocolException::~ProtocolException()
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

GarbageReceivedException::~GarbageReceivedException()
{
}


//
// NotEnoughDataReceivedException
//

NotEnoughDataReceivedException::NotEnoughDataReceivedException()
	:	ProtocolException( QLatin1String( "Not enough data." ) )
{
}

NotEnoughDataReceivedException::~NotEnoughDataReceivedException()
{
}


//
// Protocol
//

//! Magic number for the Como protocol #1.
static const quint64 c_magicNumber = 0x434F4D4F50524F31;

//! Magic number for the Como protocol #2.
static const quint64 c_magicNumber2 = 0x434F4D4F50524F32;

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

#ifdef COMO_BOOST_PROTOBUF

static inline QDateTime parseDateTime( const std::string & v )
{
	static const std::regex reg( "(\\d{4})-(\\d{2})-(\\d{2})T"
		"(\\d{2}):(\\d{2}):(\\d{2}),(\\d{9})" );

	std::smatch match;

	if( std::regex_match( v, match, reg ) )
	{
		const QDate date( std::atoi( match[ 0 ].str().c_str() ),
			std::atoi( match[ 1 ].str().c_str() ),
			std::atoi( match[ 2 ].str().c_str() ) );

		const QTime time( std::atoi( match[ 3 ].str().c_str() ),
			std::atoi( match[ 4 ].str().c_str() ),
			std::atoi( match[ 5 ].str().c_str() ),
			std::atoi( match[ 6 ].str().c_str() ) / 1000000 );

		return QDateTime( date, time );
	}
	else
		return QDateTime::currentDateTime();
}

static inline QVariant parseValue( const std::string & v, Source::Type type )
{
	switch( type )
	{
		case Source::String :
			return QVariant( QString( v.c_str() ) );

		case Source::Int :
			return QVariant( std::atoi( v.c_str() ) );

		case Source::UInt :
			return QVariant( (unsigned int) std::atol( v.c_str() ) );

		case Source::LongLong :
			return QVariant( std::atoll( v.c_str() ) );

		case Source::ULongLong :
		{
			char * end;
			return QVariant( std::strtoull( v.c_str(), &end, 10 ) );
		}

		case Source::Double :
			return QVariant( std::atof( v.c_str() ) );

		case Source::DateTime :
			return QVariant( parseDateTime( v ) );

		case Source::Time :
			return QVariant( parseDateTime( v ).time() );

		default :
			return QVariant();
	}
}

static inline Source parseSource( const ComoMessage & msg )
{
	Source source( ( Source::Type ) msg.type(),
		QString( msg.name().c_str() ),
		QString( msg.typename_().c_str() ),
		parseValue( msg.value(), ( Source::Type ) msg.type() ),
		QString( msg.description().c_str() ) );

	source.setDateTime( parseDateTime( msg.datetime() ) );

	return source;
}

#endif // COMO_BOOST_PROTOBUF

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

	if( magicNumber == c_magicNumber2 )
	{
#ifdef COMO_BOOST_PROTOBUF
		QByteArray data( messageLength, 0x00 );

		dataStream.readRawData( data.data(), messageLength );

		ComoMessage msg;
		msg.ParseFromArray( data.constData(), messageLength );

		switch( messageType )
		{
			case GetListOfSourcesMessage::messageType :
				return QSharedPointer< Message > (
					new GetListOfSourcesMessage );

			case SourceMessage::messageType :
				return QSharedPointer< Message > (
					new SourceMessage( parseSource( msg ) ) );

			case DeinitSourceMessage::messageType :
				return QSharedPointer< Message > (
					new DeinitSourceMessage( parseSource( msg ) ) );

			default :
				return QSharedPointer < Message > ();
		}
#endif // COMO_BOOST_PROTOBUF
	}
	else
	{
		if( magicNumber != c_magicNumber )
			throw GarbageReceivedException();

		if( messageType != GetListOfSourcesMessage::messageType &&
			messageType != SourceMessage::messageType &&
			messageType != DeinitSourceMessage::messageType )
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
		}

		if( !msg->deserialize( msgData ) )
			throw GarbageReceivedException();

		return msg;
	}
}

} /* namespace Como */
