/****************************************************************************
** Meta object code from reading C++ file 'PlotManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../PlotManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PlotManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PlotManager_t {
    QByteArrayData data[16];
    char stringdata0[235];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PlotManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PlotManager_t qt_meta_stringdata_PlotManager = {
    {
QT_MOC_LITERAL(0, 0, 11), // "PlotManager"
QT_MOC_LITERAL(1, 12, 14), // "sigAddPlotPair"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 12), // "onTWSclicked"
QT_MOC_LITERAL(4, 41, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(5, 58, 4), // "item"
QT_MOC_LITERAL(6, 63, 1), // "i"
QT_MOC_LITERAL(7, 65, 13), // "onTWSPclicked"
QT_MOC_LITERAL(8, 79, 15), // "onAddNewClicked"
QT_MOC_LITERAL(9, 95, 21), // "onBtnAxisColorClicked"
QT_MOC_LITERAL(10, 117, 21), // "onBtnGridColorClicked"
QT_MOC_LITERAL(11, 139, 20), // "onBtnGridFillClicked"
QT_MOC_LITERAL(12, 160, 21), // "spinboxBetweenChanged"
QT_MOC_LITERAL(13, 182, 18), // "spinboxLeftChanged"
QT_MOC_LITERAL(14, 201, 19), // "spinboxRightChanged"
QT_MOC_LITERAL(15, 221, 13) // "onAddPlotPair"

    },
    "PlotManager\0sigAddPlotPair\0\0onTWSclicked\0"
    "QTreeWidgetItem*\0item\0i\0onTWSPclicked\0"
    "onAddNewClicked\0onBtnAxisColorClicked\0"
    "onBtnGridColorClicked\0onBtnGridFillClicked\0"
    "spinboxBetweenChanged\0spinboxLeftChanged\0"
    "spinboxRightChanged\0onAddPlotPair"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PlotManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    2,   70,    2, 0x0a /* Public */,
       7,    2,   75,    2, 0x0a /* Public */,
       8,    0,   80,    2, 0x0a /* Public */,
       9,    0,   81,    2, 0x0a /* Public */,
      10,    0,   82,    2, 0x0a /* Public */,
      11,    0,   83,    2, 0x0a /* Public */,
      12,    0,   84,    2, 0x0a /* Public */,
      13,    0,   85,    2, 0x0a /* Public */,
      14,    0,   86,    2, 0x0a /* Public */,
      15,    2,   87,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4, QMetaType::Int,    5,    6,
    QMetaType::Void, 0x80000000 | 4, QMetaType::Int,    5,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,

       0        // eod
};

void PlotManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PlotManager *_t = static_cast<PlotManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigAddPlotPair(); break;
        case 1: _t->onTWSclicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->onTWSPclicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->onAddNewClicked(); break;
        case 4: _t->onBtnAxisColorClicked(); break;
        case 5: _t->onBtnGridColorClicked(); break;
        case 6: _t->onBtnGridFillClicked(); break;
        case 7: _t->spinboxBetweenChanged(); break;
        case 8: _t->spinboxLeftChanged(); break;
        case 9: _t->spinboxRightChanged(); break;
        case 10: _t->onAddPlotPair((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (PlotManager::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PlotManager::sigAddPlotPair)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject PlotManager::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PlotManager.data,
      qt_meta_data_PlotManager,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *PlotManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PlotManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PlotManager.stringdata0))
        return static_cast<void*>(const_cast< PlotManager*>(this));
    return QWidget::qt_metacast(_clname);
}

int PlotManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void PlotManager::sigAddPlotPair()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
