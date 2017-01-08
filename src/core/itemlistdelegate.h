#ifndef ITEMLISTDELEGATE_H
#define ITEMLISTDELEGATE_H

#include <QItemDelegate>

class ItemListDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ItemListDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option, const QModelIndex &index);

private:

};

#endif // ITEMLISTDELEGATE_H
