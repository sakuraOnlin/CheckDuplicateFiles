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
    {
        switch (type)
        {
        case QCryptographicHash::Sha1:
            m_hashType = util::SHA1;
            m_typeName = "SHA1";
            m_crypto = new QCryptographicHash(QCryptographicHash::Sha1);
            break;
        case QCryptographicHash::Md5:
        default:
            m_hashType = util::MD5;
            m_typeName = "MD5";
            m_crypto = new QCryptographicHash(QCryptographicHash::Md5);
            break;
        }
    }

    ~Crypto()
    {
        delete m_crypto;
    }

    void update(QByteArray &data, QString oldComputeHash)
    {
        Q_UNUSED(oldComputeHash)
        m_crypto->addData(data);
    }

    QString getFinalResult()
    {
        return QString(m_crypto->result().toHex().toUpper());
    }

    void reset()
    {
        m_crypto->reset();
    }

    util::CheckType getType()
    {
        return m_hashType;
    }
    QString getTypeName()
    {
        return m_typeName;
    }

private:
    QCryptographicHash *m_crypto;
    util::CheckType m_hashType;
    QString m_typeName;
};

#endif // CRYPTO_H
