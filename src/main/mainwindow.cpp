#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QFileInfo>
#include <QProgressBar>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "util/util.h"

class MainWindowPrivate
{
    MainWindow *q_ptr;
    Q_DECLARE_PUBLIC(MainWindow)
public:
    MainWindowPrivate(MainWindow *publicMsinWindow)
        :q_ptr(publicMsinWindow),
          m_dirPath(QString()),
          m_fileTotalLabel(new QLabel),
          m_calculationProgress(new QProgressBar),
          m_checkBoxState(util::MD5 | util::SHA1 | util::CRC32),
          m_isStart(false)
    {
        init();
    }

    void init();
    inline void updateUIButton();
    void onSelectDirPath();
    void onStartCheck();
    void onFileStatistics(WidgetUtil::Progress progress);
    void onCheckBox(int state);

    QString m_dirPath;
    QString m_pButEnStyleSheet;
    QString m_pButDisStyleSheet;
    QList<QPushButton*> m_pButAddressList;
    QLabel *m_fileTotalLabel;
    QProgressBar *m_calculationProgress;
    int m_checkBoxState;
    bool m_isStart;
};

void MainWindowPrivate::init()
{
    q_ptr->ui->pushBut_StartCheck->setEnabled(false);
    m_fileTotalLabel->setText(QObject::tr("File Total : %1 , Calculation progress : %2").arg("0").arg("0"));
    q_ptr->ui->statusbar->addPermanentWidget(m_fileTotalLabel);
    m_calculationProgress->setEnabled(true);
    m_calculationProgress->setMinimum(0);
    m_calculationProgress->setMaximum(0);
    m_calculationProgress->setValue(0);
    m_calculationProgress->setFormat("%v / %m");
    m_calculationProgress->setAlignment(Qt::AlignCenter);
    m_calculationProgress->setMaximumWidth(150);
    q_ptr->ui->statusbar->addPermanentWidget(m_calculationProgress);

    q_ptr->ui->pushBut_SelectDir->setEnabled(true);
    q_ptr->ui->lineEdit_ShowDIrPath->setEnabled(true);
    q_ptr->ui->pushBut_FindDuplicateFiles->setEnabled(false);
    q_ptr->ui->pushBut_DelFile->setEnabled(false);
    q_ptr->ui->pushBut_DelAllFiles->setEnabled(false);

    m_pButAddressList.append(q_ptr->ui->pushBut_SelectDir);
    m_pButAddressList.append(q_ptr->ui->pushBut_FindDuplicateFiles);
    m_pButAddressList.append(q_ptr->ui->pushBut_DelFile);
    m_pButAddressList.append(q_ptr->ui->pushBut_DelAllFiles);

    m_pButEnStyleSheet = " QPushButton#pushBut_StartCheck"
                         "{ background-color:rgba(255,255,255,0); border-image: url(:/img/image/start.png); }"
                         "QPushButton#pushBut_StartCheck:hover{ border-image: url(:/img/image/start_highlight.png); }"
                         "QPushButton#pushBut_StartCheck:pressed{ border-image: url(:/img/image/start_pushed.png); }"
                         "QPushButton#pushBut_StartCheck:disabled{ border-image: url(:/img/image/start_disabled.png); }";

    m_pButDisStyleSheet = "QPushButton#pushBut_StartCheck"
                          "{ background-color:rgba(255,255,255,0); border-image:url(:/img/image/stop.png); }"
                          "QPushButton#pushBut_StartCheck:hover{ border-image: url(:/img/image/stop_highlight.png); }"
                          "QPushButton#pushBut_StartCheck:pressed{ border-image: url(:/img/image/stop_pushed.png); }"
                          "QPushButton#pushBut_StartCheck:disabled{ border-image: url(:/img/image/stop_disabled.png); }";
    q_ptr->ui->pushBut_StartCheck->setStyleSheet(m_pButEnStyleSheet);

    QObject::connect(q_ptr->ui->pushBut_SelectDir, SIGNAL(clicked()), q_ptr, SLOT(onSelectDirPath()) );
    QObject::connect(q_ptr->ui->pushBut_StartCheck, SIGNAL(clicked()), q_ptr, SLOT(onStartCheck()));
    QObject::connect(q_ptr->ui->actionSelect_Dir_Path, SIGNAL(triggered()), q_ptr, SLOT(onSelectDirPath()));
    QObject::connect(q_ptr->ui->actionExit, SIGNAL(triggered()), q_ptr, SLOT(onExit()));
    QObject::connect(q_ptr->ui->actionHelp, SIGNAL(triggered()), q_ptr, SLOT(onHelp()));
    QObject::connect(q_ptr->ui->actionAbout, SIGNAL(triggered()), q_ptr, SLOT(onAbout()));
    QObject::connect(q_ptr->ui->pushBut_DelFile, SIGNAL(clicked()), q_ptr, SLOT(onDelFile()));
    QObject::connect(q_ptr->ui->pushBut_DelAllFiles, SIGNAL(clicked()), q_ptr, SLOT(onDelFile()));
    QObject::connect(q_ptr->ui->listWidget, SIGNAL(signalFileStatistics(WidgetUtil::Progress)),
                     q_ptr, SLOT(onFileStatistics(WidgetUtil::Progress)));
    QObject::connect(q_ptr->ui->checkBox_MD5, SIGNAL(stateChanged(int)),
                     q_ptr, SLOT(onCheckBox(int)) );
    QObject::connect(q_ptr->ui->checkBox_SHA1, SIGNAL(stateChanged(int)),
                     q_ptr, SLOT(onCheckBox(int)) );
    QObject::connect(q_ptr->ui->checkBox_CRC32, SIGNAL(stateChanged(int)),
                     q_ptr, SLOT(onCheckBox(int)) );
}

void MainWindowPrivate::updateUIButton()
{
    q_ptr->ui->lineEdit_ShowDIrPath->setEnabled(!m_isStart);
    q_ptr->ui->actionSelect_Dir_Path->setEnabled(!m_isStart);
    foreach (QPushButton *value, m_pButAddressList)
    {
        value->setEnabled(!value->isEnabled());
    }
    m_isStart ? q_ptr->ui->pushBut_StartCheck->setStyleSheet(m_pButDisStyleSheet) :
                q_ptr->ui->pushBut_StartCheck->setStyleSheet(m_pButEnStyleSheet);
}

void MainWindowPrivate::onSelectDirPath()
{
    QString dirPath("");
    dirPath = QFileDialog::getExistingDirectory(q_ptr, QObject::tr("Select Directory"),
                                                      m_dirPath,
                                                      QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);
    if(dirPath.isEmpty()) return;

    QFileInfo isFilePath(dirPath);
    if(!isFilePath.exists() && !isFilePath.isDir())
    {
        QMessageBox::warning(q_ptr, QObject::tr("Error!"),
                                         QObject::tr("Please select a valid directory!\n"),
                                         QMessageBox::Ok);
        return;
    }

    m_dirPath = dirPath;
    q_ptr->ui->lineEdit_ShowDIrPath->setText(m_dirPath);
    q_ptr->ui->pushBut_StartCheck->setEnabled(true);
    q_ptr->ui->listWidget->setDirPath(m_dirPath);
}

void MainWindowPrivate::onStartCheck()
{
    if(m_isStart)
    {
        m_isStart = false;
        q_ptr->ui->listWidget->onStop();
    }
    else
    {
        m_isStart = true;
        q_ptr->ui->listWidget->onStart(m_checkBoxState);
    }
    updateUIButton();
}

void MainWindowPrivate::onFileStatistics(WidgetUtil::Progress progress)
{
    QString fileTotal(QString::number(progress.FileStatistics));
    QString ComputeProgress(QString::number(progress.ComputeProgress));
    m_fileTotalLabel->setText(
                QObject::tr("File Total : %1 , Calculation progress : %2")
                .arg(fileTotal).arg(ComputeProgress)  );
    m_calculationProgress->setMaximum(progress.FileStatistics);
    m_calculationProgress->setValue(progress.ComputeProgress);
}

void MainWindowPrivate::onCheckBox(int)
{
    QCheckBox *value = dynamic_cast<QCheckBox*>(q_ptr->sender());
    if(nullptr == value)
        return;

    if(value == q_ptr->ui->checkBox_MD5)
    {
        m_checkBoxState = value->isChecked() ? (m_checkBoxState | util::MD5) :
                                                (m_checkBoxState ^ util::MD5) ;
    }
    if(value == q_ptr->ui->checkBox_SHA1)
    {
        m_checkBoxState = value->isChecked() ? (m_checkBoxState | util::SHA1) :
                                                (m_checkBoxState ^ util::SHA1) ;
    }
    if(value == q_ptr->ui->checkBox_CRC32)
    {
        m_checkBoxState = (value->isChecked()) ? (m_checkBoxState | util::CRC32) :
                                                (m_checkBoxState ^ util::CRC32) ;
    }
}

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
    d_ptr->onSelectDirPath();
}

void MainWindow::onStartCheck()
{
    d_ptr->onStartCheck();
}

void MainWindow::onExit()
{
    qApp->exit();
}

void MainWindow::onDelFile()
{
    ui->listWidget->onDelFile();
}

void MainWindow::onDelAllDupFiles()
{

}

void MainWindow::onHelp()
{

}

void MainWindow::onAbout()
{

}

void MainWindow::onCheckBox(int state)
{
    d_ptr->onCheckBox(state);
}

void MainWindow::onFileStatistics(WidgetUtil::Progress progress)
{
    d_ptr->onFileStatistics(progress);
}
