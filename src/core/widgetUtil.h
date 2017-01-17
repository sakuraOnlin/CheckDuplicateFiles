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
        FilePath    ,
        FileSize    ,
        FileTime    ,
        FileIco     ,
        CheckResult   ,       //util::ComputeResult
        ItemSelect
    };
};

#endif // WIDGETUTIL_H
