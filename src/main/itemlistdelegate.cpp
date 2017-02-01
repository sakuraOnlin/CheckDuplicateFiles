#include <QApplication>
#include <QPainter>
#include <QPen>
#include <QFileIconProvider>
#include <QIcon>
#include <QPair>
#include <QDebug>
#include <QProgressBar>

#include "itemlistdelegate.h"
#include "core/widgetUtil.h"
#include "util/util.h"

Q_DECLARE_METATYPE(util::ComputeResult)
Q_DECLARE_METATYPE(util::ResultMessageType)

ItemListDelegate::ItemListDelegate(QObject *parent)
    :QStyledItemDelegate(parent),
      m_pixmapPoint(QPoint(5,5)),
      m_pixmapSize(QSize(48, 48))
{
    m_interval = 5;
    m_rectLabelWidth = 50;
    m_rectLabelHeight = 17;
    m_pButWidth = 80;
    m_pButHeight = 22;
    m_labelHeightAndInterva = m_rectLabelHeight + m_interval;
    m_labelWidthAndInterva = m_rectLabelWidth + m_interval;
}

ItemListDelegate::~ItemListDelegate()
{
}

void ItemListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    //Data
    QFont font(qApp->font());
    QFontMetrics fontMetrics(font);
    QString fileName(index.data(WidgetUtil::FileName).toString());
    QString filePath(index.data(WidgetUtil::FilePathRole).toString());
    QString fileSize(index.data(WidgetUtil::FileSizeRole).toString());
    QString fileTime(index.data(WidgetUtil::FileTimeRole).toString());
    QPixmap fileIco(index.data(WidgetUtil::FileIcoRole).value<QPixmap>());
    bool isSelected = index.data(WidgetUtil::ItemSelectRole).toBool();

    QList<util::ComputeResult> resultList = index.data(
                WidgetUtil::CheckResultRole).value<QList<util::ComputeResult> >();

    QRect optRect(option.rect);
    optRect.setWidth(optRect.width() -1);
    QRect fileIcoRect(optRect.x() + m_interval, optRect.y() + m_interval,32 ,32);
    int labelDatumPositioningX = optRect.x() + 32 + m_interval *2 ;
    int labelDatumPositioningY = optRect.y() + m_interval;
    int puButDatumPositioningX = optRect.width() - m_interval - m_pButWidth;
    int contentLengthX = optRect.width() - fileIcoRect.width()
            - m_labelWidthAndInterva - m_pButWidth - m_interval * 4;

    QRect pButOpenDirRect(puButDatumPositioningX, labelDatumPositioningY,
                          m_pButWidth, m_pButHeight);
    QRect pButDelFileRect(puButDatumPositioningX, labelDatumPositioningY +
                              m_pButHeight + m_interval,
                              m_pButWidth, m_pButHeight);
    QRect fileNameLabelRect(labelDatumPositioningX, labelDatumPositioningY,
                        m_rectLabelWidth, m_rectLabelHeight);
    QRect fileNameRect(labelDatumPositioningX + m_labelWidthAndInterva,
                   fileNameLabelRect.y(), contentLengthX, m_rectLabelHeight);
    QRect filePathLabelRect(labelDatumPositioningX, labelDatumPositioningY +
                        m_labelHeightAndInterva * 1,
                        m_rectLabelWidth, m_rectLabelHeight);
    QRect filePathRect(labelDatumPositioningX + m_labelWidthAndInterva,
                       filePathLabelRect.y(), contentLengthX, m_rectLabelHeight);
    QRect fileSizeLabelRect(labelDatumPositioningX, labelDatumPositioningY +
                        m_labelHeightAndInterva * 2,
                        m_rectLabelWidth, m_rectLabelHeight);
    QRect fileSizeRect(labelDatumPositioningX + m_labelWidthAndInterva,
                       fileSizeLabelRect.y(), m_rectLabelWidth, m_rectLabelHeight);
    QRect fileTimeLabelRect(fileSizeRect.x() + m_labelWidthAndInterva, fileSizeRect.y(),
                            m_rectLabelWidth, m_rectLabelHeight);
    QRect fileTimeRect(fileTimeLabelRect.x() + m_labelWidthAndInterva, fileSizeRect.y(),
                       fontMetrics.size(Qt::TextSingleLine,fileTime).width(),
                       m_rectLabelHeight);

    QList<QPair<QRect, QRect> > resultRectList;
    if(isSelected)
    {
        /*
         * Line 0 is the starting point.
         * All file fingerprint results are displayed starting at the third line.
        */
        int contentStartingPoint = 3;
        for(int i = 0 ; i < resultList.length(); i++)
        {
            QRect resultNameRect(labelDatumPositioningX, labelDatumPositioningY +
                                 m_labelHeightAndInterva * contentStartingPoint,
                                 m_rectLabelWidth, m_rectLabelHeight);

            QRect resultRect(labelDatumPositioningX + m_labelWidthAndInterva,
                             resultNameRect.y(), contentLengthX, m_rectLabelHeight);
            resultRectList.append(qMakePair(resultNameRect,resultRect));
            contentStartingPoint++;
        }
    }

    QStyleOptionButton pButtOpenFile;
    pButtOpenFile.rect = pButOpenDirRect;
    pButtOpenFile.text = tr("Open File Dir");
    pButtOpenFile.state |= QStyle::State_Enabled;

    QStyleOptionButton pButtDelFile;
    pButtDelFile.rect = pButDelFileRect;
    pButtDelFile.text = tr("Del File");
    pButtDelFile.state |= QStyle::State_Enabled;

    painter->save();
    //TODO: 按钮显示状态的处理，包括按下和悬浮状态的处理，需要一个bool值来表示
    qApp->style()->drawControl(QStyle::CE_PushButton, &pButtOpenFile, painter);
    qApp->style()->drawControl(QStyle::CE_PushButton, &pButtDelFile, painter);
    qApp->style()->drawItemPixmap(painter, fileIcoRect,
                                Qt::AlignLeft | Qt::AlignTop, fileIco);
    qApp->style()->drawItemText(painter, fileNameLabelRect,Qt::AlignLeft,
                                option.palette, true, "FileName:");
    qApp->style()->drawItemText(painter, fileNameRect ,Qt::AlignLeft,
                                option.palette, true, fileName);
    qApp->style()->drawItemText(painter, filePathLabelRect,Qt::AlignLeft,
                                option.palette, true, "FilePath:");
    qApp->style()->drawItemText(painter, filePathRect ,Qt::AlignLeft,
                                option.palette, true, filePath);
    qApp->style()->drawItemText(painter, fileSizeLabelRect,Qt::AlignLeft,
                                option.palette, true, "FileSize:");
    qApp->style()->drawItemText(painter, fileSizeRect ,Qt::AlignLeft,
                                option.palette, true, fileSize);
    qApp->style()->drawItemText(painter, fileTimeLabelRect,Qt::AlignLeft,
                                option.palette, true, "FileTime:");
    qApp->style()->drawItemText(painter, fileTimeRect ,Qt::AlignLeft,
                                option.palette, true, fileTime);

    if(isSelected)
    {
        //TODO: Item在各种状态下的数据显示，包括未计算时的数据显示
        //计算进度条或是指纹结果的偏移量
        for(int i = 0 ; i < resultRectList.length(); i++ )
        {
            util::ComputeResult result = resultList[i];
            util::ResultMessageType messageType = index.
                    data(WidgetUtil::CheckTypeRole).value<util::ResultMessageType>();
            qApp->style()->drawItemText(painter, resultRectList[i].first ,Qt::AlignLeft,
                                        option.palette, true,
                                        result.checkTypeName);
            switch (messageType) {
            case util::NoCheck:
            {
                // 未开始
                qApp->style()->drawItemText(painter, resultRectList[i].second ,Qt::AlignLeft,
                                            option.palette, true, tr("Not yet started"));
                break;
            }
            case util::CheckIng:
            {
                // 设置进度条的风格
                QStyleOptionProgressBar progressBarOption;
                progressBarOption.initFrom(option.widget);
                progressBarOption.rect = resultRectList[i].second;
                progressBarOption.minimum = 0;
                progressBarOption.maximum = result.fileSize;
                progressBarOption.textAlignment = Qt::AlignCenter;
                progressBarOption.progress = result.computeProgress;
                progressBarOption.text = QString("%1 / %2")
                       .arg(QString::number(result.computeProgress))
                       .arg(QString::number(result.fileSize));
                progressBarOption.textVisible = true;
                QProgressBar progressBar;
                QApplication::style()->drawControl(QStyle::CE_ProgressBar,
                                    &progressBarOption, painter, &progressBar);
                break;
            }
            case util::CheckOver:
            {
                qApp->style()->drawItemText(painter, resultRectList[i].second ,Qt::AlignLeft,
                                            option.palette, true, result.resultStr);
                break;
            }
            case util::CheckError:
            {
                // 检查错误！错误原因:
                qApp->style()->drawItemText(painter, resultRectList[i].second ,Qt::AlignLeft,
                                            option.palette, true,
                                            tr("Check for errors! Cause of error:"));
                break;
            }
            default:
                break;
            }
        }
    }

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
    //TODO:按钮事件的处理
    if (event->type() == QEvent::MouseButtonPress)
    {

        QMouseEvent* e =(QMouseEvent*)event;
        qDebug() << "editorEvent MouseButtonPress";
//        if (m_btns.contains(index))
//        {
//            QPair<QStyleOptionButton*, QStyleOptionButton*>* btns = m_btns.value(index);
//            if (btns->first->rect.contains(e->x(), e->y())) {
//                btns->first->state |= QStyle::State_Sunken;
//            }
//            else if(btns->second->rect.contains(e->x(), e->y())) {
//                btns->second->state |= QStyle::State_Sunken;
//            }
//        }
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* e =(QMouseEvent*)event;
        qDebug() << "editorEvent MouseButtonRelease";
//        if (m_btns.contains(index))
//        {
//            QPair<QStyleOptionButton*, QStyleOptionButton*>* btns = m_btns.value(index);
//            if (btns->first->rect.contains(e->x(), e->y())) {
//                btns->first->state &= (~QStyle::State_Sunken);
//                showMsg(tr("btn1 column %1").arg(index.column()));
//            } else if(btns->second->rect.contains(e->x(), e->y())) {
//                btns->second->state &= (~QStyle::State_Sunken);
//                showMsg(tr("btn2 row %1").arg(index.row()));
//            }
//        }
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
