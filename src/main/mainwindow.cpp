#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSelectDirPath()
{

}

void MainWindow::onStartCheck()
{

}

void MainWindow::onExit()
{

}

void MainWindow::init()
{
    connect(ui->pushBut_SelectDir,SIGNAL(clicked()),this,SLOT(onSelectDirPath()) );
    connect(ui->pushBut_StartCheck,SIGNAL(clicked()),SLOT(onStartCheck()));
    connect(ui->actionExit,SIGNAL(changed()),this,SLOT(onExit()));
}
