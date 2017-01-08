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

    bool setCheckFilePath(QString filePath);
    QString getError();

    void setUserFactore(Factory *userFacrory);

signals:
    void signalFinalResult(util::computeResult result);
    void signalStartCheck();

public slots:
    void onStopCompute();

private:
    ComputeHashPrivate *d_ptr;
    Q_DECLARE_PRIVATE(ComputeHash)
};

#endif // COMPUTEHASH_H
