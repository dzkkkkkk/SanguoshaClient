/****************************************************************************
** Meta object code from reading C++ file 'networkmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "network/networkmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networkmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NetworkManager_t {
    QByteArrayData data[27];
    char stringdata0[406];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetworkManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetworkManager_t qt_meta_stringdata_NetworkManager = {
    {
QT_MOC_LITERAL(0, 0, 14), // "NetworkManager"
QT_MOC_LITERAL(1, 15, 9), // "connected"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 12), // "disconnected"
QT_MOC_LITERAL(4, 39, 13), // "errorOccurred"
QT_MOC_LITERAL(5, 53, 11), // "errorString"
QT_MOC_LITERAL(6, 65, 21), // "loginResponseReceived"
QT_MOC_LITERAL(7, 87, 24), // "sanguosha::LoginResponse"
QT_MOC_LITERAL(8, 112, 8), // "response"
QT_MOC_LITERAL(9, 121, 20), // "roomResponseReceived"
QT_MOC_LITERAL(10, 142, 23), // "sanguosha::RoomResponse"
QT_MOC_LITERAL(11, 166, 17), // "gameStateReceived"
QT_MOC_LITERAL(12, 184, 20), // "sanguosha::GameState"
QT_MOC_LITERAL(13, 205, 5), // "state"
QT_MOC_LITERAL(14, 211, 17), // "gameStartReceived"
QT_MOC_LITERAL(15, 229, 20), // "sanguosha::GameStart"
QT_MOC_LITERAL(16, 250, 5), // "start"
QT_MOC_LITERAL(17, 256, 16), // "gameOverReceived"
QT_MOC_LITERAL(18, 273, 19), // "sanguosha::GameOver"
QT_MOC_LITERAL(19, 293, 8), // "gameOver"
QT_MOC_LITERAL(20, 302, 11), // "onConnected"
QT_MOC_LITERAL(21, 314, 11), // "onReadyRead"
QT_MOC_LITERAL(22, 326, 14), // "onDisconnected"
QT_MOC_LITERAL(23, 341, 15), // "onErrorOccurred"
QT_MOC_LITERAL(24, 357, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(25, 386, 5), // "error"
QT_MOC_LITERAL(26, 392, 13) // "sendHeartbeat"

    },
    "NetworkManager\0connected\0\0disconnected\0"
    "errorOccurred\0errorString\0"
    "loginResponseReceived\0sanguosha::LoginResponse\0"
    "response\0roomResponseReceived\0"
    "sanguosha::RoomResponse\0gameStateReceived\0"
    "sanguosha::GameState\0state\0gameStartReceived\0"
    "sanguosha::GameStart\0start\0gameOverReceived\0"
    "sanguosha::GameOver\0gameOver\0onConnected\0"
    "onReadyRead\0onDisconnected\0onErrorOccurred\0"
    "QAbstractSocket::SocketError\0error\0"
    "sendHeartbeat"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,
       3,    0,   80,    2, 0x06 /* Public */,
       4,    1,   81,    2, 0x06 /* Public */,
       6,    1,   84,    2, 0x06 /* Public */,
       9,    1,   87,    2, 0x06 /* Public */,
      11,    1,   90,    2, 0x06 /* Public */,
      14,    1,   93,    2, 0x06 /* Public */,
      17,    1,   96,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      20,    0,   99,    2, 0x08 /* Private */,
      21,    0,  100,    2, 0x08 /* Private */,
      22,    0,  101,    2, 0x08 /* Private */,
      23,    1,  102,    2, 0x08 /* Private */,
      26,    0,  105,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 10,    8,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void, 0x80000000 | 18,   19,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 24,   25,
    QMetaType::Void,

       0        // eod
};

void NetworkManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetworkManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->errorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->loginResponseReceived((*reinterpret_cast< const sanguosha::LoginResponse(*)>(_a[1]))); break;
        case 4: _t->roomResponseReceived((*reinterpret_cast< const sanguosha::RoomResponse(*)>(_a[1]))); break;
        case 5: _t->gameStateReceived((*reinterpret_cast< const sanguosha::GameState(*)>(_a[1]))); break;
        case 6: _t->gameStartReceived((*reinterpret_cast< const sanguosha::GameStart(*)>(_a[1]))); break;
        case 7: _t->gameOverReceived((*reinterpret_cast< const sanguosha::GameOver(*)>(_a[1]))); break;
        case 8: _t->onConnected(); break;
        case 9: _t->onReadyRead(); break;
        case 10: _t->onDisconnected(); break;
        case 11: _t->onErrorOccurred((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 12: _t->sendHeartbeat(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NetworkManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::connected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::disconnected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::errorOccurred)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const sanguosha::LoginResponse & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::loginResponseReceived)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const sanguosha::RoomResponse & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::roomResponseReceived)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const sanguosha::GameState & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::gameStateReceived)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const sanguosha::GameStart & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::gameStartReceived)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const sanguosha::GameOver & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::gameOverReceived)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NetworkManager::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_NetworkManager.data,
    qt_meta_data_NetworkManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NetworkManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NetworkManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void NetworkManager::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void NetworkManager::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void NetworkManager::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void NetworkManager::loginResponseReceived(const sanguosha::LoginResponse & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void NetworkManager::roomResponseReceived(const sanguosha::RoomResponse & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void NetworkManager::gameStateReceived(const sanguosha::GameState & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void NetworkManager::gameStartReceived(const sanguosha::GameStart & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void NetworkManager::gameOverReceived(const sanguosha::GameOver & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
