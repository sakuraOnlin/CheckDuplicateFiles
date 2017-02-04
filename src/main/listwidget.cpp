#include <QListWidget>
#include <QDateTime>
#include <QVariant>
#include <QString>
#include <QFile>
#include <QDesktopServices>
#include <QUrl>
#include <QFileInfo>

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
    void onStart(int checkType);
    void onStop();
    void onClickItem(QListWidgetItem *item);
    void onOpenFileDir();
    void onDelFile();
    void onOpenFileDir(QString filePath);
    void onDelFile(QString filePath);

    ItemListDelegate m_dselegate;
    QListWidgetItem *m_selectItem;
    QHash<QString, QListWidgetItem*> m_fileItemHash;
    QStringList m_filePathList;
    Backstage m_backstage;
    QFile m_removeFile;
};

void ListWidgetPrivate::init()
{
    qRegisterMetaType<WidgetUtil::Progress>("WidgetUtil::Progress");
    QSize IconSize(QSize(32,32));
    BackstageWork *backstageWork = m_backstage.getBackstagwWork();
    backstageWork->setListWidget(q_ptr->ui->listWidget);
    backstageWork->setFilePath(&m_filePathList);
    backstageWork->setFileItem(&m_fileItemHash, IconSize);
    q_ptr->ui->listWidget->setItemDelegate(&m_dselegate);
    q_ptr->ui->listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    q_ptr->ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QObject::connect(q_ptr->ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)),
                     q_ptr, SLOT(onClickItem(QListWidgetItem*)));
    QObject::connect(backstageWork, SIGNAL(signalFileStatistics(WidgetUtil::Progress)),
                     q_ptr, SIGNAL(signalFileStatistics(WidgetUtil::Progress)) );
    QObject::connect(&m_dselegate, SIGNAL(signalOpenFileDir(QString)),
                     q_ptr, SLOT(onOpenFileDir(QString))  );
    QObject::connect(&m_dselegate, SIGNAL(signalDelFile(QString)),
                     q_ptr, SLOT(onDelFile(QString)) );
    QObject::connect(&m_dselegate, SIGNAL(signalDelFile(QString)),
                     backstageWork, SLOT(onStopCheckFile(QString))  );

}

void ListWidgetPrivate::onStart(int checkType)
{
    q_ptr->ui->listWidget->clear();
    m_fileItemHash.clear();
    m_filePathList.clear();
    m_selectItem = nullptr;
    m_backstage.getBackstagwWork()->onStart(checkType);
}

void ListWidgetPrivate::onStop()
{
    m_backstage.getBackstagwWork()->onStop();
}

void ListWidgetPrivate::onClickItem(QListWidgetItem *item)
{
    if(nullptr == item )
        return;
    int width = item->sizeHint().width();
    if(nullptr == m_selectItem)
        m_selectItem = item;
    else
    {
        m_selectItem->setSizeHint(QSize(width, 70));
        m_selectItem->setData(WidgetUtil::ItemSelectRole, false);
    }
    m_selectItem = item;
    item->setSizeHint(QSize(item->sizeHint().width(), 140));
    item->setData(WidgetUtil::ItemSelectRole, true);
}

void ListWidgetPrivate::onOpenFileDir()
{
    int itemRow = q_ptr->ui->listWidget->currentRow();
    if(itemRow < 0)
        return;

    QListWidgetItem *item = q_ptr->ui->listWidget->item(itemRow);
    QString filePath(item->data(WidgetUtil::FilePathRole).toString());
    if(filePath.isEmpty())
        return;
    onOpenFileDir(filePath);
}

void ListWidgetPrivate::onDelFile()
{
    int itemRow = q_ptr->ui->listWidget->currentRow();
    if(itemRow < 0)
        return;

    QListWidgetItem *item = q_ptr->ui->listWidget->item(itemRow);
    QString filePath(item->data(WidgetUtil::FilePathRole).toString());
    if(filePath.isEmpty())
        return;
    onDelFile(filePath);
}

void ListWidgetPrivate::onOpenFileDir(QString filePath)
{
    QFileInfo fileinfo(filePath);
    QString validFolderPath;

    //TODO : 对文件做链接类判断，同时，在扫描添加的时候，元需要做判断
#ifdef Q_OS_LINUX
    validFolderPath = fileinfo.absolutePath();
#endif

#ifdef Q_OS_WIN
    if(fileinfo.isSymLink())
        validFolderPath = fileinfo.symLinkTarget();
    else
        validFolderPath = fileinfo.absolutePath();
#endif
    QDesktopServices::openUrl(QUrl(validFolderPath , QUrl::TolerantMode));
}

void ListWidgetPrivate::onDelFile(QString filePath)
{
    m_filePathList.removeAt(m_filePathList.indexOf(filePath));
    m_removeFile.remove(filePath);
    m_fileItemHash.take(filePath);
    q_ptr->ui->listWidget->takeItem(q_ptr->ui->listWidget->currentRow());
    m_selectItem = nullptr;
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

void ListWidget::onStart(int checkType)
{
    d_ptr->onStart(checkType);
}

void ListWidget::onStop()
{
    d_ptr->onStop();
}

void ListWidget::onClickItem(QListWidgetItem *item)
{
    d_ptr->onClickItem(item);
}

void ListWidget::onOpenFileDir()
{
    d_ptr->onDelFile();
}

void ListWidget::onDelFile()
{
    d_ptr->onDelFile();
}

void ListWidget::onOpenFileDir(QString filePath)
{
    d_ptr->onOpenFileDir(filePath);
}

void ListWidget::onDelFile(QString filePath)
{
    d_ptr->onDelFile(filePath);
}
