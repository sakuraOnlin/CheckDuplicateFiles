#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "core/widgetUtil.h"

namespace Ui {
class MainWindow;
class MainWindowPrivate;
}

class MainWindowPrivate;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void signalStartCheck();
    void signalStopCheck();
    void signalStopSelectFiles();

private slots:
    void onSelectDirPath();
    void onStartCheck();
    void onExit();
    void onDelFile();
    void onDelAllDupFiles();
    void onSetting();
    void onHelp();
    void onAbout();
    void onCheckBox(int state);
    void onFileStatistics(WidgetUtil::Progress progress);
    void onSettingDataChange(bool click);
    void onClearLineEdit();
    void onFindAllRepeat();
    void onFindText();
    void onFindNestText();
    void onClearFindRepeat();
    void onExportResult();

private:
    Ui::MainWindow *ui;

    MainWindowPrivate *d_ptr;
    Q_DECLARE_PRIVATE(MainWindow)
};

#endif // MAINWINDOW_H
