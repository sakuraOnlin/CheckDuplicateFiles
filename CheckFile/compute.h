#ifndef COMPUTE_H
#define COMPUTE_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include "util/util.h"

class Compute : public QObject
{
    Q_OBJECT
public:
    explicit Compute(QObject *parnet =0);
    virtual ~Compute();

    virtual void update(QByteArray &data, QString oldComputeHash = QString()) =0;
    virtual QString getFinalResult() =0;
    virtual void reset() =0;
    virtual util::CheckType getType() =0;
    virtual QString getTypeName() =0;

};

#endif // COMPUTE_H
