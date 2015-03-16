
TEMPLATE = subdirs

SUBDIRS = Como \
          samples

samples.depends = Como

libDir.commands = mkdir lib

QMAKE_EXTRA_TARGETS += libDir
