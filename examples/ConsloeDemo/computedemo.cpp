#include <QDebug>
#include "computedemo.h"

ComputeDemo::ComputeDemo(int ComputeType, QString filePath, QObject *parent)
    :QObject(parent), 
      m_compute(new ComputeHash(ComputeType))
{
    connect( m_compute, &ComputeHash::signalFinalResult, this, &ComputeDemo::onGetResult );
    connect(m_compute, &ComputeHash::signalCalculationComplete, this, &ComputeDemo::onCalculationComplete);
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

void ComputeDemo::onCalculationComplete()
{
    qDebug() << "onCalculationComplete()";
}

void ComputeDemo::print(util::computeResult &result)
{
    qDebug() << "+++++++++++++++++++++++++++++++++++++++";
    qDebug() << "  Type        : " << (int)result.computeHashType;
    qDebug() << "  TypeName    : " << result.checkTypeName;
    qDebug() << "  MessageType : " << (int)result.resultMessageType;
    qDebug() << "  Size        : " << result.fileSize;
    qDebug() << "  Progress    : " << result.computeProgress;
    qDebug() << "  FilePath    : " << result.filePath;
    qDebug() << "  ResultStr   : " << result.resultStr;
    qDebug() << "|||||||||---------------------|||||||||";
}
