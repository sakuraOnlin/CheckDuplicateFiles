#ifndef CRYPTO_H
#define CRYPTO_H

#include <QCryptographicHash>
#include <QString>
#include <QByteArray>
#include "compute.h"

template <typename T>
class Crypto : public Compute
{
public:
    explicit Crypto (T type)
        :m_crypto(new QCryptographicHash(type) )
    {
    }

    ~Crypto()
    {
        if(NULL != m_crypto)
            delete m_crypto;
    }

    void update(QString &s ,QString oldComputeStr)
    {
        if(NULL == m_crypto)
            return;

        Q_UNUSED(oldComputeStr)
        QByteArray byteAdta(s.toUtf8());
        m_crypto->addData(byteAdta);
    }

    QString getFinalResult()
    {
        if(NULL == m_crypto)
            return QString();

        return QString(m_crypto->result());
    }

    void stopCheck(){}

private:
    QCryptographicHash *m_crypto;
};

#endif // CRYPTO_H