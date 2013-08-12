/****************************************************************************
** Meta object code from reading C++ file 'client_socket.hpp'
**
** Created: Thu 23. Aug 21:41:50 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "client_socket.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'client_socket.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Como__ClientSocket[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x05,
      56,   19,   19,   19, 0x05,
      90,   19,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
     124,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Como__ClientSocket[] = {
    "Como::ClientSocket\0\0"
    "sourceHasUpdatedValue(Como::Source)\0"
    "getListOfSourcesMessageReceived()\0"
    "sourceDeinitialized(Como::Source)\0"
    "slotReadyRead()\0"
};

void Como::ClientSocket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ClientSocket *_t = static_cast<ClientSocket *>(_o);
        switch (_id) {
        case 0: _t->sourceHasUpdatedValue((*reinterpret_cast< const Como::Source(*)>(_a[1]))); break;
        case 1: _t->getListOfSourcesMessageReceived(); break;
        case 2: _t->sourceDeinitialized((*reinterpret_cast< const Como::Source(*)>(_a[1]))); break;
        case 3: _t->slotReadyRead(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Como::ClientSocket::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Como::ClientSocket::staticMetaObject = {
    { &QTcpSocket::staticMetaObject, qt_meta_stringdata_Como__ClientSocket,
      qt_meta_data_Como__ClientSocket, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Como::ClientSocket::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Como::ClientSocket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Como::ClientSocket::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Como__ClientSocket))
        return static_cast<void*>(const_cast< ClientSocket*>(this));
    return QTcpSocket::qt_metacast(_clname);
}

int Como::ClientSocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpSocket::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Como::ClientSocket::sourceHasUpdatedValue(const Como::Source & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Como::ClientSocket::getListOfSourcesMessageReceived()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void Como::ClientSocket::sourceDeinitialized(const Como::Source & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
