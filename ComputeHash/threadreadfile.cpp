#include <QString>
#include <QFile>

#include "threadreadfile.h"
#include "compute.h"

#ifdef _DEBUG
#include <QDebug>
#endif

ThreadReadFile::ThreadReadFile(QObject *parent)
    :QObject(parent)
{
}

void ThreadReadFile::doWork(util::factoryCreateResult computeStruct, QString filePath)
{
    qint64 fileSize = 0;
    qint64 fileProgress = 0;
    qint64 loadFileData = 0;

    Compute *compute = computeStruct.creatorComputr;
    if(NULL == compute)
    {
        emitResult(util::CheckError ,computeStruct.computeHashType ,filePath ,
                   fileSize ,fileProgress ,computeStruct.creatorErrStr);
        return;
    }

    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        emitResult(util::CheckError ,computeStruct.computeHashType ,filePath ,
                   fileSize ,fileProgress ,tr("File open errors!"));
        return;
    }
    fileSize = file.size();
    loadFileData = automaticDivision(fileSize);

    //start read file data to Compute Hash
    while(!file.atEnd())
    {
        QByteArray readFileRawData = file.read(loadFileData);
        compute->update(readFileRawData);
        fileProgress += loadFileData;
        emitResult(util::CheckIng ,compute->getType() ,filePath ,fileSize ,fileProgress);
    }

    //read file atEnd, emit Hash
    file.close();
    QString computeResultStr(compute->getFinalResult());
    emitResult(util::CheckOver ,compute->getType() ,filePath ,fileSize ,
               fileProgress ,computeResultStr);

#ifdef _DEBUG
    qDebug() << "Result Valur :" << filePath + " , util::ComputeType :" +
                QString::number((int)compute->getType()) + " , " + computeResultStr;
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
    emit resultReady( computeResult);
}

qint64 ThreadReadFile::automaticDivision(qint64 fileSize)
{
    qint64 defaultSize = 1024;
    if(fileSize < defaultSize * (qint64)20)
    {
        return fileSize;
    }
    else if(fileSize <= defaultSize * 50)
    {
        return (fileSize / (qint64)2);
    }
    else if(fileSize <= defaultSize *100)
    {
        return (fileSize / (qint64)4);
    }
    else
    {
        return defaultSize * (qint64)20;
    }

    return defaultSize;
}
