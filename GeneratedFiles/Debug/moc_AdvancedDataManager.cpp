/****************************************************************************
** Meta object code from reading C++ file 'AdvancedDataManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../AdvancedDataManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AdvancedDataManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AdvancedDataManager_t {
    QByteArrayData data[12];
    char stringdata0[220];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AdvancedDataManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AdvancedDataManager_t qt_meta_stringdata_AdvancedDataManager = {
    {
QT_MOC_LITERAL(0, 0, 19), // "AdvancedDataManager"
QT_MOC_LITERAL(1, 20, 23), // "updateColorThresholdMap"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 31), // "QMap<QString,QMap<int,QColor> >"
QT_MOC_LITERAL(4, 77, 23), // "onBtnColorChooseClicked"
QT_MOC_LITERAL(5, 101, 9), // "onBtnMore"
QT_MOC_LITERAL(6, 111, 14), // "onBtnColorMore"
QT_MOC_LITERAL(7, 126, 8), // "onBtnAdd"
QT_MOC_LITERAL(8, 135, 17), // "onBtnColorClicked"
QT_MOC_LITERAL(9, 153, 28), // "onAdvancedDataManagerAddPair"
QT_MOC_LITERAL(10, 182, 20), // "onCurrentItemChanged"
QT_MOC_LITERAL(11, 203, 16) // "QTreeWidgetItem*"

    },
    "AdvancedDataManager\0updateColorThresholdMap\0"
    "\0QMap<QString,QMap<int,QColor> >\0"
    "onBtnColorChooseClicked\0onBtnMore\0"
    "onBtnColorMore\0onBtnAdd\0onBtnColorClicked\0"
    "onAdvancedDataManagerAddPair\0"
    "onCurrentItemChanged\0QTreeWidgetItem*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AdvancedDataManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   57,    2, 0x0a /* Public */,
       5,    0,   58,    2, 0x0a /* Public */,
       6,    0,   59,    2, 0x0a /* Public */,
       7,    0,   60,    2, 0x0a /* Public */,
       8,    0,   61,    2, 0x0a /* Public */,
       9,    2,   62,    2, 0x0a /* Public */,
      10,    2,   67,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, 0x80000000 | 11, 0x80000000 | 11,    2,    2,

       0        // eod
};

void AdvancedDataManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AdvancedDataManager *_t = static_cast<AdvancedDataManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateColorThresholdMap((*reinterpret_cast< QMap<QString,QMap<int,QColor> >(*)>(_a[1]))); break;
        case 1: _t->onBtnColorChooseClicked(); break;
        case 2: _t->onBtnMore(); break;
        case 3: _t->onBtnColorMore(); break;
        case 4: _t->onBtnAdd(); break;
        case 5: _t->onBtnColorClicked(); break;
        case 6: _t->onAdvancedDataManagerAddPair((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 7: _t->onCurrentItemChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QTreeWidgetItem*(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AdvancedDataManager::*_t)(QMap<QString,QMap<int,QColor>> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AdvancedDataManager::updateColorThresholdMap)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject AdvancedDataManager::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_AdvancedDataManager.data,
      qt_meta_data_AdvancedDataManager,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *AdvancedDataManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AdvancedDataManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AdvancedDataManager.stringdata0))
        return static_cast<void*>(const_cast< AdvancedDataManager*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int AdvancedDataManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void AdvancedDataManager::updateColorThresholdMap(QMap<QString,QMap<int,QColor>> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
