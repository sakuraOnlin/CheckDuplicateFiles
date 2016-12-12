#include <QFileInfo>
#include <QFile>
#include "computehash.h"
#include "factory.h"
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

bool ComputeHash::setComputeHsahFile(QString filePath)
{
    if(filePath.isEmpty())
    {
        d_ptr->m_errorStr = tr("Please select a valid file for file fingerprinting");
        return false;
    }
    QFileInfo fileInfo(filePath);

    return true;
}

QString ComputeHash::getError()
{
    return d_ptr->m_errorStr;
}

void ComputeHash::onStopCompute()
{

}
