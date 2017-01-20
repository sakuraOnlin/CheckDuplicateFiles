#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QProgressBar>
#include "mainwindow.h"
#include "ui_mainwindow.h"

class MainWindowPrivate
{
    MainWindow *q_ptr;
    Q_DECLARE_PUBLIC(MainWindow)
public:
    MainWindowPrivate(MainWindow *publicMsinWindow)
        :q_ptr(publicMsinWindow),
          m_dirPath(QString()),
          m_isStart(false),
          m_fileTotalLabel(new QLabel),
          m_calculationProgress(new QProgressBar)
    {
        init();
    }

    void init();
    inline void updateUIButton();

    QString m_dirPath;
    bool m_isStart;
    QLabel *m_fileTotalLabel;
    QProgressBar *m_calculationProgress;
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
    ui->listWidget->setDirPath(d_ptr->m_dirPath);
}

void MainWindow::onStartCheck()
{
    if(d_ptr->m_isStart)
    {
        d_ptr->m_isStart = false;
        ui->listWidget->onStop();
    }
    else
    {
        d_ptr->m_isStart = true;
        ui->listWidget->onStart();
    }
    d_ptr->updateUIButton();
}

void MainWindow::onExit()
{
    qApp->exit();
}

void MainWindow::onDelFile()
{
    ui->listWidget->onDelFile();
}

void MainWindow::onHelp()
{

}

void MainWindow::onAbout()
{

}

void MainWindow::onFileStatistics(WidgetUtil::Progress progress)
{
    QString fileTotal(QString::number(progress.FileStatistics));
    QString ComputeProgress(QString::number(progress.ComputeProgress));
    d_ptr->m_fileTotalLabel->setText(
                QObject::tr("File Total : %1 , Calculation progress : %2")
                .arg(fileTotal).arg(ComputeProgress)  );
}

void MainWindowPrivate::init()
{
    updateUIButton();
    q_ptr->ui->pushBut_StartCheck->setEnabled(false);
    m_fileTotalLabel->setText(QObject::tr("File Total : %1 , Calculation progress : %2").arg("0").arg("0"));
    q_ptr->ui->statusbar->addPermanentWidget(m_fileTotalLabel);
    m_calculationProgress->setEnabled(false);
    m_calculationProgress->setMinimum(0);
    m_calculationProgress->setMaximum(0);
    m_calculationProgress->setValue(0);
    m_calculationProgress->setFormat("%v / %m");
    m_calculationProgress->setAlignment(Qt::AlignCenter);
    m_calculationProgress->setMaximumWidth(150);
    m_calculationProgress->setHidden(true);
    q_ptr->ui->statusbar->addPermanentWidget(m_calculationProgress);

    QObject::connect(q_ptr->ui->pushBut_SelectDir, SIGNAL(clicked()), q_ptr, SLOT(onSelectDirPath()) );
    QObject::connect(q_ptr->ui->pushBut_StartCheck, SIGNAL(clicked()), q_ptr, SLOT(onStartCheck()));
    QObject::connect(q_ptr->ui->actionSelect_Dir_Path, SIGNAL(triggered()), q_ptr, SLOT(onSelectDirPath()));
    QObject::connect(q_ptr->ui->actionExit, SIGNAL(triggered()), q_ptr, SLOT(onExit()));
    QObject::connect(q_ptr->ui->actionHelp, SIGNAL(triggered()), q_ptr, SLOT(onHelp()));
    QObject::connect(q_ptr->ui->actionAbout, SIGNAL(triggered()), q_ptr, SLOT(onAbout()));
    QObject::connect(q_ptr->ui->pushBut_DelFile, SIGNAL(clicked()), q_ptr, SLOT(onDelFile()));
    QObject::connect(q_ptr->ui->pushBut_DelAllFiles, SIGNAL(clicked()), q_ptr, SLOT(onDelFile()));
    QObject::connect(q_ptr->ui->listWidget, SIGNAL(
                         signalFileStatistics(WidgetUtil::Progress)),q_ptr,
                     SLOT(onFileStatistics(WidgetUtil::Progress))  );
}

void MainWindowPrivate::updateUIButton()
{
    if(m_isStart)
    {
        q_ptr->ui->pushBut_SelectDir->setEnabled(false);
        q_ptr->ui->lineEdit_ShowDIrPath->setEnabled(false);
        q_ptr->ui->pushBut_DelFile->setEnabled(true);
        q_ptr->ui->pushBut_DelAllFiles->setEnabled(true);
        q_ptr->ui->pushBut_StartCheck->setIcon(QIcon(":/img/image/stop.png"));
        q_ptr->ui->pushBut_StartCheck->setIconSize(QSize(24,24));
    }
    else
    {
        q_ptr->ui->pushBut_SelectDir->setEnabled(true);
        q_ptr->ui->lineEdit_ShowDIrPath->setEnabled(true);
        q_ptr->ui->pushBut_DelFile->setEnabled(false);
        q_ptr->ui->pushBut_DelAllFiles->setEnabled(false);
        q_ptr->ui->pushBut_StartCheck->setIcon(QIcon(":/img/image/start.png"));
        q_ptr->ui->pushBut_StartCheck->setIconSize(QSize(24,24));
    }
}
