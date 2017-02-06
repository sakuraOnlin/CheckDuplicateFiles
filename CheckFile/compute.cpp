#include "compute.h"

Compute::Compute(QObject *parnet)
    :QObject(parnet),
      m_isRunning(false)
{

}

Compute::~Compute()
{

}
