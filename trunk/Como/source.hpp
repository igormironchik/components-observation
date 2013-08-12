
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

#ifndef COMO__SOURCE_HPP__INCLUDED
#define COMO__SOURCE_HPP__INCLUDED

// Qt include.
#include <QtCore/QDateTime>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QObject>
#include <QtCore/QMetaType>


namespace Como {

class ServerSocket;


//
// Source
//

/*!
	This is the main class, which provides the distribution of information
	on the status of an application component. An instance of this class
	must be present at each location where monitoring is required.

	Don't forget register Source metatype information in Qt with:

	\code
	qRegisterMetaType< Como::Source > ( "Como::Source" );
	\endcode
*/
class Source
	:	public QObject
{
	Q_OBJECT

signals:
	//! Emitted when source change his value.
	void valueChanged( const QVariant& );

public:
	//! Type of the source.
	enum Type {
		//! Source with string value.
		String = 0x01,
		//! Source with integer value.
		Int = 0x02,
		//! Source with double value.
		Double = 0x03
	}; /* enum Type */

	//! Type of the source will be Int.
	Source( QObject * parent = 0 );

	Source(
		//! Type of the source.
		Type type,
		//! Name of the source.
		const QString & name,
		//! Name of the type of the source.
		const QString & typeName,
		//! Initial value of the source.
		const QVariant & value,
		//! Description of the source.
		const QString & desc,
		/*!
			Server socket. If not null then source will
			send out the information about itself.
		*/
		ServerSocket * serverSocket = 0,
		//! Parent object.
		QObject * parent = 0 );

	virtual ~Source();

	/*!
		Be careful, m_serverSocket will not copied and
		QObject::parent will set to NULL.
	*/
	Source( const Source & other );

	/*!
		Be careful, m_serverSocket will not copied and
		QObject::parent will not change.
	*/
	Source & operator = ( const Source & other );

	/*!
		Init server socket with this source.

		When this method invoked server socket
		initialize his internal state and send message
		to all clients about new source.
	*/
	virtual void initSource();

	/*!
		Deinit server socket with this source.

		When this method invoked server socket
		deinitialzie his internals state with this
		source and send message to all clients
		about source deinitialization.
	*/
	virtual void deinitSource();

	//! \return Value of the source.
	const QVariant & value() const;
	/*!
		Set value of the source. If serverSocket was defined in the
		constructor then information will send out.

		m_dateTime updates automatically to current system date and time.

		Emit valueChanged( const QVariant& ).
	*/
	void setValue( const QVariant & v );
	/*!
		Set value of the source.

		This function is provided for convenience.

		If type of the source is not String then nothing will happen.
	*/
	void setValue( const QString & v );
	/*!
		Set value of the source.

		This function is provided for convenience.

		If type of the source is not Int then nothing will happen.
	*/
	void setValue( int v );
	/*!
		Set value of the source.

		This function is provided for convenience.

		If type of the source is not Double then nothing will happen.
	*/
	void setValue( double v );

	//! \return Time of the update.
	const QDateTime & dateTime() const;
	//! Set date and time.
	void setDateTime( const QDateTime & dt );

	//! \return Type of the source.
	Type type() const;
	//! Set type of the source.
	void setType( Type t );

	//! \return Name of the source.
	const QString & name() const;
	//! Set name of the source.
	void setName( const QString & name );

	//! \return Type name of the source.
	const QString & typeName() const;
	//! Set type name of the source.
	void setTypeName( const QString & typeName );

	//! \return Description of the source.
	const QString & description() const;
	//! Set description of the source.
	void setDescription( const QString & desc );

	//! \return Server socket.
	ServerSocket * serverSocket() const;
	/*!
		Set server socket.

		If server socket was defined before this operation then
		this source will be deinitialized with previous server socket
		and initialized with new one.
	*/
	void setServerSocket( ServerSocket * socket );

	//! Equality operator.
	friend bool operator == ( const Source & s1, const Source & s2 );
	//! Not-equality operator.
	friend bool operator != ( const Source & s1, const Source & s2 );

private:
	//! Type of the source.
	Type m_type;
	//! Name of the source.
	QString m_name;
	//! Name of the type of the source.
	QString m_typeName;
	//! Server socket.
	ServerSocket * m_serverSocket;
	//! Date and time of the update.
	QDateTime m_dateTime;
	//! Description of the source.
	QString m_desc;
	//! Value of the source.
	QVariant m_value;
}; /* class Source */

} /* namespace Como */

Q_DECLARE_METATYPE( Como::Source )

#endif // COMO__SOURCE_HPP__INCLUDED
