#ifndef BACKSTAGE_H
#define BACKSTAGE_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QListWidget>
#include <QListWidgetItem>
#include <QFile>
#include <QMutex>

#include "util/util.h"
#include "core/widgetUtil.h"
#include "core/threadselectfiles.h"
#include "CheckFile.h"
#include "core/computemodule.h"
#include "core/findrepeat.h"

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
    void onDelFile(QString filePath);
    void onStopCheckFile(QString filePath);
    void onFindAllRepeat();
    void onFindText(QString text);
    void onFindNextText();
    void onClearFindRepeat();
    void onClickItem(QListWidgetItem *item);

private slots:
    void onListWidgetAddItem(QStringList filePathList);
    void onItemSetData(util::ComputeResult result);
    void onItemComputeErr(QString filePath, QString errStr);
    void onItemCalculationComplete(QString filePath);
    void onTimeSengProgress();

private:
    void init();

private:
    QFile m_removeFile;
    QString m_dirPath;
    QStringList m_fileFilters;
    QSize m_iconSize;
    bool m_operatingStatus;
    ThreadSelectFiles m_selectFiles;
    ComputeModule m_computeModule;
    FindRepeat m_findRepeat;
    QTimer m_time;
    QListWidget *m_listWidget;
    QStringList *m_filePathList;
    QListWidgetItem *m_selectItem;
    QHash<QString, QListWidgetItem*> *m_fileItemHash;

};

class Backstage : public QObject
{
    Q_OBJECT
public:
    static Backstage &getInstance();
    BackstageWork *getBackstagWork() const;

private:
    explicit Backstage(QObject *parent = 0);
    ~Backstage();

    static QScopedPointer<Backstage> instance;
    static QMutex mutex;
    friend class QScopedPointer<Backstage>;
    friend struct QScopedPointerDeleter<Backstage>;

    QThread m_thread;
    BackstageWork *m_backstageWork;

};

#endif // BACKSTAGE_H
