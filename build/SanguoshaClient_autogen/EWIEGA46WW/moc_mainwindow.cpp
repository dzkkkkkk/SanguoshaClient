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
    QByteArrayData data[50];
    char stringdata0[809];
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
QT_MOC_LITERAL(10, 146, 8), // "username"
QT_MOC_LITERAL(11, 155, 8), // "password"
QT_MOC_LITERAL(12, 164, 19), // "onCreateRoomClicked"
QT_MOC_LITERAL(13, 184, 17), // "onJoinRoomClicked"
QT_MOC_LITERAL(14, 202, 8), // "uint32_t"
QT_MOC_LITERAL(15, 211, 6), // "roomId"
QT_MOC_LITERAL(16, 218, 17), // "onPlayCardClicked"
QT_MOC_LITERAL(17, 236, 6), // "cardId"
QT_MOC_LITERAL(18, 243, 12), // "targetPlayer"
QT_MOC_LITERAL(19, 256, 16), // "onEndTurnClicked"
QT_MOC_LITERAL(20, 273, 19), // "handleLoginResponse"
QT_MOC_LITERAL(21, 293, 24), // "sanguosha::LoginResponse"
QT_MOC_LITERAL(22, 318, 8), // "response"
QT_MOC_LITERAL(23, 327, 18), // "handleRoomResponse"
QT_MOC_LITERAL(24, 346, 23), // "sanguosha::RoomResponse"
QT_MOC_LITERAL(25, 370, 15), // "handleGameState"
QT_MOC_LITERAL(26, 386, 20), // "sanguosha::GameState"
QT_MOC_LITERAL(27, 407, 5), // "state"
QT_MOC_LITERAL(28, 413, 15), // "handleGameStart"
QT_MOC_LITERAL(29, 429, 20), // "sanguosha::GameStart"
QT_MOC_LITERAL(30, 450, 5), // "start"
QT_MOC_LITERAL(31, 456, 14), // "onCardSelected"
QT_MOC_LITERAL(32, 471, 23), // "onPlayCardButtonClicked"
QT_MOC_LITERAL(33, 495, 21), // "onCancelButtonClicked"
QT_MOC_LITERAL(34, 517, 18), // "updateButtonStates"
QT_MOC_LITERAL(35, 536, 20), // "sanguosha::GamePhase"
QT_MOC_LITERAL(36, 557, 5), // "phase"
QT_MOC_LITERAL(37, 563, 8), // "isMyTurn"
QT_MOC_LITERAL(38, 572, 14), // "handleGameOver"
QT_MOC_LITERAL(39, 587, 19), // "sanguosha::GameOver"
QT_MOC_LITERAL(40, 607, 8), // "gameOver"
QT_MOC_LITERAL(41, 616, 22), // "handleRoomListResponse"
QT_MOC_LITERAL(42, 639, 27), // "sanguosha::RoomListResponse"
QT_MOC_LITERAL(43, 667, 15), // "requestRoomList"
QT_MOC_LITERAL(44, 683, 13), // "QTableWidget*"
QT_MOC_LITERAL(45, 697, 9), // "roomTable"
QT_MOC_LITERAL(46, 707, 25), // "handleGameStartInUIThread"
QT_MOC_LITERAL(47, 733, 25), // "handleGameStateInUIThread"
QT_MOC_LITERAL(48, 759, 24), // "handleGameActionResponse"
QT_MOC_LITERAL(49, 784, 24) // "handleGameOverInUIThread"

    },
    "MainWindow\0onConnectionStatusChanged\0"
    "\0connected\0onMessageReceived\0"
    "sanguosha::GameMessage\0message\0"
    "onErrorOccurred\0errorString\0"
    "onLoginButtonClicked\0username\0password\0"
    "onCreateRoomClicked\0onJoinRoomClicked\0"
    "uint32_t\0roomId\0onPlayCardClicked\0"
    "cardId\0targetPlayer\0onEndTurnClicked\0"
    "handleLoginResponse\0sanguosha::LoginResponse\0"
    "response\0handleRoomResponse\0"
    "sanguosha::RoomResponse\0handleGameState\0"
    "sanguosha::GameState\0state\0handleGameStart\0"
    "sanguosha::GameStart\0start\0onCardSelected\0"
    "onPlayCardButtonClicked\0onCancelButtonClicked\0"
    "updateButtonStates\0sanguosha::GamePhase\0"
    "phase\0isMyTurn\0handleGameOver\0"
    "sanguosha::GameOver\0gameOver\0"
    "handleRoomListResponse\0"
    "sanguosha::RoomListResponse\0requestRoomList\0"
    "QTableWidget*\0roomTable\0"
    "handleGameStartInUIThread\0"
    "handleGameStateInUIThread\0"
    "handleGameActionResponse\0"
    "handleGameOverInUIThread"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,  129,    2, 0x08 /* Private */,
       4,    1,  132,    2, 0x08 /* Private */,
       7,    1,  135,    2, 0x08 /* Private */,
       9,    2,  138,    2, 0x08 /* Private */,
      12,    0,  143,    2, 0x08 /* Private */,
      13,    1,  144,    2, 0x08 /* Private */,
      16,    2,  147,    2, 0x08 /* Private */,
      19,    0,  152,    2, 0x08 /* Private */,
      20,    1,  153,    2, 0x08 /* Private */,
      23,    1,  156,    2, 0x08 /* Private */,
      25,    1,  159,    2, 0x08 /* Private */,
      28,    1,  162,    2, 0x08 /* Private */,
      31,    0,  165,    2, 0x08 /* Private */,
      32,    0,  166,    2, 0x08 /* Private */,
      33,    0,  167,    2, 0x08 /* Private */,
      34,    2,  168,    2, 0x08 /* Private */,
      38,    1,  173,    2, 0x08 /* Private */,
      41,    1,  176,    2, 0x08 /* Private */,
      43,    1,  179,    2, 0x08 /* Private */,
      46,    1,  182,    2, 0x08 /* Private */,
      47,    1,  185,    2, 0x08 /* Private */,
      48,    1,  188,    2, 0x08 /* Private */,
      49,    1,  191,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   10,   11,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, 0x80000000 | 14, 0x80000000 | 14,   17,   18,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void, 0x80000000 | 24,   22,
    QMetaType::Void, 0x80000000 | 26,   27,
    QMetaType::Void, 0x80000000 | 29,   30,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 35, QMetaType::Bool,   36,   37,
    QMetaType::Void, 0x80000000 | 39,   40,
    QMetaType::Void, 0x80000000 | 42,   22,
    QMetaType::Void, 0x80000000 | 44,   45,
    QMetaType::Void, 0x80000000 | 29,   30,
    QMetaType::Void, 0x80000000 | 26,   27,
    QMetaType::Void, 0x80000000 | 26,   27,
    QMetaType::Void, 0x80000000 | 39,   40,

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
        case 3: _t->onLoginButtonClicked((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->onCreateRoomClicked(); break;
        case 5: _t->onJoinRoomClicked((*reinterpret_cast< uint32_t(*)>(_a[1]))); break;
        case 6: _t->onPlayCardClicked((*reinterpret_cast< uint32_t(*)>(_a[1])),(*reinterpret_cast< uint32_t(*)>(_a[2]))); break;
        case 7: _t->onEndTurnClicked(); break;
        case 8: _t->handleLoginResponse((*reinterpret_cast< const sanguosha::LoginResponse(*)>(_a[1]))); break;
        case 9: _t->handleRoomResponse((*reinterpret_cast< const sanguosha::RoomResponse(*)>(_a[1]))); break;
        case 10: _t->handleGameState((*reinterpret_cast< const sanguosha::GameState(*)>(_a[1]))); break;
        case 11: _t->handleGameStart((*reinterpret_cast< const sanguosha::GameStart(*)>(_a[1]))); break;
        case 12: _t->onCardSelected(); break;
        case 13: _t->onPlayCardButtonClicked(); break;
        case 14: _t->onCancelButtonClicked(); break;
        case 15: _t->updateButtonStates((*reinterpret_cast< sanguosha::GamePhase(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 16: _t->handleGameOver((*reinterpret_cast< const sanguosha::GameOver(*)>(_a[1]))); break;
        case 17: _t->handleRoomListResponse((*reinterpret_cast< const sanguosha::RoomListResponse(*)>(_a[1]))); break;
        case 18: _t->requestRoomList((*reinterpret_cast< QTableWidget*(*)>(_a[1]))); break;
        case 19: _t->handleGameStartInUIThread((*reinterpret_cast< const sanguosha::GameStart(*)>(_a[1]))); break;
        case 20: _t->handleGameStateInUIThread((*reinterpret_cast< const sanguosha::GameState(*)>(_a[1]))); break;
        case 21: _t->handleGameActionResponse((*reinterpret_cast< const sanguosha::GameState(*)>(_a[1]))); break;
        case 22: _t->handleGameOverInUIThread((*reinterpret_cast< const sanguosha::GameOver(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 18:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTableWidget* >(); break;
            }
            break;
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
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
