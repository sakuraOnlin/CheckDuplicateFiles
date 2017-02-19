#include <QListWidget>
#include <QDateTime>
#include <QVariant>
#include <QString>
#include <QDesktopServices>
#include <QUrl>
#include <QFileInfo>
#include <QMessageBox>

#ifdef _DEBUG
#include <QDebug>
#endif

#include "listwidget.h"
#include "ui_listwidget.h"
#include "CheckFile.h"
#include "itemlistdelegate.h"
#include "core/widgetUtil.h"
#include "core/backstage.h"

class ListWidgetPrivate
{
    ListWidget *q_ptr;
    Q_DECLARE_PUBLIC(ListWidget)

public:
    ListWidgetPrivate(ListWidget *publicListWidget)
        :q_ptr(publicListWidget)
    {
        init();
    }
    ~ListWidgetPrivate();

    void init();
    void onStart(int checkType);
    void onStop();
    void onClickItem(QListWidgetItem *item);
    void onOpenFileDir();
    void onDelFile();
    void onOpenFileDir(QString filePath);
    void onDelFile(QString filePath);
    void onFindAllRepeat();
    void onFindText(QString text);
    void onFindNextText();
    void onClearRepeat();
    void onExportResult(QString filePath);

    ItemListDelegate m_dselegate;
    QHash<QString, QListWidgetItem*> m_fileItemHash;
    QStringList m_filePathList;
};

ListWidgetPrivate::~ListWidgetPrivate()
{
    onStop();
}

void ListWidgetPrivate::init()
{
    qRegisterMetaType<WidgetUtil::Progress>("WidgetUtil::Progress");
    QSize IconSize(QSize(32,32));
    BackstageWork *backstageWork = Backstage::getInstance().getBackstagWork();
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
    Backstage::getInstance().getBackstagWork()->onStart(checkType);
}

void ListWidgetPrivate::onStop()
{
    Backstage::getInstance().getBackstagWork()->onStop();
}

void ListWidgetPrivate::onClickItem(QListWidgetItem *item)
{
    Backstage::getInstance().getBackstagWork()->onClickItem(item);
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


#ifdef Q_OS_WIN
    if(fileinfo.isSymLink())
        validFolderPath = fileinfo.symLinkTarget();
    else
#endif
    validFolderPath = fileinfo.absolutePath();
    QDesktopServices::openUrl(QUrl::fromLocalFile(validFolderPath));
}

void ListWidgetPrivate::onDelFile(QString filePath)
{
    int ret = QMessageBox::warning(q_ptr, QObject::tr("Delete Files"),
                                     QObject::tr("Are you sure you want to delete the file? \n"
                                                 "Note: This delete file can not be undone!"),
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if(ret != QMessageBox::Yes)
        return;

    Backstage::getInstance().getBackstagWork()->onDelFile(filePath);
    q_ptr->ui->listWidget->takeItem(q_ptr->ui->listWidget->currentRow());
}

void ListWidgetPrivate::onFindAllRepeat()
{
    Backstage::getInstance().getBackstagWork()->onFindAllRepeat();
}

void ListWidgetPrivate::onFindText(QString text)
{
    Backstage::getInstance().getBackstagWork()->onFindText(text);
}

void ListWidgetPrivate::onFindNextText()
{
    Backstage::getInstance().getBackstagWork()->onFindNextText();
}

void ListWidgetPrivate::onClearRepeat()
{
    Backstage::getInstance().getBackstagWork()->onClearFindRepeat();
}

void ListWidgetPrivate::onExportResult(QString filePath)
{
    Backstage::getInstance().getBackstagWork()->onExportResult(filePath);
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
    if(Backstage::getInstance().getBackstagWork()->getOperatingStatus())
        return false;

    Backstage::getInstance().getBackstagWork()->setFileFilters(filters);
    return true;
}

bool ListWidget::setDirPath(QString dirPath)
{
    if(Backstage::getInstance().getBackstagWork()->getOperatingStatus())
        return false;

    Backstage::getInstance().getBackstagWork()->setDirPath(dirPath);
    return true;
}

bool ListWidget::operatingStatus()
{
    return Backstage::getInstance().getBackstagWork()->getOperatingStatus();
}

bool ListWidget::setCheckThreadNum(int num)
{
    return Backstage::getInstance().getBackstagWork()->setCheckThreadNum(num);
}

int ListWidget::getCheckThreadNum()
{
    return Backstage::getInstance().getBackstagWork()->getCheckThreadNum();
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
    d_ptr->onOpenFileDir();
}

void ListWidget::onDelFile()
{
    d_ptr->onDelFile();
}

void ListWidget::onFindAllRepeat()
{
    d_ptr->onFindAllRepeat();
}

void ListWidget::onFindText(QString text)
{
    d_ptr->onFindText(text);
}

void ListWidget::onFindNextText()
{
    d_ptr->onFindNextText();
}

void ListWidget::onClearRepeat()
{
    d_ptr->onClearRepeat();
}

void ListWidget::onExportResult(QString filePath)
{
    d_ptr->onExportResult(filePath);
}

void ListWidget::onOpenFileDir(QString filePath)
{
    d_ptr->onOpenFileDir(filePath);
}

void ListWidget::onDelFile(QString filePath)
{
    d_ptr->onDelFile(filePath);
}
