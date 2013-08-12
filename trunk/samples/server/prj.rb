require 'rubygems'

gem 'Mxx_ru', '>= 1.4.7'

require 'mxx_ru/cpp/qt4'

Mxx_ru::Cpp::exe_target {

	rtl_mode( Mxx_ru::Cpp::RTL_SHARED )
	rtti_mode( Mxx_ru::Cpp::RTTI_ENABLED )
	threading_mode( Mxx_ru::Cpp::THREADING_MULTI )

	required_prj( "Como/prj.rb" )

	qt = generator( MxxRu::Cpp::Qt4.new( self ) )
	qt.use_modules( QT_CORE, QT_NETWORK )

	target( "Server" )

	qt.h2moc( "first_thread.hpp" )
	qt.h2moc( "second_thread.hpp" )
	qt.h2moc( "third_thread.hpp" )

	cpp_source( "first_thread.cpp" )
	cpp_source( "second_thread.cpp" )
	cpp_source( "third_thread.cpp" )
	cpp_source( "main.cpp" )
}
