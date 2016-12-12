#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include "mainwindow.h"
#include "ui_mainwindow.h"

class MainWindowPrivate
{
    MainWindow *q_ptr;
    Q_DECLARE_PUBLIC(MainWindow)
public:
    MainWindowPrivate(MainWindow *publicFatherPoint)
        :q_ptr(publicFatherPoint),
          m_dirPath(QString()),
          m_isStart(false)
    {
        init();
    }

    void init();
    inline void updateUIButton();

    QString m_dirPath;
    bool m_isStart;

};


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    d_ptr = new MainWindowPrivate(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete d_ptr;
}

void MainWindow::onSelectDirPath()
{
    QString dirPath("");
    dirPath = QFileDialog::getExistingDirectory(this, tr("Select Directory"),
                                                      d_ptr->m_dirPath,
                                                      QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);
    if(dirPath.isEmpty()) return;

    QFileInfo isFilePath(dirPath);
    if(!isFilePath.exists() && !isFilePath.isDir())
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
/*
 * 重写此处的判定规则，并把 m_isStart 单独处理。
*/

}

void MainWindow::onExit()
{
    qApp->exit();
}

void MainWindow::onDelFile()
{
//删除列表框中选定的文件

}

void MainWindow::onDelAllFiles()
{
//删除所有的重复文件

}

void MainWindow::onHelp()
{

}

void MainWindow::onAbout()
{

}

void MainWindowPrivate::init()
{
    updateUIButton();
    q_ptr->ui->pushBut_StartCheck->setEnabled(false);

    QObject::connect(q_ptr->ui->pushBut_SelectDir, SIGNAL(clicked()), q_ptr, SLOT(onSelectDirPath()) );
    QObject::connect(q_ptr->ui->pushBut_StartCheck, SIGNAL(clicked()), q_ptr, SLOT(onStartCheck()));
    QObject::connect(q_ptr->ui->actionSelect_Dir_Path, SIGNAL(triggered()), q_ptr, SLOT(onSelectDirPath()));
    QObject::connect(q_ptr->ui->actionExit, SIGNAL(triggered()), q_ptr, SLOT(onExit()));
    QObject::connect(q_ptr->ui->actionHelp, SIGNAL(triggered()), q_ptr, SLOT(onHelp()));
    QObject::connect(q_ptr->ui->actionAbout, SIGNAL(triggered()), q_ptr, SLOT(onAbout()));
    QObject::connect(q_ptr->ui->pushBut_DelFile, SIGNAL(clicked()), q_ptr, SLOT(onDelFile()));
    QObject::connect(q_ptr->ui->pushBut_DelAllFiles, SIGNAL(clicked()), q_ptr, SLOT(onDelAllFiles()));
}

void MainWindowPrivate::updateUIButton()
{
    if(m_isStart)
    {
        q_ptr->ui->pushBut_SelectDir->setEnabled(false);
        q_ptr->ui->lineEdit_ShowDIrPath->setEnabled(false);
        q_ptr->ui->pushBut_DelFile->setEnabled(true);
        q_ptr->ui->pushBut_DelAllFiles->setEnabled(true);
        q_ptr->ui->pushBut_StartCheck->setText(QObject::tr("Stop Check"));
        q_ptr->ui->pushBut_StartCheck->setIcon(QIcon(":/img/image/stop.png"));
        q_ptr->ui->pushBut_StartCheck->setIconSize(QSize(24,24));
    }
    else
    {
        q_ptr->ui->pushBut_SelectDir->setEnabled(true);
        q_ptr->ui->lineEdit_ShowDIrPath->setEnabled(true);
        q_ptr->ui->pushBut_DelFile->setEnabled(false);
        q_ptr->ui->pushBut_DelAllFiles->setEnabled(false);
        q_ptr->ui->pushBut_StartCheck->setText(QObject::tr("Start Check"));
        q_ptr->ui->pushBut_StartCheck->setIcon(QIcon(":/img/image/start.png"));
        q_ptr->ui->pushBut_StartCheck->setIconSize(QSize(24,24));
    }
}

