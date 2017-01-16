#include <QApplication>
#include <QPainter>
#include <QPen>
#include <QFileIconProvider>
#include <QIcon>
#include <QDebug>

#include "itemlistdelegate.h"
#include "core/widgetUtil.h"
#include "util/util.h"


ItemListDelegate::ItemListDelegate(QObject *parent)
    :QStyledItemDelegate(parent),
      m_pixmapPoint(QPoint(5,5)),
      m_pixmapSize(QSize(48, 48))
{
    m_interval = 5;
    m_rectLabelWidth = 50;
    m_rectLabelHeight = 17;
    m_rectWidth = 400;
    m_rectHeight = 17;
    m_pButWidth =40;
    m_pButHeight = 28;
    m_labelHeightAndInterva = m_rectLabelHeight + m_interval;
    m_labelWidthAndInterva = m_rectLabelWidth + m_interval;
}

void ItemListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);
//    return;

    QRect optRect(option.rect);
    QRect defaultRect(optRect.x() + m_interval, optRect.y() + m_interval,
                      optRect.width() - 10, optRect.height() - 10);
//    paintFileIco(painter, option, index);

    int strRectWidth = defaultRect.width() - 48 - m_rectLabelWidth - m_pButWidth - (m_interval * 3);

    QRect fileIcon(defaultRect.x(), defaultRect.y() , 48,48);

    QRect pButOpenDirRect(optRect.width() - m_interval - m_pButWidth, m_interval,
                           m_pButWidth, m_pButHeight);

    QRect pButOpenDelFileRect(pButOpenDirRect.x(), pButOpenDirRect.y() +
                              m_pButHeight + m_interval, m_pButWidth, m_pButHeight);

    QRect fileNameLabel(fileIcon.x() + fileIcon.width() + m_interval,
                         fileIcon.y(), m_rectLabelWidth, m_rectLabelHeight);

    QRect fileNameRect(fileNameLabel.x() + m_rectLabelWidth + m_interval,
                        fileNameLabel.y(),strRectWidth,m_rectLabelHeight);

    QRect filePathLabel(fileNameLabel.x(), fileNameLabel.y() + m_labelHeightAndInterva,
                        m_rectLabelWidth, m_rectLabelHeight);

    QRect filePathRect(fileNameRect.x(), fileNameRect.y() + m_labelHeightAndInterva,
                       strRectWidth, m_rectLabelHeight);

    QRect fileSizeLabel(filePathLabel.x(), filePathLabel.y() + m_labelHeightAndInterva,
                        m_rectLabelWidth, m_rectLabelHeight);

    QRect fileSizeRect(fileSizeLabel.x() + m_labelWidthAndInterva,
                       fileSizeLabel.y(), m_rectLabelWidth, m_rectLabelHeight);

    QRect fileTimeLabel(fileSizeRect.x() + m_labelWidthAndInterva,
                        fileSizeRect.y(), m_rectLabelWidth, m_rectLabelHeight);

    QRect fileTimeRect(fileTimeLabel.x() + m_labelWidthAndInterva,
                       fileTimeLabel.y(), m_rectLabelWidth, m_rectLabelHeight);

    bool isSelected = index.data(WidgetUtil::ItemSelect).toBool();
    if(isSelected)
    {
        //计算进度条或是指纹结果的偏移量
    }
    //draw FileNameLabel
    painter->save();
    QString testStr(index.data(Qt::DisplayRole).toString());
    qApp->style()->drawItemText(painter,fileNameLabel,Qt::AlignLeft,
                                option.palette,true,"FileName:");
    //fileNameRect length >> fontMet...Length
    qApp->style()->drawItemText(painter,fileNameRect ,Qt::AlignLeft,option.palette
                                ,true,testStr + "cccuseesedfsfs.db");
    qApp->style()->drawItemText(painter,filePathLabel,Qt::AlignLeft,option.palette
                                ,true,"FilePath:");
    qApp->style()->drawItemText(painter,filePathRect ,Qt::AlignLeft,option.palette
                                ,true,"/lib/modules/4.4.0-57-generic/build/net/ipv4");
    qApp->style()->drawItemText(painter,fileSizeLabel,Qt::AlignLeft,option.palette
                                ,true,"FileSize:");
    qApp->style()->drawItemText(painter,fileSizeRect ,Qt::AlignLeft,option.palette
                                ,true,"10.8G");
    qApp->style()->drawItemText(painter,fileTimeLabel,Qt::AlignLeft,option.palette
                                ,true,"FileTime:");
    qApp->style()->drawItemText(painter,fileTimeRect ,Qt::AlignLeft,option.palette
                                ,true,"2016-12-12");
    painter->restore();

 }

QSize ItemListDelegate::sizeHint(const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option,index);
}

bool ItemListDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                   const QStyleOptionViewItem &option,
                                   const QModelIndex &index)
{
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

void ItemListDelegate::paintFileIco(QPainter *painter, const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
    //TODO:移动到外部来添加，内部直接绘制
    QFileIconProvider fileIco;
    QIcon ico(fileIco.icon(QFileInfo(index.data(WidgetUtil::FilePath).toString())));
    qApp->style()->drawItemPixmap(painter, QRect(m_pixmapPoint,m_pixmapSize),
                                  Qt::AlignCenter,ico.pixmap(32, 32));
}

void ItemListDelegate::paintText(QPainter *painter, QRectF &rect, QString &text) const
{

}


