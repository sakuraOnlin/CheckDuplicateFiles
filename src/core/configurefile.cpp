#include "configurefile.h"

#include <QDebug>

ConfigureFile::ConfigureFile()
{
    m_setting = new QSettings("./setting.ini",
                              QSettings::IniFormat);
}

ConfigureFile::~ConfigureFile()
{
    delete  m_setting;
}

void ConfigureFile::setThreadNum(int num)
{
    m_setting->setValue("Thread/CurrentNum", QString::number(num));
}

int ConfigureFile::getThreadNum()
{
    int value = m_setting->value("Thread/CurrentNum").toInt();
    if(value <= 0 || value > 4)
        value = 1;
    return value;
}

void ConfigureFile::setFileFilters(QList<WidgetUtil::FiltersType> filtersList)
{
    QString writeStr;
    for(int i = 0 ; i < filtersList.length(); i++)
    {
        QString filter(filtersList.at(i).filtess);
        QString checked(filtersList.at(i).checked ? "1" : "0");
        writeStr += filter + "," + checked + "|";
    }
    m_setting->setValue("File/Filters", writeStr);
}

QList<WidgetUtil::FiltersType> ConfigureFile::getFileFilters()
{
    QString value(m_setting->value("File/Filters").toString());
    QStringList translationStr(value.split('|', QString::SkipEmptyParts));
    QList<WidgetUtil::FiltersType> filterList;
    foreach (QString value, translationStr)
    {
        WidgetUtil::FiltersType filterType;
        filterType.filtess = value.section(',',0,0);
        filterType.checked = (value.section(',',1).toInt() >= 1) ? true : false;
        filterList.append(filterType);
    }

    if(filterList.length() == 0)
    {
        WidgetUtil::FiltersType value;
        value.checked = true;
        value.filtess = "*.*";
        filterList.append(value);
    }

    return filterList;
}
