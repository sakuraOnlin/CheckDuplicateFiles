#include <QListWidget>
#include <QDateTime>
#include <QVariant>

#ifdef _DEBUG
#include <QDebug>
#endif

#include "listwidget.h"
#include "ui_listwidget.h"
#include "computehash.h"
#include "itemlistdelegate.h"
#include "core/widgetUtil.h"
#include "core/backstage.h"

class ListWidgetPrivate
{
    ListWidget *q_ptr;
    Q_DECLARE_PUBLIC(ListWidget)

public:
    ListWidgetPrivate(ListWidget *publicListWidget)
        :q_ptr(publicListWidget),
          m_selectItem(nullptr)
    {
        init();
    }
    void init();
    void onStart();
    void onStop();
    void onClickItem(QListWidgetItem *item);

    ItemListDelegate m_dselegate;
    QListWidgetItem *m_selectItem;
    QHash<QString, QListWidgetItem*> m_fileItemHash;
    QStringList m_filePathList;
    Backstage m_backstage;
};

void ListWidgetPrivate::init()
{
    qRegisterMetaType<WidgetUtil::Progress>("WidgetUtil::Progress");
    QSize IconSize(QSize(32,32));
    BackstageWork *backstageWork = m_backstage.getBackstagwWork();
    backstageWork->setListWidget(q_ptr->ui->listWidget);
    backstageWork->setFilePath(m_filePathList);
    backstageWork->setFileItem(m_fileItemHash, IconSize);
    q_ptr->ui->listWidget->setItemDelegate(&m_dselegate);
    q_ptr->ui->listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    q_ptr->ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QObject::connect(q_ptr->ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)),
                     q_ptr, SLOT(onClickItem(QListWidgetItem*)));
    QObject::connect(backstageWork, SIGNAL(signalFileStatistics(WidgetUtil::Progress)),
                     q_ptr, SIGNAL(signalFileStatistics(WidgetUtil::Progress)) );

}

void ListWidgetPrivate::onStart()
{
    q_ptr->ui->listWidget->clear();
    m_fileItemHash.clear();
    m_filePathList.clear();
    m_selectItem = nullptr;
    m_backstage.getBackstagwWork()->onStart();
}

void ListWidgetPrivate::onStop()
{
    m_backstage.getBackstagwWork()->onStop();
}

void ListWidgetPrivate::onClickItem(QListWidgetItem *item)
{
    int width = item->sizeHint().width();
    if(nullptr == m_selectItem)
        m_selectItem = item;
    else
    {
        m_selectItem->setSizeHint(QSize(width, 70));
        m_selectItem->setData(WidgetUtil::ItemSelect, false);
    }
    m_selectItem = item;
    item->setSizeHint(QSize(item->sizeHint().width(), 140));
    item->setData(WidgetUtil::ItemSelect, true);
}

ListWidget::ListWidget(QWidget *parent)
    :QWidget(parent),
    ui(new Ui::ListWidget)
{
    ui->setupUi(this);
    d_ptr = new ListWidgetPrivate(this);
}

ListWidget::~ListWidget()
{
    qDebug() << "ListWidget::~ListWidget()";
    delete d_ptr;
    delete ui;
}

bool ListWidget::setFileFilters(QStringList filters)
{
    if(d_ptr->m_backstage.getBackstagwWork()->getOperatingStatus())
        return false;

    d_ptr->m_backstage.getBackstagwWork()->setFileFilters(filters);
    return true;
}

bool ListWidget::setDirPath(QString dirPath)
{
    if(d_ptr->m_backstage.getBackstagwWork()->getOperatingStatus())
        return false;

    d_ptr->m_backstage.getBackstagwWork()->setDirPath(dirPath);
    return true;
}

bool ListWidget::operatingStatus()
{
    return d_ptr->m_backstage.getBackstagwWork()->getOperatingStatus();
}

void ListWidget::onStart()
{
    d_ptr->onStart();
}

void ListWidget::onStop()
{
    d_ptr->onStop();
}

void ListWidget::onDelFile()
{

}

void ListWidget::onClickItem(QListWidgetItem *item)
{
    d_ptr->onClickItem(item);
}

