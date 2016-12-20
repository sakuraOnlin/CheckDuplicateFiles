#ifndef COMPUTEFACTORY_H
#define COMPUTEFACTORY_H

#include "util/util.h"
#include "compute.h"

class COMPUTEHASHSHARED_EXPORT Factory
{
public:
    virtual ~Factory();
    virtual Compute* createCompute(const util::ComputeType type);
};

#endif // COMPUTEFACTORY_H
