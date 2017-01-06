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
    inline void stopAndClearThreadList();

    QString m_errorStr;
    bool m_isStart;
    util::ComputeType m_conputeType;
    QList<QThread*>m_readFileThreadList;
    QThread m_readFileThread;
    Factory *m_factory;
};

ComputeHash::ComputeHash(int type, QObject *parent)
    :QObject(parent)
{
    d_ptr = new ComputeHashPrivate((util::ComputeType)type);
    qRegisterMetaType<util::factoryCreateResult>("util::factoryCreateResult");
    qRegisterMetaType<util::computeResult>("util::computeResult");

}

ComputeHash::~ComputeHash()
{
    delete d_ptr;
}

bool ComputeHash::setCheckFilePath(QString filePath)
{
    QFileInfo fileInfo(filePath);
    if(filePath.isEmpty() && !fileInfo.isFile())
    {
        d_ptr->m_errorStr = tr("Please select a valid file for file fingerprinting! \n"
                               "Wrong address :") + filePath;
        return false;
    }

    QList<util::factoryCreateResult> computeList = d_ptr->m_factory->createCompute(d_ptr->m_conputeType);
    if(0 == computeList.length())
    {
        d_ptr->m_errorStr = QObject::tr("Check for module initialization errors!");
        return false;
    }

    for(int i = 0 ; i < computeList.length();i++)
    {
        QThread *thread = new QThread;
        util::factoryCreateResult factoryValue = computeList[i];
        ThreadReadFile *work = new ThreadReadFile(factoryValue , filePath);
        work->moveToThread( thread );
        connect( thread ,SIGNAL(finished()) ,work ,SLOT(deleteLater()) );
        connect( work ,SIGNAL(signalResultReady(util::computeResult)) ,this ,
                          SIGNAL(signalFinalResult(util::computeResult)) );
        connect(this ,SIGNAL(signalStartCheck()) , work ,SLOT(doWork()) );
        thread->start();
        d_ptr->m_readFileThreadList.append(thread);
    }
    emit signalStartCheck();

    return true;
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
    d_ptr->stopAndClearThreadList();
    d_ptr->m_errorStr = tr("Failed to check the file for fingerprint verification!");
}

ComputeHashPrivate::ComputeHashPrivate(util::ComputeType type)
    :m_errorStr(QString()) ,
      m_isStart(false) ,
      m_conputeType(type) ,
      m_factory(new Factory)
{

}

ComputeHashPrivate::~ComputeHashPrivate()
{
    stopAndClearThreadList();
    delete m_factory;
}

void ComputeHashPrivate::stopAndClearThreadList()
{
    for(int i = 0 ; i < m_readFileThreadList.length() ; i = 0)
    {
        m_readFileThreadList[i]->quit();
        m_readFileThreadList[i]->wait(100);
        delete m_readFileThreadList[i];
        m_readFileThreadList.removeAt(i);
    }
}
