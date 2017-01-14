#ifndef THREADREADFILE_H
#define THREADREADFILE_H

#include <QObject>
#include <QList>
#include "util/util.h"

#ifdef _DEBUG
#include <QDebug>
#endif

class ThreadReadFile : public QObject
{
    Q_OBJECT
public:
    explicit ThreadReadFile(util::factoryCreateResult result, QObject *parent = 0);
    ~ThreadReadFile();

public slots:
    void onDoWork (QString filePath = QString());
    void onStop();
    void onRestore();

signals:
    void signalResultReady(util::computeResult result);
    void signalCalculationComplete();

private:
    inline void emitResult(util::ResultMessageType resultType, util::ComputeType computeType, 
                    QString filePath, qint64 fileSize, qint64 fileProgress, 
                    QString typeName =QString("NoType"), QString result = QString());
    inline qint64 automaticDivision(qint64 fileSize);

private:
    util::factoryCreateResult m_result;
    QString m_filePath;
    bool m_isWork;

};

class ThreadControl : public QObject
{
    Q_OBJECT

public:
    explicit ThreadControl(QObject *parent = 0);
    ~ThreadControl();
    void setDirPath(QString dirPath);
    void setFactorys(QList<util::factoryCreateResult> &list);

    void start();
    void stop();
    void restore();

signals:
    void signalFinalResult(util::computeResult result);
    void signalError(QString errStr);
    void signalCalculationComplete();
    void signalStartCheck(QString dirPath);
    void signalRestore();
    void signalStop();

private:
    QString m_dirPath;
    QList<util::factoryCreateResult> m_listFactorys;
    QList<QThread*> m_readFileThreadList;

};

#endif // THREADREADFILE_H
