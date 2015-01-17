
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
#include <Como/Source>
#include <Como/ServerSocket>


namespace Como {

//
// Source
//

Source::Source( QObject * parent )
	:	QObject( parent )
	,	m_type( Int )
	,	m_value( QVariant( (int) 0 ) )
	,	m_serverSocket( 0 )
	,	m_dateTime( QDateTime::currentDateTime() )
{
}

Source::Source( Type type, const QString & name,
	const QString & typeName,
	const QVariant & value,
	const QString & desc,
	ServerSocket * serverSocket,
	QObject * parent )
	:	QObject( parent )
	,	m_type( type )
	,	m_name( name )
	,	m_typeName( typeName )
	,	m_serverSocket( serverSocket )
	,	m_dateTime( QDateTime::currentDateTime() )
	,	m_desc( desc )
	,	m_value( value )
{
	initSource();
}

Source::~Source()
{
	deinitSource();
}

Source::Source( const Source & other )
	:	m_type( other.type() )
	,	m_name( other.name() )
	,	m_typeName( other.typeName() )
	,	m_serverSocket( 0 )
	,	m_dateTime( other.dateTime() )
	,	m_desc( other.description() )
	,	m_value( other.value() )
{
}

Source &
Source::operator = ( const Source & other )
{
	if( this != &other )
	{
		m_type = other.type();
		m_name = other.name();
		m_typeName = other.typeName();
		m_serverSocket = 0;
		m_dateTime = other.dateTime();
		m_desc = other.description();
		m_value = other.value();
	}

	return *this;
}

void
Source::initSource()
{
	if( m_serverSocket )
		m_serverSocket->initSource( *this );
}

void
Source::deinitSource()
{
	if( m_serverSocket )
		m_serverSocket->deinitSource( *this );
}

const QVariant &
Source::value() const
{
	return m_value;
}


void
Source::setValue( const QVariant & v )
{
	m_value = v;

	m_dateTime = QDateTime::currentDateTime();

	if( m_serverSocket )
		m_serverSocket->updateSource( *this );

	emit valueChanged( m_value );
}

const QDateTime &
Source::dateTime() const
{
	return m_dateTime;
}

void
Source::setDateTime( const QDateTime & dt )
{
	m_dateTime = dt;

	if( m_serverSocket )
		m_serverSocket->updateSource( *this );
}

Source::Type
Source::type() const
{
	return m_type;
}

void
Source::setType( Type t )
{
	m_type = t;
}

const QString &
Source::name() const
{
	return m_name;
}

void
Source::setName( const QString & name )
{
	m_name = name;
}

const QString &
Source::typeName() const
{
	return m_typeName;
}

void
Source::setTypeName( const QString & typeName )
{
	m_typeName = typeName;
}

const QString &
Source::description() const
{
	return m_desc;
}

void
Source::setDescription( const QString & desc )
{
	m_desc = desc;
}

ServerSocket *
Source::serverSocket() const
{
	return m_serverSocket;
}

void
Source::setServerSocket( ServerSocket * socket )
{
	deinitSource();

	m_serverSocket = socket;

	initSource();
}

bool operator == ( const Source & s1, const Source & s2 )
{
	return ( s1.name() == s2.name() &&
		s1.typeName() == s2.typeName() );
}

bool operator != ( const Source & s1, const Source & s2 )
{
	return ( s1.name() != s2.name() ||
		s1.typeName() != s2.typeName() );
}

} /* namespace Como */
