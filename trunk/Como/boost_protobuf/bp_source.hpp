
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

#ifndef COMO__BOOSTPROTOBUF__BP_SOURCE_HPP__INCLUDED
#define COMO__BOOSTPROTOBUF__BP_SOURCE_HPP__INCLUDED

// boost include.
#include <boost/any.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

// C++ include.
#include <string>


namespace Como {

namespace BoostProtobuf {

class ServerSocket;


//
// Source
//

/*!
	This is the main class, which provides the distribution of information
	on the status of an application component. An instance of this class
	must be present at each location where monitoring is required.
*/
class Source
{
public:
	//! Type of the source.
	enum Type {
		//! Source with string value.
		String = 0x01,
		//! Source with integer value.
		Int = 0x02,
		//! Source with unsigned integer value.
		UInt = 0x03,
		//! Source with long long value.
		LongLong = 0x04,
		//! Source with unsigned long long value.
		ULongLong = 0x05,
		//! Source with double value.
		Double = 0x06,
		//! Source with date and time.
		DateTime = 0x07,
		//! Source with time.
		Time = 0x08
	}; /* enum Type */

	//! Type of the source will be Int.
	Source();

	Source(
		//! Type of the source.
		Type type,
		//! Name of the source.
		const std::string & name,
		//! Name of the type of the source.
		const std::string & typeName,
		//! Initial value of the source.
		const boost::any & value,
		//! Description of the source.
		const std::string & desc,
		/*!
			Server socket. If not null then source will
			send out the information about itself.
		*/
		ServerSocket * serverSocket = 0 );

	virtual ~Source();

	/*!
		Be careful, m_serverSocket will not copied.
	*/
	Source( const Source & other );

	/*!
		Be careful, m_serverSocket will not copied.
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
	const boost::any & value() const;
	/*!
		Set value of the source. If serverSocket was defined in the
		constructor then information will send out.

		m_dateTime updates automatically to current system date and time.
	*/
	void setValue( const boost::any & v );

	//! \return Time of the update.
	const boost::posix_time::ptime & dateTime() const;
	//! Set date and time.
	void setDateTime( const boost::posix_time::ptime & dt );

	//! \return Type of the source.
	Type type() const;
	//! Set type of the source.
	void setType( Type t );

	//! \return Name of the source.
	const std::string & name() const;
	//! Set name of the source.
	void setName( const std::string & name );

	//! \return Type name of the source.
	const std::string & typeName() const;
	//! Set type name of the source.
	void setTypeName( const std::string & typeName );

	//! \return Description of the source.
	const std::string & description() const;
	//! Set description of the source.
	void setDescription( const std::string & desc );

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
	std::string m_name;
	//! Name of the type of the source.
	std::string m_typeName;
	//! Server socket.
	ServerSocket * m_serverSocket;
	//! Date and time of the update.
	boost::posix_time::ptime m_dateTime;
	//! Description of the source.
	std::string m_desc;
	//! Value of the source.
	boost::any m_value;
}; /* class Source */

} /* namespace BoostProtobuf */

} /* namespace Como */

#endif // COMO__BOOSTPROTOBUF__BP_SOURCE_HPP__INCLUDED

#endif // COMO_BOOST_PROTOBUF
