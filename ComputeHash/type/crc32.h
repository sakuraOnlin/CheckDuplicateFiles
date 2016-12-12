#ifndef CRC32_H
#define CRC32_H

#include <cstdint>
#include <iostream>
#include <string>
#include <QString>
#include "compute.h"

class CRC32 : public Compute
{
public:
    explicit CRC32();
    void update(QString &s);
    QString getFinalResult();
    void onStop();

private:
    void reset();

private:
    bool m_isStart;
    uint32_t state;

};

#endif // CRC32_H
