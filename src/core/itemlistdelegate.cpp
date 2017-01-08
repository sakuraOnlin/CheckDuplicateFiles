#include "itemlistdelegate.h"

ItemListDelegate::ItemListDelegate(QObject *parent)
    :QItemDelegate(parent)
{

}

void ItemListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{

}

QSize ItemListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{

}

bool ItemListDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                   const QStyleOptionViewItem &option,
                                   const QModelIndex &index)
{

}
