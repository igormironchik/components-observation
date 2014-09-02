
TEMPLATE = app
TARGET = Boost.Protobuf.Server
DESTDIR = ../..
DEPENDPATH += . ../..
INCLUDEPATH += . ../..
CONFIG += console
CONFIG -= qt
LIBS += -L../../lib -lComo -llibprotobuf

DEFINES += COMO_BOOST_PROTOBUF

SOURCES += main.cpp
