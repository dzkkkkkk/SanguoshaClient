#include "mainwindow.h"
#include <QApplication>
#include <QFont>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // 设置应用程序字体，优先使用微软雅黑，其次是文泉驿微米黑
    QFont font;
    if (QFontDatabase::families().contains("Microsoft YaHei")) {
        font.setFamily("Microsoft YaHei");
    } else if (QFontDatabase::families().contains("WenQuanYi Micro Hei")) {
        font.setFamily("WenQuanYi Micro Hei");
    } else {
        // 使用系统默认字体，但设置合适的字号
        font = QApplication::font();
    }
    font.setPointSize(9);
    QApplication::setFont(font);
    
    MainWindow w;
    w.show();
    return a.exec();
}