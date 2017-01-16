#ifdef _DEBUG
#include <QDebug>
#endif

#include "listwidget.h"
#include "ui_listwidget.h"
#include "computehash.h"
#include "itemlistdelegate.h"
#include "core/widgetUtil.h"

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

    bool m_isStart;
    QString m_dirPath;
    ItemListDelegate m_dselegate;
    QListWidgetItem *m_selectItem;
};

void ListWidgetPrivate::init()
{
    q_ptr->ui->listWidget->setItemDelegate(&m_dselegate);
    q_ptr->ui->listWidget->setIconSize(QSize(48,48));
    q_ptr->ui->listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    q_ptr->ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QObject::connect(q_ptr->ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)),
                     q_ptr, SLOT(onClickItem(QListWidgetItem*)));
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

bool ListWidget::onStart()
{
    ui->listWidget->clear();
    d_ptr->m_selectItem = nullptr;
    //TODO:开始遍循环
}

bool ListWidget::onStop()
{

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
    int width = item->sizeHint().width();
    if(nullptr == d_ptr->m_selectItem)
        d_ptr->m_selectItem = item;
    else
    {
        d_ptr->m_selectItem->setSizeHint(QSize(width, 70));
        d_ptr->m_selectItem->setData(WidgetUtil::ItemSelect, false);
    }
    d_ptr->m_selectItem = item;
    item->setSizeHint(QSize(item->sizeHint().width(), 140));
    item->setData(WidgetUtil::ItemSelect, true);
}
