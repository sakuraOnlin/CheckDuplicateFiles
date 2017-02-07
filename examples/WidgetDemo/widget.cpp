#include <QFileDialog>
#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    m_filePath(QString("."))
{
    ui->setupUi(this);
    init();
}

Widget::~Widget()
{
    delete ui;
    delete m_compute;
}

void Widget::onSelectDir()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                      m_filePath,
                                                      tr("All (*.*)"));

    if(fileName.isEmpty()) return;

    m_filePath = fileName;
    m_compute->setFilePath(m_filePath);
    ui->lineEdit->setText(m_filePath);
    ui->pBStart->setEnabled(true);
    ui->laCRC32->clear();
    ui->laSHA1->clear();
    ui->lbMD5->clear();
}

void Widget::onStart()
{
    emit signalStart();
    ui->pBSelectDir->setEnabled(false);
    ui->pBStart->setEnabled(false);
    ui->pBStop->setEnabled(true);
}

void Widget::onStop()
{
    ui->pBSelectDir->setEnabled(true);
    ui->pBStart->setEnabled(false);
    ui->pBStop->setEnabled(false);
    m_compute->onStopCheck();
}

void Widget::onCalculationComplete()
{
    ui->pBSelectDir->setEnabled(true);
    ui->pBStart->setEnabled(false);
    ui->pBStop->setEnabled(false);
}

void Widget::onFinalResult(util::ComputeResult result)
{
    bool showNewsBool = false;
    result.resultMessageType == util::CheckIng ? showNewsBool = true: showNewsBool;
    switch (result.checkHashType) {
    case util::MD5:
    {
        ui->lbMD5->setText(showNewsBool?
                               QString::number(result.computeProgress) :
                                         result.resultStr);
        break;
    }
    case util::SHA1:
    {
        ui->laSHA1->setText(showNewsBool?
                                QString::number(result.computeProgress) :
                                          result.resultStr);
        break;
    }
    case util::CRC32:
    {
        ui->laCRC32->setText(showNewsBool?
                                 QString::number(result.computeProgress) :
                                           result.resultStr);
        break;
    }
    default:
        break;
    }
}

void Widget::init()
{
    ui->pBStart->setEnabled(false);
    ui->pBStop->setEnabled(false);
    int computeType = util::MD5 | util::SHA1 | util::CRC32;
    m_compute = new CheckFile;
    m_compute->setCheckType(computeType);
    connect(ui->pBSelectDir, SIGNAL(clicked()), this,SLOT(onSelectDir()) );
    connect(ui->pBStart, SIGNAL(clicked()), this, SLOT(onStart()) );
    connect(ui->pBStop, SIGNAL(clicked()), this,SLOT(onStop()) );
    connect(this, SIGNAL(signalStart()), m_compute, SLOT(onStart()) );
    connect(m_compute, SIGNAL(signalCalculationComplete()), this,
            SLOT(onCalculationComplete()));
    connect(m_compute, SIGNAL(signalFinalResult(util::ComputeResult)), this,
            SLOT(onFinalResult(util::ComputeResult)));
}
