#include <QApplication>
#include <QTranslator>

#include "main/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationVersion("0.0.1");

    QTranslator qtTranslator;
    qtTranslator.load(":/lang/translator/translator_cn.qm");
    a.installTranslator(&qtTranslator);

    QTranslator computeTranslator;
    computeTranslator.load(":/lang/translator/computeHash_cn.qm");
    a.installTranslator(&computeTranslator);

    MainWindow w;
    w.show();

    return a.exec();
}
