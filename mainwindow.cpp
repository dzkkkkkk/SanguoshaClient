#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_networkManager(new NetworkManager(this))
    , m_loginScreen(nullptr)
    , m_lobbyScreen(nullptr)
{
    ui->setupUi(this);

    // 初始化登录界面
    setupLoginScreen();
    // 默认显示登录界面
    showScreen(m_loginScreen);

    // ... 其他现有的连接信号槽的代码
    connect(m_networkManager, &NetworkManager::connectionStatusChanged, this, &MainWindow::onConnectionStatusChanged);
    connect(m_networkManager, &NetworkManager::messageReceived, this, &MainWindow::onMessageReceived);
    connect(m_networkManager, &NetworkManager::errorOccurred, this, &MainWindow::onErrorOccurred);

    // 连接到服务器，端口号已改为9527
    m_networkManager->connectToServer("127.0.0.1", 9527);
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

// 登录处理
void MainWindow::onLoginButtonClicked(const QString &username, const QString &password)
{
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

// 房间创建
void MainWindow::onCreateRoomClicked()
{
    sanguosha::GameMessage message;
    message.set_type(sanguosha::ROOM_REQUEST);

    sanguosha::RoomRequest* roomRequest = new sanguosha::RoomRequest();
    roomRequest->set_action(sanguosha::CREATE_ROOM);

    message.set_allocated_room_request(roomRequest);
    m_networkManager->sendMessage(message);
}

// 房间加入
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

// 基本游戏操作
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

// 处理登录响应
void MainWindow::handleLoginResponse(const sanguosha::LoginResponse &response)
{
    if (response.success()) {
        ui->statusbar->showMessage(tr("登录成功！"));
        // 初始化并切换到大厅界面
        setupLobbyScreen();
        showScreen(m_lobbyScreen);
        // 可以在这里请求房间列表
    } else {
        QMessageBox::warning(this, tr("登录失败"), 
                            QString::fromStdString(response.error_message()));
    }
}

// 处理房间响应
void MainWindow::handleRoomResponse(const sanguosha::RoomResponse &response)
{
    if (response.success()) {
        ui->statusbar->showMessage(tr("房间操作成功"));
        if (response.has_room_info()) {
            // 更新房间界面信息
        }
    } else {
        QMessageBox::warning(this, tr("房间操作失败"), 
                            QString::fromStdString(response.error_message()));
    }
}


// 处理游戏状态更新
void MainWindow::handleGameState(const sanguosha::GameState &state)
{
    // 更新玩家状态
    for (int i = 0; i < state.player_states_size(); ++i) {
        const sanguosha::PlayerState &player = state.player_states(i);
        // 更新UI中的玩家血量、手牌数等信息
    }

    // 更新当前回合信息
    if (state.has_current_player()) {
        // 高亮显示当前回合玩家
    }

    // 更新手牌信息
    if (state.has_hand_cards()) {
        // 清空并重新加载手牌显示
    }

    // 其他游戏状态更新...
}

// 处理游戏开始
void MainWindow::handleGameStart(const sanguosha::GameStart &start)
{
    ui->statusbar->showMessage(tr("游戏开始！"));
    // 这里需要初始化并切换到游戏界面
    // setupGameScreen();
    // showScreen(m_gameScreen);
}

//界面初始化函数
void MainWindow::setupLoginScreen()
{
    // 如果已经创建，则直接返回
    if (m_loginScreen) return;

    m_loginScreen = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(m_loginScreen);

    QLineEdit *usernameEdit = new QLineEdit;
    usernameEdit->setPlaceholderText(tr("用户名"));
    layout->addWidget(usernameEdit);

    QLineEdit *passwordEdit = new QLineEdit;
    passwordEdit->setPlaceholderText(tr("密码"));
    passwordEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordEdit);

    QPushButton *loginButton = new QPushButton(tr("登录"));
    layout->addWidget(loginButton);

    // 连接登录按钮的信号
    connect(loginButton, &QPushButton::clicked, this, [this, usernameEdit, passwordEdit]() {
        QString username = usernameEdit->text();
        QString password = passwordEdit->text();
        if (!username.isEmpty() && !password.isEmpty()) {
            onLoginButtonClicked(username, password);
        }
    });
}

void MainWindow::showScreen(QWidget *screen)
{
    // 隐藏所有可能的屏幕
    if (m_loginScreen) m_loginScreen->hide();
    if (m_lobbyScreen) m_lobbyScreen->hide();

    // 显示请求的屏幕
    if (screen) {
        screen->show();
        setCentralWidget(screen);
    }
}

//大厅创建界面
void MainWindow::setupLobbyScreen()
{
    if (m_lobbyScreen) return;

    m_lobbyScreen = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(m_lobbyScreen);

    // 房间列表
    QTableWidget *roomTable = new QTableWidget(0, 3); // 3列：ID, 玩家数, 状态
    roomTable->setHorizontalHeaderLabels(QStringList() << tr("房间ID") << tr("玩家") << tr("状态"));
    mainLayout->addWidget(roomTable);

    // 按钮区域
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *createRoomBtn = new QPushButton(tr("创建房间"));
    QPushButton *refreshBtn = new QPushButton(tr("刷新列表"));
    QPushButton *joinBtn = new QPushButton(tr("加入房间"));

    buttonLayout->addWidget(createRoomBtn);
    buttonLayout->addWidget(refreshBtn);
    buttonLayout->addWidget(joinBtn);
    mainLayout->addLayout(buttonLayout);

    // 连接信号
    connect(createRoomBtn, &QPushButton::clicked, this, &MainWindow::onCreateRoomClicked);
    connect(refreshBtn, &QPushButton::clicked, this, [this]() {
        // 实现刷新房间列表的请求
    });
    connect(joinBtn, &QPushButton::clicked, this, [this, roomTable]() {
        QList<QTableWidgetItem*> selected = roomTable->selectedItems();
        if (!selected.isEmpty()) {
            int roomId = roomTable->item(selected.first()->row(), 0)->text().toInt();
            onJoinRoomClicked(roomId);
        }
    });
}

//游戏界面初始化
void MainWindow::setupGameScreen()
{
    if (m_gameScreen) return;

    m_gameScreen = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(m_gameScreen);

    // 左侧：玩家信息面板
    QVBoxLayout *playerLayout = new QVBoxLayout();
    // 这里添加玩家状态、血量等信息显示
    mainLayout->addLayout(playerLayout, 1); // 1表示占比

    // 中间：游戏区域
    QVBoxLayout *gameLayout = new QVBoxLayout();
    // 这里添加游戏牌桌、角色显示等
    mainLayout->addLayout(gameLayout, 2);

    // 右侧：手牌和操作区域
    QVBoxLayout *handLayout = new QVBoxLayout();
    // 这里添加手牌显示和操作按钮
    QPushButton *endTurnBtn = new QPushButton(tr("结束回合"));
    handLayout->addWidget(endTurnBtn);
    connect(endTurnBtn, &QPushButton::clicked, this, &MainWindow::onEndTurnClicked);
    mainLayout->addLayout(handLayout, 1);
}