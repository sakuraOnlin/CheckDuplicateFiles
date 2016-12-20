#ifndef COMPUTE_H
#define COMPUTE_H

#include <QObject>
#include <QString>

class Compute
{
public:
    Compute();
    virtual ~Compute();

    virtual void update(QString &s ,QString oldComputeStr) =0;
    virtual QString getFinalResult() =0;
    virtual void stopCheck() =0;

};

#endif // COMPUTE_H
