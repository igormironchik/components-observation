
project( LibComo )

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )

find_package( Qt6Core REQUIRED )
find_package( Qt6Network REQUIRED )

set( SRC client_socket.cpp
    client_socket.hpp
    server_socket.cpp
    server_socket.hpp
    source.cpp
    source.hpp
    private/buffer.cpp
    private/buffer.hpp
    private/messages.cpp
    private/messages.hpp
    private/protocol.cpp
    private/protocol.hpp )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_library( Como STATIC ${SRC} )

target_link_libraries( Como Qt6::Network Qt6::Core )
