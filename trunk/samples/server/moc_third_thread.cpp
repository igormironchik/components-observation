/****************************************************************************
** Meta object code from reading C++ file 'third_thread.hpp'
**
** Created: Thu 23. Aug 21:42:08 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "third_thread.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'third_thread.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ThirdObject[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      36,   12,   12,   12, 0x08,
      59,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ThirdObject[] = {
    "ThirdObject\0\0slotSwitchToStateOne()\0"
    "slotSwitchToStateTwo()\0slotSwitchToStateThree()\0"
};

void ThirdObject::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ThirdObject *_t = static_cast<ThirdObject *>(_o);
        switch (_id) {
        case 0: _t->slotSwitchToStateOne(); break;
        case 1: _t->slotSwitchToStateTwo(); break;
        case 2: _t->slotSwitchToStateThree(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData ThirdObject::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ThirdObject::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ThirdObject,
      qt_meta_data_ThirdObject, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ThirdObject::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ThirdObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ThirdObject::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ThirdObject))
        return static_cast<void*>(const_cast< ThirdObject*>(this));
    return QObject::qt_metacast(_clname);
}

int ThirdObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
