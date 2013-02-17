/****************************************************************************
** Meta object code from reading C++ file 'camldevwindow.h'
**
** Created: Sun Feb 17 18:49:00 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "camldevwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'camldevwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CamlDevWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,
      26,   14,   14,   14, 0x0a,
      37,   14,   14,   14, 0x0a,
      48,   14,   14,   14, 0x0a,
      57,   14,   14,   14, 0x0a,
      78,   14,   73,   14, 0x0a,
      87,   14,   73,   14, 0x0a,
      94,   14,   14,   14, 0x0a,
     101,   14,   14,   14, 0x0a,
     115,   14,   14,   14, 0x0a,
     125,   14,   14,   14, 0x0a,
     142,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CamlDevWindow[] = {
    "CamlDevWindow\0\0sendCaml()\0readCaml()\0"
    "stopCaml()\0camlOK()\0interruptCaml()\0"
    "bool\0saveAs()\0save()\0open()\0textChanged()\0"
    "newFile()\0readCamlErrors()\0print()\0"
};

void CamlDevWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CamlDevWindow *_t = static_cast<CamlDevWindow *>(_o);
        switch (_id) {
        case 0: _t->sendCaml(); break;
        case 1: _t->readCaml(); break;
        case 2: _t->stopCaml(); break;
        case 3: _t->camlOK(); break;
        case 4: _t->interruptCaml(); break;
        case 5: { bool _r = _t->saveAs();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: { bool _r = _t->save();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: _t->open(); break;
        case 8: _t->textChanged(); break;
        case 9: _t->newFile(); break;
        case 10: _t->readCamlErrors(); break;
        case 11: _t->print(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CamlDevWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CamlDevWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CamlDevWindow,
      qt_meta_data_CamlDevWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CamlDevWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CamlDevWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CamlDevWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CamlDevWindow))
        return static_cast<void*>(const_cast< CamlDevWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int CamlDevWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
