#ifndef FINDREPEAT_H
#define FINDREPEAT_H

#include <QObject>
#include <QThread>
#include <QMultiHash>
#include <QListWidget>

class FindRepeatWork : public QObject
{
    Q_OBJECT

public:
    explicit FindRepeatWork(QObject *parent = 0);
    ~FindRepeatWork();
    void onAddResultID(QListWidgetItem *item);
    void setListWidget(QListWidget *listWidget);
    void setFileItem(QHash<QString, QListWidgetItem*> *fileItemHash);

signals:
    void signalFindDone(int num);

public slots:
    void onFindAllRepeat();
    void onFindText(QString text);
    void onFindNextText();
    void onDelFile(QString filePath);
    void onRestoreBackground();
    void onClear();

public:
    QMultiHash<QString, QListWidgetItem*> m_fileIDHash;   // resule ID, FilePath
    QHash<QString, QListWidgetItem*> *m_fileItemHash;
    QList<QListWidgetItem*> m_repeatList;
    int m_findNextIndex;
    bool m_operatingStatus;
    QListWidget *m_listWidget;

private:
    inline void setItemBackGroundData(QListWidgetItem *item, bool setBackground = false);
};

class FindRepeat : public QObject
{
    Q_OBJECT
public:
    explicit FindRepeat(QObject *parent = 0);
    ~FindRepeat();
    void setListWidget(QListWidget *widget);
    void onAddResultID(QListWidgetItem *item);
    void setFileItem(QHash<QString, QListWidgetItem*> *fileItemHash);

signals:
    void signalFindDone(int num);

    void signalFindAllRepeat();
    void signalFindText(QString);
    void signalFindNextText();

public slots:
    void onStart();
    void onStop();
    void onFindAllRepeat();
    void onFindText(QString text);
    void onFindNextText();
    void onRestoreBackground();
    void onClear();

private:
    QThread m_thread;
    FindRepeatWork *m_findRepeatWork;
};

#endif // FINDREPEAT_H
