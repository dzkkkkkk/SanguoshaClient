#include "networkmanager.h"
#include <QHostAddress>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <stdexcept>
#include <arpa/inet.h>
#include <cstring>  // 添加这行

NetworkManager& NetworkManager::instance() {
    static NetworkManager instance;
    return instance;
}

NetworkManager::NetworkManager(QObject *parent) 
    : QObject(parent), 
      m_socket(new QTcpSocket(this)),
      m_heartbeatTimer(new QTimer(this)),
      m_expectedBodySize(0),
      m_headerRead(false) {
    
    connect(m_socket, &QTcpSocket::connected, this, &NetworkManager::onConnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    connect(m_socket, &QTcpSocket::disconnected, this, &NetworkManager::disconnected);
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
        this, SLOT(onErrorOccurred(QAbstractSocket::SocketError)));
    
    connect(m_heartbeatTimer, &QTimer::timeout, this, &NetworkManager::sendHeartbeat);
    m_heartbeatTimer->setInterval(25000); // 25秒发送一次心跳
}

NetworkManager::~NetworkManager() {
    m_heartbeatTimer->stop();
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->disconnectFromHost();
    }
}

void NetworkManager::connectToServer(const QString& host, quint16 port) {
    m_socket->connectToHost(host, port);
}

bool NetworkManager::isConnected() const {
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

void NetworkManager::onConnected() {
    m_heartbeatTimer->start();
    m_buffer.clear();
    m_expectedBodySize = 0;
    m_headerRead = false;
    emit connected();
}

void NetworkManager::onReadyRead() {
    QByteArray data = m_socket->readAll();
    qDebug() << "Received" << data.size() << "bytes from server";
    m_buffer.insert(m_buffer.end(), data.begin(), data.end());
    
    while (!m_buffer.empty()) {
        if (!m_headerRead && m_buffer.size() >= 4) {
            // 读取消息头（4字节长度）
            uint32_t net_size;
            memcpy(&net_size, m_buffer.data(), 4);
            m_expectedBodySize = ntohl(net_size);
            m_headerRead = true;
            
            // 移除已处理的头数据
            m_buffer.erase(m_buffer.begin(), m_buffer.begin() + 4);
            continue;
        }
        
        if (m_headerRead && m_buffer.size() >= m_expectedBodySize) {
            // 读取完整的消息体
            std::vector<char> messageData(m_buffer.begin(), m_buffer.begin() + m_expectedBodySize);
            parseMessage(messageData);
            
            // 移除已处理的消息体数据
            m_buffer.erase(m_buffer.begin(), m_buffer.begin() + m_expectedBodySize);
            m_headerRead = false;
            m_expectedBodySize = 0;
            continue;
        }
        
        break; // 数据不足，等待下次接收
    }
}

// networkmanager.cpp - 确保消息处理在UI线程
// networkmanager.cpp - 修改parseMessage函数
void NetworkManager::parseMessage(const std::vector<char>& buffer) {
    sanguosha::GameMessage message;
    if (!message.ParseFromArray(buffer.data(), buffer.size())) {
        qWarning() << "Failed to parse message from server";
        return;
    }
    
    // 确保在UI线程处理消息
    QMetaObject::invokeMethod(this, [this, message]() {
        qDebug() << "Received message type:" << message.type();
        
        switch (message.type()) {
        case sanguosha::LOGIN_RESPONSE:
            emit loginResponseReceived(message.login_response());
            break;
        case sanguosha::ROOM_RESPONSE:
            emit roomResponseReceived(message.room_response());
            break;
        case sanguosha::GAME_STATE:
            emit gameStateReceived(message.game_state());
            break;
        case sanguosha::GAME_START:
            emit gameStartReceived(message.game_start());
            break;
        case sanguosha::GAME_OVER:
            emit gameOverReceived(message.game_over());
            break;
        case sanguosha::ROOM_LIST_RESPONSE:
            emit roomListResponseReceived(message.room_list_response());
            break;
        case sanguosha::HEARTBEAT:
            // 心跳包，无需处理
            break;
        default:
            qWarning() << "Unknown message type received:" << message.type();
            break;
        }
    }, Qt::QueuedConnection);
}

// 在NetworkManager::sendMessage中，确保编码正确
void NetworkManager::sendMessage(const sanguosha::GameMessage& message) {
    if (!isConnected()) {
        qWarning() << "Not connected to server";
        return;
    }
    
    // 计算消息体大小
    size_t body_size = message.ByteSizeLong();
    size_t total_size = 4 + body_size;
    
    std::vector<char> buffer(total_size);
    
    // 写入消息头（长度）- 使用网络字节序
    uint32_t net_size = htonl(static_cast<uint32_t>(body_size));
    memcpy(buffer.data(), &net_size, 4);  // 去掉 std:: 前缀
    
    // 写入消息体
    if (!message.SerializeToArray(buffer.data() + 4, body_size)) {
        qWarning() << "Failed to serialize message";
        return;
    }
    
    // 发送完整消息
    qint64 bytesWritten = m_socket->write(buffer.data(), total_size);
    if (bytesWritten == -1) {
        qWarning() << "Write error:" << m_socket->errorString();
    } else if (bytesWritten != total_size) {
        qWarning() << "Incomplete write:" << bytesWritten << "of" << total_size;
    }
    
    m_socket->flush(); // 确保数据被发送
}

void NetworkManager::sendHeartbeat() {
    sanguosha::GameMessage message;
    message.set_type(sanguosha::HEARTBEAT);
    sendMessage(message);
}

void NetworkManager::login(const QString& username) {
    sanguosha::GameMessage message;
    message.set_type(sanguosha::LOGIN_REQUEST);
    message.mutable_login_request()->set_username(username.toStdString());
    sendMessage(message);
}

void NetworkManager::onDisconnected() {
    m_heartbeatTimer->stop();
    emit disconnected();
}

void NetworkManager::onErrorOccurred(QAbstractSocket::SocketError error) {
    emit errorOccurred(m_socket->errorString());
}

void NetworkManager::sendGameAction(uint32_t cardId, uint32_t targetPlayer) {
    sanguosha::GameMessage message;
    message.set_type(sanguosha::GAME_ACTION);
    
    sanguosha::GameAction* gameAction = message.mutable_game_action();
    gameAction->set_type(sanguosha::ACTION_PLAY_CARD);
    gameAction->set_card_id(cardId);
    gameAction->set_target_player(targetPlayer);
    
    sendMessage(message);
}