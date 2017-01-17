#include "backstage.h"

#include <QFileIconProvider>
#include <QFileInfo>
#include <QDateTime>

BackstageWork::BackstageWork(QObject *parnet)
    :QObject(parnet)
{

}

void BackstageWork::setListWidget(QListWidget *listWidget)
{
    m_listWidget = listWidget;
}

void BackstageWork::setFilePath(QStringList &filePathList)
{
    m_filePathList = filePathList;
}

void BackstageWork::setFileItem(QHash<QString, QListWidgetItem *> &fileItemHash,
                                QSize &iconSize)
{
    m_fileItemHash = fileItemHash;
    m_iconSize = iconSize;
}

void BackstageWork::doListWidgetAddItem(QString filePath)
{
    if(filePath.isEmpty())
        return;

    QListWidgetItem *item = m_fileItemHash.value(filePath);
    if(nullptr == item)
    {
        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(QSize(m_listWidget->sizeHint().width(), 70));
        item->setData(WidgetUtil::FilePath,filePath);
        QFileInfo fileInfo(filePath);
        QFileIconProvider fileIco;
        QIcon ico(fileIco.icon(fileInfo));
        QPixmap pixmap(ico.pixmap(m_iconSize));
        item->setData(WidgetUtil::FileIco,pixmap);
        item->setData(WidgetUtil::FileSize, QString::number(fileInfo.size()));
        item->setData(WidgetUtil::FileName, fileInfo.fileName());
        item->setData(WidgetUtil::FileTime, fileInfo.lastModified().
                      toString("yyyy-dd-MM hh:mm:ss"));
        m_listWidget->addItem(item);
        m_filePathList.append(filePath);
    }
}

Backstage::Backstage(QObject *parent)
    : QObject(parent)
{
    m_backstageWork = new BackstageWork;
    m_backstageWork->moveToThread(&m_thread);
    connect(&m_thread, SIGNAL(finished()),m_backstageWork,SLOT(deleteLater()) );
    m_thread.start();
}

Backstage::~Backstage()
{
    m_thread.quit();
    m_thread.wait(200);
    m_backstageWork = nullptr;
}

BackstageWork *Backstage::getBackstagwWork()
{
    return m_backstageWork;
}
