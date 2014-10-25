
TEMPLATE = app
TARGET = Boost.Protobuf.Server
DESTDIR = ../..
DEPENDPATH += . ../..
INCLUDEPATH += . ../..
CONFIG += console
CONFIG -= qt
LIBS += -L../../lib -lComo -llibprotobuf

SOURCES += main.cpp

exists( ../../como_defines.pri ) {
    include( ../../como_defines.pri )
}
