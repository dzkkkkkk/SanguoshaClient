#include "login_dialog.h"
#include "ui_login_dialog.h"
#include "network_manager.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    
    // 连接网络管理器的信号
    NetworkManager& nm = NetworkManager::instance();
    connect(&nm, &NetworkManager::loginResponseReceived, this, &LoginDialog::onLoginResponseReceived);
    connect(&nm, &NetworkManager::connected, this, &LoginDialog::onConnected);
    connect(&nm, &NetworkManager::disconnected, this, &LoginDialog::onDisconnected);
    connect(&nm, &NetworkManager::errorOccurred, this, &LoginDialog::onErrorOccurred);
    
    // 设置默认值
    ui->serverEdit->setText("127.0.0.1");
    ui->portEdit->setText("9527");
}

LoginDialog::~LoginDialog() {
    delete ui;
}

void LoginDialog::on_connectButton_clicked() {
    QString host = ui->serverEdit->text();
    quint16 port = ui->portEdit->text().toUShort();
    
    NetworkManager::instance().connectToServer(host, port);
}

void LoginDialog::on_loginButton_clicked() {
    QString username = ui->usernameEdit->text();
    if (username.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter username");
        return;
    }
    
    if (!NetworkManager::instance().isConnected()) {
        QMessageBox::warning(this, "Error", "Not connected to server");
        return;
    }
    
    NetworkManager::instance().login(username);
}

void LoginDialog::onLoginResponseReceived(const sanguosha::LoginResponse& response) {
    if (response.success()) {
        qDebug() << "Login successful, user ID:" << response.user_id();
        // 保存用户ID，关闭登录对话框，打开大厅界面
        accept(); // 关闭对话框
    } else {
        QMessageBox::warning(this, "Login Failed", 
                           QString::fromStdString(response.error_message()));
    }
}

void LoginDialog::onConnected() {
    ui->statusLabel->setText("Connected to server");
    ui->loginButton->setEnabled(true);
}

void LoginDialog::onDisconnected() {
    ui->statusLabel->setText("Disconnected");
    ui->loginButton->setEnabled(false);
}

void LoginDialog::onErrorOccurred(const QString& errorString) {
    QMessageBox::critical(this, "Connection Error", errorString);
}