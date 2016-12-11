#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"

class MainWindowPrivate
{
    MainWindow *q_ptr;
    Q_DECLARE_PUBLIC(MainWindow)
public:
    MainWindowPrivate(MainWindow *publicFatherPoint)
        :q_ptr(publicFatherPoint) ,m_dirPath(QString()) ,m_isStart(false)
    {

    }

    inline void updateSelectDirPBut();


    QString m_dirPath;
    bool m_isStart;
};



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
    QString dirPath("");
    dirPath = QFileDialog::getExistingDirectory(this, tr("Select Directory"),
                                                      d_ptr->m_dirPath,
                                                      QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);
    if(dirPath.isEmpty())
    {
        QMessageBox::warning(this, tr("Error!"),
                                         tr("Please select a valid directory!.\n"),
                                         QMessageBox::Ok);
        return;
    }

    d_ptr->m_dirPath = dirPath;
    ui->lineEdit_ShowDIrPath->setText(d_ptr->m_dirPath);
    ui->pushBut_StartCheck->setEnabled(true);
}

void MainWindow::onStartCheck()
{
    if(d_ptr->m_isStart)
    // is start checking
    {
        d_ptr->updateSelectDirPBut();
    }
    else
    // is not start check
    {
        d_ptr->updateSelectDirPBut();
    }
}

void MainWindow::onExit()
{

}

void MainWindow::init()
{
    d_ptr = new MainWindowPrivate(this);

    d_ptr->updateSelectDirPBut();

    ui->pushBut_StartCheck->setEnabled(false);

    connect(ui->pushBut_SelectDir,SIGNAL(clicked()),this,SLOT(onSelectDirPath()) );
    connect(ui->pushBut_StartCheck,SIGNAL(clicked()),SLOT(onStartCheck()));
    connect(ui->actionExit,SIGNAL(changed()),this,SLOT(onExit()));
}

void MainWindowPrivate::updateSelectDirPBut()
{
    if(m_isStart)
    {
        q_ptr->ui->pushBut_SelectDir->setEnabled(false);
        q_ptr->ui->lineEdit_ShowDIrPath->setEnabled(false);
        q_ptr->ui->pushBut_StartCheck->setText(QObject::tr("Stop Check"));
        m_isStart = false;
    }
    else
    {
        q_ptr->ui->pushBut_SelectDir->setEnabled(true);
        q_ptr->ui->lineEdit_ShowDIrPath->setEnabled(true);
        q_ptr->ui->pushBut_StartCheck->setText(QObject::tr("Start Check"));
        m_isStart = true;
    }
}
