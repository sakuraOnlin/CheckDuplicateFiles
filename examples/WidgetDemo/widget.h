#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "computehash.h"
#include "util/util.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

signals:
    void signalStart();

private slots:
    void onSelectDir();
    void onStart();
    void onCalculationComplete();
    void onFinalResult(util::ComputeResult result);

private:
    void init();

private:
    Ui::Widget *ui;
    ComputeHash *m_compute;
    QString m_filePath;

};

#endif // WIDGET_H
