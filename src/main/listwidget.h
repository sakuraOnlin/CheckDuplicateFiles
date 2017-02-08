#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QWidget>
#include "core/widgetUtil.h"

namespace Ui {
class ListWidget;
class ListWidgetPrivate;
}

class QListWidgetItem;
class ListWidgetPrivate;
class ListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ListWidget(QWidget *parent = 0);
    ~ListWidget();
    bool setFileFilters(QStringList filters);
    bool setDirPath(QString dirPath);
    bool operatingStatus();
    bool setCheckThreadNum(int num);
    int getCheckThreadNum();

signals:
    void signalFileTotal(int fileTotal);
    void signalFileStatistics(WidgetUtil::Progress progress);

public slots:
    void onStart(int checkType);    // it's util::CheckType
    void onStop();
    void onClickItem(QListWidgetItem *item);
    void onOpenFileDir();
    void onDelFile();

private slots:
    void onOpenFileDir(QString filePath);
    void onDelFile(QString filePath);

private:
    Ui::ListWidget *ui;

    ListWidgetPrivate *d_ptr;
    Q_DECLARE_PRIVATE(ListWidget)
};

#endif // LISTWIDGET_H
