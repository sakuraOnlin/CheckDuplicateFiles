#include "listwidget.h"
#include "ui_listwidget.h"

ListWidget::ListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListWidget)
{
    ui->setupUi(this);
    ui->listWidget->setItemDelegate(&m_dselegate);
    ui->listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}

ListWidget::~ListWidget()
{
    delete ui;
}
