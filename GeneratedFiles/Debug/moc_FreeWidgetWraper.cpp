/****************************************************************************
** Meta object code from reading C++ file 'FreeWidgetWraper.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../FreeWidgetWraper.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FreeWidgetWraper.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FreeWidgetWraper_t {
    QByteArrayData data[13];
    char stringdata0[146];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FreeWidgetWraper_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FreeWidgetWraper_t qt_meta_stringdata_FreeWidgetWraper = {
    {
QT_MOC_LITERAL(0, 0, 16), // "FreeWidgetWraper"
QT_MOC_LITERAL(1, 17, 10), // "setPadding"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 7), // "padding"
QT_MOC_LITERAL(4, 37, 13), // "setMoveEnable"
QT_MOC_LITERAL(5, 51, 10), // "moveEnable"
QT_MOC_LITERAL(6, 62, 15), // "setResizeEnable"
QT_MOC_LITERAL(7, 78, 12), // "resizeEnable"
QT_MOC_LITERAL(8, 91, 15), // "setMousePressed"
QT_MOC_LITERAL(9, 107, 12), // "mousePressed"
QT_MOC_LITERAL(10, 120, 9), // "setWidget"
QT_MOC_LITERAL(11, 130, 8), // "QWidget*"
QT_MOC_LITERAL(12, 139, 6) // "widget"

    },
    "FreeWidgetWraper\0setPadding\0\0padding\0"
    "setMoveEnable\0moveEnable\0setResizeEnable\0"
    "resizeEnable\0setMousePressed\0mousePressed\0"
    "setWidget\0QWidget*\0widget"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FreeWidgetWraper[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x0a /* Public */,
       4,    1,   42,    2, 0x0a /* Public */,
       6,    1,   45,    2, 0x0a /* Public */,
       8,    1,   48,    2, 0x0a /* Public */,
      10,    1,   51,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, 0x80000000 | 11,   12,

       0        // eod
};

void FreeWidgetWraper::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FreeWidgetWraper *_t = static_cast<FreeWidgetWraper *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setPadding((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setMoveEnable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->setResizeEnable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->setMousePressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->setWidget((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject FreeWidgetWraper::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FreeWidgetWraper.data,
      qt_meta_data_FreeWidgetWraper,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *FreeWidgetWraper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FreeWidgetWraper::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FreeWidgetWraper.stringdata0))
        return static_cast<void*>(const_cast< FreeWidgetWraper*>(this));
    return QObject::qt_metacast(_clname);
}

int FreeWidgetWraper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
