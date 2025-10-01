#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
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
#include <QThread>
//#include <QFlowLayout> 拟删除


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_networkManager(&NetworkManager::instance())
    //, m_loginScreen(nullptr) 移除
    , m_lobbyScreen(nullptr)
    , m_gameScreen(nullptr)
    , m_playerInfoTable(nullptr)
    , m_gameLog(nullptr)
    , m_deckCountLabel(nullptr)
    , m_gameArea(nullptr)
    , m_turnInfoLabel(nullptr)
    , m_handCardsLayout(nullptr)
    , m_playCardButton(nullptr)
    , m_endTurnButton(nullptr)
    , m_cancelButton(nullptr)
    , m_selectedCard(0)
    , m_selfUserId(0)
{
    ui->setupUi(this);
    
    // 设置窗口标题
    this->setWindowTitle(tr("三国杀客户端"));
    
    // 设置状态栏初始消息
    ui->statusbar->showMessage(tr("正在连接服务器..."));

    // 添加连接超时检查
    QTimer::singleShot(5000, this, [this]() {
        if (!m_networkManager->isConnected()) {
            ui->statusbar->showMessage(tr("连接服务器失败，请检查服务器状态"));
            QMessageBox::warning(this, tr("连接失败"), 
                                tr("无法连接到服务器，请确保服务器已启动"));
        }
    });

    m_roomOperationTimer = new QTimer(this);
    m_roomOperationTimer->setSingleShot(true);
    connect(m_roomOperationTimer, &QTimer::timeout, this, [this]() {
    if (!m_networkManager->isConnected()) {
        QMessageBox::warning(this, tr("操作超时"), 
                            tr("网络操作超时，请检查网络连接"));
        }
    });

    // 直接使用UI文件中的组件
    connect(ui->loginButton, &QPushButton::clicked, this, [this]() {
        QString username = ui->usernameEdit->text();
        QString password = ui->passwordEdit->text();
        if (!username.isEmpty() && !password.isEmpty()) {
            onLoginButtonClicked(username, password);
        }
    });

    // 修改连接信号槽的代码
    connect(m_networkManager, &NetworkManager::connected, this, [this]() { onConnectionStatusChanged(true); });
    connect(m_networkManager, &NetworkManager::disconnected, this, [this]() { onConnectionStatusChanged(false); });

    connect(m_networkManager, &NetworkManager::loginResponseReceived, this, &MainWindow::handleLoginResponse);
    connect(m_networkManager, &NetworkManager::roomResponseReceived, this, &MainWindow::handleRoomResponse);
    connect(m_networkManager, &NetworkManager::gameStateReceived, this, &MainWindow::handleGameState);
    connect(m_networkManager, &NetworkManager::gameStartReceived, this, &MainWindow::handleGameStart);

    // 添加房间列表响应信号连接
    connect(m_networkManager, &NetworkManager::roomListResponseReceived, this, &MainWindow::handleRoomListResponse);

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



// 房间创建
void MainWindow::onCreateRoomClicked()
{
    m_roomOperationTimer->start(5000); // 5秒超时

    sanguosha::GameMessage message;
    message.set_type(sanguosha::ROOM_REQUEST);

    sanguosha::RoomRequest* roomRequest = new sanguosha::RoomRequest();
    roomRequest->set_action(sanguosha::CREATE_ROOM);

    message.set_allocated_room_request(roomRequest);
    m_networkManager->sendMessage(message);
}

// 房间加入
void MainWindow::onJoinRoomClicked(uint32_t roomId) {
    m_roomOperationTimer->start(5000); // 5秒超时
    
    sanguosha::GameMessage message;
    message.set_type(sanguosha::ROOM_REQUEST);
    
    sanguosha::RoomRequest* roomRequest = new sanguosha::RoomRequest();
    roomRequest->set_action(sanguosha::JOIN_ROOM);
    roomRequest->set_room_id(roomId);
    
    message.set_allocated_room_request(roomRequest);
    m_networkManager->sendMessage(message);
    
    qDebug() << "Join room request sent for room:" << roomId;
}

void MainWindow::onErrorOccurred(const QString &errorString)
{
    QMessageBox::critical(this, tr("网络错误"), 
                         tr("发生网络错误: %1").arg(errorString));
    
    // 尝试重新连接
    QTimer::singleShot(3000, this, [this]() {
        m_networkManager->connectToServer("127.0.0.1", 9527);
    });
}

// 基本游戏操作
void MainWindow::onPlayCardClicked(uint32_t cardId, uint32_t targetPlayer)
{
    // 根据卡牌类型执行不同的逻辑
    QString cardName = getCardName(cardId);
    
    if (cardName == "杀") {
        // 处理杀牌逻辑
        sanguosha::GameMessage message;
        message.set_type(sanguosha::GAME_ACTION);
        
        sanguosha::GameAction* gameAction = new sanguosha::GameAction();
        gameAction->set_type(sanguosha::ACTION_PLAY_CARD);
        gameAction->set_card_id(cardId);
        gameAction->set_target_player(targetPlayer);
        
        message.set_allocated_game_action(gameAction);
        m_networkManager->sendMessage(message);
        
        // 添加到游戏日志
        //addToGameLog(tr("您对玩家%1使用了【杀】").arg(targetPlayer));    移除
        m_gameLog->append(tr("您对玩家%1使用了【杀】").arg(targetPlayer));
    }
    else if (cardName == "闪") {
        // 处理闪牌逻辑
        // ...
    }
    else if (cardName == "桃") {
        // 处理桃牌逻辑
        // ...
    }
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
    m_roomOperationTimer->stop();
    
    if (response.success()) {
        ui->statusbar->showMessage(tr("房间操作成功"));
        
        if (response.has_room_info()) {
            const sanguosha::RoomInfo& roomInfo = response.room_info();
            
            // 显示房间信息
            QString message = tr("房间ID: %1, 玩家: %2/%3, 状态: %4")
                .arg(roomInfo.room_id())
                .arg(roomInfo.current_players())
                .arg(roomInfo.max_players())
                .arg(roomInfo.status() == sanguosha::WAITING ? tr("等待中") : tr("游戏中"));
                
            QMessageBox::information(this, tr("房间操作成功"), message);
            
            // 如果是创建房间，等待其他玩家加入
            if (response.room_info().current_players() == 1) {
                ui->statusbar->showMessage(tr("房间已创建，等待其他玩家加入..."));
            }
        }
    } else {
        QMessageBox::warning(this, tr("房间操作失败"), 
                            QString::fromStdString(response.error_message()));
    }
}


// 处理游戏状态更新
void MainWindow::handleGameState(const sanguosha::GameState& state) {
    // 更新玩家信息
    updatePlayerInfoTable(state);

    // 更新手牌
    updateHandCards(state);

    // 更新回合信息
    updateTurnInfo(state);

    // 确定是否是我的回合
    bool isMyTurn = (state.current_player() == m_selfUserId);

    // 更新按钮状态
    updateButtonStates(state.phase(), isMyTurn);

    // 检查游戏结束条件
    checkGameEndCondition(state);

    // 更新游戏日志
    if (!state.game_log().empty()) {
        addToGameLog(QString::fromStdString(state.game_log()));
    }
}

// 处理游戏开始
// mainwindow.cpp - 修复handleGameStart函数
void MainWindow::handleGameStart(const sanguosha::GameStart &start)
{
    qDebug() << "Handling game start, room ID:" << start.room_id();
    qDebug() << "Player IDs count:" << start.player_ids_size();
    qDebug() << "Current thread:" << QThread::currentThread();
    qDebug() << "Game screen pointer:" << m_gameScreen;
    qDebug() << "Game log pointer:" << m_gameLog;
    
    // 添加更严格的安全检查
    if (start.player_ids_size() < 2) {
        qWarning() << "GameStart message has insufficient player ids!";
        return;
    }
    
    // 确保在UI线程中执行界面操作
    QMetaObject::invokeMethod(this, [this, start]() {
        ui->statusbar->showMessage(tr("游戏开始！"));
        
        // 确保游戏界面已初始化
        if (!m_gameScreen) {
            setupGameScreen();
        }
        
        showScreen(m_gameScreen);
        
        // 清空游戏日志 - 添加空指针检查
        if (m_gameLog) {
            m_gameLog->clear();
            m_gameLog->append(tr("游戏开始！"));
        } else {
            qWarning() << "Game log widget is not initialized!";
            // 尝试重新初始化
            setupGameScreen();
            if (m_gameLog) {
                m_gameLog->clear();
                m_gameLog->append(tr("游戏开始！"));
            }
        }
    }, Qt::QueuedConnection);
}

//界面初始化函数
void MainWindow::setupLoginScreen()   //拟移除
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
    ui->centralwidget->setVisible(false);
    if (m_lobbyScreen) m_lobbyScreen->hide();
    if (m_gameScreen) m_gameScreen->hide();

    // 显示请求的屏幕
    if (screen) {
        screen->show();
        setCentralWidget(screen);
    } else {
        // 显示主UI
        ui->centralwidget->setVisible(true);
        setCentralWidget(ui->centralwidget);
    }
}

//大厅创建界面
void MainWindow::setupLobbyScreen()
{
    if (m_lobbyScreen) return;

    m_lobbyScreen = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(m_lobbyScreen);

    // 房间列表
    QTableWidget *roomTable = new QTableWidget(0, 3);
    roomTable->setHorizontalHeaderLabels(QStringList() << tr("房间ID") << tr("玩家") << tr("状态"));
    roomTable->setSelectionBehavior(QAbstractItemView::SelectRows);
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
    connect(refreshBtn, &QPushButton::clicked, this, [this, roomTable]() {
        // 实现刷新房间列表的请求
        requestRoomList(roomTable);
    });
    
    // 修正lambda捕获
    connect(joinBtn, &QPushButton::clicked, this, [this, roomTable]() {
        QList<QTableWidgetItem*> selected = roomTable->selectedItems();
        if (!selected.isEmpty()) {
            int row = selected.first()->row();
            int roomId = roomTable->item(row, 0)->text().toInt();
            onJoinRoomClicked(roomId);
        } else {
            QMessageBox::warning(this, tr("选择房间"), tr("请先选择一个房间"));
        }
    });
    
    // 初始刷新房间列表
    requestRoomList(roomTable);
}

//游戏界面初始化
// mainwindow.cpp - 修复setupGameScreen函数
void MainWindow::setupGameScreen()
{
    if (m_gameScreen) {
        // 如果已经存在，先清理再重新创建
        delete m_gameScreen;
        m_gameScreen = nullptr;
    }

    // 确保所有成员变量都被正确初始化
    m_playerInfoTable = nullptr;
    m_gameLog = nullptr;
    m_deckCountLabel = nullptr;
    m_gameArea = nullptr;
    m_turnInfoLabel = nullptr;
    m_handCardsLayout = nullptr;
    m_playCardButton = nullptr;
    m_endTurnButton = nullptr;
    m_cancelButton = nullptr;

    m_gameScreen = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(m_gameScreen);
    
    // 左侧：玩家信息面板
    QVBoxLayout *leftLayout = new QVBoxLayout();
    m_playerInfoTable = new QTableWidget(0, 3); // 玩家ID, 名称, 血量
    m_playerInfoTable->setHorizontalHeaderLabels(QStringList() << tr("ID") << tr("名称") << tr("血量"));
    m_playerInfoTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    leftLayout->addWidget(m_playerInfoTable);
    
    // 游戏日志
    m_gameLog = new QTextEdit();
    m_gameLog->setReadOnly(true);
    leftLayout->addWidget(m_gameLog);
    mainLayout->addLayout(leftLayout, 1);
    
    // 中间：游戏区域
    QVBoxLayout *centerLayout = new QVBoxLayout();
    
    // 牌堆区域（暂时用标签表示）
    QHBoxLayout *deckLayout = new QHBoxLayout();
    deckLayout->addWidget(new QLabel(tr("牌堆:")));
    m_deckCountLabel = new QLabel("0");
    deckLayout->addWidget(m_deckCountLabel);
    deckLayout->addStretch();
    centerLayout->addLayout(deckLayout);
    
    // 游戏桌面（用于显示打出的牌）
    m_gameArea = new QLabel(tr("游戏桌面"));
    m_gameArea->setAlignment(Qt::AlignCenter);
    m_gameArea->setFrameStyle(QFrame::Box);
    m_gameArea->setMinimumSize(400, 200);
    centerLayout->addWidget(m_gameArea);
    
    // 当前回合信息
    m_turnInfoLabel = new QLabel();
    centerLayout->addWidget(m_turnInfoLabel);
    
    mainLayout->addLayout(centerLayout, 2);
    
    // 右侧：手牌和操作区域
    QVBoxLayout *rightLayout = new QVBoxLayout();
    
    // 手牌区域
    m_handCardsLayout = new QHBoxLayout(); // 或者 QGridLayout
    QWidget *handCardsWidget = new QWidget();
    handCardsWidget->setLayout(m_handCardsLayout);
    
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(handCardsWidget);
    scrollArea->setWidgetResizable(true);
    rightLayout->addWidget(scrollArea);
    
    // 操作按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_playCardButton = new QPushButton(tr("出牌"));
    m_endTurnButton = new QPushButton(tr("结束回合"));
    m_cancelButton = new QPushButton(tr("取消"));
    
    buttonLayout->addWidget(m_playCardButton);
    buttonLayout->addWidget(m_endTurnButton);
    buttonLayout->addWidget(m_cancelButton);
    rightLayout->addLayout(buttonLayout);
    
    mainLayout->addLayout(rightLayout, 1);
    
    // 连接信号
    connect(m_playCardButton, &QPushButton::clicked, this, &MainWindow::onPlayCardButtonClicked);
    connect(m_endTurnButton, &QPushButton::clicked, this, &MainWindow::onEndTurnClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &MainWindow::onCancelButtonClicked);
    
    // 初始状态
    m_playCardButton->setEnabled(false);
    m_cancelButton->setEnabled(false);

    // 确保所有控件都已创建
    Q_ASSERT(m_playerInfoTable != nullptr);
    Q_ASSERT(m_gameLog != nullptr);
    Q_ASSERT(m_deckCountLabel != nullptr);
    Q_ASSERT(m_gameArea != nullptr);
    Q_ASSERT(m_turnInfoLabel != nullptr);
    Q_ASSERT(m_handCardsLayout != nullptr);
    Q_ASSERT(m_playCardButton != nullptr);
    Q_ASSERT(m_endTurnButton != nullptr);
    Q_ASSERT(m_cancelButton != nullptr);
}

//卡牌使用逻辑
void MainWindow::onCardSelected()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        uint32_t cardId = button->property("cardId").toUInt();
        m_selectedCard = cardId;
        
        // 高亮选中的卡牌
        for (int i = 0; i < m_handCardsLayout->count(); ++i) {
            QWidget *widget = m_handCardsLayout->itemAt(i)->widget();
            if (QPushButton *btn = qobject_cast<QPushButton*>(widget)) {
                btn->setStyleSheet(btn == button ? "background-color: lightblue;" : "");
            }
        }
        
        m_playCardButton->setEnabled(true);
    }
}

// 修改onPlayCardButtonClicked，直接确定目标
void MainWindow::onPlayCardButtonClicked() {
    if (m_selectedCard == 0) return;

    // 在1v1中，目标要么是对手，要么是自己（对于桃）
    uint32_t targetPlayer = m_selfUserId; // 默认是自己
    QString cardName = getCardName(m_selectedCard);

    // 如果是杀，目标是对手
    if (cardName == "杀") {
        // 查找对手ID
        for (int i = 0; i < m_playerInfoTable->rowCount(); ++i) {
            int playerId = m_playerInfoTable->item(i, 0)->text().toInt();
            if (playerId != m_selfUserId) {
                targetPlayer = playerId;
                break;
            }
        }
    }

    onPlayCardClicked(m_selectedCard, targetPlayer);

    // 重置选择状态
    m_selectedCard = 0;
    m_playCardButton->setEnabled(false);
}

void MainWindow::onCancelButtonClicked()
{
    // 取消当前选中的卡牌
    m_selectedCard = 0;
    m_playCardButton->setEnabled(false);
    
    // 清除高亮
    for (int i = 0; i < m_handCardsLayout->count(); ++i) {
        if (QPushButton *btn = qobject_cast<QPushButton*>(m_handCardsLayout->itemAt(i)->widget())) {
            btn->setStyleSheet("");
        }
    }
}

//禁将
void MainWindow::updateButtonStates(sanguosha::GamePhase phase, bool isMyTurn)
{
    bool canPlayCard = false;
    bool canEndTurn = false;
    
    // 根据游戏阶段和是否是自己回合决定按钮状态
    switch (phase) {
    case sanguosha::DRAW_PHASE: // 使用proto中定义的枚举值
        canPlayCard = false;
        canEndTurn = isMyTurn;
        break;
    case sanguosha::PLAY_PHASE:
        canPlayCard = isMyTurn && m_selectedCard != 0;
        canEndTurn = isMyTurn;
        break;
    case sanguosha::DISCARD_PHASE:
        canPlayCard = false;
        canEndTurn = isMyTurn;
        break;
    default:
        canPlayCard = false;
        canEndTurn = false;
    }
    
    m_playCardButton->setEnabled(canPlayCard);
    m_endTurnButton->setEnabled(canEndTurn);
    
    // 更新状态栏信息
    if (isMyTurn) {
        ui->statusbar->showMessage(tr("轮到您的回合，请操作"));
    } else {
        ui->statusbar->showMessage(tr("等待其他玩家操作"));
    }
}

//玩家信息列表
void MainWindow::updatePlayerInfoTable(const sanguosha::GameState &state)
{
    m_playerInfoTable->setRowCount(state.players_size());
    for (int i = 0; i < state.players_size(); ++i) {
        const sanguosha::PlayerState &player = state.players(i);
        
        // 设置玩家ID
        m_playerInfoTable->setItem(i, 0, new QTableWidgetItem(QString::number(player.player_id())));
        
        // 设置玩家名称
        m_playerInfoTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(player.username())));
        
        // 设置玩家血量
        m_playerInfoTable->setItem(i, 2, new QTableWidgetItem(
            QString("%1/%2").arg(player.hp()).arg(player.max_hp())));
        
        // 高亮当前玩家
        for (int col = 0; col < 3; ++col) {
            if (player.player_id() == state.current_player()) {
                m_playerInfoTable->item(i, col)->setBackground(Qt::yellow);
            } else {
                m_playerInfoTable->item(i, col)->setBackground(Qt::white);
            }
        }
    }
}

//手牌显示
void MainWindow::updateHandCards(const sanguosha::GameState &state)
{
    // 清除现有手牌
    QLayoutItem *item;
    while ((item = m_handCardsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    // 查找当前玩家手牌
    for (int i = 0; i < state.players_size(); ++i) {
        const sanguosha::PlayerState &player = state.players(i);
        if (player.player_id() == m_selfUserId) {
            // 添加手牌按钮
            for (int j = 0; j < player.hand_cards_size(); ++j) {
                uint32_t cardId = player.hand_cards(j);
                addCardToHand(cardId);
            }
            break;
        }
    }
}

//添加手牌
void MainWindow::addCardToHand(uint32_t cardId)
{
    QPushButton *cardButton = new QPushButton(getCardName(cardId));
    cardButton->setProperty("cardId", cardId);
    cardButton->setMinimumSize(80, 120);
    cardButton->setMaximumSize(80, 120);
    
    // 根据卡牌类型设置不同颜色
    QString style = QString("QPushButton { background-color: %1; border: 2px solid black; }")
                   .arg(getCardColor(cardId));
    cardButton->setStyleSheet(style);
    
    connect(cardButton, &QPushButton::clicked, this, &MainWindow::onCardSelected);
    m_handCardsLayout->addWidget(cardButton);
}

QString MainWindow::getCardName(uint32_t cardId)
{
    // 简单的卡牌ID到名称映射
    static QMap<uint32_t, QString> cardNames = {
        {1, "杀"}, {2, "闪"}, {3, "桃"},
        {4, "过河拆桥"}, {5, "顺手牵羊"}, {6, "无中生有"}
    };
    return cardNames.value(cardId, QString::number(cardId));
}

QString MainWindow::getCardColor(uint32_t cardId)
{
    // 根据卡牌类型设置颜色
    if (cardId == 1) return "#ff6666"; // 杀 - 红色
    if (cardId == 2) return "#66aaff"; // 闪 - 蓝色  
    if (cardId == 3) return "#66ff66"; // 桃 - 绿色
    return "#ffff66"; // 其他 - 黄色
}

//日志系统
void MainWindow::updateGameLog(const sanguosha::GameState &state)
{
    // 修复：GameState没有game_log字段，需要从其他地方获取日志
    // 这里可以添加其他方式获取游戏日志的逻辑
    // 例如从网络消息中获取或维护本地日志
}

//回合管理
void MainWindow::updateTurnInfo(const sanguosha::GameState &state)
{
    QString phaseName;
    
    // 修复：使用正确的阶段枚举值
    // 根据proto文件，phase是uint32，没有定义具体枚举值
    // 这里需要与服务器协商阶段值的含义
    uint32_t phase = state.phase();
    
    // 假设阶段值约定：
    // 1: 摸牌阶段, 2: 出牌阶段, 3: 弃牌阶段
    switch (phase) {
        case 1: phaseName = tr("摸牌阶段"); break;
        case 2: phaseName = tr("出牌阶段"); break;
        case 3: phaseName = tr("弃牌阶段"); break;
        default: phaseName = tr("未知阶段");
    }
    
    m_turnInfoLabel->setText(tr("当前回合: 玩家%1, 阶段: %2")
                            .arg(state.current_player())
                            .arg(phaseName));
}



// 修复onMessageReceived函数
void MainWindow::onMessageReceived(const sanguosha::GameMessage &message)
{
    switch (message.type()) {
    // ... 其他case语句
    case sanguosha::GAME_OVER:
        handleGameOver(message.game_over());
        break;
    // ...
    }
}

void MainWindow::handleGameOver(const sanguosha::GameOver& gameOver) {
    if (gameOver.winner_id() == m_selfUserId) {
        addToGameLog("恭喜！你获得了胜利！");
        QMessageBox::information(this, "游戏结束", "你赢了！");
    } else {
        addToGameLog("游戏结束，你输了。");
        QMessageBox::information(this, "游戏结束", "你输了！");
    }
    
    // 返回大厅
    showScreen(m_lobbyScreen);
}

//调试功能
void MainWindow::debugSendTestMessage()
{
    // 仅在调试模式下使用
    #ifdef QT_DEBUG
    sanguosha::GameMessage message;
    message.set_type(sanguosha::GAME_ACTION);
    
    sanguosha::GameAction* gameAction = new sanguosha::GameAction();
    gameAction->set_type(sanguosha::ACTION_PLAY_CARD);
    gameAction->set_card_id(1); // 测试卡牌
    gameAction->set_target_player(2); // 测试目标
    
    message.set_allocated_game_action(gameAction);
    m_networkManager->sendMessage(message);
    #endif
}

void MainWindow::addToGameLog(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    m_gameLog->append(QString("[%1] %2").arg(timestamp).arg(message));
    m_gameLog->moveCursor(QTextCursor::End);
}

void MainWindow::resetGameState()
{
    m_selectedCard = 0;
    m_playCardButton->setEnabled(false);
    m_endTurnButton->setEnabled(false);
}

void MainWindow::checkGameEndCondition(const sanguosha::GameState &state)
{
    // 检查是否有玩家死亡
    for (int i = 0; i < state.players_size(); ++i) {
        const sanguosha::PlayerState &player = state.players(i);
        if (player.hp() <= 0) {
            // 玩家死亡，游戏结束
            addToGameLog(tr("玩家%1死亡").arg(player.player_id()));
            // 服务器应该会发送GAME_OVER消息
            return;
        }
    }
}

// 添加请求房间列表的函数
void MainWindow::requestRoomList(QTableWidget* roomTable)
{
    sanguosha::GameMessage message;
    message.set_type(sanguosha::ROOM_LIST_REQUEST);
    
    // 清空现有房间列表
    roomTable->setRowCount(0);
    
    // 发送请求
    m_networkManager->sendMessage(message);
    
    ui->statusbar->showMessage(tr("正在获取房间列表..."));
}

void MainWindow::handleRoomListResponse(const sanguosha::RoomListResponse &response)
{
    // 获取大厅界面中的房间表格
    QTableWidget* roomTable = m_lobbyScreen->findChild<QTableWidget*>();
    if (!roomTable) return;
    
    // 清空现有房间列表
    roomTable->setRowCount(0);
    
    // 添加房间信息
    for (int i = 0; i < response.rooms_size(); i++) {
        const sanguosha::RoomInfo& roomInfo = response.rooms(i);
        
        int row = roomTable->rowCount();
        roomTable->insertRow(row);
        
        roomTable->setItem(row, 0, new QTableWidgetItem(QString::number(roomInfo.room_id())));
        roomTable->setItem(row, 1, new QTableWidgetItem(
            QString("%1/%2").arg(roomInfo.current_players()).arg(roomInfo.max_players())));
        
        // 根据房间状态设置状态文本
        QString statusText;
        switch (roomInfo.status()) {
            case sanguosha::WAITING: statusText = tr("等待中"); break;
            case sanguosha::PLAYING: statusText = tr("游戏中"); break;
            default: statusText = tr("未知");
        }
        roomTable->setItem(row, 2, new QTableWidgetItem(statusText));
    }
    
    ui->statusbar->showMessage(tr("房间列表已更新"));
}