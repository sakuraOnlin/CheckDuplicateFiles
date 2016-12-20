#include <QObject>
#include <QFileInfo>
#include <QFile>
#include <QDataStream>
#include "computehash.h"
#include "util/computefactory.h"
#include "compute.h"

class ComputeHashPrivate
{
    ComputeHash *q_ptr;
    Q_DECLARE_PUBLIC(ComputeHash)
public:
    ComputeHashPrivate(util::ComputeType type);

    ~ComputeHashPrivate();
    bool startCheck(QString filePath);

    QString m_errorStr;
    bool m_isStart;
    util::ComputeType m_conputeType;
    Compute *m_computeHash;
    Factory *m_factory;
};

ComputeHash::ComputeHash(QObject *parent, util::ComputeType type)
    :QObject(parent)
{
    d_ptr = new ComputeHashPrivate(type);
}

ComputeHash::~ComputeHash()
{
    delete d_ptr;
}

//设置好文件路径后就自动开始检查文件指纹
bool ComputeHash::setComputeHsahFile(QString filePath)
{
    QFileInfo fileInfo(filePath);
    if(filePath.isEmpty() && !fileInfo.isFile())
    {
        //请选择有效的文件进行文件指纹识别！错误地址： + filePath
        d_ptr->m_errorStr = tr("Please select a valid file for file fingerprinting! \n"
                               "Wrong address :") + filePath;
        return false;
    }

    return d_ptr->startCheck(filePath);
}

QString ComputeHash::getError()
{
    return d_ptr->m_errorStr;
}

void ComputeHash::setUserFactore(Factory *userFacrory)
{
    if(NULL == userFacrory)
        return;

    if(NULL == d_ptr->m_factory)
    {
        d_ptr->m_factory = userFacrory;
        return;
    }

    delete d_ptr->m_factory;
    d_ptr->m_factory = userFacrory;
    return;
}

void ComputeHash::onStopCompute()
{
    if(NULL == d_ptr->m_computeHash)
        return;
    //因停止检查，文件指纹效验失败
    d_ptr->m_errorStr = tr("Failed to check the file for fingerprint verification!");
    d_ptr->m_computeHash->stopCheck();
}

ComputeHashPrivate::ComputeHashPrivate(util::ComputeType type)
    :m_errorStr(QString()),
     m_isStart(false),
     m_conputeType(type),
     m_computeHash(NULL),
     m_factory(new Factory)
{

}

ComputeHashPrivate::~ComputeHashPrivate()
{
    if(NULL != m_computeHash)
        delete m_computeHash;
    delete m_factory;
}

bool ComputeHashPrivate::startCheck(QString filePath)
{
    m_computeHash = m_factory->createCompute(m_conputeType);
    if(NULL == m_computeHash)
    {
        //文件指纹模块初始化错误
        m_errorStr = QObject::tr("Check for module initialization errors!");
        return false;
    }


    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        m_errorStr = QObject::tr("File open errors!");
        return false;
    }

    QDataStream dataStream(&file);

    return true;
}
