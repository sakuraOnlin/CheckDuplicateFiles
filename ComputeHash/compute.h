#ifndef COMPUTE_H
#define COMPUTE_H

#include <QObject>
#include <QString>

class Compute : public QObject
{
    Q_OBJECT

public:
    Compute(QObject *parent =0);
    virtual ~Compute();

    virtual void update(QString &s) =0;
    virtual QString getFinalResult() =0;

private:
    virtual void final() =0;

public slots:
    virtual void onStop() =0;

};

#endif // COMPUTE_H
