#ifndef COMPUTEHASH_H
#define COMPUTEHASH_H

#include <QObject>
#include "computehash_global.h"
#include "util/util.h"

class Factory;
class ComputeHashPrivate;
class COMPUTEHASHSHARED_EXPORT ComputeHash : public QObject
{
    Q_OBJECT

public:
    ComputeHash(int ComputeType = 1, QObject *parent = 0);
    ~ComputeHash();

    bool setFilePath(QString filePath);
    void setUserFactore(Factory *userFacrory);

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
    ComputeHashPrivate *d_ptr;
    Q_DECLARE_PRIVATE(ComputeHash)
};

#endif // COMPUTEHASH_H
