
project( qtreeview_client )

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )

find_package( Qt6Core REQUIRED )
find_package( Qt6Network REQUIRED )
find_package( Qt6Gui REQUIRED )
find_package( Qt6Widgets REQUIRED )

set( SRC main.cpp
    mainwindow.cpp
    mainwindow.hpp
    model.cpp
    model.hpp
    treeview.cpp
    treeview.hpp )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../.. )

add_executable( Como.Treeview.Example ${SRC} )

add_dependencies( Como.Treeview.Example Como )

target_link_libraries( Como.Treeview.Example Como Qt6::Widgets Qt6::Gui Qt6::Network Qt6::Core )
