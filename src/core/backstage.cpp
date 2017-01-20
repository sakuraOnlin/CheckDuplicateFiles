#include "backstage.h"

#include <QFileIconProvider>
#include <QFileInfo>
#include <QDateTime>

Q_DECLARE_METATYPE(util::ComputeResult)
Q_DECLARE_METATYPE(WidgetUtil::Progress)

BackstageWork::BackstageWork(QObject *parnet)
    :QObject(parnet)
{
    init();
}

BackstageWork::~BackstageWork()
{
}

void BackstageWork::setListWidget(QListWidget *listWidget)
{
    m_listWidget = listWidget;
}

void BackstageWork::setDirPath(QString dirPath)
{
    m_dirPath = dirPath;
}

void BackstageWork::setFileFilters(QStringList fileFIlters)
{
    m_fileFilters = fileFIlters;
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

void BackstageWork::setComputeMaxThreadCount(int threadCount)
{
    m_computeThreadCount = threadCount;
}

bool BackstageWork::getOperatingStatus()
{
    return m_operatingStatus;
}

void BackstageWork::onStart()
{
    m_filePathList.clear();
    m_fileItemHash.clear();
    m_operatingStatus = true;
    m_selectFiles.setDirPath(m_dirPath);
    m_selectFiles.setFilters(m_fileFilters);
    m_selectFiles.onStartSelectFiles();
    m_computeFileIndex = 0;
}

void BackstageWork::onStop()
{
    m_operatingStatus = false;
    m_selectFiles.onStopSelectFiles();
}

void BackstageWork::onListWidgetAddItem(QString filePath)
{
    if(filePath.isEmpty())
        return;

    QListWidgetItem *item = m_fileItemHash.value(filePath);
    if(nullptr == item)
    {
        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(QSize(400, 70));
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
        WidgetUtil::Progress progress;
        progress.FileStatistics = m_filePathList.length();
        progress.ComputeProgress = m_computeFileIndex;
        emit signalFileStatistics(progress);
    }
}

void BackstageWork::onItemSetData(util::ComputeResult result)
{
    util::ResultMessageType messageType = result.resultMessageType;
    util::ComputeType computeHashType = result.computeHashType;
    QString filePath = result.filePath;

    QListWidgetItem *item = m_fileItemHash.value(filePath);

    if(nullptr == item)
        return;
    QList<util::ComputeResult> resultList = item->data(
                WidgetUtil::CheckResult).value<QList<util::ComputeResult> >();
    int resultListIndex = -1;
    for(int i = 0 ; i < resultList.length() ; i++)
    {
        if(computeHashType == resultList[i].computeHashType)
        {
            resultListIndex = i;
            break;
        }
    }
    if(resultListIndex == -1)
        resultList.append(result);
    else
        resultList.replace(resultListIndex, result);
    QVariant data;
    data.setValue(result);
    item->setData(WidgetUtil::CheckResult,data);

    if(messageType == util::CheckOver)
        m_computeThreadCount--;

}

void BackstageWork::onThreadEffectiveness()
{
    while (m_operatingStatus)
    {
        if(m_computeFileIndex >= m_filePathList.length())
        {
            m_operatingStatus = false;
            continue;
        }
        if(m_computeThreadCount >= m_computeMaxThreadCount)
            continue;
        QThread::msleep(35);
        m_computeFileIndex++;
        m_computeThreadCount++;
        WidgetUtil::Progress progress;
        progress.FileStatistics = m_filePathList.length();
        progress.ComputeProgress = m_computeFileIndex;
        emit signalFileStatistics(progress);
    }
}

void BackstageWork::init()
{
    m_listWidget = nullptr;
    m_dirPath = QString("..");
    m_fileFilters.append(QString("*.*"));
    m_computeThreadCount = 3;
    m_computeFileIndex = 0;
    m_operatingStatus = false;
    QObject::connect(&m_selectFiles, SIGNAL(signalFilePath(QString)), this, SLOT(onListWidgetAddItem(QString))  );
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
