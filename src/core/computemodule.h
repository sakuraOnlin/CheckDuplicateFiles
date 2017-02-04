#ifndef COMPUTEMODULE_H
#define COMPUTEMODULE_H

#include <QObject>
#include <QThread>
#include <QHash>
#include "util/util.h"

class CheckFile;

class ComputeWork : public QObject
{
    Q_OBJECT

public:
    explicit ComputeWork(QObject *parent = 0);
    ~ComputeWork();
    void setFilePathList(QStringList *filePathList);
    int getComputeProgress();
    void createCheck(util::ComputeType checkType);

public slots:
    void onWork();

public:
    int m_computeIndex;
    int m_computeThreadMaxNum;
    int m_threadRunCount;
    bool m_operatingStatus;
    QList<QPair<CheckFile*, bool> > m_checkFilelist;
    QHash<CheckFile*, QString> m_computeHash;

private:
    QStringList *m_filePaths;

};

class ComputeModule : public QObject
{
    Q_OBJECT

public:
    explicit ComputeModule(QObject *parent = 0);
    ~ComputeModule();

    void setFilePathList(QStringList *filePathList);
    int getComputeProgress();

signals:
    void signalFinalResult(util::ComputeResult result);
    void signalError(QString filePath,QString errStr);
    void signalCalculationComplete(QString file);
    void signalStart();

public slots:
    void onStart(int checkType);
    void onStop();
    void onStopCheckFile(QString filePath);

private slots:
    void onHandleErrStr(QString err);
    void onHandleCalculationComplete();

private:
    void init();
    void connectWork();

private:
    QThread *m_thread;
    ComputeWork *m_computeWork;
};

#endif // COMPUTEMODULE_H
