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
    virtual ~CRC32();
    void update(QByteArray &data, QString oldComputeHash = QString());
    QString getFinalResult();
    void reset();
    util::CheckType getType();
    QString getTypeName();

private:
    void initCRC32Table();
    inline unsigned long reflect(unsigned long ref, char ch);

private:
    bool m_isStart;
    uint32_t state;
    unsigned long m_crc32Str;
    unsigned long crc32_table[256];

};

#endif // CRC32_H
