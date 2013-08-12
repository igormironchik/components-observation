require 'rubygems'

gem 'Mxx_ru', '>= 1.4.7'

require 'mxx_ru/cpp'

MxxRu::Cpp::composite_target() {
	required_prj( "samples/server/prj.rb" )
	required_prj( "samples/qtreeview_client/prj.rb" )
}
