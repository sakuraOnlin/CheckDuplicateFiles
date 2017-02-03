#ifndef COMPUTEMODULE_H
#define COMPUTEMODULE_H

#include <QObject>
#include <QThread>
#include <QHash>
#include "util/util.h"

class ComputeHash;

class ComputeWork : public QObject
{
    Q_OBJECT

public:
    explicit ComputeWork(QObject *parent = 0);
    ~ComputeWork();
    void setFilePathList(QStringList *filePathList);
    int getComputeProgress();

public slots:
    void onWork();

private:
    void init();

public:
    int m_computeIndex;
    int m_computeThreadMaxNum;
    int m_threadRunCount;
    bool m_operatingStatus;
    QList<ComputeHash*> m_computeRunList;
    QList<ComputeHash*> m_computeRestList;
    QHash<ComputeHash*, QString> m_computeHash;

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
    void onStart();
    void onStop();
    void onStopCheckFile(QString filePath);

private slots:
    void onHandleErrStr(QString err);
    void onHandleCalculationComplete();

private:
    void init();

private:
    QThread *m_thread;
    ComputeWork *m_computeWork;


};

#endif // COMPUTEMODULE_H
