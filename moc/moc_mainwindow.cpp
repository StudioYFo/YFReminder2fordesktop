/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Sun Sep 23 04:46:31 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      46,   11,   11,   11, 0x0a,
      78,   11,   11,   11, 0x08,
     109,   11,   11,   11, 0x08,
     138,  133,   11,   11, 0x08,
     182,  133,   11,   11, 0x08,
     232,   11,   11,   11, 0x08,
     258,   11,   11,   11, 0x08,
     279,   11,   11,   11, 0x08,
     309,  304,   11,   11, 0x08,
     343,   11,   11,   11, 0x08,
     370,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0on_downloadDataBaseFileComplete()\0"
    "on_uploadDataBaseFileComplete()\0"
    "on_ledReminder_returnPressed()\0"
    "on_btnConfirm_clicked()\0item\0"
    "on_listWidget_itemClicked(QListWidgetItem*)\0"
    "on_listWidget_itemDoubleClicked(QListWidgetItem*)\0"
    "on_MainWindow_destroyed()\0"
    "on_btnExit_clicked()\0on_btnSettings_clicked()\0"
    "arg1\0on_MainWindow_destroyed(QObject*)\0"
    "on_btnRemoteSync_clicked()\0"
    "on_btnRemoteDownload_clicked()\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_downloadDataBaseFileComplete(); break;
        case 1: on_uploadDataBaseFileComplete(); break;
        case 2: on_ledReminder_returnPressed(); break;
        case 3: on_btnConfirm_clicked(); break;
        case 4: on_listWidget_itemClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 5: on_listWidget_itemDoubleClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 6: on_MainWindow_destroyed(); break;
        case 7: on_btnExit_clicked(); break;
        case 8: on_btnSettings_clicked(); break;
        case 9: on_MainWindow_destroyed((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 10: on_btnRemoteSync_clicked(); break;
        case 11: on_btnRemoteDownload_clicked(); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
