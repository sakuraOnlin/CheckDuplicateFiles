#include "computemodule.h"
#include "computehash.h"

ComputeWork::ComputeWork(QObject *parent)
    :QObject(parent),
      m_computeIndex(0),
      m_computeThreadMaxNum(3),
      m_threadRunCount(0),
      m_operatingStatus(false)
{
    init();
}

ComputeWork::~ComputeWork()
{
    m_operatingStatus = false;
    QThread::msleep(50);
    for(int i = 0 ; i < m_checkFilelist.length(); i++)
    {
        m_checkFilelist[i].first->onStop();
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
    return m_computeIndex;
}

void ComputeWork::onWork()
{
    QThread::msleep(50);
    m_computeIndex = -1;
    while (m_operatingStatus)
    {
        QThread::msleep(35);
        if(m_computeIndex >= m_filePaths->length())
        {
            m_operatingStatus = false;
            continue;
        }
        if(m_threadRunCount >= m_computeThreadMaxNum)
            continue;

        for(int i = 0 ; i< m_checkFilelist.length(); i++)
        {
            if(!m_checkFilelist[i].second)
            {
                ComputeHash *compute = m_checkFilelist[i].first;
                compute->onRestore();
                compute->setFilePath(m_filePaths->value(++m_computeIndex));
                compute->onStart();
                m_checkFilelist[i].second = true;
                m_computeHash.insert(compute, m_filePaths->value(m_computeIndex));
                m_threadRunCount++;
            }
        }
    }
}

void ComputeWork::init()
{
    for(int i = 0 ; i < m_computeThreadMaxNum ; i++)
    {
        ComputeHash *value = new ComputeHash(util::MD5 | util::SHA1 |util::CRC32);
        m_checkFilelist.append(qMakePair(value, false));

    }

}


ComputeModule::ComputeModule(QObject *parent) : QObject(parent)
{
    init();
}

ComputeModule::~ComputeModule()
{
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

void ComputeModule::onStart()
{
    m_computeWork->m_operatingStatus = true;
    emit signalStart();
}

void ComputeModule::onStop()
{
    m_computeWork->m_operatingStatus = false;
    QThread::msleep(30);
    for(int i = 0 ; i < m_computeWork->m_checkFilelist.length(); i++)
    {
        m_computeWork->m_checkFilelist[i].first->onStop();
        m_computeWork->m_checkFilelist[i].second = false;
    }
}

void ComputeModule::onStopCheckFile(QString filePath)
{
    if(filePath.isEmpty())
        return;
    ComputeHash* value = m_computeWork->m_computeHash.key(filePath);
    if(nullptr == value)
        return;
    value->onStop();
}

void ComputeModule::onHandleErrStr(QString err)
{
    ComputeHash* value = dynamic_cast<ComputeHash*>(sender());
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
    ComputeHash* value = dynamic_cast<ComputeHash*>(sender());
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

    for(int i = 0 ; i < m_computeWork->m_checkFilelist.length(); i++)
    {
        ComputeHash *comHash = m_computeWork->m_checkFilelist[i].first;
        connect(comHash, SIGNAL(signalFinalResult(util::ComputeResult)), this,
                SIGNAL(signalFinalResult(util::ComputeResult)) );
        connect(comHash, SIGNAL(signalError(QString)), this,
                SLOT(onHandleErrStr(QString)) );
        connect(comHash, SIGNAL(signalCalculationComplete()), this,
                SLOT(onHandleCalculationComplete()) );
    }

    m_computeWork->moveToThread(m_thread);
    m_thread->start();
}

