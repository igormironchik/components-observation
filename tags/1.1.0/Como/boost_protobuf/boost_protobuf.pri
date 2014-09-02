
DEFINES += COMO_BOOST_PROTOBUF

HEADERS += $$PWD/bp_client_socket.hpp \
		   $$PWD/bp_server_socket.hpp \
		   $$PWD/bp_source.hpp

SOURCES += $$PWD/bp_client_socket.cpp \
		   $$PWD/bp_server_socket.cpp \
		   $$PWD/bp_source.cpp

OTHER_FILES += $$PWD/messages.proto

PRE_TARGETDEPS += $$PWD/messages.pb.cc $$PWD/messages.pb.h
QMAKE_EXTRA_TARGETS += protobufc protobufh

protobufc.target = $$PWD/messages.pb.cc
protobufc.depends = FORCE
protobufc.commands = protoc --proto_path=$$PWD --cpp_out=$$PWD $$PWD/messages.proto

protobufh.target = $$PWD/messages.pb.h
protobufh.depends = protobufc

HEADERS += $$PWD/messages.pb.h
SOURCES += $$PWD/messages.pb.cc
