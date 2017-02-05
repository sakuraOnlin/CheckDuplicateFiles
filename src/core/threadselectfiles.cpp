#include "threadselectfiles.h"

#include <QDir>
#include <QDirIterator>

#ifdef _DEBUG
#include <QDebug>
#endif

DoWork::DoWork(QObject *parent)
    : QObject(parent), 
      m_operatingStatus(true),
      m_dirPath(QString()),
      m_filters(QString())
{

}

void DoWork::setDirPath(QString dirPath)
{
    m_dirPath = dirPath;
}

void DoWork::setFilters(QStringList filters)
{
    m_filters = filters;
}

void DoWork::onDoWork()
{
    QDir dir(m_dirPath);
    if(!dir.exists())
    {
        return;
    }

    QDirIterator dir_iterator(m_dirPath, m_filters, QDir::Files | QDir::NoSymLinks,
        QDirIterator::Subdirectories);
    QStringList fileList;
    int msleepCount = 0;
    while(m_operatingStatus && dir_iterator.hasNext())
    {
        dir_iterator.next();
        QFileInfo file_info(dir_iterator.fileInfo());
        QThread::msleep(10);
        fileList.append(file_info.absoluteFilePath());
        msleepCount += 5;
        if(msleepCount >= 600)
        {
            QThread::msleep(20);
            emit signalFilePathList(fileList);
            msleepCount = 0;
            fileList.clear();
        }
    }
    if(fileList.length() >=0)
        emit signalFilePathList(fileList);
}

void DoWork::startSelectFiles()
{
    m_operatingStatus = true;
}

void DoWork::stopSelectFiles()
{
    m_operatingStatus = false;
}

ThreadSelectFiles::ThreadSelectFiles(QObject *parent)
    :QObject(parent), 
      m_thread(nullptr), 
      m_dirPath(QString("./")), 
      m_filters(QStringList("*.*"))
{
    init();
}

ThreadSelectFiles::~ThreadSelectFiles()
{
    m_work->stopSelectFiles();
    m_thread->quit();
    m_thread->wait(100);
    delete m_thread;
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

    m_work->stopSelectFiles();
    m_work->startSelectFiles();
    m_work->setDirPath(m_dirPath);
    m_work->setFilters(m_filters);
    emit signalStartSelectFiles();
}

void ThreadSelectFiles::onStopSelectFiles()
{
    m_work->stopSelectFiles();
}

void ThreadSelectFiles::init()
{
    m_thread = new QThread;
    m_work = new DoWork;
    m_work->moveToThread(m_thread);
    connect(m_thread, SIGNAL(finished()), m_work, SLOT(deleteLater()) );
    connect(m_work, SIGNAL(signalFilePathList(QStringList)), this,
            SIGNAL(signalFilePathList(QStringList)));
    connect(this, SIGNAL(signalStartSelectFiles()), m_work, SLOT(onDoWork()) );
    m_thread->start();
}

