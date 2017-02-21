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
      m_font(qApp->font()),
      m_fontMetrics(m_font),
      m_fileNameLabelText(tr("FileName:")),
      m_filePathLabelText(tr("FilePath:")),
      m_fileSizeLabelText(tr("FileSize:")),
      m_fileTimeLabelText(tr("FileTime:"))
{
#ifdef Q_OS_WIN
    m_baseDouNum = 1024.0;
#else
    m_baseDouNum = 1000.0;
#endif
    m_kb = m_baseDouNum;
    m_mb = m_kb * m_baseDouNum;
    m_gb = m_mb * m_baseDouNum;
    m_labelSize = QSize(55, 18);
    m_textAlignment = Qt::AlignLeft | Qt::AlignVCenter;
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
    QStyleOptionViewItem itemOption(option);
    if (itemOption.state & QStyle::State_HasFocus)
    {
    itemOption.state = itemOption.state ^ QStyle::State_HasFocus;
    }
    QStyledItemDelegate::paint(painter, itemOption, index);

    //Data
    QString fileName(index.data(WidgetUtil::FileName).toString());
    QString filePath(index.data(WidgetUtil::FilePathRole).toString());
    QString fileSize(calculateFileSize(index.data(WidgetUtil::FileSizeRole).toLongLong()));
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
                       m_fontMetrics.size(Qt::TextSingleLine,fileTime).width(),
                       m_labelSize.height());

    filePath = cutString(filePath, filePathRect);
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
    painter->setFont(m_font);
    qApp->style()->drawItemPixmap(painter, fileIcoRect,
                                Qt::AlignCenter, fileIco);
    qApp->style()->drawItemText(painter, fileNameLabelRect, m_textAlignment,
                                option.palette, true, m_fileNameLabelText);
    qApp->style()->drawItemText(painter, fileNameRect , m_textAlignment,
                                option.palette, true, fileName);
    qApp->style()->drawItemText(painter, filePathLabelRect, m_textAlignment,
                                option.palette, true, m_filePathLabelText);
    qApp->style()->drawItemText(painter, filePathRect , m_textAlignment,
                                option.palette, true, filePath);
    qApp->style()->drawItemText(painter, fileSizeLabelRect, m_textAlignment,
                                option.palette, true, m_fileSizeLabelText);
    qApp->style()->drawItemText(painter, fileSizeRect , m_textAlignment,
                                option.palette, true, fileSize);
    qApp->style()->drawItemText(painter, fileTimeLabelRect, m_textAlignment,
                                option.palette, true, m_fileTimeLabelText);
    qApp->style()->drawItemText(painter, fileTimeRect , m_textAlignment,
                                option.palette, true, fileTime);

    if(isSelected)
    {
        qApp->style()->drawControl(QStyle::CE_PushButton, &pButtOpenFile, painter);
        qApp->style()->drawControl(QStyle::CE_PushButton, &pButtDelFile, painter);
        if(resultRectList.length() <= 0)
        {
            int contentStartingPoint = 3;
            QRect resultRect(labelDatumPositioningX + m_labelIntervaSize.width(),
                             labelDatumPositioningY + m_labelIntervaSize.height() *
                             contentStartingPoint, contentLengthX, m_labelSize.height());
            qApp->style()->drawItemText(painter, resultRect , Qt::AlignLeft,
                                        option.palette, true, tr("Not yet started"));
        }
        else
        {
            util::ResultMessageType messageType = index.data(
                        WidgetUtil::CheckTypeRole).value<util::ResultMessageType>();
            for(int i = 0 ; i < resultRectList.length(); i++ )
            {
                util::ComputeResult result = resultList[i];
                qApp->style()->drawItemText(painter, resultRectList[i].first , Qt::AlignLeft,
                                            option.palette, true,
                                            result.checkTypeName);
                switch (messageType) {
                case util::CheckIng:
                {
                    QStyleOptionProgressBar progressBarOption;
                    progressBarOption.initFrom(option.widget);
                    progressBarOption.rect = resultRectList[i].second;
                    progressBarOption.textAlignment = Qt::AlignCenter;
                    progressBarOption.minimum = 0;
                    bool fileSizeMaxBool = false;
                    if(result.fileSize >= m_mb)
                    {
                        double size = double(result.fileSize) / m_mb;
                        progressBarOption.maximum = int(size);
                        double progres = double(result.computeProgress) / m_mb ;
                        progressBarOption.progress = int(progres);
                        fileSizeMaxBool = true;
                    }
                    else
                    {
                        progressBarOption.maximum = int(result.fileSize);
                        progressBarOption.progress = int(result.computeProgress);
                    }
                    progressBarOption.text = QString("%1 / %2 %3")
                           .arg(QString::number(progressBarOption.progress))
                           .arg(QString::number(progressBarOption.maximum))
                           .arg(fileSizeMaxBool?"Mb" : "bit");
                    progressBarOption.textVisible = true;
                    QProgressBar progressBar;
                    QApplication::style()->drawControl(QStyle::CE_ProgressBar,
                                        &progressBarOption, painter, &progressBar);
                    break;
                }
                case util::CheckOver:
                {
                    qApp->style()->drawItemText(painter, resultRectList[i].second, Qt::AlignLeft,
                                                option.palette, true, result.resultStr);
                    break;
                }
                case util::CheckError:
                {
                    // 检查错误！错误原因:
                    qApp->style()->drawItemText(painter, resultRectList[i].second, Qt::AlignLeft,
                                                option.palette, true,
                                                tr("Check for errors! Cause of error:"));
                    break;
                }
                default:
                    break;
                }
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
    if(!index.data(WidgetUtil::ItemSelectRole).toBool())
        return QStyledItemDelegate::editorEvent(event, model, option, index);
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
            emit signalDelFile(filePath);
        }
        m_mouseType.first = false;
        m_mouseType.second = false;
        m_mousePoint = QPoint();
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QString ItemListDelegate::calculateFileSize(qint64 fileSize) const
{
    QString fileSizeStr("%1 %2");
    if (fileSize >= m_gb)
    {
        double size = fileSize / m_gb;
        fileSizeStr = fileSizeStr.arg(QString::number(size,'g',4)).arg("Gb");
    }
    else if (fileSize >= m_mb)
    {
        double size = fileSize / m_mb;
        fileSizeStr = fileSizeStr.arg(QString::number(size,'g',4)).arg("Mb");
    }
    else if (fileSize >= m_kb)
    {
        double size = fileSize / m_kb;
        fileSizeStr = fileSizeStr.arg(QString::number(size,'g',4)).arg("Kb");
    }
    else
        fileSizeStr = fileSizeStr.arg(QString::number(fileSize,'g',3)).arg("Kb");
    return fileSizeStr;
}

QString ItemListDelegate::cutString(QString text, QRect rect) const
{
    QString point("......");
    QString cutString(text);
    int average = text.length() / 2;
    while (m_fontMetrics.size(Qt::TextSingleLine,cutString).width() > rect.width())
    {
        if(cutString.length() <= 20)
            break;
        cutString = cutString.remove(average, point.length() + 1);
        cutString = cutString.insert(average,point);
    }
    return  cutString;
}
