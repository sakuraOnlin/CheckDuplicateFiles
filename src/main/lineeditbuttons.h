#ifndef LINEEDITBUTTONS_H
#define LINEEDITBUTTONS_H

#include <QObject>
#include <QLineEdit>
#include <QLabel>

class LineEditButtons : public QLineEdit
{
    Q_OBJECT
public:
    explicit LineEditButtons(QWidget *parent = nullptr);
    ~LineEditButtons();

signals:
    void signalCleanText();

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void onTextUpdata(const QString &);

private:
    void init();

private:
    QLabel *m_labelClear;
};

#endif // LINEEDITBUTTONS_H
