#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void onHelp();
    void onAbout();

private:
    Ui::MainWindow *ui;

    MainWindowPrivate *d_ptr;
    Q_DECLARE_PRIVATE(MainWindow)
};

#endif // MAINWINDOW_H
