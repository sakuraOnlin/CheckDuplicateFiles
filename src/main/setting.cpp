#include "setting.h"
#include "ui_setting.h"

#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QDebug>

class SettingPrivate
{
    Setting *q_ptr;
    Q_DECLARE_PUBLIC(Setting)
public:
    SettingPrivate(Setting *publicSetting)
        :q_ptr(publicSetting)
    {
        init();
    }

    inline void init();
    void setThreadNum(int num);
    void onListWidgetClic(int row);
    void onHotSolidThreadNum(int index);
    void onAddFilters();
    void onDelFilters();
    void setInitData(QList<WidgetUtil::FiltersType> &fileTilters,
                     int threadNum);

    void setFilterList(QList<WidgetUtil::FiltersType> &fileTilters);
    QList<WidgetUtil::FiltersType> getAllFileFilters();

    inline void addItem(QString &filters, bool checked = false);
    QHash<QString, bool> m_filtersListHash;
    QString m_soliderThreadNum;
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
    QObject::connect(q_ptr->ui->pButAddFilters, SIGNAL(clicked()), q_ptr,
                     SLOT(onAddFilters())  );
    QObject::connect(q_ptr->ui->pButDelFilters, SIGNAL(clicked()), q_ptr,
                     SLOT(onDelFilters())  );

    for (int i = 0; i < q_ptr->ui->listWidget->count(); i++)
    {
        QListWidgetItem *item = q_ptr->ui->listWidget->item(i);
        item->setSizeHint(QSize(item->sizeHint().width(), 40 ));
    }
}

void SettingPrivate::setThreadNum(int num)
{
    onHotSolidThreadNum(num);
    q_ptr->ui->horSlidThreadNum->setValue(num);
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
    if(index >=3)
        q_ptr->ui->labelCheckType->setText(QObject::tr("Thread number is too large"
                                                      " can easily cause serious Caton system!"));
    else
        q_ptr->ui->labelCheckType->clear();
}

void SettingPrivate::onAddFilters()
{
    QString filters(q_ptr->ui->lineEdit->text());
    if(filters.isEmpty())
        return;
    addItem(filters);
}

void SettingPrivate::onDelFilters()
{
    int curRow = q_ptr->ui->listWidgetFileFilters->currentRow();
    if(q_ptr->ui->listWidgetFileFilters->count() == 1)
    {
        QMessageBox::warning(q_ptr, QObject::tr("Warring!"),
                             QObject::tr("List must have a file format!"),
                             QMessageBox::Ok);
        return;
    }

    if(curRow>=0 && curRow < q_ptr->ui->listWidgetFileFilters->count())
        delete q_ptr->ui->listWidgetFileFilters->takeItem(curRow);

    if(q_ptr->ui->listWidgetFileFilters->count()==1)
        q_ptr->ui->listWidgetFileFilters->item(0)->setCheckState(Qt::Checked);
}

void SettingPrivate::setInitData(QList<WidgetUtil::FiltersType> &fileTilters,
                                 int threadNum)
{
    setThreadNum(threadNum);
    setFilterList(fileTilters);
}

void SettingPrivate::setFilterList(QList<WidgetUtil::FiltersType> &fileTilters)
{
    for(int i = 0 ; i < fileTilters.length(); i++)
    {
        WidgetUtil::FiltersType value = fileTilters.value(i);
        if(!m_filtersListHash.value(value.filtess))
        {
            m_filtersListHash.insert(value.filtess, true);
            addItem(value.filtess, value.checked);
        }
    }
}

QList<WidgetUtil::FiltersType> SettingPrivate::getAllFileFilters()
{
    QList<WidgetUtil::FiltersType> list;
    for(int i = 0 ; i < q_ptr->ui->listWidgetFileFilters->count(); i++)
    {
        WidgetUtil::FiltersType value;
        QListWidgetItem *item = q_ptr->ui->listWidgetFileFilters->item(i);
        value.filtess = item->data(Qt::DisplayRole).toString();
        value.checked = (item->checkState() & Qt::Checked) ? true: false;
        list.append(value);
    }
    return list;
}

void SettingPrivate::addItem(QString &filters, bool checked)
{
    QListWidgetItem *item = new QListWidgetItem(filters);
    checked ? item->setCheckState(Qt::Checked) : item->setCheckState(Qt::Unchecked);
    q_ptr->ui->listWidgetFileFilters->addItem(item);
}


Setting::Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);
    d_ptr = new SettingPrivate(this);
    qRegisterMetaType<WidgetUtil::FiltersType>("WidgetUtil::FiltersType");
}

Setting::~Setting()
{
    delete ui;
}

void Setting::setInitData(QList<WidgetUtil::FiltersType> fileTilters, int threadNum)
{
    d_ptr->setInitData(fileTilters, threadNum);
}

int Setting::getThreadNum()
{
    return ui->horSlidThreadNum->value();
}

QList<WidgetUtil::FiltersType> Setting::getFileFilters()
{
    return  d_ptr->getAllFileFilters();
}

void Setting::onPButOK()
{
    emit signalDataChange(true);
}

void Setting::onPButCancel()
{
    emit signalDataChange(false);
}

void Setting::onListWidgetClick(int row)
{
    d_ptr->onListWidgetClic(row);
}

void Setting::onHorSolidThreadNum(int index)
{
    d_ptr->onHotSolidThreadNum(index);
}

void Setting::onAddFilters()
{
    d_ptr->onAddFilters();
}

void Setting::onDelFilters()
{
    d_ptr->onDelFilters();
}

