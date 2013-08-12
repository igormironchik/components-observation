
TEMPLATE = app
TARGET = QTreeViewClient
DESTDIR = ../..
DEPENDPATH += . ../..
INCLUDEPATH += . ../..
CONFIG += windows qt
QT += network gui
LIBS += -L../../lib -lComo

HEADERS += treeview.hpp \
           mainwindow.hpp \
           model.hpp

SOURCES += treeview.cpp \
           mainwindow.cpp \
           model.cpp \
           main.cpp
