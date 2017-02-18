#include <QList>
#include <QDebug>

#include "findrepeat.h"
#include "core/backstage.h"
#include "core/widgetUtil.h"
#include "util/util.h"

Q_DECLARE_METATYPE(util::ComputeResult)

FindRepeatWork::FindRepeatWork(QObject *parent)
    :QObject(parent)
{
    m_operatingStatus = false;
}

FindRepeatWork::~FindRepeatWork()
{

}

void FindRepeatWork::onAddResultID(QListWidgetItem *item)
{
    if(nullptr == item)
        return;
    QList<util::ComputeResult> resultList = item->data(
                    WidgetUtil::CheckResultRole).value<QList<util::ComputeResult> >();
    for(int i = 0 ; i < resultList.length(); i++)
    {
        m_fileIDHash.insert(resultList.at(i).resultStr, item);
    }
}

void FindRepeatWork::setListWidget(QListWidget *listWidget)
{
    m_listWidget = listWidget;
}

void FindRepeatWork::setFileItem(QHash<QString, QListWidgetItem *> *fileItemHash)
{
    m_fileItemHash = fileItemHash;
}

void FindRepeatWork::onFindAllRepeat()
{
    if(!m_operatingStatus)
        return;

    onRestoreBackground();
    m_findNextIndex = 0;
    QList<QString> keys(m_fileIDHash.keys());
    QStringList notRepeatingTextList;
    foreach (QString text, keys)
    {
        if(notRepeatingTextList.indexOf(text) >= 0 )
            continue;
        notRepeatingTextList.append(text);
    }

    QList<QListWidgetItem*> values;
    foreach (QString text, notRepeatingTextList)
    {
        if(!m_operatingStatus)
            return;
        if(m_fileIDHash.count(text) > 1)
        {
            QList<QListWidgetItem*> repeatItemList(m_fileIDHash.values(text));
            QListWidgetItem *item = repeatItemList.at(0);
            int itemRow = 0;
            if(item == nullptr)
                itemRow = 0;
            else
                itemRow = m_listWidget->row(item);
            int rowIndex = 0;
            foreach (QListWidgetItem *item, repeatItemList)
            {
                if(values.indexOf(item) >= 0)
                    continue;
                setItemBackGroundData(item, true);
                m_listWidget->takeItem(m_listWidget->row(item));
                int insertRow = itemRow + rowIndex;
                m_listWidget->insertItem(insertRow, item);
                values.append(item);
            }
        }
    }
    m_repeatList = values;
    emit signalFindDone(values.length());
}

void FindRepeatWork::onFindText(QString text)
{
    if(!m_operatingStatus)
        return;
    QString textMax = text.toUpper();
    QList<QListWidgetItem*> values = m_fileIDHash.values(textMax);
    if(values.length() ==0)
    {
        emit signalFindDone(0); ;
        return;
    }

    onRestoreBackground();

    m_findNextIndex = 0;
    for(int i = 0 ; i < values.length(); i++)
    {
        QListWidgetItem *item = nullptr;
        item = values.at(i);
        if(item == nullptr)
            continue;
        setItemBackGroundData(item, true);
    }
    m_listWidget->setCurrentItem(values.at(m_findNextIndex));
    m_repeatList = values;
    emit signalFindDone(m_repeatList.length());
}

void FindRepeatWork::onFindNextText()
{
    if(!m_operatingStatus)
        return;

    if(m_repeatList.length() == 0)
        return;

    if(m_findNextIndex >= m_repeatList.length())
        m_findNextIndex = 0;

    QListWidgetItem* item = m_repeatList.at(m_findNextIndex);
    m_listWidget->setCurrentItem(item);
    Backstage::getInstance().getBackstagWork()->onClickItem(item);
    m_findNextIndex++;
}

void FindRepeatWork::onDelFile(QString filePath)
{
    if(!m_operatingStatus)
        return;
    QListWidgetItem *item = nullptr;
    item = m_fileItemHash->value(filePath);
    if(nullptr == item)
        return;

    QList<util::ComputeResult> resultList = item->data(
                    WidgetUtil::CheckResultRole).value<QList<util::ComputeResult> >();
    for(int i = 0 ; i < resultList.length(); i++)
    {
        m_fileIDHash.remove(resultList.at(i).resultStr);
    }

    int index = m_repeatList.indexOf(item);
    if(index >=0 && index < m_repeatList.length())
        m_repeatList.takeAt(index);
}

void FindRepeatWork::onRestoreBackground()
{
    for(int i = 0; i< m_repeatList.length(); i++)
    {
        setItemBackGroundData(m_repeatList.at(i));
    }
    m_repeatList.clear();
}

void FindRepeatWork::onClear()
{
    m_repeatList.clear();
    m_fileIDHash.clear();
}

void FindRepeatWork::setItemBackGroundData(QListWidgetItem *item, bool setBackground)
{
    setBackground? item->setData(Qt::BackgroundColorRole, QColor(QColor(255,167,167))):
                   item->setData(Qt::BackgroundColorRole, QColor(255,255,255,0));
}

FindRepeat::FindRepeat(QObject *parent)
    :QObject (parent)
{
    m_findRepeatWork = new FindRepeatWork;
    m_findRepeatWork->moveToThread(&m_thread);
    connect(&m_thread, SIGNAL(finished()),
            m_findRepeatWork, SLOT(deleteLater()) );
    connect(this, SIGNAL(signalFindAllRepeat() ),
            m_findRepeatWork, SLOT(onFindAllRepeat() ));
    connect(this, SIGNAL(signalFindText(QString)),
            m_findRepeatWork, SLOT(onFindText(QString)) );
    connect(this, SIGNAL(signalFindNextText()),
            m_findRepeatWork, SLOT(onFindNextText()));
    connect(m_findRepeatWork, SIGNAL(signalFindDone(int)),
            this, SIGNAL(signalFindDone(int)));
    m_thread.start();
}

FindRepeat::~FindRepeat()
{
    m_thread.quit();
    m_thread.wait(200);
    m_findRepeatWork = nullptr;
}

void FindRepeat::setListWidget(QListWidget *widget)
{
    m_findRepeatWork->setListWidget(widget);
}

void FindRepeat::onAddResultID(QListWidgetItem *item)
{
    m_findRepeatWork->onAddResultID(item);
}

void FindRepeat::setFileItem(QHash<QString, QListWidgetItem *> *fileItemHash)
{
    m_findRepeatWork->setFileItem(fileItemHash);
}

void FindRepeat::onStart()
{
    m_findRepeatWork->onClear();
    m_findRepeatWork->m_operatingStatus = true;
}

void FindRepeat::onStop()
{
    m_findRepeatWork->m_operatingStatus = false;
}

void FindRepeat::onFindAllRepeat()
{
    emit signalFindAllRepeat();
}

void FindRepeat::onFindText(QString text)
{
    emit signalFindText(text);
}

void FindRepeat::onFindNextText()
{
    emit signalFindNextText();
}

void FindRepeat::onRestoreBackground()
{
    m_findRepeatWork->onRestoreBackground();
}

void FindRepeat::onClear()
{
    m_findRepeatWork->onClear();
}
