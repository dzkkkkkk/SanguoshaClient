#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include "sanguosha.pb.h"

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    static NetworkManager& instance();
    ~NetworkManager();

    void connectToServer(const QString& host, quint16 port);
    void sendMessage(const sanguosha::GameMessage& message);
    bool isConnected() const;

    // 登录相关
    void login(const QString& username);

signals:
    // 连接状态信号
    void connected();
    void disconnected();
    void errorOccurred(const QString& errorString);

    // 服务器消息信号
    void loginResponseReceived(const sanguosha::LoginResponse& response);
    void roomResponseReceived(const sanguosha::RoomResponse& response);
    void gameStateReceived(const sanguosha::GameState& state);
    void gameStartReceived(const sanguosha::GameStart& start);
    void gameOverReceived(const sanguosha::GameOver& gameOver);

    void roomListResponseReceived(const sanguosha::RoomListResponse &response);

private slots:
    void onConnected();
    void onReadyRead();
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError error);
    void sendHeartbeat();

private:
    explicit NetworkManager(QObject *parent = nullptr);
    void parseMessage(const std::vector<char>& buffer);

    QTcpSocket* m_socket;
    QTimer* m_heartbeatTimer;
    std::vector<char> m_buffer;
    size_t m_expectedBodySize;
    bool m_headerRead;
};

#endif // NETWORK_MANAGER_H