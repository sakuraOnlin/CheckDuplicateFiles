#ifndef COMPUTEFACTORY_H
#define COMPUTEFACTORY_H

#include "util/util.h"
#include "compute.h"

class Factory
{
public:
    Factory();

    Compute* createCompute(const util::ComputeType type);
};

#endif // COMPUTEFACTORY_H
