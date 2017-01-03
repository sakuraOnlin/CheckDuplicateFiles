#include <QCoreApplication>
#include <QApplication>
#include <QFileDialog>
#include "computedemo.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString file(QFileDialog::getOpenFileName(NULL, "Open File",".", "All (*.*)"));

    ComputeDemo demo((util::CRC32 | util::SHA1 | util::MD5) ,file);

    return a.exec();
}

