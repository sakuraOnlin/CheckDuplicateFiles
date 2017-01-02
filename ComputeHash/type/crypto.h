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
        switch (type)
        {
        case QCryptographicHash::Sha1:
            m_hashType = util::SHA1;
            break;
        case QCryptographicHash::Md5:
            m_hashType = util::MD5;
        default:
            m_hashType = util::NOTYPE;
            break;
        }
    }

    ~Crypto()
    {
        if(NULL != m_crypto)
            delete m_crypto;
    }

    void update(QString &data ,QString oldComputeHash)
    {
        if(NULL == m_crypto)
            return;

        Q_UNUSED(oldComputeHash)
        QByteArray byteAdta(data.toUtf8());
        m_crypto->addData(byteAdta);
    }

    QString getFinalResult()
    {
        if(NULL == m_crypto)
            return QString();

        return QString(m_crypto->result());
    }

    void reset()
    {
        m_crypto->reset();
    }

    util::ComputeType getType()
    {
        return m_hashType;
    }

private:
    QCryptographicHash *m_crypto;
    util::ComputeType m_hashType;
};

#endif // CRYPTO_H
