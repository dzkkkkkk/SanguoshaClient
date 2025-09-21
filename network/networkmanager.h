#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QDataStream>  // 添加这行
#include <QDateTime>    // 添加这行
#include "proto/sanguosha.pb.h"

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

    // 连接服务器
    void connectToServer(const QString &host, quint16 port);
    // 断开连接
    void disconnectFromServer();
    // 检查连接状态
    bool isConnected() const;
    // 发送消息
    void sendMessage(const sanguosha::GameMessage &message);

signals:
    // 连接状态变化信号
    void connectionStatusChanged(bool connected);
    // 收到消息信号
    void messageReceived(const sanguosha::GameMessage &message);
    // 错误信号
    void errorOccurred(const QString &errorString);

private slots:
    // 处理连接建立
    void onConnected();
    // 处理连接断开
    void onDisconnected();
    // 处理数据接收
    void onReadyRead();
    // 处理错误
    void onError(QAbstractSocket::SocketError error);
    // 发送心跳包
    void sendHeartbeat();

private:
    QTcpSocket *m_socket;
    QTimer *m_heartbeatTimer;
    QByteArray m_buffer;
    quint32 m_messageSize;

    // 读取消息大小
    bool readMessageSize();
    // 读取消息体
    bool readMessageBody();
};

#endif // NETWORKMANAGER_H