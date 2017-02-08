#include "computemodule.h"
#include "CheckFile.h"

ComputeWork::ComputeWork(QObject *parent)
    :QObject(parent),
      m_computeIndex(0),
      m_computeThreadMaxNum(3),
      m_threadRunCount(0),
      m_operatingStatus(false)
{
}

ComputeWork::~ComputeWork()
{
    m_operatingStatus = false;
    QThread::msleep(50);
    for(int i = 0 ; i < m_checkFilelist.length(); i++)
    {
        m_checkFilelist[i].first->onStopCheck();
        delete  m_checkFilelist[i].first;
        m_checkFilelist[i].second = false;
    }

}

void ComputeWork::setFilePathList(QStringList *filePathList)
{
    m_filePaths = filePathList;
}

int ComputeWork::getComputeProgress()
{
    // m_computeIndex start 0
    return m_computeIndex + 1;
}

void ComputeWork::createCheck(util::CheckType checkType)
{
    for(int i = 0 ; i < m_computeThreadMaxNum ; i++)
    {
        CheckFile *value = new CheckFile;
        value->setCheckType(checkType);
        m_checkFilelist.append(qMakePair(value, false));
    }
}

bool ComputeWork::setCheckThreadNum(int num)
{
    if(m_operatingStatus)
        return false;
    if(num > 4)
        return false;
    m_computeThreadMaxNum = num;
    return true;
}

int ComputeWork::getCheckThreadNum()
{
    return m_computeThreadMaxNum;
}

void ComputeWork::onWork()
{
    m_computeIndex = -1;
    while (m_operatingStatus)
    {
        if( (m_filePaths->length() <= 0) ||
                (m_computeIndex >= m_filePaths->length()) ||
                (m_threadRunCount >= m_computeThreadMaxNum))
            continue;

        for(int i = 0 ; (i< m_checkFilelist.length()) && m_operatingStatus; i++)
        {
            if(!m_checkFilelist[i].second)
            {
                QString file(m_filePaths->value(m_computeIndex + 1));
                if(file.isEmpty())
                    break;

                CheckFile *compute = m_checkFilelist[i].first;
                compute->onRestore();
                compute->setFilePath(file);
                compute->onStart();
                if(!m_operatingStatus) break;
                m_computeHash.insert(compute, file);
                m_computeIndex++;
                m_threadRunCount++;
                m_checkFilelist[i].second = true;
            }
        }
    }
}

ComputeModule::ComputeModule(QObject *parent) : QObject(parent)
{
    init();
}

ComputeModule::~ComputeModule()
{
    onStop();
    m_thread->quit();
    m_thread->wait(100);
    delete m_thread;
    m_thread = nullptr;
}

void ComputeModule::setFilePathList(QStringList *filePathList)
{
    m_computeWork->setFilePathList(filePathList);
}

int ComputeModule::getComputeProgress()
{
    return m_computeWork->getComputeProgress();
}

bool ComputeModule::setCheckThreadNum(int num)
{
    return m_computeWork->setCheckThreadNum(num);
}

int ComputeModule::getCheckThreadNum()
{
    return m_computeWork->getCheckThreadNum();
}

void ComputeModule::onStart(int checkType)
{
    m_computeWork->m_operatingStatus = true;
    m_computeWork->createCheck(util::CheckType(checkType));
    connectWork();
    emit signalStart();
}

void ComputeModule::onStop()
{
    m_computeWork->m_operatingStatus = false;
    for(int i = 0 ; i < m_computeWork->m_checkFilelist.length(); i = 0)
    {
        QPair<CheckFile*, bool> value = m_computeWork->m_checkFilelist.takeAt(i);
        value.first->onStopCheck();
        value.second = false;
        delete  value.first;
    }
}

void ComputeModule::onStopCheckFile(QString filePath)
{
    if(filePath.isEmpty())
        return;
    CheckFile* value = m_computeWork->m_computeHash.key(filePath);
    if(nullptr == value)
        return;
    value->onStopCheck();
}

void ComputeModule::onHandleErrStr(QString err)
{
    CheckFile* value = dynamic_cast<CheckFile*>(sender());
    if(nullptr == value)
        return;

    for(int i = 0 ; i < m_computeWork->m_checkFilelist.length(); i++)
    {
        if(value == m_computeWork->m_checkFilelist[i].first)
            m_computeWork->m_checkFilelist[i].second = false;
    }

    QString filePath(m_computeWork->m_computeHash.value(value));
    m_computeWork->m_computeHash.remove(value);
    emit signalError(filePath, err);
    m_computeWork->m_threadRunCount--;
}

void ComputeModule::onHandleCalculationComplete()
{
    CheckFile* value = dynamic_cast<CheckFile*>(sender());
    if(nullptr == value)
        return;

    for(int i = 0 ; i < m_computeWork->m_checkFilelist.length(); i++)
    {
        if(value == m_computeWork->m_checkFilelist[i].first)
            m_computeWork->m_checkFilelist[i].second = false;
    }

    QString filePath(m_computeWork->m_computeHash.value(value));
    m_computeWork->m_computeHash.remove(value);
    emit signalCalculationComplete(filePath);
    m_computeWork->m_threadRunCount--;
}

void ComputeModule::init()
{
    m_thread = new QThread;
    m_computeWork = new ComputeWork;
    connect(m_thread, SIGNAL(finished()), m_computeWork, SLOT(deleteLater()) );
    connect(this, SIGNAL(signalStart()), m_computeWork, SLOT(onWork()) );

    m_computeWork->moveToThread(m_thread);
    m_thread->start();
}

void ComputeModule::connectWork()
{
    for(int i = 0 ; i < m_computeWork->m_checkFilelist.length(); i++)
    {
        CheckFile *comHash = m_computeWork->m_checkFilelist[i].first;
        connect(comHash, SIGNAL(signalFinalResult(util::ComputeResult)), this,
                SIGNAL(signalFinalResult(util::ComputeResult)) );
        connect(comHash, SIGNAL(signalError(QString)), this,
                SLOT(onHandleErrStr(QString)) );
        connect(comHash, SIGNAL(signalCalculationComplete()), this,
                SLOT(onHandleCalculationComplete()) );
    }
}

