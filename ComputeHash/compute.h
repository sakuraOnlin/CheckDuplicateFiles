#ifndef COMPUTE_H
#define COMPUTE_H

#include <QObject>
#include <QString>

class Compute
{
public:
    Compute();
    virtual ~Compute();

    virtual void update(QString &data ,QString oldComputeHash = QString()) =0;
    virtual QString getFinalResult() =0;
    virtual void stopCheck() =0;
    virtual void reset() =0;

};

#endif // COMPUTE_H
