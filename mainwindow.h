#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDataStream>
#include <QDateTime>
#include "proto/sanguosha.pb.h"
#include "network/networkmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onConnectionStatusChanged(bool connected);
    void onMessageReceived(const sanguosha::GameMessage &message);
    void onErrorOccurred(const QString &errorString);
    
    // 添加以下槽函数声明
    void onLoginButtonClicked();
    void onCreateRoomClicked();
    void onJoinRoomClicked(uint32_t roomId);
    void onPlayCardClicked(uint32_t cardId, uint32_t targetPlayer);
    void onEndTurnClicked();

    void handleLoginResponse(const sanguosha::LoginResponse &response);
    void handleRoomResponse(const sanguosha::RoomResponse &response);
    void handleGameState(const sanguosha::GameState &state);
    void handleGameStart(const sanguosha::GameStart &start);
    
private:
    Ui::MainWindow *ui;
    NetworkManager *m_networkManager;
    
    // 添加一个成员变量来跟踪最后的房间操作
    sanguosha::RoomAction m_lastRoomAction;
};

#endif // MAINWINDOW_H