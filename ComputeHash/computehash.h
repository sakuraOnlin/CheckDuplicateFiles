#ifndef COMPUTEHASH_H
#define COMPUTEHASH_H

#include <QObject>
#include "computehash_global.h"
#include "util/util.h"

class ComputeHashPrivate;
class COMPUTEHASHSHARED_EXPORT ComputeHash : public QObject
{
    Q_OBJECT

public:
    ComputeHash(QObject *parent = 0 , util::ComputeType type = util::MD5);
    ~ComputeHash();

    bool setComputeHsahFile(QString filePath);
    QString getError();

signals:
    void signalFinalResult(QString result);
    void signalProgress(ulong step ,ulong max);

public slots:
    void onStopCompute();

private:
    ComputeHashPrivate *d_ptr;
    Q_DECLARE_PRIVATE(ComputeHash)
};

#endif // COMPUTEHASH_H
