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
//#include <QFlowLayout> 拟删除


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_networkManager(new NetworkManager(this))
    , m_loginScreen(nullptr)
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

void MainWindow::handleGameState(const sanguosha::GameState &state)
{
    // 更新玩家信息表
    updatePlayerInfoTable(state);
    
    // 更新手牌显示
    updateHandCards(state);
    
    // 更新游戏日志
    updateGameLog(state);
    
    // 更新回合信息
    updateTurnInfo(state);
    
    // 更新按钮状态
    updateButtonStates(state.phase());
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
    // 1. 更新玩家信息表
    m_playerInfoTable->setRowCount(state.players_size());
    for (int i = 0; i < state.players_size(); ++i) {
        const sanguosha::PlayerState &player = state.players(i);
        
        // 设置玩家ID
        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(player.player_id()));
        m_playerInfoTable->setItem(i, 0, idItem);
        
        // 设置玩家名称
        QTableWidgetItem *nameItem = new QTableWidgetItem(QString::fromStdString(player.username()));
        m_playerInfoTable->setItem(i, 1, nameItem);
        
        // 设置玩家血量
        QTableWidgetItem *hpItem = new QTableWidgetItem(
            QString("%1/%2").arg(player.hp()).arg(player.max_hp()));
        m_playerInfoTable->setItem(i, 2, hpItem);
        
        // 高亮当前玩家
        if (player.player_id() == state.current_player()) {
            for (int col = 0; col < 3; ++col) {
                m_playerInfoTable->item(i, col)->setBackground(Qt::yellow);
            }
        }
    }
    
    // 2. 更新手牌显示
    // 先清除现有手牌
    QLayoutItem *item;
    while ((item = m_handCardsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    // 查找当前玩家手牌
    for (int i = 0; i < state.players_size(); ++i) {
        const sanguosha::PlayerState &player = state.players(i);
        if (player.player_id() == m_selfUserId) { // 假设m_selfUserId已保存
            // 添加手牌按钮
            for (int j = 0; j < player.hand_cards_size(); ++j) {
                uint32_t cardId = player.hand_cards(j);
                QPushButton *cardButton = new QPushButton(QString::number(cardId));
                cardButton->setProperty("cardId", cardId);
                connect(cardButton, &QPushButton::clicked, this, &MainWindow::onCardSelected);
                m_handCardsLayout->addWidget(cardButton);
            }
            break;
        }
    }
    
    // 3. 更新回合信息
    m_turnInfoLabel->setText(tr("当前回合: 玩家%1, 阶段: %2")
                            .arg(state.current_player())
                            .arg(state.phase()));
    
    // 4. 根据游戏阶段启用/禁用按钮
    updateButtonStates(state.phase());
}

// 处理游戏开始
void MainWindow::handleGameStart(const sanguosha::GameStart &start)
{
    ui->statusbar->showMessage(tr("游戏开始！"));
    
    // 保存自己的用户ID
    m_selfUserId = start.player_id();
    
    // 初始化游戏界面
    setupGameScreen();
    showScreen(m_gameScreen);
    
    // 清空游戏日志
    m_gameLog->clear();
    m_gameLog->append(tr("游戏开始！"));
    
    // 请求初始游戏状态
    sanguosha::GameMessage message;
    message.set_type(sanguosha::GAME_STATE_REQUEST);
    m_networkManager->sendMessage(message);
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
    
    // 修正lambda捕获 - 确保roomTable被正确捕获
    connect(joinBtn, &QPushButton::clicked, this, [this, roomTable]() {
        QList<QTableWidgetItem*> selected = roomTable->selectedItems();
        if (!selected.isEmpty()) {
            int row = selected.first()->row();
            int roomId = roomTable->item(row, 0)->text().toInt();
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

void MainWindow::onPlayCardButtonClicked()
{
    if (m_selectedCard == 0) return;
    
    // 这里需要根据游戏逻辑确定目标玩家
    // 暂时简化：弹出对话框选择目标
    QDialog dialog(this);
    QVBoxLayout layout(&dialog);
    QComboBox playerCombo;
    
    // 添加玩家选项
    for (int i = 0; i < m_playerInfoTable->rowCount(); ++i) {
        int playerId = m_playerInfoTable->item(i, 0)->text().toInt();
        QString playerName = m_playerInfoTable->item(i, 1)->text();
        if (playerId != m_selfUserId) { // 不能选择自己
            playerCombo.addItem(QString("%1: %2").arg(playerId).arg(playerName), playerId);
        }
    }
    
    layout.addWidget(new QLabel(tr("选择目标玩家:")));
    layout.addWidget(&playerCombo);
    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout.addWidget(&buttons);
    
    connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        uint32_t targetPlayer = playerCombo.currentData().toUInt();
        onPlayCardClicked(m_selectedCard, targetPlayer);
        
        // 重置选择状态
        m_selectedCard = 0;
        m_playCardButton->setEnabled(false);
        
        // 清除高亮
        for (int i = 0; i < m_handCardsLayout->count(); ++i) {
            if (QPushButton *btn = qobject_cast<QPushButton*>(m_handCardsLayout->itemAt(i)->widget())) {
                btn->setStyleSheet("");
            }
        }
    }
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
void MainWindow::updateButtonStates(uint32_t phase)
{
    // 根据游戏阶段启用/禁用按钮
    // 这里需要根据实际游戏规则实现
    bool canPlayCard = (phase == 1); // 假设阶段1是出牌阶段
    bool canEndTurn = true;
    
    m_playCardButton->setEnabled(canPlayCard && m_selectedCard != 0);
    m_endTurnButton->setEnabled(canEndTurn);
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
    if (state.has_game_log() && !state.game_log().empty()) {
        QString logEntry = QString::fromStdString(state.game_log());
        if (m_gameLog->toPlainText().isEmpty() || 
            !m_gameLog->toPlainText().endsWith(logEntry)) {
            m_gameLog->append(logEntry);
            m_gameLog->moveCursor(QTextCursor::End);
        }
    }
}

//回合管理
void MainWindow::updateTurnInfo(const sanguosha::GameState &state)
{
    QString phaseName;
    switch (state.phase()) {
        case sanguosha::DRAW_PHASE: phaseName = tr("摸牌阶段"); break;
        case sanguosha::PLAY_PHASE: phaseName = tr("出牌阶段"); break;
        case sanguosha::DISCARD_PHASE: phaseName = tr("弃牌阶段"); break;
        default: phaseName = tr("未知阶段");
    }
    
    m_turnInfoLabel->setText(tr("当前回合: 玩家%1, 阶段: %2")
                            .arg(state.current_player())
                            .arg(phaseName));
}

void MainWindow::updateButtonStates(uint32_t phase)
{
    bool isMyTurn = false;
    // 检查是否是自己的回合（需要从游戏状态中获取当前玩家）
    
    bool canPlayCard = (phase == sanguosha::PLAY_PHASE) && isMyTurn;
    bool canEndTurn = isMyTurn;
    
    m_playCardButton->setEnabled(canPlayCard && m_selectedCard != 0);
    m_endTurnButton->setEnabled(canEndTurn);
}