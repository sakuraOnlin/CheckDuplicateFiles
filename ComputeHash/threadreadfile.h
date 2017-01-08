#ifndef THREADREADFILE_H
#define THREADREADFILE_H

#include <QObject>
#include "util/util.h"

#ifdef _DEBUG
#include <QDebug>
#endif

class ThreadReadFile : public QObject
{
    Q_OBJECT
public:
    explicit ThreadReadFile(util::factoryCreateResult result ,QString filePath ,
                            QObject *parent = 0);

public slots:
    void doWork();

signals:
    void signalResultReady(util::computeResult result);

private:
    inline void emitResult(util::ResultMessageType resultType ,util::ComputeType computeType ,
                    QString filePath ,qint64 fileSize ,qint64 fileProgress ,
                    QString typeName =QString("NoType") ,QString result = QString());
    inline qint64 automaticDivision(qint64 fileSize);

private:
    util::factoryCreateResult m_result;
    QString m_filePath;

};

#endif // THREADREADFILE_H
