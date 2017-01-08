#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include "util/util.h"

class util
{
public:
    util();

    enum ItemDelegate
    {
        FileName = Qt::UserRole +100 ,
        FilePath    ,
        FileSize    ,
        FileTime    ,
        FIleIco     ,
        CheckType   ,
        CheckResult
    };
};

#endif // UTIL_H
