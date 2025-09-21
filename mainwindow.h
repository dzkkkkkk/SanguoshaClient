#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDataStream>
#include <QDateTime>
#include <QMessageBox>
#include <QTableWidget>
#include <QTextEdit>
#include <QLabel>
#include <QScrollArea>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QFlowLayout> // 添加QFlowLayout头文件
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
    
    // 修正参数声明
    void onLoginButtonClicked(const QString &username, const QString &password);
    void onCreateRoomClicked();
    void onJoinRoomClicked(uint32_t roomId);
    void onPlayCardClicked(uint32_t cardId, uint32_t targetPlayer);
    void onEndTurnClicked();

    void handleLoginResponse(const sanguosha::LoginResponse &response);
    void handleRoomResponse(const sanguosha::RoomResponse &response);
    void handleGameState(const sanguosha::GameState &state);
    void handleGameStart(const sanguosha::GameStart &start);
    
    // 添加缺失的槽函数声明
    void onCardSelected();
    void onPlayCardButtonClicked();
    void onCancelButtonClicked();
    void updateButtonStates(uint32_t phase);
    
private:
    Ui::MainWindow *ui;
    NetworkManager *m_networkManager;
    
    // 添加一个成员变量来跟踪最后的房间操作
    sanguosha::RoomAction m_lastRoomAction;

    void setupLoginScreen();    // 初始化登录界面
    void setupLobbyScreen();    // 初始化大厅界面
    void showScreen(QWidget *screen); // 用于切换界面

    QWidget *m_loginScreen;     // 登录界面控件容器
    QWidget *m_lobbyScreen;     // 大厅界面控件容器
    // 后续可以添加 m_gameScreen 等

    void setupGameScreen(); // 初始化游戏界面
    QWidget *m_gameScreen;  // 游戏界面控件容器

    // 游戏界面控件
    QTableWidget *m_playerInfoTable;
    QTextEdit *m_gameLog;
    QLabel *m_deckCountLabel;
    QLabel *m_gameArea;
    QLabel *m_turnInfoLabel;
    QFlowLayout *m_handCardsLayout; // 修改为QFlowLayout
    QPushButton *m_playCardButton;
    QPushButton *m_endTurnButton;
    QPushButton *m_cancelButton;
    
    uint32_t m_selectedCard; // 当前选中的卡牌
    uint32_t m_selfUserId;   // 自己的用户ID
};

#endif // MAINWINDOW_H