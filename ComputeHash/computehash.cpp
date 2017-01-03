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
    QList<QThread*> m_readFileThreadList;
    Factory *m_factory;
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

bool ComputeHash::setCheckFilePath(QString filePath)
{
    QFileInfo fileInfo(filePath);
    if(filePath.isEmpty() && !fileInfo.isFile())
    {
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
    //因停止检查，文件指纹效验失败
    for(int i = 0 ; i < d_ptr->m_readFileThreadList.length() ;i++)
    {
        d_ptr->m_readFileThreadList[i]->quit();
        d_ptr->m_readFileThreadList[i]->wait(100);
        delete d_ptr->m_readFileThreadList[i];
    }

    d_ptr->m_readFileThreadList.clear();
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
    //TODO: 此处需要删除线程链表
    delete m_factory;
}

bool ComputeHashPrivate::startCheck(QString filePath)
{
    QList<util::factoryCreateResult> computeList = m_factory->createCompute(m_conputeType);
    if(0 == computeList.length())
    {
        m_errorStr = QObject::tr("Check for module initialization errors!");
        return false;
    }

    for(int i = 0 ; i < computeList.length();i++)
    {
        util::factoryCreateResult factoryValue = computeList[i];
        QThread *thread = new QThread;
        ThreadReadFile *work = new ThreadReadFile;
        work->moveToThread(thread);
        QObject::connect( thread ,SIGNAL(finished()) ,work ,SLOT(deleteLater()) );
        QObject::connect( work ,SIGNAL(resultReady(util::computeResult)) ,q_ptr ,
                          SIGNAL(signalFinalResult(util::computeResult)) );
        thread->start();
        work->doWork(factoryValue ,filePath);
        m_readFileThreadList.append(thread);
    }

    return true;
}
