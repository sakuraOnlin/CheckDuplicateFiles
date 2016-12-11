#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindowPrivate;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onSelectDirPath();
    void onStartCheck();
    void onExit();

private:
    void init();

private:
    Ui::MainWindow *ui;

    MainWindowPrivate *d_ptr;
    Q_DECLARE_PRIVATE(MainWindow)
};

#endif // MAINWINDOW_H
