
TEMPLATE = lib
TARGET = Como
DESTDIR = ../lib
DEPENDPATH += . private
INCLUDEPATH += . ..
CONFIG += staticlib
QT -= gui
QT += network

HEADERS += client_socket.hpp \
           server_socket.hpp \
           source.hpp \
           private/buffer.hpp \
           private/messages.hpp \
           private/protocol.hpp

SOURCES += client_socket.cpp \
           server_socket.cpp \
           source.cpp \
           private/buffer.cpp \
           private/messages.cpp \
           private/protocol.cpp

contains( DEFINES, COMO_BOOST_PROTOBUF ) {
    include( boost_protobuf/boost_protobuf.pri )
}
