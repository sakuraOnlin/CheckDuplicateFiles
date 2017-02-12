#include "lineeditbuttons.h"

#include <QEvent>
#include <QMouseEvent>

LineEditButtons::LineEditButtons(QWidget *parent)
    : QLineEdit(parent)
{
    init();
}

LineEditButtons::~LineEditButtons()
{
    delete m_labelClear;
}

bool LineEditButtons::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouse = nullptr;
        mouse = dynamic_cast<QMouseEvent*>(event);
        if(nullptr == mouse)
            return true;
        if(mouse->buttons() & Qt::LeftButton)
        {
            if(QLineEdit::isEnabled())
            {
                clear();
                emit signalCleanText();
                m_labelClear->hide();
                return true;
            }
        }
    }
    return QObject::eventFilter(watched, event);
}

void LineEditButtons::resizeEvent(QResizeEvent *event)
{
    m_labelClear->move(width() - m_labelClear->width() - 2,
                       height() /2 - 10);
    return QLineEdit::resizeEvent(event);
}

void LineEditButtons::onTextUpdata(const QString &str)
{
    if(str.isEmpty())
    {
        m_labelClear->hide();
        return;
    }

    m_labelClear->show();
}

void LineEditButtons::init()
{
    QPixmap delLineeditPixmap(":/img/image/lineeditDelete.png");
    m_labelClear = new QLabel(this);
    m_labelClear->setPixmap(delLineeditPixmap);
    m_labelClear->setMaximumSize(30,20);
    m_labelClear->installEventFilter(this);
    m_labelClear->hide();
    setTextMargins(0,0,31,0);

    connect(this, SIGNAL(textChanged(QString)), this, SLOT(onTextUpdata(QString)) );
}
