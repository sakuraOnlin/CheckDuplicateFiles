#ifndef COMPUTEHASH_H
#define COMPUTEHASH_H

#include <QObject>
#include "CheckFile_global.h"
#include "util/util.h"

class Factory;
class CheckFilePrivate;
class CHECKFILESHARED_EXPORT CheckFile : public QObject
{
    Q_OBJECT

public:
    explicit CheckFile(QObject *parent = 0);
    ~CheckFile();

    void setCheckType(int CheckType);
    bool setFilePath(QString filePath);
    void setUserFactore(Factory *userFacrory);
    bool getOperatingStatus();

signals:
    void signalFinalResult(util::ComputeResult result);
    void signalError(QString errStr);
    void signalCalculationComplete();

public slots:
    void onStart();
    void onStopCheck();
    void onRestore();

private:
    CheckFilePrivate *d_ptr;
    Q_DECLARE_PRIVATE(CheckFile)
};

#endif // COMPUTEHASH_H
