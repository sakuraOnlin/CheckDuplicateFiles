#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QWidget>
#include "itemlistdelegate.h"

namespace Ui {
class ListWidget;
}

class ListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ListWidget(QWidget *parent = 0);
    ~ListWidget();

private:
    Ui::ListWidget *ui;
    ItemListDelegate m_dselegate;
};

#endif // LISTWIDGET_H
