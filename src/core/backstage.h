#ifndef BACKSTAGE_H
#define BACKSTAGE_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QListWidget>
#include <QListWidgetItem>

#include "util/util.h"
#include "core/widgetUtil.h"
#include "core/threadselectfiles.h"
#include "CheckFile.h"
#include "core/computemodule.h"

class BackstageWork : public QObject
{
     Q_OBJECT

public:
    explicit BackstageWork(QObject *parnet = 0);
    ~BackstageWork();

    void setListWidget(QListWidget *listWidget);
    void setDirPath(QString dirPath);
    void setFileFilters(QStringList fileFIlters);
    void setFilePath(QStringList *filePathList);
    void setFileItem(QHash<QString, QListWidgetItem*> *fileItemHash, QSize &iconSize);
    bool getOperatingStatus();
    bool setCheckThreadNum(int num);
    int getCheckThreadNum();

signals:
    void signalFileStatistics(WidgetUtil::Progress progress);

public slots:
    void onStart(int checkType);
    void onStop();
    void onStopCheckFile(QString filePath);

private slots:
    void onListWidgetAddItem(QStringList filePathList);
    void onItemSetData(util::ComputeResult result);
    void onItemComputeErr(QString filePath, QString errStr);
    void onItemCalculationComplete(QString filePath);
    void onTimeSengProgress();

private:
    void init();

private:
    QListWidget *m_listWidget;
    QString m_dirPath;
    QStringList m_fileFilters;
    QStringList *m_filePathList;
    QHash<QString, QListWidgetItem*> *m_fileItemHash;
    QSize m_iconSize;
    bool m_operatingStatus;
    ThreadSelectFiles m_selectFiles;
    ComputeModule m_computeModule;
    QTimer m_time;

};

class Backstage : public QObject
{
    Q_OBJECT
public:
    explicit Backstage(QObject *parent = 0);
    ~Backstage();
    BackstageWork *getBackstagwWork();

private:
    QThread m_thread;
    BackstageWork *m_backstageWork;

};

#endif // BACKSTAGE_H
