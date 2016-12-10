#include "main/mainwindows.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationVersion("0.0.1");

    QTranslator qtTranslator;
    qtTranslator.load(":/lang/translator/translator_cn.qm");
    a.installTranslator(&qtTranslator);

    MainWindows w;
    w.show();

    return a.exec();
}
