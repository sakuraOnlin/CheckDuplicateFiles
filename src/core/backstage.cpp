#include "backstage.h"
#include "core/computemodule.h"

#include <QFileIconProvider>
#include <QFileInfo>
#include <QDateTime>
#include <QVector>

QMutex Backstage::mutex;
QScopedPointer<Backstage> Backstage::instance;

Q_DECLARE_METATYPE(util::ComputeResult)

BackstageWork::BackstageWork(QObject *parnet)
    :QObject(parnet),
    m_selectItem(nullptr)
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
    m_findRepeat.setListWidget(m_listWidget);
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
    m_findRepeat.setFileItem(m_fileItemHash);
}

bool BackstageWork::getOperatingStatus()
{
    return m_operatingStatus;
}

bool BackstageWork::setCheckThreadNum(int num)
{
    return m_computeModule.setCheckThreadNum(num);
}

int BackstageWork::getCheckThreadNum()
{
    return m_computeModule.getCheckThreadNum();
}

void BackstageWork::onStart(int checkType)
{
    m_selectItem = nullptr;
    m_filePathList->clear();
    m_fileItemHash->clear();
    m_operatingStatus = true;
    m_selectFiles.setDirPath(m_dirPath);
    m_selectFiles.setFilters(m_fileFilters);
    m_selectFiles.onStartSelectFiles();
    m_computeModule.setFilePathList(m_filePathList);
    m_computeModule.onStart(checkType);
    m_findRepeat.onStart();
    m_time.start(30);
}

void BackstageWork::onStop()
{
    m_operatingStatus = false;
    m_selectFiles.onStopSelectFiles();
    m_computeModule.onStop();
    m_findRepeat.onStop();
    m_time.stop();
}

void BackstageWork::onDelFile(QString filePath)
{
    m_filePathList->removeAt(m_filePathList->indexOf(filePath));
    m_removeFile.remove(filePath);
    m_fileItemHash->take(filePath);
    m_selectItem = nullptr;
}

void BackstageWork::onStopCheckFile(QString filePath)
{
    m_computeModule.onStopCheckFile(filePath);
}

void BackstageWork::onFindAllRepeat()
{
    m_findRepeat.onFindAllRepeat();
}

void BackstageWork::onFindText(QString text)
{
    m_findRepeat.onFindText(text);
}

void BackstageWork::onFindNextText()
{
    m_findRepeat.onFindNextText();
}

void BackstageWork::onClearFindRepeat()
{
    m_findRepeat.onRestoreBackground();
}

void BackstageWork::onClickItem(QListWidgetItem *item)
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

void BackstageWork::onListWidgetAddItem(QStringList filePathList)
{
    if(filePathList.length() == 0)
        return;

    foreach (QString filePath, filePathList)
    {
        QListWidgetItem *item = nullptr;
        m_fileItemHash->contains(filePath) ? item = m_fileItemHash->value(filePath): item = nullptr;
        if(nullptr == item)
        {
            QFileInfo fileInfo(filePath);
            QFileIconProvider fileIco;
            QIcon ico(fileIco.icon(fileInfo));
            QPixmap pixmap(ico.pixmap(m_iconSize));
            QListWidgetItem *item = new QListWidgetItem;
            item->setSizeHint(QSize(400, 70));
            item->setData(WidgetUtil::FilePathRole,filePath);
            item->setData(WidgetUtil::FileIcoRole,pixmap);
            item->setData(WidgetUtil::FileSizeRole, QString::number(fileInfo.size()));
            item->setData(WidgetUtil::FileName, fileInfo.fileName());
            item->setData(WidgetUtil::FileTimeRole, fileInfo.lastModified().
                          toString("yyyy-MM-dd hh:mm:ss"));
            item->setData(WidgetUtil::CheckTypeRole, util::NoCheck);
//            item->setData(Qt::BackgroundColorRole, QColor(125, 174, 255, 254));
            m_listWidget->addItem(item);
            m_filePathList->append(filePath);
            m_fileItemHash->insert(filePath, item);
        }
    }
}

void BackstageWork::onItemSetData(util::ComputeResult result)
{
    util::CheckType checkHashType = result.checkHashType;
    QString filePath = result.filePath;

    QListWidgetItem *item = nullptr;
    m_fileItemHash->contains(filePath) ? item = m_fileItemHash->value(filePath): item = nullptr;

    if(nullptr == item)
        return;
    QList<util::ComputeResult> resultList = item->data(
                WidgetUtil::CheckResultRole).value<QList<util::ComputeResult> >();
    int resultListIndex = -1;
    for(int i = 0 ; i < resultList.length() ; i++)
    {
        if(checkHashType == resultList.value(i).checkHashType)
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
    item->setData(WidgetUtil::CheckResultRole, data);
    item->setData(WidgetUtil::CheckTypeRole, util::CheckIng);
}

void BackstageWork::onItemComputeErr(QString filePath, QString errStr)
{
    QListWidgetItem *item = nullptr;
    m_fileItemHash->contains(filePath) ? item = m_fileItemHash->value(filePath): item = nullptr;
    if(nullptr == item)
        return;
    item->setData(WidgetUtil::CheckResultRole,errStr);
    item->setData(WidgetUtil::CheckTypeRole, util::CheckError);
}

void BackstageWork::onItemCalculationComplete(QString filePath)
{
    QListWidgetItem *item = nullptr;
    m_fileItemHash->contains(filePath) ? item = m_fileItemHash->value(filePath): item = nullptr;
    if(nullptr == item)
        return;
    item->setData(WidgetUtil::CheckTypeRole, util::CheckOver);
    m_findRepeat.onAddResultID(item);
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
    m_filePathList = nullptr;
    m_fileItemHash = nullptr;
    m_operatingStatus = false;
    QObject::connect(&m_selectFiles, SIGNAL(signalFilePathList(QStringList)), this, SLOT(onListWidgetAddItem(QStringList))  );
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

Backstage &Backstage::getInstance()
{
    if(instance.isNull())
    {
        mutex.lock();
        if(instance.isNull())
        {
            instance.reset(new Backstage());
        }
        mutex.unlock();
    }
    return *instance.data();
}


BackstageWork *Backstage::getBackstagWork() const
{
    return m_backstageWork;
}
