/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[29];
    char stringdata0[438];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 25), // "onConnectionStatusChanged"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 9), // "connected"
QT_MOC_LITERAL(4, 48, 17), // "onMessageReceived"
QT_MOC_LITERAL(5, 66, 22), // "sanguosha::GameMessage"
QT_MOC_LITERAL(6, 89, 7), // "message"
QT_MOC_LITERAL(7, 97, 15), // "onErrorOccurred"
QT_MOC_LITERAL(8, 113, 11), // "errorString"
QT_MOC_LITERAL(9, 125, 20), // "onLoginButtonClicked"
QT_MOC_LITERAL(10, 146, 19), // "onCreateRoomClicked"
QT_MOC_LITERAL(11, 166, 17), // "onJoinRoomClicked"
QT_MOC_LITERAL(12, 184, 8), // "uint32_t"
QT_MOC_LITERAL(13, 193, 6), // "roomId"
QT_MOC_LITERAL(14, 200, 17), // "onPlayCardClicked"
QT_MOC_LITERAL(15, 218, 6), // "cardId"
QT_MOC_LITERAL(16, 225, 12), // "targetPlayer"
QT_MOC_LITERAL(17, 238, 16), // "onEndTurnClicked"
QT_MOC_LITERAL(18, 255, 19), // "handleLoginResponse"
QT_MOC_LITERAL(19, 275, 24), // "sanguosha::LoginResponse"
QT_MOC_LITERAL(20, 300, 8), // "response"
QT_MOC_LITERAL(21, 309, 18), // "handleRoomResponse"
QT_MOC_LITERAL(22, 328, 23), // "sanguosha::RoomResponse"
QT_MOC_LITERAL(23, 352, 15), // "handleGameState"
QT_MOC_LITERAL(24, 368, 20), // "sanguosha::GameState"
QT_MOC_LITERAL(25, 389, 5), // "state"
QT_MOC_LITERAL(26, 395, 15), // "handleGameStart"
QT_MOC_LITERAL(27, 411, 20), // "sanguosha::GameStart"
QT_MOC_LITERAL(28, 432, 5) // "start"

    },
    "MainWindow\0onConnectionStatusChanged\0"
    "\0connected\0onMessageReceived\0"
    "sanguosha::GameMessage\0message\0"
    "onErrorOccurred\0errorString\0"
    "onLoginButtonClicked\0onCreateRoomClicked\0"
    "onJoinRoomClicked\0uint32_t\0roomId\0"
    "onPlayCardClicked\0cardId\0targetPlayer\0"
    "onEndTurnClicked\0handleLoginResponse\0"
    "sanguosha::LoginResponse\0response\0"
    "handleRoomResponse\0sanguosha::RoomResponse\0"
    "handleGameState\0sanguosha::GameState\0"
    "state\0handleGameStart\0sanguosha::GameStart\0"
    "start"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x08 /* Private */,
       4,    1,   77,    2, 0x08 /* Private */,
       7,    1,   80,    2, 0x08 /* Private */,
       9,    0,   83,    2, 0x08 /* Private */,
      10,    0,   84,    2, 0x08 /* Private */,
      11,    1,   85,    2, 0x08 /* Private */,
      14,    2,   88,    2, 0x08 /* Private */,
      17,    0,   93,    2, 0x08 /* Private */,
      18,    1,   94,    2, 0x08 /* Private */,
      21,    1,   97,    2, 0x08 /* Private */,
      23,    1,  100,    2, 0x08 /* Private */,
      26,    1,  103,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, 0x80000000 | 12, 0x80000000 | 12,   15,   16,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, 0x80000000 | 22,   20,
    QMetaType::Void, 0x80000000 | 24,   25,
    QMetaType::Void, 0x80000000 | 27,   28,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onConnectionStatusChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->onMessageReceived((*reinterpret_cast< const sanguosha::GameMessage(*)>(_a[1]))); break;
        case 2: _t->onErrorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->onLoginButtonClicked(); break;
        case 4: _t->onCreateRoomClicked(); break;
        case 5: _t->onJoinRoomClicked((*reinterpret_cast< uint32_t(*)>(_a[1]))); break;
        case 6: _t->onPlayCardClicked((*reinterpret_cast< uint32_t(*)>(_a[1])),(*reinterpret_cast< uint32_t(*)>(_a[2]))); break;
        case 7: _t->onEndTurnClicked(); break;
        case 8: _t->handleLoginResponse((*reinterpret_cast< const sanguosha::LoginResponse(*)>(_a[1]))); break;
        case 9: _t->handleRoomResponse((*reinterpret_cast< const sanguosha::RoomResponse(*)>(_a[1]))); break;
        case 10: _t->handleGameState((*reinterpret_cast< const sanguosha::GameState(*)>(_a[1]))); break;
        case 11: _t->handleGameStart((*reinterpret_cast< const sanguosha::GameStart(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
