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
        CheckIng    ,
        CheckOver   ,
        CheckError  ,
        Warring
    };

    struct computeResult
    {
        ResultMessageType   resultMessageType;
        ComputeType         computeHashType;
        qint64              fileSize;
        qint64              computeProgress;
        QString             resultStr;
        QString             filePath;
        QString             checkTypeName;
    };

    struct factoryCreateResult
    {
        Compute             *creatorComputr;
        ComputeType         computeHashType;
        QString             creatorErrStr;
    };

    util();
};

#endif // UTIL_H
