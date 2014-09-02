
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2014 Igor Mironchik

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

#ifdef COMO_BOOST_PROTOBUF

// Como include.
#include <Como/boost_protobuf/bp_source.hpp>
#include <Como/boost_protobuf/bp_server_socket.hpp>


namespace Como {

namespace BoostProtobuf {

//
// Source
//

Source::Source()
	:	m_type( Int )
	,	m_value( (int) 0 )
	,	m_serverSocket( nullptr )
	,	m_dateTime( boost::posix_time::second_clock::local_time() )
{
}

Source::Source( Type type, const std::string & name,
	const std::string & typeName,
	const boost::any & value,
	const std::string & desc,
	ServerSocket * serverSocket )
	:	m_type( type )
	,	m_name( name )
	,	m_typeName( typeName )
	,	m_serverSocket( serverSocket )
	,	m_dateTime( boost::posix_time::second_clock::local_time() )
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
	,	m_serverSocket( nullptr )
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
		m_serverSocket = nullptr;
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

const boost::any &
Source::value() const
{
	return m_value;
}


void
Source::setValue( const boost::any & v )
{
	m_value = v;

	m_dateTime = boost::posix_time::second_clock::local_time();

	if( m_serverSocket )
		m_serverSocket->updateSource( *this );
}

const boost::posix_time::ptime &
Source::dateTime() const
{
	return m_dateTime;
}

void
Source::setDateTime( const boost::posix_time::ptime & dt )
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

const std::string &
Source::name() const
{
	return m_name;
}

void
Source::setName( const std::string & name )
{
	m_name = name;
}

const std::string &
Source::typeName() const
{
	return m_typeName;
}

void
Source::setTypeName( const std::string & typeName )
{
	m_typeName = typeName;
}

const std::string &
Source::description() const
{
	return m_desc;
}

void
Source::setDescription( const std::string & desc )
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

} /* namespace BoostProtobuf */

} /* namespace Como */

#endif // COMO_BOOST_PROTOBUF
