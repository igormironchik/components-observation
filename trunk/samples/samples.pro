
TEMPLATE = subdirs


SUBDIRS = server \
          qtreeview_client
          
contains( DEFINES, COMO_BOOST_PROTOBUF ) {
    SUBDIRS += boost_protobuf_server
}
