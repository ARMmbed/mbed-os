#include "rtos/Mutex.h"

rtos::Mutex::Mutex()
{
    return;
}

rtos::Mutex::~Mutex()
{
    return;
}

osStatus rtos::Mutex::lock(unsigned int)
{
    return osOK;
}

osStatus rtos::Mutex::unlock()
{
    return osOK;
}
