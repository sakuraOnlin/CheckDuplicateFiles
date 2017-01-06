#ifndef THREADSELECTFILES_H
#define THREADSELECTFILES_H

#include <QObject>
#include <QThread>

class DoWork : public QObject
{
    Q_OBJECT

public:
    explicit DoWork(QString dirPath ,QStringList filters,QObject *parent = 0);

signals:
    void signalFilePath(QString);

public slots:
    void doWork();
    void onStopSelectFiles();

private:
    bool m_isStart;
    QString m_dirPath;
    QStringList m_filters;
};

class ThreadSelectFiles : public QObject
{
    Q_OBJECT
public:
    explicit ThreadSelectFiles(QObject *parent = 0);
    void setDirPath(QString dirPath);
    void setFilters(QStringList filters);

signals:
    void signalFilePath(QString);
    void signalStartSelectFiles();
    void signalStopSelectFiles();

public slots:
    void onStartSelectFiles();
    void onStopSelectFiles();

private:
    QThread *m_thread;
    QString m_dirPath;
    QStringList m_filters;
};

#endif // THREADSELECTFILES_H
