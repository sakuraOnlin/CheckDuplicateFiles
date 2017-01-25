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
    foreach (ComputeHash *value, m_computeRunList)
    {
        value->onStop();
        delete value;
    }

    foreach (ComputeHash *value, m_computeRestList)
    {
        value->onStop();
        delete value;
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
    m_computeIndex = 0;
    while (m_operatingStatus)
    {
        if(m_computeRestList.length() == 0)
            continue;
        QThread::msleep(35);
        if(m_computeIndex >= m_filePaths->length())
        {
            m_operatingStatus = false;
            continue;
        }
        if(m_threadRunCount >= m_computeThreadMaxNum)
            continue;
        ComputeHash *compute = m_computeRestList.takeFirst();
        compute->onRestore();
        compute->setFilePath(m_filePaths->value(m_computeIndex));
        compute->onStart();
        m_computeRunList.append(compute);
        m_computeHash.insert(compute, m_filePaths->value(m_computeIndex));
        m_computeIndex++;
        m_threadRunCount++;
    }
}

void ComputeWork::init()
{
    for(int i = 0 ; i < m_computeThreadMaxNum ; i++)
    {
        m_computeRestList.append(new ComputeHash(
                                     util::MD5 |
                                     util::SHA1 |
                                     util::CRC32) );
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
    for(int i = 0 ; i < m_computeWork->m_computeRunList.length(); i = 0)
    {
        ComputeHash *value = m_computeWork->m_computeRunList.takeAt(i);
        value->onStop();
        m_computeWork->m_computeRestList.append(value);
    }
}

void ComputeModule::onHandleErrStr(QString err)
{
    ComputeHash* value = (ComputeHash*)sender();
    QString filePath(m_computeWork->m_computeHash.value(value));
    int index = m_computeWork->m_computeRunList.indexOf(value);
    m_computeWork->m_computeRestList.append(
                m_computeWork->m_computeRunList.takeAt(index));
    emit signalError(filePath, err);
    m_computeWork->m_threadRunCount--;
}

void ComputeModule::onHandleCalculationComplete()
{
    ComputeHash* value = (ComputeHash*)sender();
    QString filePath(m_computeWork->m_computeHash.value(value));
    int index = m_computeWork->m_computeRunList.indexOf(value);
    m_computeWork->m_computeRestList.append(
                m_computeWork->m_computeRunList.takeAt(index));
    emit signalCalculationComplete(filePath);
    m_computeWork->m_threadRunCount--;
}

void ComputeModule::init()
{
    m_thread = new QThread;
    m_computeWork = new ComputeWork;
    connect(m_thread, SIGNAL(finished()), m_computeWork, SLOT(deleteLater()) );
    connect(this, SIGNAL(signalStart()), m_computeWork, SLOT(onWork()) );

    for(int i = 0 ; i < m_computeWork->m_computeRestList.length(); i++)
    {
        ComputeHash *comHash = m_computeWork->m_computeRestList.value(i);
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

