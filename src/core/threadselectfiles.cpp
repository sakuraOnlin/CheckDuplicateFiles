#include "threadselectfiles.h"

#include <QDir>
#include <QDirIterator>

DoWork::DoWork(QString dirPath, QStringList filters, QObject *parent)
    : QObject(parent), 
      m_isStart(true), 
      m_dirPath(dirPath), 
      m_filters(filters)
{

}

void DoWork::doWork()
{
    //判断路径是否存在
    QDir dir(m_dirPath);
    if(!dir.exists())
    {
        return;
    }

    // m_filters 文件类型过滤器

    //定义迭代器并设置过滤器
    QDirIterator dir_iterator(m_dirPath, m_filters, QDir::Files | QDir::NoSymLinks,
        QDirIterator::Subdirectories);
    while(dir_iterator.hasNext() && m_isStart)
    {
        dir_iterator.next();
        QFileInfo file_info = dir_iterator.fileInfo();
        QString filePath = file_info.absoluteFilePath();
        emit signalFilePath(filePath);
    }
}

void DoWork::onStopSelectFiles()
{
    m_isStart = false;
}

ThreadSelectFiles::ThreadSelectFiles(QObject *parent)
    :QObject(parent), 
      m_thread(nullptr), 
      m_dirPath(QString("./")), 
      m_filters(QStringList("*.*"))
{

}

void ThreadSelectFiles::setDirPath(QString dirPath)
{
    m_dirPath = dirPath;
}

void ThreadSelectFiles::setFilters(QStringList filters)
{
    m_filters = filters;
}

void ThreadSelectFiles::onStartSelectFiles()
{
    if(nullptr != m_thread)
        onStopSelectFiles();

    m_thread = new QThread;
    DoWork *work = new DoWork(m_dirPath, m_filters);
    work->moveToThread(m_thread);
    connect(m_thread, SIGNAL(finished()), work, SLOT(deleteLater()) );
    connect(work, SIGNAL(signalFilePath(QString)), this,
            SIGNAL(signalFilePath(QString)));
    connect(this, SIGNAL(signalStartSelectFiles()), work, SLOT(doWork()) );
    connect(this, SIGNAL(signalStopSelectFiles()), work, 
            SLOT(onStopSelectFiles()) );
    m_thread->start();
    emit signalStartSelectFiles();

}

void ThreadSelectFiles::onStopSelectFiles()
{
    if(nullptr == m_thread)
        return;

    emit signalStopSelectFiles();
    m_thread->quit();
    m_thread->wait(100);
    delete m_thread;
    m_thread = nullptr;
}

