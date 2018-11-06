
/** \addtogroup frameworks */
/** @{*/
#ifndef GREENTEA_SERIAL_H
#define GREENTEA_SERIAL_H

#include "RawSerial.h"
#include "SingletonPtr.h"
#include "PlatformMutex.h"

class GreenteaSerial : public mbed::RawSerial {
public:
    GreenteaSerial();
    void set_trace_mutex(PlatformMutex *trace_mutex);
    void lock_trace();
    void unlock_trace();
private:
    PlatformMutex *_trace_mutex;
};

extern SingletonPtr<GreenteaSerial> greentea_serial;
#endif

/** @}*/
