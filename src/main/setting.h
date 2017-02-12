#ifndef SETTING_H
#define SETTING_H

#include <QDialog>
#include <QListWidget>
#include "core/widgetUtil.h"

namespace Ui {
class Setting;
}

class SettingPrivate;
class Setting : public QDialog
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = 0);
    ~Setting();

    void setCheckType(bool isRunning);
    void setInitData(QList<WidgetUtil::FiltersType> fileTilters,
                     int threadNum);

    int getThreadNum();
    QList<WidgetUtil::FiltersType> getFileFilters();

signals:
    void signalDataChange(bool);    //true : data change

private slots:
    void onPButOK();
    void onPButCancel();
    void onListWidgetClick(int row);
    void onHorSolidThreadNum(int index);
    void onAddFilters();
    void onDelFilters();

private:
    Ui::Setting *ui;

    SettingPrivate *d_ptr;
    Q_DECLARE_PRIVATE(Setting)
};

#endif // SETTING_H
