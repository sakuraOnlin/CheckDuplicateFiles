#ifndef CRC32_H
#define CRC32_H

#include <QObject>

class CRC32 : public QObject
{
    Q_OBJECT
public:
    explicit CRC32(QObject *parent = 0);

signals:

public slots:
};

#endif // CRC32_H