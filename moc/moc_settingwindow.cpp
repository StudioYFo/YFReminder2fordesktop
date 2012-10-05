/****************************************************************************
** Meta object code from reading C++ file 'settingwindow.h'
**
** Created: Sun Sep 23 04:34:40 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../settingwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'settingwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SettingWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,
      41,   36,   14,   14, 0x08,
      71,   36,   14,   14, 0x08,
     102,   36,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SettingWindow[] = {
    "SettingWindow\0\0on_btnBack_clicked()\0"
    "arg1\0on_spinYear_valueChanged(int)\0"
    "on_spinMonth_valueChanged(int)\0"
    "on_spinDay_valueChanged(int)\0"
};

const QMetaObject SettingWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_SettingWindow,
      qt_meta_data_SettingWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SettingWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SettingWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SettingWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SettingWindow))
        return static_cast<void*>(const_cast< SettingWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int SettingWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_btnBack_clicked(); break;
        case 1: on_spinYear_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: on_spinMonth_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: on_spinDay_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
