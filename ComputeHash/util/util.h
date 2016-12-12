#ifndef UTIL_H
#define UTIL_H

#include "computehash_global.h"

class COMPUTEHASHSHARED_EXPORT util
{
public:
    enum ComputeType
    {
        MD5,
        SHA1,
        CRC32
    };
    util();
};

#endif // UTIL_H
