#include <QFileInfo>
#include <QFile>
#include "computehash.h"
#include "util/computefactory.h"
#include "compute.h"

class ComputeHashPrivate
{
    ComputeHash *q_ptr;
    Q_DECLARE_PUBLIC(ComputeHash)
public:
    ComputeHashPrivate(util::ComputeType type)
        :m_errorStr(QString()),
         m_isStart(false),
         m_conputeType(type),
         m_computeHash(NULL)
    {

    }

    QString m_errorStr;
    bool m_isStart;
    util::ComputeType m_conputeType;
    Compute *m_computeHash;
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
    if(filePath.isEmpty())
    {
        //请选择有效的文件进行文件指纹识别！错误地址： + filePath
        d_ptr->m_errorStr = tr("Please select a valid file for file fingerprinting! \n"
                               "Wrong address :") + filePath;
        return false;
    }
    QFileInfo fileInfo(filePath);



    d_ptr->m_computeHash = Factory::createCompute(d_ptr->m_conputeType);
    if(NULL == d_ptr->m_computeHash)
    {
        //文件指纹模块初始化错误
        d_ptr->m_errorStr = tr("Check for module initialization errors!");
        return false;
    }

    return true;
}

QString ComputeHash::getError()
{
    return d_ptr->m_errorStr;
}

void ComputeHash::onStopCompute()
{
    if(NULL == d_ptr->m_computeHash)
        return;
    //因停止检查，文件指纹效验失败
    d_ptr->m_errorStr = tr("Failed to check the file for fingerprint verification!");
    d_ptr->m_computeHash->onStop();
}
