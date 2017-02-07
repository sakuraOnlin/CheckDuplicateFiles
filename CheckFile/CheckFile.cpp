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
    CheckFilePrivate(CheckFile *parent);
    ~CheckFilePrivate();
    void init();
    void setCheckType(int CheckType);
    void createFactory();
    bool setFilePath(QString filePath);
    inline bool checkDirValid(QString &filePath);
    void setUserFactore(Factory *userFacrory);
    void onStart();

    QString m_filePath;
    ThreadControl m_threadControl;
    QList<util::factoryCreateResult> m_factoryList;
    Factory *m_factory;
    util::CheckType m_conputeType;
    bool m_isCreatrFactoryBool;
    bool m_isStart;
};

CheckFilePrivate::CheckFilePrivate(CheckFile *parent)
    :q_ptr(parent),
      m_filePath(QString()),
      m_factory(new Factory),
      m_isCreatrFactoryBool(false),
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

void CheckFilePrivate::setCheckType(int CheckType)
{
    m_conputeType = util::CheckType(CheckType);
}

void CheckFilePrivate::createFactory()
{
    m_factoryList = m_factory->createCompute(m_conputeType);
    m_threadControl.setFactorys(m_factoryList);
    m_isCreatrFactoryBool = true;
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
    m_threadControl.setDirPath(m_filePath);
    m_threadControl.start();
}

CheckFile::CheckFile(QObject *parent)
    :QObject(parent)
{
    d_ptr = new CheckFilePrivate(this);
    qRegisterMetaType<util::factoryCreateResult>("util::factoryCreateResult");
    qRegisterMetaType<util::ComputeResult>("util::ComputeResult");
}

CheckFile::~CheckFile()
{
    delete d_ptr;
}

void CheckFile::setCheckType(int CheckType)
{
    d_ptr->setCheckType(CheckType);
    d_ptr->createFactory();
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
    if(!d_ptr->m_isCreatrFactoryBool)
    {
        emit signalError(tr("The file verification type is not specified!"
                         "Please set the file verification type"));
        return;
    }
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
