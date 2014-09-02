
TEMPLATE = subdirs

SUBDIRS = server \
          qtreeview_client \
          
exists( ../como_defines.pri ) {
    include( ../como_defines.pri )
}

contains( DEFINES, COMO_BOOST_PROTOBUF ) {
    SUBDIRS += boost_protobuf_server
}
