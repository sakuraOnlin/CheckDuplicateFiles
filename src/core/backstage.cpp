#include "backstage.h"
#include "core/computemodule.h"

#include <QFileIconProvider>
#include <QFileInfo>
#include <QDateTime>
#include <QVector>

Q_DECLARE_METATYPE(util::ComputeResult)

BackstageWork::BackstageWork(QObject *parnet)
    :QObject(parnet)
{
    init();
}

BackstageWork::~BackstageWork()
{
    m_time.stop();
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

void BackstageWork::setFilePath(QStringList *filePathList)
{
    m_filePathList = filePathList;
}

void BackstageWork::setFileItem(QHash<QString, QListWidgetItem *> *fileItemHash,
                                QSize &iconSize)
{
    m_fileItemHash = fileItemHash;
    m_iconSize = iconSize;
}

bool BackstageWork::getOperatingStatus()
{
    return m_operatingStatus;
}

void BackstageWork::onStart()
{
    m_filePathList->clear();
    m_fileItemHash->clear();
    m_operatingStatus = true;
    m_selectFiles.setDirPath(m_dirPath);
    m_selectFiles.setFilters(m_fileFilters);
    m_selectFiles.onStartSelectFiles();
    m_computeModule.setFilePathList(m_filePathList);
    m_computeModule.onStart();
    m_time.start(30);
}

void BackstageWork::onStop()
{
    m_operatingStatus = false;
    m_selectFiles.onStopSelectFiles();
    m_time.stop();
}

void BackstageWork::onListWidgetAddItem(QString filePath)
{
    if(filePath.isEmpty())
        return;

    QListWidgetItem *item = m_fileItemHash->value(filePath);
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
        m_filePathList->append(filePath);
        m_fileItemHash->insert(filePath, item);
    }
}

void BackstageWork::onItemSetData(util::ComputeResult result)
{
    util::ComputeType computeHashType = result.computeHashType;
    QString filePath = result.filePath;

    QListWidgetItem *item = m_fileItemHash->value(filePath);

    if(nullptr == item)
        return;
    QList<util::ComputeResult> resultList = item->data(
                WidgetUtil::CheckResult).value<QList<util::ComputeResult> >();
    int resultListIndex = -1;
    for(int i = 0 ; i < resultList.length() ; i++)
    {
        if(computeHashType == resultList.value(i).computeHashType)
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
    data.setValue(resultList);
    item->setData(WidgetUtil::CheckResult, data);
}

void BackstageWork::onItemComputeErr(QString filePath, QString errStr)
{
    Q_UNUSED(errStr)

    QListWidgetItem *item = m_fileItemHash->value(filePath);
    if(nullptr == item)
        return;

}

void BackstageWork::onItemCalculationComplete(QString filePath)
{
    QListWidgetItem *item = m_fileItemHash->value(filePath);
    if(nullptr == item)
        return;
}

void BackstageWork::onTimeSengProgress()
{
    WidgetUtil::Progress progress;
    progress.FileStatistics = m_filePathList->length();
    progress.ComputeProgress = m_computeModule.getComputeProgress();
    emit signalFileStatistics(progress);
}

void BackstageWork::init()
{
    qRegisterMetaType<QVector<int> >("QVector<int>");
    m_listWidget = nullptr;
    m_dirPath = QString("..");
    m_fileFilters.append(QString("*.*"));
    m_filePathList = nullptr;
    m_fileItemHash = nullptr;
    m_operatingStatus = false;
    QObject::connect(&m_selectFiles, SIGNAL(signalFilePath(QString)), this, SLOT(onListWidgetAddItem(QString))  );
    QObject::connect(&m_computeModule, SIGNAL(signalFinalResult(util::ComputeResult)),
                     this, SLOT(onItemSetData(util::ComputeResult)) );
    QObject::connect(&m_computeModule, SIGNAL(signalError(QString,QString)),
                     this, SLOT(onItemComputeErr(QString,QString)) );
    QObject::connect(&m_computeModule, SIGNAL(signalCalculationComplete(QString)),
                     this, SLOT(onItemCalculationComplete(QString)));
    QObject::connect(&m_time, SIGNAL(timeout()), this, SLOT(onTimeSengProgress()) );
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
