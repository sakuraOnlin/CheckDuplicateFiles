#ifndef BACKSTAGE_H
#define BACKSTAGE_H

#include <QObject>
#include <QThread>
#include <QListWidget>
#include <QListWidgetItem>

#include "util/util.h"
#include "core/widgetUtil.h"

class BackstageWork : public QObject
{
     Q_OBJECT
public:
    explicit BackstageWork(QObject *parnet = 0);

    void setListWidget(QListWidget *listWidget);
    void setFilePath(QStringList &filePathList);
    void setFileItem(QHash<QString, QListWidgetItem*> &fileItemHash,
                     QSize &iconSize);

public slots:
    void doListWidgetAddItem(QString filePath);

private:
    QListWidget *m_listWidget;
    QStringList m_filePathList;
    QHash<QString, QListWidgetItem*> m_fileItemHash;
    QSize m_iconSize;

};

class Backstage : public QObject
{
    Q_OBJECT
public:
    explicit Backstage(QObject *parent = 0);
    ~Backstage();
    BackstageWork *getBackstagwWork();

signals:

public slots:

private:
    QThread m_thread;
    BackstageWork *m_backstageWork;

};

#endif // BACKSTAGE_H
