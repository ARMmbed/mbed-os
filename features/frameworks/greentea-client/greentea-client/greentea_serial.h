
/** \addtogroup frameworks */
/** @{*/
#ifndef GREENTEA_SERIAL_H
#define GREENTEA_SERIAL_H

#include "RawSerial.h"
#include "SingletonPtr.h"
#include "Mutex.h"

class GreenteaSerial : public mbed::RawSerial {
public:
    GreenteaSerial();
    void virtual lock();
    void virtual unlock();
private:
    rtos::Mutex _mutex;
};

extern SingletonPtr<GreenteaSerial> greentea_serial;
#endif

/** @}*/
