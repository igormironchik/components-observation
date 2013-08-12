require 'rubygems'

gem 'Mxx_ru', '>= 1.4.7'

require 'mxx_ru/cpp/qt4'

Mxx_ru::Cpp::exe_target {

	rtl_mode( Mxx_ru::Cpp::RTL_SHARED )
	rtti_mode( Mxx_ru::Cpp::RTTI_ENABLED )
	threading_mode( Mxx_ru::Cpp::THREADING_MULTI )

	screen_mode( Mxx_ru::Cpp::SCREEN_WINDOW )

	required_prj( "Como/prj.rb" )

	qt = generator( MxxRu::Cpp::Qt4.new( self ) )
	qt.use_modules( QT_NETWORK, QT_GUI, QT_CORE )

	target( "QTreeViewClient" )

	if 'mswin' == toolset.tag( 'target_os' )
		lib( "qtmain" )
	end

	qt.h2moc( "treeview.hpp" )
	qt.h2moc( "mainwindow.hpp" )
	qt.h2moc( "model.hpp" )

	cpp_source( "treeview.cpp" )
	cpp_source( "mainwindow.cpp" )
	cpp_source( "model.cpp" )
	cpp_source( "main.cpp" )
}
