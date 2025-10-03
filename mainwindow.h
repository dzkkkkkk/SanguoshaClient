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
    
    void onLoginButtonClicked(const QString &username, const QString &password);
    void onCreateRoomClicked();
    void onJoinRoomClicked(uint32_t roomId);
    void onPlayCardClicked(uint32_t cardId, uint32_t targetPlayer);
    void onEndTurnClicked();

    void handleLoginResponse(const sanguosha::LoginResponse &response);
    void handleRoomResponse(const sanguosha::RoomResponse &response);
    void handleGameState(const sanguosha::GameState &state);
    void handleGameStart(const sanguosha::GameStart &start);
    
    void onCardSelected();
    void onPlayCardButtonClicked();
    void onCancelButtonClicked();
    void updateButtonStates(sanguosha::GamePhase phase, bool isMyTurn);
    
    void handleGameOver(const sanguosha::GameOver &gameOver);

    void handleRoomListResponse(const sanguosha::RoomListResponse &response);
    void requestRoomList(QTableWidget* roomTable); // 添加函数声明
    void handleGameStartInUIThread(const sanguosha::GameStart &start);
    void handleGameStateInUIThread(const sanguosha::GameState& state);
    void handleGameActionResponse(const sanguosha::GameState& state);
    void handleGameOverInUIThread(const sanguosha::GameOver &gameOver);
    
private:
    Ui::MainWindow *ui;
    NetworkManager *m_networkManager;
    
    sanguosha::RoomAction m_lastRoomAction;

    void setupLoginScreen();
    void setupLobbyScreen();
    void showScreen(QWidget *screen);
    void debugSendTestMessage();

    QWidget *m_loginScreen;
    QWidget *m_lobbyScreen;

    void setupGameScreen();
    QWidget *m_gameScreen;

    QTableWidget *m_playerInfoTable;
    QTextEdit *m_gameLog;
    QLabel *m_deckCountLabel;
    QLabel *m_gameArea;
    QLabel *m_turnInfoLabel;
    QHBoxLayout *m_handCardsLayout;
    QPushButton *m_playCardButton;
    QPushButton *m_endTurnButton;
    QPushButton *m_cancelButton;
    QTimer *m_roomOperationTimer;
    
    uint32_t m_selectedCard;
    uint32_t m_selfUserId;

    void updatePlayerInfoTable(const sanguosha::GameState &state);
    void updateHandCards(const sanguosha::GameState &state);
    void updateGameLog(const sanguosha::GameState &state);
    void updateTurnInfo(const sanguosha::GameState &state);
    void addCardToHand(uint32_t cardId);
    QString getCardName(uint32_t cardId);
    QString getCardColor(uint32_t cardId);
    
    // 添加缺失的函数声明
    void addToGameLog(const QString &message);
    void resetGameState();
    void checkGameEndCondition(const sanguosha::GameState &state);
    bool isAlive;
    
};

#endif // MAINWINDOW_H