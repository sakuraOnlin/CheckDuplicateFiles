#ifndef ITEMLISTDELEGATE_H
#define ITEMLISTDELEGATE_H

#include <QStyledItemDelegate>
#include <QPointF>
#include <QRectF>
#include "util/util.h"

class ItemListDelegate : public QStyledItemDelegate
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
    inline void paintFileIco(QPainter *painter, const QStyleOptionViewItem &option,
                      const QModelIndex &index ) const;
    inline void paintText(QPainter *painter, QRectF &rect, QString &text) const;

private:
    QPoint m_pixmapPoint;
    QSize m_pixmapSize;
    int m_pixmapWidth;
    int m_pixmapHeight;
    int m_pButWidth;
    int m_pButHeight;
    int m_interval;
    int m_rectLabelWidth;
    int m_rectLabelHeight;
    int m_rectWidth;
    int m_rectHeight;
    int m_labelHeightAndInterva;
    int m_labelWidthAndInterva;

};

#endif // ITEMLISTDELEGATE_H
