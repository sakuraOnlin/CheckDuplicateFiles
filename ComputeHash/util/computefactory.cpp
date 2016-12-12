#include "computefactory.h"
#include "type/sha1.h"
#include "type/crc32.h"

Factory::Factory()
{

}

Compute *Factory::createCompute(const util::ComputeType type)
{
    Compute *factoryType = NULL;
    switch (type) {
    case util::SHA1:
        factoryType = new SHA1;
        break;
    case util::MD5:
        factoryType = new CRC32;
        break;
    default:
        break;
    }
    return factoryType;
}
