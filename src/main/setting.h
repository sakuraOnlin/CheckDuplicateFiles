#ifndef SETTING_H
#define SETTING_H

#include <QDialog>
#include <QListWidget>

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
    void setCheckThreadNum(int *num);
    void setFileFilters(QStringList *fileFilters);

signals:
    void signalDataChange();

private slots:
    void onPButOK();
    void onPButCancel();
    void onListWidgetClick(int row);
    void onHorSolidThreadNum(int index);

private:
    Ui::Setting *ui;

    SettingPrivate *d_ptr;
    Q_DECLARE_PRIVATE(Setting)
};

#endif // SETTING_H
