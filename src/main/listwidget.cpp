#ifdef _DEBUG
#include <QDebug>
#include <QFileIconProvider>
#endif

#include "listwidget.h"
#include "ui_listwidget.h"
#include "computehash.h"
#include "itemlistdelegate.h"
#include "core/widgetUtil.h"
#include "core/threadselectfiles.h"
#include "core/backstage.h"

class ListWidgetPrivate
{
    ListWidget *q_ptr;
    Q_DECLARE_PUBLIC(ListWidget)

public:
    ListWidgetPrivate(ListWidget *publicListWidget)
        :q_ptr(publicListWidget),
          m_isStart(false),
          m_dirPath(QString()),
          m_selectItem(nullptr)
    {
        init();
    }
    void init();
    void onStart();
    void onStop();
    void onClickItem(QListWidgetItem *item);

    bool m_isStart;
    QString m_dirPath;
    QStringList m_fileFilters;
    ItemListDelegate m_dselegate;
    QListWidgetItem *m_selectItem;
    ThreadSelectFiles m_selectFiles;
    QHash<QString, QListWidgetItem*> m_fileItemHash;
    QStringList m_filePathList;
    Backstage m_backstage;
};

void ListWidgetPrivate::init()
{
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
    QObject::connect(&m_selectFiles, SIGNAL(signalFilePath(QString)), backstageWork,
                     SLOT(doListWidgetAddItem(QString)));
}

void ListWidgetPrivate::onStart()
{
    q_ptr->ui->listWidget->clear();
    m_fileItemHash.clear();
    m_filePathList.clear();
    m_selectItem = nullptr;
    //TODO:开始遍循环
    m_selectFiles.setDirPath(m_dirPath);
    m_selectFiles.setFilters(m_fileFilters);
    m_selectFiles.onStartSelectFiles();
}

void ListWidgetPrivate::onStop()
{
    m_selectFiles.onStopSelectFiles();
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
    delete ui;
}

bool ListWidget::setFileFilters(QStringList filters)
{
    if(d_ptr->m_isStart)
        return false;

    d_ptr->m_fileFilters = filters;
    return true;
}

bool ListWidget::setDirPath(QString &dirPath)
{
    if(d_ptr->m_isStart)
        return false;

    d_ptr->m_dirPath = dirPath;
    return true;
}

bool ListWidget::operatingStatus()
{
    return d_ptr->m_isStart;
}

void ListWidget::onStart()
{
    d_ptr->onStart();
}

void ListWidget::onStop()
{
    d_ptr->onStop();
}

void ListWidget::onReceiveFilePath(QString filePath)
{

}

void ListWidget::onUpdateItemData(int role, QString &filePath, QVariant data)
{

}

void ListWidget::onDelFile()
{

}

void ListWidget::onClickItem(QListWidgetItem *item)
{
    d_ptr->onClickItem(item);
}
