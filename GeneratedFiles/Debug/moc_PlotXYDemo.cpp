/****************************************************************************
** Meta object code from reading C++ file 'PlotXYDemo.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../PlotXYDemo.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PlotXYDemo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PlotXYDemo_t {
    QByteArrayData data[16];
    char stringdata0[220];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PlotXYDemo_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PlotXYDemo_t qt_meta_stringdata_PlotXYDemo = {
    {
QT_MOC_LITERAL(0, 0, 10), // "PlotXYDemo"
QT_MOC_LITERAL(1, 11, 14), // "onAdvancedData"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 13), // "onPlotManager"
QT_MOC_LITERAL(4, 41, 13), // "onAddPlotPair"
QT_MOC_LITERAL(5, 55, 10), // "onOpenFile"
QT_MOC_LITERAL(6, 66, 28), // "onCustomContextMenuRequested"
QT_MOC_LITERAL(7, 95, 13), // "onContextMenu"
QT_MOC_LITERAL(8, 109, 5), // "point"
QT_MOC_LITERAL(9, 115, 12), // "onAddTabPage"
QT_MOC_LITERAL(10, 128, 15), // "onRemoveTabPage"
QT_MOC_LITERAL(11, 144, 15), // "onRenameTabPage"
QT_MOC_LITERAL(12, 160, 12), // "onAddBarPlot"
QT_MOC_LITERAL(13, 173, 17), // "onAddAttitudePlot"
QT_MOC_LITERAL(14, 191, 13), // "onAddTextPlot"
QT_MOC_LITERAL(15, 205, 14) // "onAddPolarPlot"

    },
    "PlotXYDemo\0onAdvancedData\0\0onPlotManager\0"
    "onAddPlotPair\0onOpenFile\0"
    "onCustomContextMenuRequested\0onContextMenu\0"
    "point\0onAddTabPage\0onRemoveTabPage\0"
    "onRenameTabPage\0onAddBarPlot\0"
    "onAddAttitudePlot\0onAddTextPlot\0"
    "onAddPolarPlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PlotXYDemo[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x0a /* Public */,
       3,    0,   80,    2, 0x0a /* Public */,
       4,    0,   81,    2, 0x0a /* Public */,
       5,    0,   82,    2, 0x0a /* Public */,
       6,    1,   83,    2, 0x0a /* Public */,
       7,    1,   86,    2, 0x0a /* Public */,
       9,    0,   89,    2, 0x0a /* Public */,
      10,    0,   90,    2, 0x0a /* Public */,
      11,    0,   91,    2, 0x0a /* Public */,
      12,    0,   92,    2, 0x0a /* Public */,
      13,    0,   93,    2, 0x0a /* Public */,
      14,    0,   94,    2, 0x0a /* Public */,
      15,    0,   95,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,    2,
    QMetaType::Void, QMetaType::QPoint,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void PlotXYDemo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PlotXYDemo *_t = static_cast<PlotXYDemo *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onAdvancedData(); break;
        case 1: _t->onPlotManager(); break;
        case 2: _t->onAddPlotPair(); break;
        case 3: _t->onOpenFile(); break;
        case 4: _t->onCustomContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 5: _t->onContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 6: _t->onAddTabPage(); break;
        case 7: _t->onRemoveTabPage(); break;
        case 8: _t->onRenameTabPage(); break;
        case 9: _t->onAddBarPlot(); break;
        case 10: _t->onAddAttitudePlot(); break;
        case 11: _t->onAddTextPlot(); break;
        case 12: _t->onAddPolarPlot(); break;
        default: ;
        }
    }
}

const QMetaObject PlotXYDemo::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_PlotXYDemo.data,
      qt_meta_data_PlotXYDemo,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *PlotXYDemo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PlotXYDemo::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PlotXYDemo.stringdata0))
        return static_cast<void*>(const_cast< PlotXYDemo*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int PlotXYDemo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
