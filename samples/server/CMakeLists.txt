
project( server )

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )

find_package( Qt6Core REQUIRED )
find_package( Qt6Network REQUIRED )

set( SRC first_thread.cpp
    first_thread.hpp
    main.cpp
    second_thread.cpp
    second_thread.hpp
    third_thread.cpp
    third_thread.hpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../.. )

add_executable( Como.Server.Example ${SRC} )

add_dependencies( Como.Server.Example Como )

target_link_libraries( Como.Server.Example Como Qt6::Network Qt6::Core )
