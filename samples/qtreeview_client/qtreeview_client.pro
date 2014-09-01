
TEMPLATE = app
TARGET = QTreeViewClient
DESTDIR = ../..
DEPENDPATH += . ../..
INCLUDEPATH += . ../..
CONFIG += windows
QT += network gui widgets
LIBS += -L../../lib -lComo -llibprotobuf
DEFINES += COMO_BOOST_PROTOBUF

HEADERS += treeview.hpp \
           mainwindow.hpp \
           model.hpp

SOURCES += treeview.cpp \
           mainwindow.cpp \
           model.cpp \
           main.cpp
