/****************************************************************************
** Meta object code from reading C++ file 'remotedatabasemanager.h'
**
** Created: Sun Sep 23 04:34:41 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../remotedatabasemanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'remotedatabasemanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RemoteDataBaseManager[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      27,   23,   22,   22, 0x05,
      45,   22,   22,   22, 0x05,
      76,   22,   22,   22, 0x05,

 // slots: signature, parameters, type, tag, flags
     105,   22,   22,   22, 0x08,
     152,  143,   22,   22, 0x08,
     174,   22,   22,   22, 0x08,
     197,   22,   22,   22, 0x08,
     210,   22,   22,   22, 0x08,
     233,   22,   22,   22, 0x08,
     249,   22,   22,   22, 0x08,
     275,   22,   22,   22, 0x08,
     323,  301,   22,   22, 0x08,
     357,   22,   22,   22, 0x08,
     375,   22,   22,   22, 0x08,
     403,   22,   22,   22, 0x08,
     426,   22,   22,   22, 0x08,
     474,  453,   22,   22, 0x08,
     506,   22,   22,   22, 0x08,
     539,  528,   22,   22, 0x08,
     564,   22,   22,   22, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_RemoteDataBaseManager[] = {
    "RemoteDataBaseManager\0\0ret\0logInResult(bool)\0"
    "downloadDataBaseFileComplete()\0"
    "uploadDataBaseFileComplete()\0"
    "on_scanNetworkConfigurationFinished()\0"
    "logInRet\0sendAccessToken(bool)\0"
    "acesTknReplyFinished()\0reqUsrInfo()\0"
    "usrInfoReplyFinished()\0reqUploadFile()\0"
    "getUploadLocateFinished()\0"
    "upLoadFileReplyFinished()\0"
    "bytesWrite,totalBytes\0"
    "showUploadProgress(qint64,qint64)\0"
    "reqDownLoadFile()\0downLoadFileReplyFinished()\0"
    "dwnFileRealReadReady()\0"
    "dwnFileRealReplyFinished()\0"
    "bytesRead,totalBytes\0"
    "showDownProgress(qint64,qint64)\0"
    "reqTknReplyFinished()\0isFinished\0"
    "getAuthoriseResult(bool)\0on_btnBack_clicked()\0"
};

const QMetaObject RemoteDataBaseManager::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_RemoteDataBaseManager,
      qt_meta_data_RemoteDataBaseManager, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RemoteDataBaseManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RemoteDataBaseManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RemoteDataBaseManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RemoteDataBaseManager))
        return static_cast<void*>(const_cast< RemoteDataBaseManager*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int RemoteDataBaseManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: logInResult((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: downloadDataBaseFileComplete(); break;
        case 2: uploadDataBaseFileComplete(); break;
        case 3: on_scanNetworkConfigurationFinished(); break;
        case 4: sendAccessToken((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: acesTknReplyFinished(); break;
        case 6: reqUsrInfo(); break;
        case 7: usrInfoReplyFinished(); break;
        case 8: reqUploadFile(); break;
        case 9: getUploadLocateFinished(); break;
        case 10: upLoadFileReplyFinished(); break;
        case 11: showUploadProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 12: reqDownLoadFile(); break;
        case 13: downLoadFileReplyFinished(); break;
        case 14: dwnFileRealReadReady(); break;
        case 15: dwnFileRealReplyFinished(); break;
        case 16: showDownProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 17: reqTknReplyFinished(); break;
        case 18: getAuthoriseResult((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 19: on_btnBack_clicked(); break;
        default: ;
        }
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void RemoteDataBaseManager::logInResult(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RemoteDataBaseManager::downloadDataBaseFileComplete()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void RemoteDataBaseManager::uploadDataBaseFileComplete()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
