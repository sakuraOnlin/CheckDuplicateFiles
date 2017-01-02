#ifndef COMPUTE_H
#define COMPUTE_H

#include <QObject>
#include <QString>
#include "util/util.h"

class Compute
{
public:
    Compute();
    virtual ~Compute();

    virtual void update(QString &data ,QString oldComputeHash = QString()) =0;
    virtual QString getFinalResult() =0;
    virtual void reset() =0;
    virtual util::ComputeType getType() =0;

};

#endif // COMPUTE_H
