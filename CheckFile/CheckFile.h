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
    explicit CheckFile(int CheckType = 1, QObject *parent = 0);
    ~CheckFile();

    bool setFilePath(QString filePath);
    void setUserFactore(Factory *userFacrory);
    bool getOperatingStatus();

signals:
    void signalFinalResult(util::ComputeResult result);
    void signalError(QString errStr);
    void signalCalculationComplete();

public slots:
    void onStart();
    void onStop();
    void onRestore();

private slots:

private:
    CheckFilePrivate *d_ptr;
    Q_DECLARE_PRIVATE(CheckFile)
};

#endif // COMPUTEHASH_H
