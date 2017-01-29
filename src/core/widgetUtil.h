#ifndef WIDGETUTIL_H
#define WIDGETUTIL_H

#include <QObject>

class WidgetUtil
{
public:
    explicit WidgetUtil();

    enum CheckType
    {
        NoCheck    ,
        CheckIng   ,
        CheckOver  ,
        CheckError
    };

    enum ItemDelegate
    {
        FileName = Qt::UserRole +100,
        FilePathRole    ,
        FileSizeRole    ,
        FileTimeRole    ,
        FileIcoRole     ,
        CheckResultRole ,       // util::ComputeResult is list
        CheckTypeRole   ,       // item compute type ,it's CheckType
        ItemSelectRole
    };

    struct Progress
    {
        int FileStatistics;
        int ComputeProgress;
    };
};

#endif // WIDGETUTIL_H
