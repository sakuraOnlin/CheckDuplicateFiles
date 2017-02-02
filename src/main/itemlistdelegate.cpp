#include <QApplication>
#include <QPainter>
#include <QPen>
#include <QFileIconProvider>
#include <QIcon>
#include <QPair>
#include <QDebug>
#include <QProgressBar>
#include <QMouseEvent>

#include "itemlistdelegate.h"
#include "core/widgetUtil.h"
#include "util/util.h"

Q_DECLARE_METATYPE(util::ComputeResult)
Q_DECLARE_METATYPE(util::ResultMessageType)

ItemListDelegate::ItemListDelegate(QObject *parent)
    :QStyledItemDelegate(parent),
      m_pixmapPoint(QPoint(5,5)),
      m_pixmapSize(QSize(32, 32)),
      m_pButtSize(QSize(80, 22)),
      m_labelSize(QSize(50, 17))
{
    m_interval = 5;
    m_labelIntervaSize.setWidth(m_labelSize.width() + m_interval );
    m_labelIntervaSize.setHeight(m_labelSize.height() + m_interval );
    m_mouseType.first = false;
    m_mouseType.second = false;
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
    int contentStartingPoint = 1;

    QList<util::ComputeResult> resultList = index.data(
                WidgetUtil::CheckResultRole).value<QList<util::ComputeResult> >();

    QRect optRect(option.rect);
    optRect.setWidth(optRect.width() -1);
    QRect fileIcoRect(optRect.x() + m_interval, optRect.y() + m_interval,
                      m_pixmapSize.width(), m_pixmapSize.height());
    int labelDatumPositioningX = optRect.x() + 32 + m_interval *2 ;
    int labelDatumPositioningY = optRect.y() + m_interval;
    int puButDatumPositioningX = optRect.width() - m_interval - m_pButtSize.width();
    int contentLengthX = optRect.width() - fileIcoRect.width()
            - m_labelIntervaSize.width() - m_pButtSize.width() - m_interval * 4;

    QRect pButOpenDirRect(puButDatumPositioningX, labelDatumPositioningY,
                          m_pButtSize.width(), m_pButtSize.height());
    QRect pButDelFileRect(puButDatumPositioningX, labelDatumPositioningY +
                              m_pButtSize.height() + m_interval,
                              m_pButtSize.width(), m_pButtSize.height());
    QRect fileNameLabelRect(labelDatumPositioningX, labelDatumPositioningY,
                        m_labelSize.width(), m_labelSize.height());
    QRect fileNameRect(labelDatumPositioningX + m_labelIntervaSize.width(),
                   fileNameLabelRect.y(), contentLengthX, m_labelSize.height());
    QRect filePathLabelRect(labelDatumPositioningX, labelDatumPositioningY +
                        m_labelIntervaSize.height() * contentStartingPoint++,
                        m_labelSize.width(), m_labelSize.height());
    QRect filePathRect(labelDatumPositioningX + m_labelIntervaSize.width(),
                       filePathLabelRect.y(), contentLengthX, m_labelSize.height());
    QRect fileSizeLabelRect(labelDatumPositioningX, labelDatumPositioningY +
                        m_labelIntervaSize.height() * contentStartingPoint++,
                        m_labelSize.width(), m_labelSize.height());
    QRect fileSizeRect(labelDatumPositioningX + m_labelIntervaSize.width(),
                       fileSizeLabelRect.y(), m_labelSize.width(), m_labelSize.height());
    QRect fileTimeLabelRect(fileSizeRect.x() + m_labelIntervaSize.width(), fileSizeRect.y(),
                            m_labelSize.width(), m_labelSize.height());
    QRect fileTimeRect(fileTimeLabelRect.x() + m_labelIntervaSize.width(), fileSizeRect.y(),
                       fontMetrics.size(Qt::TextSingleLine,fileTime).width(),
                       m_labelSize.height());

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
                                 m_labelIntervaSize.height() * contentStartingPoint,
                                 m_labelSize.width(), m_labelSize.height());

            QRect resultRect(labelDatumPositioningX + m_labelIntervaSize.width(),
                             resultNameRect.y(), contentLengthX, m_labelSize.height());
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

    if(!m_mousePoint.isNull())
    {
        if(pButOpenDirRect.contains(m_mousePoint))
            pButtOpenFile.state |= QStyle::State_Sunken;
        if(pButDelFileRect.contains(m_mousePoint))
            pButtDelFile.state |= QStyle::State_Sunken;
    }

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
    QMouseEvent* mouseEvent = nullptr;
    mouseEvent = dynamic_cast<QMouseEvent*>(event);

    if(nullptr == mouseEvent)
    {
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }

    m_mousePoint = mouseEvent->pos();
    QRect optRect(option.rect);
    optRect.setWidth(optRect.width() -1);
    int labelDatumPositioningY = optRect.y() + m_interval;
    int puButDatumPositioningX = optRect.width() - m_interval - m_pButtSize.width();

    QRect pButOpenDirRect(puButDatumPositioningX, labelDatumPositioningY,
                          m_pButtSize.width(), m_pButtSize.height());
    QRect pButDelFileRect(puButDatumPositioningX, labelDatumPositioningY +
                              m_pButtSize.height() + m_interval,
                              m_pButtSize.width(), m_pButtSize.height());

    bool mouseInButtonArea = false;
    if (pButOpenDirRect.contains(mouseEvent->x(), mouseEvent->y()) ||
            pButDelFileRect.contains(mouseEvent->x(), mouseEvent->y()))
    {
        mouseInButtonArea = true;
        m_mouseType.first = true;
    }
    else
        m_mouseType.first = false;

    //TODO:按钮事件的处理
    if (event->type() == QEvent::MouseButtonPress)
    {
        if (mouseInButtonArea)
        {
            m_mouseType.second = true;
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        QString filePath(index.data(WidgetUtil::FilePathRole).toString());
        if(pButOpenDirRect.contains(mouseEvent->x(), mouseEvent->y()) )
        {
            emit signalOpenFileDir(filePath);
        }
        else if(pButDelFileRect.contains(mouseEvent->x(), mouseEvent->y()))
        {
            //TODO:删除文件时正在进行计算
            emit signalDelFile(filePath);
        }
        m_mouseType.first = false;
        m_mouseType.second = false;
        m_mousePoint = QPoint();
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
