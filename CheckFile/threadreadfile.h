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
    explicit ThreadReadFile(QObject *parent = 0);
    ~ThreadReadFile();
    void setCheckData(util::factoryCreateResult result);

public slots:
    void onDoWork (QString filePath = QString());
    void onRestore();

signals:
    void signalResultReady(util::ComputeResult result);
    void signalCalculationComplete();

private:
    inline void emitResult(util::ResultMessageType resultType, util::CheckType computeType,
                    QString filePath, qint64 fileSize, qint64 fileProgress, 
                    QString typeName =QString("NoType"), QString result = QString());
    inline qint64 automaticDivision(qint64 fileSize);

private:
    util::factoryCreateResult m_result;
    QString m_filePath;
    bool m_isWork;
    friend class ThreadControl;

};

class ThreadControl : public QObject
{
    Q_OBJECT

public:
    explicit ThreadControl(QObject *parent = 0);
    ~ThreadControl();
    void setDirPath(QString dirPath);
    void setFactorys(QList<util::factoryCreateResult> &list);
    bool getOperatingStatus();

    void start();
    void stop();
    void restore();

signals:
    void signalFinalResult(util::ComputeResult result);
    void signalError(QString errStr);
    void signalCalculationComplete();
    void signalStartCheck(QString dirPath);
    void signalRestore();
    void signalStop();

private slots:
    void onModuleCounter();

private:
    int m_moduleCounter;
    QString m_dirPath;
    QList<util::factoryCreateResult> m_listFactorys;
    QList<QPair<QThread *, ThreadReadFile *> > m_readFileThreadList;
    QList<QPair<QThread *, ThreadReadFile *> > m_threadList;
    bool m_operatingStatus;

};

#endif // THREADREADFILE_H
