#include "mainwindows.h"
#include "ui_mainwindows.h"

MainWindows::MainWindows(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindows)
{
    ui->setupUi(this);
}

MainWindows::~MainWindows()
{
    delete ui;
}
