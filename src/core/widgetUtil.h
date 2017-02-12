#ifndef WIDGETUTIL_H
#define WIDGETUTIL_H

#include <QObject>

class WidgetUtil
{
public:
    explicit WidgetUtil();

    enum ItemDelegate
    {
        FileName = Qt::UserRole +100,
        FilePathRole    ,
        FileSizeRole    ,
        FileTimeRole    ,
        FileIcoRole     ,
        CheckResultRole ,       // util::ComputeResult is list
        CheckTypeRole   ,       // item compute type
        ItemSelectRole
    };

    typedef struct Progress
    {
        int FileStatistics;
        int ComputeProgress;
    }Progress;

    typedef struct FiltersType
    {
        bool checked;
        QString filtess;
    }FiltersType;
};

#endif // WIDGETUTIL_H
