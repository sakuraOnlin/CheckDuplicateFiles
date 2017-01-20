#ifndef BACKSTAGE_H
#define BACKSTAGE_H

#include <QObject>
#include <QThread>
#include <QListWidget>
#include <QListWidgetItem>

#include "util/util.h"
#include "core/widgetUtil.h"
#include "core/threadselectfiles.h"
#include "computehash.h"

class BackstageWork : public QObject
{
     Q_OBJECT
public:
    explicit BackstageWork(QObject *parnet = 0);
    ~BackstageWork();

    void setListWidget(QListWidget *listWidget);
    void setDirPath(QString dirPath);
    void setFileFilters(QStringList fileFIlters);
    void setFilePath(QStringList &filePathList);
    void setFileItem(QHash<QString, QListWidgetItem*> &fileItemHash, QSize &iconSize);
    void setComputeMaxThreadCount(int threadCount);
    bool getOperatingStatus();

signals:
    void signalComputeFile(QString filePath);
    void signalFileStatistics(WidgetUtil::Progress progress);
    void signalComputeResult(util::ComputeResult result);

public slots:
    void onStart();
    void onStop();

private slots:
    void onListWidgetAddItem(QString filePath);
    void onItemSetData(util::ComputeResult result);
    void onThreadEffectiveness();

private:
    void init();

private:
    QListWidget *m_listWidget;
    QString m_dirPath;
    QStringList m_fileFilters;
    QStringList m_filePathList;
    QHash<QString, QListWidgetItem*> m_fileItemHash;
    QSize m_iconSize;
    int m_computeThreadCount;
    int m_computeMaxThreadCount;
    int m_computeFileIndex;
    bool m_operatingStatus;
    ThreadSelectFiles m_selectFiles;

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
