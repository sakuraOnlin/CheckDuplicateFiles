#ifndef CONFIGUREFILE_H
#define CONFIGUREFILE_H

#include <QSettings>
#include <QVariant>
#include "core/widgetUtil.h"

class ConfigureFile
{
public:
    ConfigureFile();
    ~ConfigureFile();
    void setThreadNum(int num);
    int getThreadNum();

    void setFileFilters(QList<WidgetUtil::FiltersType> filtersList);
    QList<WidgetUtil::FiltersType> getFileFilters();

private:
    QSettings *m_setting;
};

#endif // CONFIGUREFILE_H
