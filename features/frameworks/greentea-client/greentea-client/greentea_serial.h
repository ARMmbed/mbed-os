
/** \addtogroup frameworks */
/** @{*/
#ifndef GREENTEA_SERIAL_H
#define GREENTEA_SERIAL_H

#ifdef MBED_TEST

#include "RawSerial.h"
#include "SingletonPtr.h"

class GreenteaSerial : public mbed::RawSerial {
public:
    GreenteaSerial();
};

extern SingletonPtr<GreenteaSerial> greentea_serial;
#endif
#endif

/** @}*/
