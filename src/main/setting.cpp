#include "setting.h"
#include "ui_setting.h"

#include <QDebug>

class SettingPrivate
{
    Setting *q_ptr;
    Q_DECLARE_PUBLIC(Setting)
public:
    SettingPrivate(Setting *publicSetting)
        :q_ptr(publicSetting),
          m_threadNum(nullptr),
          m_fileFilters(nullptr)
    {
        init();
    }

    inline void init();
    void setThreadNum(int *num);
    void setFileFilters(QStringList *fileFilters);
    void onPButOK();
    void onPButCancel();
    void onListWidgetClic(int row);
    void onHotSolidThreadNum(int index);

    QString m_soliderThreadNum;
    int *m_threadNum;
    QStringList *m_fileFilters;
};


void SettingPrivate::init()
{
    m_soliderThreadNum = QObject::tr("Thunder Num: %1");
    QObject::connect(q_ptr->ui->pButOK, SIGNAL(clicked()), q_ptr,
                     SLOT(onPButOK()) );
    QObject::connect(q_ptr->ui->pButCancel, SIGNAL(clicked()), q_ptr,
                     SLOT(onPButCancel()) );
    QObject::connect(q_ptr->ui->listWidget, SIGNAL(currentRowChanged(int)),
                     q_ptr, SLOT(onListWidgetClick(int)) );
    QObject::connect(q_ptr->ui->horSlidThreadNum, SIGNAL(sliderMoved(int)),
                     q_ptr, SLOT(onHorSolidThreadNum(int)) );

    for (int i = 0; i < q_ptr->ui->listWidget->count(); i++)
    {
        QListWidgetItem *item = q_ptr->ui->listWidget->item(i);
        item->setSizeHint(QSize(item->sizeHint().width(), 40 ));
    }
}

void SettingPrivate::setThreadNum(int *num)
{
    if(nullptr == num)
        return;
    m_threadNum = num;
    onHotSolidThreadNum(*num);
}

void SettingPrivate::setFileFilters(QStringList *fileFilters)
{
    if(nullptr == fileFilters)
        return;
    m_fileFilters = fileFilters;
}

void SettingPrivate::onPButOK()
{
    q_ptr->hide();
}

void SettingPrivate::onPButCancel()
{

}

void SettingPrivate::onListWidgetClic(int row)
{
    if (row >= 0 || row < q_ptr->ui->listWidget->count()) {
        q_ptr->ui->stackedWidget->setCurrentIndex(row);
    }
}

void SettingPrivate::onHotSolidThreadNum(int index)
{
    QString text(m_soliderThreadNum.arg(QString::number(index)));
    q_ptr->ui->label_ThreadNum->setText(text);
}


Setting::Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);
    d_ptr = new SettingPrivate(this);
}

Setting::~Setting()
{
    delete ui;
}

void Setting::setCheckThreadNum(int *num)
{
    d_ptr->setThreadNum(num);
}

void Setting::setFileFilters(QStringList *fileFilters)
{
    d_ptr->setFileFilters(fileFilters);
}

void Setting::onPButOK()
{
    d_ptr->onPButOK();
}

void Setting::onPButCancel()
{
    d_ptr->onPButCancel();
}

void Setting::onListWidgetClick(int row)
{
    d_ptr->onListWidgetClic(row);
}

void Setting::onHorSolidThreadNum(int index)
{
    d_ptr->onHotSolidThreadNum(index);
}

