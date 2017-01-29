#include <QObject>
#include <QList>
#include <QFileInfo>

#include "compute.h"
#include "computehash.h"
#include "threadreadfile.h"
#include "util/computefactory.h"

class ComputeHashPrivate
{
    ComputeHash *q_ptr;
    Q_DECLARE_PUBLIC(ComputeHash)
public:
    ComputeHashPrivate(ComputeHash *parent,util::ComputeType type);
    ~ComputeHashPrivate();
    void init();
    bool setFilePath(QString filePath);
    inline bool checkDirValid(QString &filePath);
    void setUserFactore(Factory *userFacrory);
    void onStart();

    bool m_isStart;
    Factory *m_factory;
    QString m_filePath;
    util::ComputeType m_conputeType;
    ThreadControl m_threadControl;
    QList<util::factoryCreateResult> m_factoryList;
};

ComputeHashPrivate::ComputeHashPrivate(ComputeHash *parent, util::ComputeType type)
    :q_ptr(parent),
      m_isStart(false),
      m_factory(new Factory),
      m_filePath(QString()),
      m_conputeType(type)
{
    init();
}

ComputeHashPrivate::~ComputeHashPrivate()
{
    delete m_factory;
}

void ComputeHashPrivate::init()
{
    QObject::connect(&m_threadControl, SIGNAL(signalError(QString)),
                     q_ptr, SIGNAL(signalError(QString)));
    QObject::connect(&m_threadControl, SIGNAL(signalFinalResult(util::ComputeResult)),
                     q_ptr, SIGNAL(signalFinalResult(util::ComputeResult)));
    QObject::connect(&m_threadControl, SIGNAL(signalCalculationComplete()),
                     q_ptr, SIGNAL(signalCalculationComplete()));

}

bool ComputeHashPrivate::setFilePath(QString filePath)
{
    if(!checkDirValid(filePath))
        return false;

    m_filePath = filePath;
    return true;
}

bool ComputeHashPrivate::checkDirValid(QString &filePath)
{
    QFileInfo fileInfo(filePath);
    if(filePath.isEmpty() && !fileInfo.isFile())
        return false;
    return true;
}

void ComputeHashPrivate::setUserFactore(Factory *userFacrory)
{
    if(nullptr == userFacrory)
        return;

    if(nullptr == m_factory)
    {
        m_factory = userFacrory;
        return;
    }

    delete m_factory;
    m_factory = userFacrory;
    return;
}

void ComputeHashPrivate::onStart()
{
    QList<util::factoryCreateResult> factoryList = m_factory->createCompute(m_conputeType);
    m_threadControl.setFactorys(factoryList);
    m_threadControl.setDirPath(m_filePath);
    m_threadControl.start();
}

ComputeHash::ComputeHash(int type, QObject *parent)
    :QObject(parent)
{
    d_ptr = new ComputeHashPrivate(this, (util::ComputeType)type);
    qRegisterMetaType<util::factoryCreateResult>("util::factoryCreateResult");
    qRegisterMetaType<util::ComputeResult>("util::ComputeResult");
}

ComputeHash::~ComputeHash()
{
    delete d_ptr;
}

bool ComputeHash::setFilePath(QString filePath)
{
    return d_ptr->setFilePath(filePath);
}

void ComputeHash::setUserFactore(Factory *userFacrory)
{
    d_ptr->setUserFactore(userFacrory);
}

bool ComputeHash::getOperatingStatus()
{
    return d_ptr->m_threadControl.getOperatingStatus();
}

void ComputeHash::onStart()
{
    d_ptr->onStart();
}

void ComputeHash::onStop()
{
    d_ptr->m_threadControl.stop();
    emit signalError(tr("Failed to check the file for fingerprint verification!"));
}

void ComputeHash::onRestore()
{
    d_ptr->m_threadControl.restore();
}
