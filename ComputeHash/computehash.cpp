#include <QObject>
#include <QList>
#include <QFileInfo>
#include <QThread>

#include "compute.h"
#include "computehash.h"
#include "threadreadfile.h"
#include "util/computefactory.h"

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
    QList<util::factoryCreateResult> m_computeList;
    Factory *m_factory;
    QThread m_readFileThread;
};

ComputeHash::ComputeHash(util::ComputeType type, QObject *parent)
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
    //TODO: 此处需要停止线程
    //因停止检查，文件指纹效验失败
    d_ptr->m_errorStr = tr("Failed to check the file for fingerprint verification!");
}

ComputeHashPrivate::ComputeHashPrivate(util::ComputeType type)
    :m_errorStr(QString()),
     m_isStart(false),
     m_conputeType(type),
     m_factory(new Factory)
{

}

ComputeHashPrivate::~ComputeHashPrivate()
{
    //TODO: 此处需要删除线程链表
    delete m_factory;
}

bool ComputeHashPrivate::startCheck(QString filePath)
{
    m_computeList = m_factory->createCompute(m_conputeType);
    if(0 == m_computeList.length())
    {
        //文件指纹模块初始化错误
        m_errorStr = QObject::tr("Check for module initialization errors!");
        return false;
    }

    //TODO: 此处需要开始线程化检查文件。线程里的检查模块来自 m_computeList　链表里的每个节点
//    QFile file(filePath);
//    if(!file.open(QIODevice::ReadOnly))
//    {
//        m_errorStr = QObject::tr("File open errors!");
//        return false;
//    }
//    QDataStream dataStream(&file);

    return true;
}
