
TEMPLATE = app
TARGET = Server
DESTDIR = ../..
DEPENDPATH += . ../..
INCLUDEPATH += . ../..
CONFIG += console qt
QT += network
QT -= gui
LIBS += -L../../lib -lComo -llibprotobuf

HEADERS += first_thread.hpp \
           second_thread.hpp \
           third_thread.hpp

SOURCES += first_thread.cpp \
           second_thread.cpp \
           third_thread.cpp \
           main.cpp
           
exists( ../../como_defines.pri ) {
    include( ../../como_defines.pri )
}
