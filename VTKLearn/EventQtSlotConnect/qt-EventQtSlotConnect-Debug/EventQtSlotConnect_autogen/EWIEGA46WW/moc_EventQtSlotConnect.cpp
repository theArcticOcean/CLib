/****************************************************************************
** Meta object code from reading C++ file 'EventQtSlotConnect.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../EventQtSlotConnect.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EventQtSlotConnect.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_EventQtSlotConnect_t {
    QByteArrayData data[5];
    char stringdata0[62];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EventQtSlotConnect_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EventQtSlotConnect_t qt_meta_stringdata_EventQtSlotConnect = {
    {
QT_MOC_LITERAL(0, 0, 18), // "EventQtSlotConnect"
QT_MOC_LITERAL(1, 19, 12), // "slot_clicked"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 10), // "vtkObject*"
QT_MOC_LITERAL(4, 44, 17) // "slot_rightClicked"

    },
    "EventQtSlotConnect\0slot_clicked\0\0"
    "vtkObject*\0slot_rightClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EventQtSlotConnect[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    4,   24,    2, 0x0a /* Public */,
       4,    0,   33,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::ULong, QMetaType::VoidStar, QMetaType::VoidStar,    2,    2,    2,    2,
    QMetaType::Void,

       0        // eod
};

void EventQtSlotConnect::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        EventQtSlotConnect *_t = static_cast<EventQtSlotConnect *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slot_clicked((*reinterpret_cast< vtkObject*(*)>(_a[1])),(*reinterpret_cast< ulong(*)>(_a[2])),(*reinterpret_cast< void*(*)>(_a[3])),(*reinterpret_cast< void*(*)>(_a[4]))); break;
        case 1: _t->slot_rightClicked(); break;
        default: ;
        }
    }
}

const QMetaObject EventQtSlotConnect::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_EventQtSlotConnect.data,
      qt_meta_data_EventQtSlotConnect,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *EventQtSlotConnect::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EventQtSlotConnect::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_EventQtSlotConnect.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int EventQtSlotConnect::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
