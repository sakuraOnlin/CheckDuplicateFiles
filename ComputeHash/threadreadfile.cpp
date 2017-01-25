#include <QString>
#include <QFile>
#include <QThread>

#include "threadreadfile.h"
#include "compute.h"

#ifdef _DEBUG
#include <QDebug>
#endif

ThreadReadFile::ThreadReadFile(util::factoryCreateResult result, QObject *parent)
    :QObject(parent), 
      m_result(result), 
      m_isWork(false)
{
}

ThreadReadFile::~ThreadReadFile()
{
    if(nullptr != m_result.creatorComputr)
    {
        delete m_result.creatorComputr;
        m_result.creatorComputr = nullptr;
    }
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
        emitResult(util::CheckError, m_result.computeHashType, filePath,
                   fileSize, fileProgress, QString("NoType"), m_result.creatorErrStr);
        return;
    }

    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        emitResult(util::CheckError, m_result.computeHashType, filePath,
                   fileSize, fileProgress, compute->getTypeName(), tr("File open errors!"));
        return;
    }
    fileSize = file.size();
    loadFileData = automaticDivision(fileSize);
    util::ComputeType getType = compute->getType();

    m_isWork = true;
    //start read file data to Compute Hash
    while(!file.atEnd() && m_isWork)
    {
        QByteArray readFileRawData = file.read(loadFileData);
        compute->update(readFileRawData);
        fileProgress += loadFileData;
        emitResult(util::CheckIng, getType,filePath, fileSize,
                   fileProgress, compute->getTypeName());
    }

    //read file atEnd, emit Hash
    file.close();
    QString computeResultStr(compute->getFinalResult());
    emitResult(util::CheckOver, getType, filePath, fileSize,
               fileProgress, compute->getTypeName(), computeResultStr);
    compute->reset();
    emit signalCalculationComplete();

}

void ThreadReadFile::onStop()
{
    m_isWork = false;
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
                                util::ComputeType computeType, QString filePath,
                                qint64 fileSize, qint64 fileProgress, QString typeName, QString result)
{
    util::ComputeResult computeResult;
    computeResult.resultMessageType = resultType;
    computeResult.computeHashType = computeType;
    computeResult.fileSize = fileSize;
    computeResult.computeProgress = fileProgress;
    computeResult.filePath = filePath;
    computeResult.resultStr = result;
    computeResult.checkTypeName = typeName;
    emit signalResultReady( computeResult);
}

qint64 ThreadReadFile::automaticDivision(qint64 fileSize)
{
    qint64 defaultSize = 1024 ; // 1 kb
    defaultSize *= 1024; // 1MB
    if(fileSize < defaultSize * (qint64)20) // 20 MB
    {
        return fileSize;
    }
    else if(fileSize <= defaultSize * 50)   // 50 MB
    {
        return (fileSize / (qint64)2);
    }
    else if(fileSize <= defaultSize *100)   // 100 MB
    {
        return (fileSize / (qint64)3);
    }
    else                                    // fileSize > 100 MB
    {
        return defaultSize * (qint64)40;
    }

    return defaultSize;
}

ThreadControl::ThreadControl(QObject *parent)
    :QObject(parent),
      m_moduleCounter(0),
      m_dirPath(QString())
{
    m_count = 0;
}

ThreadControl::~ThreadControl()
{
    stop();
}

void ThreadControl::setDirPath(QString dirPath)
{
    m_dirPath = dirPath;
}

void ThreadControl::setFactorys(QList<util::factoryCreateResult> &list)
{
    m_listFactorys = list;
}

void ThreadControl::start()
{
    stop();

    for(int i = 0 ; i < m_listFactorys.length();i++)
    {
        QThread *thread = new QThread;
        util::factoryCreateResult factoryValue = m_listFactorys[i];
        ThreadReadFile *work = new ThreadReadFile(factoryValue);
        work->moveToThread( thread );
        connect( thread, SIGNAL(finished()), work, SLOT(deleteLater()) );
        connect( work, SIGNAL(signalResultReady(util::ComputeResult)),
                 this, SIGNAL(signalFinalResult(util::ComputeResult)) );
        connect( work, SIGNAL(signalCalculationComplete()),
                 this, SLOT(onModuleCounter()) );
        connect( this, SIGNAL(signalStartCheck(QString) ),
                 work, SLOT(onDoWork(QString)) );
        connect( this, SIGNAL(signalRestore()), work, SLOT(onRestore()) );
        connect( this, SIGNAL(signalStop()), work, SLOT(onStop()));
        thread->start();
        m_readFileThreadList.append(thread);
    }

    m_moduleCounter = m_readFileThreadList.length();
    emit signalStartCheck(m_dirPath);
}

void ThreadControl::stop()
{
    emit signalStop();

    for(int i = 0 ; i < m_readFileThreadList.length() ; i = 0)
    {
        m_readFileThreadList[i]->quit();
        m_readFileThreadList[i]->wait(300);
        delete m_readFileThreadList[i];
        m_readFileThreadList.removeAt(i);
    }
}

void ThreadControl::restore()
{
    emit signalRestore();
}

void ThreadControl::onModuleCounter()
{
    m_moduleCounter--;
    if(m_moduleCounter <= 0)
        emit signalCalculationComplete();
    m_count++;
}
