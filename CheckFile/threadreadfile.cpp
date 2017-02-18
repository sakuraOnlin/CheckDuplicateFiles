#include <QString>
#include <QFile>
#include <QThread>

#include "threadreadfile.h"
#include "compute.h"

#ifdef _DEBUG
#include <QDebug>
#endif

ThreadReadFile::ThreadReadFile(QObject *parent)
    :QObject(parent), 
      m_isWork(false)
{
    m_result.creatorComputr = nullptr;
}

ThreadReadFile::~ThreadReadFile()
{
}

void ThreadReadFile::setCheckData(util::factoryCreateResult result)
{
    m_result = result;
}

void ThreadReadFile::onDoWork(QString filePath)
{
    if(filePath.isEmpty())
        return;

    qint64 fileSize = 0;
    qint64 fileProgress = 0;
    qint64 loadFileData = 0;

    Compute *compute = m_result.creatorComputr;
    if(nullptr == compute)
    {
        emitResult(util::CheckError, m_result.checkHashType, filePath,
                   fileSize, fileProgress, QString("NoType"), m_result.creatorErrStr);
        return;
    }

    util::CheckType getType = compute->getType();
    QString typeName(compute->getTypeName());
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        emitResult(util::CheckError, m_result.checkHashType, filePath,
                   fileSize, fileProgress, typeName, tr("File open errors!"));
        return;
    }
    fileSize = file.size();
    loadFileData = automaticDivision(fileSize);

    m_isWork = true;
    //start read file data to Compute Hash
    while(!file.atEnd() && m_isWork)
    {
        if(!m_isWork)
        {
            emitResult(util::CheckError, getType,filePath, fileSize,
                       fileProgress, typeName,
                       tr("The user chooses to stop checking!"));
            return;
        }
        QByteArray readFileRawData = file.read(loadFileData);
        compute->update(readFileRawData);
        fileProgress += readFileRawData.size();
        emitResult(util::CheckIng, getType,filePath, fileSize,
                   fileProgress, typeName);
    }
    file.close();

    QString computeResultStr(compute->getFinalResult());
    emitResult(util::CheckOver, getType, filePath, fileSize,
               fileProgress, typeName, computeResultStr);

}

void ThreadReadFile::onRestore()
{
    if(nullptr != m_result.creatorComputr)
    {
        m_result.creatorComputr->reset();
    }
    m_isWork = false;
}

void ThreadReadFile::emitResult(util::ResultMessageType resultType,
                                util::CheckType computeType, QString filePath,
                                qint64 fileSize, qint64 fileProgress, QString typeName, QString result)
{
    util::ComputeResult computeResult;
    computeResult.resultMessageType = resultType;
    computeResult.checkHashType = computeType;
    computeResult.fileSize = fileSize;
    computeResult.computeProgress = fileProgress;
    computeResult.filePath = filePath;
    computeResult.resultStr = result;
    computeResult.checkTypeName = typeName;
    emit signalResultReady( computeResult);
    if(resultType == util::CheckError || resultType == util::CheckOver)
        emit signalCalculationComplete();
}

qint64 ThreadReadFile::automaticDivision(qint64 fileSize)
{
    qint64 defaultSize = 1024 ; // 1 kb
    defaultSize *= qint64(1024); // 1MB
    if(fileSize < defaultSize * qint64(20)) // fileSize <= 20 MB
    {
        return fileSize;
    }
    else if(fileSize <= defaultSize * 50)   // fileSize <= 50 MB
    {
        return (fileSize / qint64(2));
    }
    else if(fileSize <= defaultSize *100)   // fileSize <= 100 MB
    {
        return (fileSize / qint64(3));
    }
    else                                    // fileSize > 100 MB
    {
        return defaultSize * qint64(40);
    }

    return defaultSize;
}

ThreadControl::ThreadControl(QObject *parent)
    :QObject(parent),
      m_moduleCounter(0),
      m_dirPath(QString()),
      m_operatingStatus(false)
{
}

ThreadControl::~ThreadControl()
{
    stopApp();
}

void ThreadControl::setDirPath(QString dirPath)
{
    m_dirPath = dirPath;
}

void ThreadControl::setFactorys(QList<util::factoryCreateResult> &list)
{
    m_listFactorys = list;
}

bool ThreadControl::getOperatingStatus()
{
    return m_operatingStatus;
}

void ThreadControl::start()
{
    if(m_operatingStatus)
        stopCheck();

    disconnect(this, SIGNAL(signalStartCheck(QString)), 0, 0);

    for(int i = 0 ; i < m_listFactorys.length(); i++ )
    {
        QThread *thread = nullptr;
        ThreadReadFile *readThread = nullptr;
        if(i >= m_threadList.length())
        {
            thread = new QThread;
            readThread = new ThreadReadFile;
            readThread->moveToThread( thread);
            connect( thread, SIGNAL(finished()), readThread, SLOT(deleteLater()) );
            connect( readThread, SIGNAL(signalResultReady(util::ComputeResult)),
                     this, SIGNAL(signalFinalResult(util::ComputeResult)) );
            connect( readThread, SIGNAL(signalCalculationComplete()),
                     this, SLOT(onModuleCounter()) );
            connect( this, SIGNAL(signalStartCheck(QString) ),
                     readThread, SLOT(onDoWork(QString)) );
            connect( this, SIGNAL(signalRestore()), readThread, SLOT(onRestore()) );
            thread->start();
            m_threadList.append(qMakePair(thread, readThread));
        }
        else
        {
            readThread = m_threadList.value(i).second;
            connect( this, SIGNAL(signalStartCheck(QString) ),
                     readThread, SLOT(onDoWork(QString)) );
        }
        readThread->setCheckData(m_listFactorys.value(i));
        m_readFileThreadList.append(qMakePair(thread, readThread));
    }
    m_operatingStatus = true;
    emit signalStartCheck(m_dirPath);
}

void ThreadControl::stopApp()
{
    for(int i = 0 ; i < m_threadList.length() ; i = 0)
    {
        QPair<QThread*, ThreadReadFile *> value = m_threadList.takeAt(i);
        value.second->m_isWork = false;
        value.second->m_result.creatorComputr->stop();
        value.first->quit();
        value.first->wait();
        delete value.first;
    }
}

void ThreadControl::stopCheck()
{
    for(int i = 0 ; i < m_readFileThreadList.length() ; i++)
    {
        m_readFileThreadList[i].second->m_isWork = false;
        m_readFileThreadList[i].second->m_result.creatorComputr->stop();
    }
}

void ThreadControl::restore()
{
    emit signalRestore();
}

void ThreadControl::onModuleCounter()
{
    ThreadReadFile *read = dynamic_cast<ThreadReadFile*>(sender());
    for(int i = 0 ; i < m_readFileThreadList.length(); i++)
    {
        if(m_readFileThreadList.at(i).second == read)
            m_readFileThreadList.takeAt(i);
    }
    if(m_readFileThreadList.length() <= 0)
    {
        emit signalCalculationComplete();
    }
}
