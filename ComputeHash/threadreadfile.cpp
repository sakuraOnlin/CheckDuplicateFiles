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
    m_fileSize = 0;
    m_fileProgress = 0;
    m_loadFileData = 2048;
}

void ThreadReadFile::doWork(util::factoryCreateResult computeStruct, QString filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        emitResult(util::CheckError ,computeStruct.computeHashType ,filePath ,
                   m_fileSize ,m_fileProgress ,tr("File open errors!"));
        return;
    }
    m_fileSize = file.size();

    Compute *compute = computeStruct.creatorComputr;
    if(NULL == compute)
    {
        emitResult(util::CheckError ,computeStruct.computeHashType ,filePath ,
                   m_fileSize ,m_fileProgress ,computeStruct.creatorErrStr);
        file.close();
        return;
    }

    //start read file data to Compute Hash
    while(!file.atEnd())
    {
        QByteArray readFileRawData = file.read(m_loadFileData);
        compute->update(readFileRawData);
        m_fileProgress += m_loadFileData;
        emitResult(util::CheckIng ,compute->getType() ,filePath ,m_fileSize ,m_fileProgress);
    }

    //read file atEnd, emit Hash
    QString computeResultStr(compute->getFinalResult());
    emitResult(util::CheckOver ,compute->getType() ,filePath ,m_fileSize ,
               m_fileProgress ,computeResultStr);

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
