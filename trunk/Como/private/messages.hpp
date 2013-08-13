
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

#ifndef COMO__MESSAGES_HPP__INCLUDED
#define COMO__MESSAGES_HPP__INCLUDED

// Como include.
#include <Como/Source>

// Qt include.
#include <QSharedPointer>
#include <QByteArray>


namespace Como {

//
// Message
//

//! Base class of the message in Como protocol.
class Message {
public:
	virtual ~Message() = 0;

	//! \return Code (type) of the message.
	virtual quint16 type() const = 0;

	//! Serialize message.
	virtual QSharedPointer< QByteArray > serialize() const = 0;

	//! Deserialize message.
	virtual bool deserialize( const QByteArray & data ) = 0;
}; // class Message


//
// GetListOfSourcesMessage
//

/*!
	If ServerSocket recives this type of message then
	he send out list of all available sources. Each source
	will send out in separate messages.
*/
class GetListOfSourcesMessage
	:	public Message
{
public:
	//! Type of the  message.
	static const quint16 messageType = 0x0001;

	virtual ~GetListOfSourcesMessage();

	//! \return Code (type) of the message.
	virtual quint16 type() const;

	//! Serialize message.
	virtual QSharedPointer< QByteArray > serialize() const;

	//! Deserialize message.
	virtual bool deserialize( const QByteArray & data );
}; // class GetListOfSourcesMessage


//
// SourceMessage
//

/*!
	This is response to the GetListOfSourcesMessage
	message. And this type of messages used to inform
	ClientSocket about changes in the value of the source.
*/
class SourceMessage
	:	public Message
{
public:
	//! Type of the  message.
	static const quint16 messageType = 0x0002;

	SourceMessage();
	explicit SourceMessage( const Source & s );

	virtual ~SourceMessage();

	//! \return Source.
	const Source & source() const;

	//! \return Code (type) of the message.
	virtual quint16 type() const;

	//! Serialize message.
	virtual QSharedPointer< QByteArray > serialize() const;

	//! Deserialize message.
	virtual bool deserialize( const QByteArray & data );

private:
	//! Source.
	Source m_source;
}; // class SourceMessage


//
// DeinitSourceMessage
//

/*!
	This is message about deinitialization of the source.
*/
class DeinitSourceMessage
	:	public Message
{
public:
	//! Type of the  message.
	static const quint16 messageType = 0x0003;

	DeinitSourceMessage();
	explicit DeinitSourceMessage( const Source & s );

	virtual ~DeinitSourceMessage();

	//! \return Source.
	const Source & source() const;

	//! \return Code (type) of the message.
	virtual quint16 type() const;

	//! Serialize message.
	virtual QSharedPointer< QByteArray > serialize() const;

	//! Deserialize message.
	virtual bool deserialize( const QByteArray & data );

private:
	//! Source.
	Source m_source;
}; // class DeinitSourceMessage

} /* namespace Como */

#endif // COMO__MESSAGES_HPP__INCLUDED
