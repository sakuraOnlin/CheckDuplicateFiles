#include <QCryptographicHash>
#include "computefactory.h"
#include "type/crypto.h"
#include "type/crc32.h"

Factory::Factory(QObject *parent)
    :QObject(parent)
{

}

Factory::~Factory()
{

}

QList<util::factoryCreateResult> Factory::createCompute(const util::ComputeType type)
{
    QList<util::factoryCreateResult> computeList;
    if( (type & util::SHA1) == util::SHA1)
    {
        Compute *factoryType = new Crypto<QCryptographicHash::Algorithm>(QCryptographicHash::Sha1);
        util::factoryCreateResult reuslt;
        reuslt.creatorComputr = NULL;
        if(NULL == factoryType)
            reuslt.creatorErrStr = tr("Error : Factoey Creator Compute SHA1 Error!");
        else
            reuslt.creatorComputr = factoryType;
        computeList.append(reuslt);
    }

    if( (type & util::MD5) == util::MD5)
    {
        Compute *factoryType = new Crypto<QCryptographicHash::Algorithm>(QCryptographicHash::Md5);
        util::factoryCreateResult reuslt;
        reuslt.creatorComputr = NULL;
        if(NULL == factoryType)
            reuslt.creatorErrStr = tr("Error : Factoey Creator Compute MD5 Error!");
        else
            reuslt.creatorComputr = factoryType;
        computeList.append(reuslt);
    }

    if( (type & util::CRC32) == util::CRC32)
    {
        Compute *factoryType = new CRC32;
        util::factoryCreateResult reuslt;
        reuslt.creatorComputr = NULL;
        if(NULL == factoryType)
            reuslt.creatorErrStr = tr("Error : Factoey Creator Compute CRC32 Error!");
        else
            reuslt.creatorComputr = factoryType;
        computeList.append(reuslt);
    }

    return computeList;
}
