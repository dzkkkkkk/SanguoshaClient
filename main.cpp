#include <QApplication>
#include <QTextCodec>
#include <QFont>
#include <QFontDatabase>
#include "mainwindow.h"  // 确保包含 MainWindow 的头文件

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 设置编码为UTF-8
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#else
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#endif

    // 创建 QFontDatabase 对象
    QFontDatabase fontDatabase;

    // 设置应用程序字体
    QFont font;
    if (fontDatabase.families().contains("Microsoft YaHei")) {
        font.setFamily("Microsoft YaHei");
    } else if (fontDatabase.families().contains("WenQuanYi Micro Hei")) {
        font.setFamily("WenQuanYi Micro Hei");
    } else {
        font = QApplication::font();
    }
    font.setPointSize(9);
    QApplication::setFont(font);

    MainWindow w;  // 确保 MainWindow 类已正确声明
    w.show();
    return a.exec();
}