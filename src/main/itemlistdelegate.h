#ifndef ITEMLISTDELEGATE_H
#define ITEMLISTDELEGATE_H

#include <QStyledItemDelegate>
#include <QPointF>
#include <QRectF>
#include <QPair>
#include "util/util.h"

class ItemListDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ItemListDelegate(QObject *parent = 0);
    ~ItemListDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:
    void signalDelFile(QString filePath);
    void signalOpenFileDir(QString filePath);

protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    inline QString calculateFileSize(qint64 fileSize)const;
    inline QString cutString(QString text, QRect rect) const;

private:
    QPoint m_pixmapPoint;
    QSize m_pixmapSize;
    QSize m_pButtSize;
    QSize m_labelSize;
    QSize m_labelIntervaSize;
    QFont m_font;
    QFontMetrics m_fontMetrics;
    QString m_fileNameLabelText;
    QString m_filePathLabelText;
    QString m_fileSizeLabelText;
    QString m_fileTimeLabelText;
    Qt::Alignment m_textAlignment;
    int m_interval;
    QPair<bool, bool> m_mouseType;  // mouseHour, mouse click
    QPoint m_mousePoint;

};

#endif // ITEMLISTDELEGATE_H
