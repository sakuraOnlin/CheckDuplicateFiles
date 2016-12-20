#include <QCryptographicHash>
#include "computefactory.h"
#include "type/crypto.h"
#include "type/crc32.h"

Factory::~Factory()
{

}

Compute *Factory::createCompute(const util::ComputeType type)
{
    Compute *factoryType = NULL;
    switch (type) {
    case util::SHA1:
        factoryType = new Crypto<QCryptographicHash::Algorithm>(QCryptographicHash::Sha1);
        break;
    case util::MD5:
        factoryType = new Crypto<QCryptographicHash::Algorithm>(QCryptographicHash::Md5);
        break;
    case util::CRC32:
        factoryType = new CRC32;
        break;
    default:
        break;
    }
    return factoryType;
}
