#include "networkmanager.h"
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <QHostAddress>

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
    , m_heartbeatTimer(new QTimer(this))
    , m_messageSize(0)
{
    // 设置低延迟选项
    m_socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);

    // 连接信号槽
    connect(m_socket, &QTcpSocket::connected, [this]() {
        m_heartbeatTimer->start();
    });
    connect(m_socket, &QTcpSocket::disconnected, this, &NetworkManager::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &NetworkManager::onError);

    // 设置心跳定时器（每30秒发送一次）
    m_heartbeatTimer->setInterval(30000);
}

NetworkManager::~NetworkManager()
{
    disconnectFromServer();
}

void NetworkManager::connectToServer(const QString &host, quint16 port)
{
    if (m_socket->state() != QAbstractSocket::UnconnectedState) {
        m_socket->abort();
    }
    
    m_socket->connectToHost(host, port);
}

void NetworkManager::disconnectFromServer()
{
    m_heartbeatTimer->stop();
    m_socket->disconnectFromHost();
}

bool NetworkManager::isConnected() const
{
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

void NetworkManager::sendMessage(const sanguosha::GameMessage &message)
{
    if (!isConnected()) {
        emit errorOccurred(tr("Not connected to server"));
        return;
    }

    // 序列化消息
    std::string serializedMessage;
    if (!message.SerializeToString(&serializedMessage)) {
        emit errorOccurred(tr("Failed to serialize message"));
        return;
    }

    // 创建数据包：4字节消息长度 + 消息体
    QByteArray packet;
    QDataStream stream(&packet, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_15);
    
    // 写入消息长度
    quint32 messageSize = static_cast<quint32>(serializedMessage.size());
    stream << messageSize;
    
    // 写入消息体
    packet.append(serializedMessage.data(), messageSize);

    // 发送数据
    m_socket->write(packet);
}

void NetworkManager::onConnected()
{
    emit connectionStatusChanged(true);
}

void NetworkManager::onDisconnected()
{
    m_heartbeatTimer->stop();
    m_buffer.clear();
    m_messageSize = 0;
    emit connectionStatusChanged(false);
}

void NetworkManager::onReadyRead()
{
    // 将新数据添加到缓冲区
    m_buffer.append(m_socket->readAll());

    // 处理缓冲区中的所有完整消息
    while (true) {
        // 如果还不知道消息大小，尝试读取
        if (m_messageSize == 0) {
            if (!readMessageSize()) {
                break; // 没有足够的数据读取消息大小
            }
        }

        // 检查是否有足够的数据读取消息体
        if (m_buffer.size() < static_cast<int>(sizeof(quint32) + m_messageSize)) {
            break; // 没有足够的数据读取完整消息
        }

        // 读取消息体
        if (!readMessageBody()) {
            break; // 消息解析失败
        }
    }
}

void NetworkManager::onError(QAbstractSocket::SocketError error)
{
    QString errorMsg;
    switch (error) {
    case QAbstractSocket::ConnectionRefusedError:
        errorMsg = tr("Connection refused");
        break;
    case QAbstractSocket::RemoteHostClosedError:
        errorMsg = tr("Remote host closed connection");
        break;
    case QAbstractSocket::HostNotFoundError:
        errorMsg = tr("Host not found");
        break;
    case QAbstractSocket::SocketTimeoutError:
        errorMsg = tr("Connection timeout");
        break;
    default:
        errorMsg = m_socket->errorString();
    }
    emit errorOccurred(errorMsg);
}

void NetworkManager::sendHeartbeat()
{
    sanguosha::GameMessage message;
    message.set_type(sanguosha::HEARTBEAT);
    
    sanguosha::Heartbeat *heartbeat = new sanguosha::Heartbeat();
    heartbeat->set_timestamp(QDateTime::currentDateTime().toMSecsSinceEpoch());
    
    message.set_allocated_heartbeat(heartbeat);
    
    sendMessage(message);
}

bool NetworkManager::readMessageSize()
{
    // 检查是否有足够的数据读取消息大小（4字节）
    if (m_buffer.size() < static_cast<int>(sizeof(quint32))) {
        return false;
    }

    // 从缓冲区读取消息大小
    QDataStream stream(m_buffer);
    stream.setVersion(QDataStream::Qt_5_15);
    stream >> m_messageSize;

    // 移除已读取的消息大小数据
    m_buffer = m_buffer.mid(sizeof(quint32));

    return true;
}

bool NetworkManager::readMessageBody()
{
    // 检查是否有足够的数据读取消息体
    if (m_buffer.size() < static_cast<int>(m_messageSize)) {
        return false;
    }

    // 提取消息体数据
    QByteArray messageData = m_buffer.left(m_messageSize);
    m_buffer = m_buffer.mid(m_messageSize);

    // 解析Protocol Buffers消息
    sanguosha::GameMessage message;
    if (message.ParseFromArray(messageData.constData(), messageData.size())) {
        emit messageReceived(message);
    } else {
        emit errorOccurred(tr("Failed to parse message from server"));
    }

    // 重置消息大小，准备读取下一条消息
    m_messageSize = 0;

    return true;
}