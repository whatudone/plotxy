/****************************************************************************
** Meta object code from reading C++ file 'SubSettingWidgetContainer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../SubSettingWidgetContainer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SubSettingWidgetContainer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SubSettingWidgetContainer_t {
    QByteArrayData data[1];
    char stringdata0[26];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SubSettingWidgetContainer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SubSettingWidgetContainer_t qt_meta_stringdata_SubSettingWidgetContainer = {
    {
QT_MOC_LITERAL(0, 0, 25) // "SubSettingWidgetContainer"

    },
    "SubSettingWidgetContainer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SubSettingWidgetContainer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void SubSettingWidgetContainer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject SubSettingWidgetContainer::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_SubSettingWidgetContainer.data,
      qt_meta_data_SubSettingWidgetContainer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SubSettingWidgetContainer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SubSettingWidgetContainer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SubSettingWidgetContainer.stringdata0))
        return static_cast<void*>(const_cast< SubSettingWidgetContainer*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int SubSettingWidgetContainer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_General_t {
    QByteArrayData data[4];
    char stringdata0[59];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_General_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_General_t qt_meta_stringdata_General = {
    {
QT_MOC_LITERAL(0, 0, 7), // "General"
QT_MOC_LITERAL(1, 8, 25), // "sigBtnGenneralMoreclicked"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 23) // "onPushbuttonMoreClicked"

    },
    "General\0sigBtnGenneralMoreclicked\0\0"
    "onPushbuttonMoreClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_General[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   25,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void General::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        General *_t = static_cast<General *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigBtnGenneralMoreclicked(); break;
        case 1: _t->onPushbuttonMoreClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (General::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&General::sigBtnGenneralMoreclicked)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject General::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_General.data,
      qt_meta_data_General,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *General::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *General::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_General.stringdata0))
        return static_cast<void*>(const_cast< General*>(this));
    return QWidget::qt_metacast(_clname);
}

int General::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void General::sigBtnGenneralMoreclicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_Extrapolation_t {
    QByteArrayData data[1];
    char stringdata0[14];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Extrapolation_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Extrapolation_t qt_meta_stringdata_Extrapolation = {
    {
QT_MOC_LITERAL(0, 0, 13) // "Extrapolation"

    },
    "Extrapolation"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Extrapolation[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void Extrapolation::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject Extrapolation::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Extrapolation.data,
      qt_meta_data_Extrapolation,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Extrapolation::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Extrapolation::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Extrapolation.stringdata0))
        return static_cast<void*>(const_cast< Extrapolation*>(this));
    return QWidget::qt_metacast(_clname);
}

int Extrapolation::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_LabelSettings_t {
    QByteArrayData data[1];
    char stringdata0[14];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LabelSettings_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LabelSettings_t qt_meta_stringdata_LabelSettings = {
    {
QT_MOC_LITERAL(0, 0, 13) // "LabelSettings"

    },
    "LabelSettings"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LabelSettings[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void LabelSettings::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject LabelSettings::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_LabelSettings.data,
      qt_meta_data_LabelSettings,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *LabelSettings::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LabelSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LabelSettings.stringdata0))
        return static_cast<void*>(const_cast< LabelSettings*>(this));
    return QWidget::qt_metacast(_clname);
}

int LabelSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_LabelText_t {
    QByteArrayData data[1];
    char stringdata0[10];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LabelText_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LabelText_t qt_meta_stringdata_LabelText = {
    {
QT_MOC_LITERAL(0, 0, 9) // "LabelText"

    },
    "LabelText"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LabelText[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void LabelText::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject LabelText::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_LabelText.data,
      qt_meta_data_LabelText,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *LabelText::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LabelText::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LabelText.stringdata0))
        return static_cast<void*>(const_cast< LabelText*>(this));
    return QWidget::qt_metacast(_clname);
}

int LabelText::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_ColorRanges_t {
    QByteArrayData data[4];
    char stringdata0[61];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ColorRanges_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ColorRanges_t qt_meta_stringdata_ColorRanges = {
    {
QT_MOC_LITERAL(0, 0, 11), // "ColorRanges"
QT_MOC_LITERAL(1, 12, 28), // "sigBtnColorRangesMoreclicked"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 18) // "onBtnCRMoreclicked"

    },
    "ColorRanges\0sigBtnColorRangesMoreclicked\0"
    "\0onBtnCRMoreclicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ColorRanges[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   25,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void ColorRanges::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ColorRanges *_t = static_cast<ColorRanges *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigBtnColorRangesMoreclicked(); break;
        case 1: _t->onBtnCRMoreclicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ColorRanges::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ColorRanges::sigBtnColorRangesMoreclicked)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject ColorRanges::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ColorRanges.data,
      qt_meta_data_ColorRanges,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ColorRanges::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ColorRanges::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ColorRanges.stringdata0))
        return static_cast<void*>(const_cast< ColorRanges*>(this));
    return QWidget::qt_metacast(_clname);
}

int ColorRanges::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void ColorRanges::sigBtnColorRangesMoreclicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
