#include <QObject>
#include <QList>
#include <QFileInfo>

#include "compute.h"
#include "CheckFile.h"
#include "threadreadfile.h"
#include "util/computefactory.h"

class CheckFilePrivate
{
    CheckFile *q_ptr;
    Q_DECLARE_PUBLIC(CheckFile)
public:
    CheckFilePrivate(CheckFile *parent,util::ComputeType type);
    ~CheckFilePrivate();
    void init();
    bool setFilePath(QString filePath);
    inline bool checkDirValid(QString &filePath);
    void setUserFactore(Factory *userFacrory);
    void onStart();

    QString m_filePath;
    ThreadControl m_threadControl;
    QList<util::factoryCreateResult> m_factoryList;
    Factory *m_factory;
    util::ComputeType m_conputeType;
    bool m_isStart;
};

CheckFilePrivate::CheckFilePrivate(CheckFile *parent, util::ComputeType type)
    :q_ptr(parent),
      m_filePath(QString()),
      m_factory(new Factory),
      m_conputeType(type),
      m_isStart(false)
{
    init();
}

CheckFilePrivate::~CheckFilePrivate()
{
    delete m_factory;
}

void CheckFilePrivate::init()
{
    QObject::connect(&m_threadControl, SIGNAL(signalError(QString)),
                     q_ptr, SIGNAL(signalError(QString)));
    QObject::connect(&m_threadControl, SIGNAL(signalFinalResult(util::ComputeResult)),
                     q_ptr, SIGNAL(signalFinalResult(util::ComputeResult)));
    QObject::connect(&m_threadControl, SIGNAL(signalCalculationComplete()),
                     q_ptr, SIGNAL(signalCalculationComplete()));

}

bool CheckFilePrivate::setFilePath(QString filePath)
{
    if(!checkDirValid(filePath))
        return false;

    m_filePath = filePath;
    return true;
}

bool CheckFilePrivate::checkDirValid(QString &filePath)
{
    QFileInfo fileInfo(filePath);
    if(filePath.isEmpty() && !fileInfo.isFile())
        return false;
    return true;
}

void CheckFilePrivate::setUserFactore(Factory *userFacrory)
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

void CheckFilePrivate::onStart()
{
    QList<util::factoryCreateResult> factoryList = m_factory->createCompute(m_conputeType);
    m_threadControl.setFactorys(factoryList);
    m_threadControl.setDirPath(m_filePath);
    m_threadControl.start();
}

CheckFile::CheckFile(int type, QObject *parent)
    :QObject(parent)
{
    d_ptr = new CheckFilePrivate(this, (util::ComputeType)type);
    qRegisterMetaType<util::factoryCreateResult>("util::factoryCreateResult");
    qRegisterMetaType<util::ComputeResult>("util::ComputeResult");
}

CheckFile::~CheckFile()
{
    delete d_ptr;
}

bool CheckFile::setFilePath(QString filePath)
{
    return d_ptr->setFilePath(filePath);
}

void CheckFile::setUserFactore(Factory *userFacrory)
{
    d_ptr->setUserFactore(userFacrory);
}

bool CheckFile::getOperatingStatus()
{
    return d_ptr->m_threadControl.getOperatingStatus();
}

void CheckFile::onStart()
{
    d_ptr->onStart();
}

void CheckFile::onStop()
{
    d_ptr->m_threadControl.stop();
    emit signalError(tr("Failed to check the file for fingerprint verification!"));
}

void CheckFile::onRestore()
{
    d_ptr->m_threadControl.restore();
}
