#include <QString>
#include <QFile>

#include "threadreadfile.h"
#include "compute.h"

#ifdef _DEBUG
#include <QDebug>
#endif

ThreadReadFile::ThreadReadFile(util::factoryCreateResult result, QString filePath, QObject *parent)
    :QObject(parent) ,
      m_result(result) ,
      m_filePath(filePath)
{
}

void ThreadReadFile::doWork()
{
    qint64 fileSize = 0;
    qint64 fileProgress = 0;
    qint64 loadFileData = 0;

    Compute *compute = m_result.creatorComputr;
    if(NULL == compute)
    {
        emitResult(util::CheckError ,m_result.computeHashType ,m_filePath ,
                   fileSize ,fileProgress ,m_result.creatorErrStr);
        return;
    }

    QFile file(m_filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        emitResult(util::CheckError ,m_result.computeHashType ,m_filePath ,
                   fileSize ,fileProgress ,tr("File open errors!"));
        return;
    }
    fileSize = file.size();
    loadFileData = automaticDivision(fileSize);
    util::ComputeType getType = compute->getType();

    //start read file data to Compute Hash
    while(!file.atEnd())
    {
        QByteArray readFileRawData = file.read(loadFileData);
        compute->update(readFileRawData);
        fileProgress += loadFileData;
        emitResult(util::CheckIng , getType,m_filePath ,fileSize ,fileProgress);
    }

    //read file atEnd, emit Hash
    file.close();
    QString computeResultStr(compute->getFinalResult());
    emitResult(util::CheckOver ,getType ,m_filePath ,fileSize ,
               fileProgress ,computeResultStr);

#ifdef _DEBUG
    qDebug() << "Result Valur :" << m_filePath + " , util::ComputeType :" +
                QString::number((int)getType) + " , " + computeResultStr;
#endif
    delete compute;
}

void ThreadReadFile::emitResult(util::ResultMessageType resultType,
                                util::ComputeType computeType, QString filePath,
                                qint64 fileSize, qint64 fileProgress, QString result)
{
    util::computeResult computeResult;
    computeResult.resultMessageType = resultType;
    computeResult.computeHashType = computeType;
    computeResult.fileSize = fileSize;
    computeResult.computeProgress = fileProgress;
    computeResult.filePath = filePath;
    computeResult.resultStr = result;
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
