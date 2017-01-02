#ifndef THREADREADFILE_H
#define THREADREADFILE_H

#include <QObject>

#ifdef _DEBUG
#include <QDebug>
#endif

class Compute;

class ThreadReadFile : public QObject
{
    Q_OBJECT
public:
    explicit ThreadReadFile(QObject *parent = 0);
    void doWork(Compute *compute ,QString filePath);

signals:
    void resultReady(const QString &result);

};

#endif // THREADREADFILE_H
