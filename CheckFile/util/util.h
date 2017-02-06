#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include "CheckFile_global.h"

#ifdef _DEBUG
#include <QtDebug>
#endif

class Compute;
class CHECKFILESHARED_EXPORT util
{
public:
    enum CheckType
    {
        NOTYPE      = 0,
        MD5         = 1,
        SHA1        = 2,
        CRC32       = 4
    };

    enum ResultMessageType
    {
        NoCheck    ,
        CheckIng   , 
        CheckOver  , 
        CheckError , 
        Warring
    };

    struct ComputeResult
    {
        ResultMessageType   resultMessageType;
        CheckType           checkHashType;
        qint64              fileSize;
        qint64              computeProgress;
        QString             resultStr;
        QString             filePath;
        QString             checkTypeName;
    };

    struct factoryCreateResult
    {
        Compute             *creatorComputr;
        CheckType           checkHashType;
        QString             creatorErrStr;
    };

    util();
};

#endif // UTIL_H
