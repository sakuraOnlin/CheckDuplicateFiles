#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QWidget>
#include <QVariant>
#include <QListWidget>


namespace Ui {
class ListWidget;
class ListWidgetPrivate;
}

class ListWidgetPrivate;
class ListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ListWidget(QWidget *parent = 0);
    ~ListWidget();
    bool setDirPath(QString &dirPath);
    bool operatingStatus();

public slots:
    bool onStart();
    bool onStop();
    void onReceiveFilePath(QString filePath);
    void onUpdateItemData(int role, QString &filePath, QVariant data);
    void onDelFile();
    void onClickItem(QListWidgetItem *item);

private:
    Ui::ListWidget *ui;

    ListWidgetPrivate *d_ptr;
    Q_DECLARE_PRIVATE(ListWidget)
};

#endif // LISTWIDGET_H
