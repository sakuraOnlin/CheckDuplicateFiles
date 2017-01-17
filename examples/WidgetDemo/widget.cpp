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
}

void Widget::onSelectDir()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                      m_filePath,
                                                      tr("All (*.*)"));

    if(fileName.isEmpty()) return;

    m_filePath = fileName;
    m_compute->setDirPath(m_filePath);
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
}

void Widget::onCalculationComplete()
{
    ui->pBSelectDir->setEnabled(true);
    ui->pBStart->setEnabled(false);
}

void Widget::onFinalResult(util::ComputeResult result)
{
    switch (result.computeHashType) {
    case util::MD5:
    {
        ui->lbMD5->setText(result.resultStr);
        break;
    }
    case util::SHA1:
    {
        ui->laSHA1->setText(result.resultStr);
        break;
    }
    case util::CRC32:
    {
        ui->laCRC32->setText(result.resultStr);
        break;
    }
    default:
        break;
    }
}

void Widget::init()
{
    int computeType = util::MD5 | util::SHA1 | util::CRC32;
    m_compute = new ComputeHash(computeType);
    connect(ui->pBSelectDir, SIGNAL(clicked()), this,SLOT(onSelectDir()) );
    connect(ui->pBStart, SIGNAL(clicked()), this, SLOT(onStart()) );
    connect(this, SIGNAL(signalStart()), m_compute, SLOT(onStart()) );
    connect(m_compute, SIGNAL(signalCalculationComplete()), this,
            SLOT(onCalculationComplete()));
    connect(m_compute, SIGNAL(signalFinalResult(util::ComputeResult)), this,
            SLOT(onFinalResult(util::ComputeResult)));
}
