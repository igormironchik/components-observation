
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

// Como include.
#include <Como/boost_protobuf/bp_server_socket.hpp>
#include <Como/boost_protobuf/bp_source.hpp>

// boost include.
#include <boost/asio.hpp>
#include <boost/date_time.hpp>

// C++ include.
#include <thread>
#include <chrono>


using namespace Como::BoostProtobuf;
using namespace boost::asio::ip;


//
// Sources
//

class Sources {
public:
	Sources( ServerSocket * server )
		:	m_intSource( Source::Int, "test::int.source",
				"test::int.source", 0, "this is integer source",
				server )
		,	m_doubleSource( Source::Double, "test::double.source",
				"test::double.source", 0.0, "this is double source",
				server )
		,	m_dateTimeSource( Source::DateTime, "test::date.time.source",
				"test::date.time.source",
				boost::posix_time::second_clock::local_time(),
				"this is date & time source",
				server )
	{
	}

	void start()
	{
		while( true )
		{
			try {
				m_intSource.setValue(
					boost::any_cast< int > ( m_intSource.value() ) + 1 );

				m_doubleSource.setValue(
					boost::any_cast< double > ( m_doubleSource.value() ) + 1.1 );

				m_dateTimeSource.setValue(
					boost::posix_time::second_clock::local_time() );

				std::this_thread::sleep_for( std::chrono::milliseconds( 300 ) );
			}
			catch( const boost::bad_any_cast & )
			{
				m_intSource.setValue( 0 );
				m_doubleSource.setValue( 0.0 );
			}
		}
	}

private:
	//! Integer Como source.
	Source m_intSource;
	//! Double Como source.
	Source m_doubleSource;
	//! DateTime Como source.
	Source m_dateTimeSource;
}; // class Sources


//
// runSources
//

void runSources( ServerSocket * server )
{
	Sources sources( server );

	sources.start();
} // runSources


int main( int, char ** )
{
	boost::asio::io_service service;
	ServerSocket server( service, tcp::endpoint( tcp::v4(), 4545 ) );

	std::thread t( runSources, &server );

	service.run();

	return 0;
}
