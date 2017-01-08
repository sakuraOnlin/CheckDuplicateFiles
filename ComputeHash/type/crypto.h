#ifndef CRYPTO_H
#define CRYPTO_H

#include <QCryptographicHash>
#include <QString>
#include <QByteArray>
#include "compute.h"

#ifdef _DEBUG
#include <QDebug>
#endif

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
            m_typeName = "SHA1";
            break;
        case QCryptographicHash::Md5:
            m_hashType = util::MD5;
            m_typeName = "MD5";
            break;
        default:
            m_hashType = util::NOTYPE;
            m_typeName = "NoType";
            break;
        }
    }

    ~Crypto()
    {
        if(NULL != m_crypto)
            delete m_crypto;
    }

    void update(QByteArray &data ,QString oldComputeHash)
    {
        if(NULL == m_crypto)
            return;

        Q_UNUSED(oldComputeHash)
        m_crypto->addData(data);
    }

    QString getFinalResult()
    {
        if(NULL == m_crypto)
            return QString();

#ifdef _DEBUG
        QByteArray data(m_crypto->result());
        qDebug() << "Crypto Result" << data << " , " << data.toHex().toUpper();
#endif
        return QString(m_crypto->result().toHex().toUpper());
    }

    void reset()
    {
        m_crypto->reset();
    }

    util::ComputeType getType()
    {
        return m_hashType;
    }
    QString getTypeName()
    {
        return m_typeName;
    }

private:
    QCryptographicHash *m_crypto;
    util::ComputeType m_hashType;
    QString m_typeName;
};

#endif // CRYPTO_H
