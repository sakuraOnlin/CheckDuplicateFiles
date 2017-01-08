#ifndef COMPUTEDEMO_H
#define COMPUTEDEMO_H

#include <QFileDialog>
#include <QObject>
#include "computehash.h"

class ComputeDemo :public QObject
{
    Q_OBJECT

public:
    ComputeDemo(int ComputeType, QString filePath,  QObject *parent = 0);
    ~ComputeDemo();

public slots:
    void onGetResult(util::computeResult result);

private:
    void print(util::computeResult &result);

private:
    ComputeHash *m_compute;
};

#endif // COMPUTEDEMO_H
