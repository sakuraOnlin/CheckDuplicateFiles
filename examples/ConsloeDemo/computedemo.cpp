#include <QDebug>
#include "computedemo.h"

ComputeDemo::ComputeDemo(int ComputeType, QString filePath, QObject *parent)
    :QObject(parent) ,
      m_compute(new ComputeHash(ComputeType))
{
    connect( m_compute ,&ComputeHash::signalFinalResult ,this ,&ComputeDemo::onGetResult );
    m_compute->setCheckFilePath(filePath);
}

ComputeDemo::~ComputeDemo()
{
    delete m_compute;
}

void ComputeDemo::onGetResult(util::computeResult result)
{
    switch (result.resultMessageType) {
    case util::CheckError:
    {
        qDebug() << "CheckError";
        print(result);
        qDebug() << "EndCheckError";
        break;
    }
    case util::CheckIng:
    {
        qDebug() << "CheckIng";
        print(result);
        qDebug() << "EndCheckIng";
        break;
    }
    case util::CheckOver:
    {
        qDebug() << "CheckOver";
        print(result);
        qDebug() << "EndCheckOver";
        break;
    }
    default:
        break;
    }
}

void ComputeDemo::print(util::computeResult result)
{
    qDebug() << "+++++++++++++++++++++++++++++++++++++++";
    qDebug() << "  Type        : " << (int)result.computeHashType;
    qDebug() << "  MessageType : " << (int)result.resultMessageType;
    qDebug() << "  Size        : " << result.fileSize;
    qDebug() << "  Progress    : " << result.computeProgress;
    qDebug() << "  FilePath    : " << result.filePath;
    qDebug() << "  ResultStr   : " << result.resultStr;
    qDebug() << "|||||||||---------------------|||||||||";
}
