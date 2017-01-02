#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include "computehash_global.h"

class Compute;
class COMPUTEHASHSHARED_EXPORT util
{
public:
    enum ComputeType
    {
        NOTYPE      = 0,
        MD5         = 1,
        SHA1        = 2,
        CRC32       = 4
    };

    enum ResultMessageType
    {
        RESULT_OK           ,
        RESULT_WASRRING     ,
        RESULT_ERROR
    };

    struct result
    {
        ResultMessageType   resultMessageType;
        ComputeType         computeHashType;
        QString             resultStr;
        QString             filePath;
    }computeResult;

    struct factoryCreateResult
    {
        Compute             *creatorComputr;
        QString             creatorErrStr;
    };

    util();
};

#endif // UTIL_H
