#include "help.h"
#include "ui_help.h"
#include <QDebug>
Help::Help(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Help)
{
    ui->setupUi(this);
    qDebug() << "newwww";
    connect(ui->pushButton, &QPushButton::clicked, this, [=](){ qDebug() << "delete this";delete this;} );
}

Help::~Help()
{
    delete ui;
}
