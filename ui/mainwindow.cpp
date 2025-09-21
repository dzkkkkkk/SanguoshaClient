#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_networkManager(new NetworkManager(this))
{
    ui->setupUi(this);

    // 连接NetworkManager的信号槽
    connect(m_networkManager, &NetworkManager::connectionStatusChanged,
            this, &MainWindow::onConnectionStatusChanged);
    connect(m_networkManager, &NetworkManager::messageReceived,
            this, &MainWindow::onMessageReceived);
    connect(m_networkManager, &NetworkManager::errorOccurred,
            this, &MainWindow::onErrorOccurred);

    // 连接到服务器（示例地址和端口）
    m_networkManager->connectToServer("127.0.0.1", 8888);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 实现槽函数
void MainWindow::onConnectionStatusChanged(bool connected)
{
    // 更新UI显示连接状态
    ui->statusbar->showMessage(connected ? 
        tr("Connected to server") : tr("Disconnected from server"));
}

//登录处理
void MainWindow::onLoginButtonClicked()
{
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();
    
    sanguosha::GameMessage message;
    message.set_type(sanguosha::LOGIN_REQUEST);
    
    sanguosha::LoginRequest* loginRequest = new sanguosha::LoginRequest();
    loginRequest->set_username(username.toStdString());
    loginRequest->set_password(password.toStdString());
    
    message.set_allocated_login_request(loginRequest);
    m_networkManager->sendMessage(message);
}

void MainWindow::onMessageReceived(const sanguosha::GameMessage &message)
{
    switch (message.type()) {
    case sanguosha::LOGIN_RESPONSE:
        handleLoginResponse(message.login_response());
        break;
    case sanguosha::ROOM_RESPONSE:
        handleRoomResponse(message.room_response());
        break;
    case sanguosha::GAME_STATE:
        handleGameState(message.game_state());
        break;
    case sanguosha::GAME_START:
        handleGameStart(message.game_start());
        break;
    case sanguosha::HEARTBEAT:
        // 心跳包，通常不需要处理
        break;
    default:
        qWarning() << "Received unknown message type:" << message.type();
        break;
    }
}

//房间创建
void MainWindow::onCreateRoomClicked()
{
    sanguosha::GameMessage message;
    message.set_type(sanguosha::ROOM_REQUEST);
    
    sanguosha::RoomRequest* roomRequest = new sanguosha::RoomRequest();
    roomRequest->set_action(sanguosha::CREATE_ROOM);
    
    message.set_allocated_room_request(roomRequest);
    m_networkManager->sendMessage(message);
}

//房间加入
void MainWindow::onJoinRoomClicked(uint32_t roomId)
{
    sanguosha::GameMessage message;
    message.set_type(sanguosha::ROOM_REQUEST);
    
    sanguosha::RoomRequest* roomRequest = new sanguosha::RoomRequest();
    roomRequest->set_action(sanguosha::JOIN_ROOM);
    roomRequest->set_room_id(roomId);
    
    message.set_allocated_room_request(roomRequest);
    m_networkManager->sendMessage(message);
}

void MainWindow::onErrorOccurred(const QString &errorString)
{
    // 显示错误信息
    ui->statusbar->showMessage(tr("Error: %1").arg(errorString));
}

//基本游戏操作
void MainWindow::onPlayCardClicked(uint32_t cardId, uint32_t targetPlayer)
{
    sanguosha::GameMessage message;
    message.set_type(sanguosha::GAME_ACTION);
    
    sanguosha::GameAction* gameAction = new sanguosha::GameAction();
    gameAction->set_type(sanguosha::ACTION_PLAY_CARD);
    gameAction->set_card_id(cardId);
    gameAction->set_target_player(targetPlayer);
    
    message.set_allocated_game_action(gameAction);
    m_networkManager->sendMessage(message);
}

void MainWindow::onEndTurnClicked()
{
    sanguosha::GameMessage message;
    message.set_type(sanguosha::GAME_ACTION);
    
    sanguosha::GameAction* gameAction = new sanguosha::GameAction();
    gameAction->set_type(sanguosha::ACTION_END_TURN);
    
    message.set_allocated_game_action(gameAction);
    m_networkManager->sendMessage(message);
}
