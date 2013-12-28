
require 'mxx_ru/cpp/qt5'

Mxx_ru::Cpp::lib_target {

	rtl_mode( Mxx_ru::Cpp::RTL_SHARED )
	rtti_mode( Mxx_ru::Cpp::RTTI_ENABLED )
	threading_mode( Mxx_ru::Cpp::THREADING_MULTI )

	target( "lib/Como" )
	
	qt = generator( MxxRu::Cpp::Qt5.new( self ) )
	qt.use_modules( QT_CORE, QT_NETWORK )

	sources_root( "private" ) {
		cpp_source( "buffer.cpp" )
		cpp_source( "messages.cpp" )
		cpp_source( "protocol.cpp" )
	}
	
	qt.h2moc( "client_socket.hpp" )
	qt.h2moc( "server_socket.hpp" )
	qt.h2moc( "source.hpp" )

	cpp_source( "client_socket.cpp" )
	cpp_source( "server_socket.cpp" )
	cpp_source( "source.cpp" )
}
