#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
    NetworkManager *m_networkManager;
};
#endif // MAINWINDOW_H
