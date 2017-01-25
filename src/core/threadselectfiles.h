#ifndef THREADSELECTFILES_H
#define THREADSELECTFILES_H

#include <QObject>
#include <QThread>

class DoWork : public QObject
{
    Q_OBJECT

public:
    explicit DoWork(QObject *parent = 0);
    void setDirPath(QString dirPath);
    void setFilters(QStringList filters);
    void startSelectFiles();
    void stopSelectFiles();

signals:
    void signalFilePath(QString);

public slots:
    void onDoWork();

private:
    bool m_operatingStatus;
    QString m_dirPath;
    QStringList m_filters;
};

class ThreadSelectFiles : public QObject
{
    Q_OBJECT
public:
    explicit ThreadSelectFiles(QObject *parent = 0);
    ~ThreadSelectFiles();
    void setDirPath(QString dirPath);
    void setFilters(QStringList filters);

signals:
    void signalFilePath(QString);
    void signalStartSelectFiles();

public slots:
    void onStartSelectFiles();
    void onStopSelectFiles();

private:
    void init();

private:
    QThread *m_thread;
    DoWork *m_work;
    QString m_dirPath;
    QStringList m_filters;
};

#endif // THREADSELECTFILES_H
