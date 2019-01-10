#include "mbed_trace.h"

class MbedTraceInitialiser {
public:
    MbedTraceInitialiser() {
        mbed_trace_init();
    }
};

#if MBED_CONF_MBED_TRACE_ENABLE
MbedTraceInitialiser initialiser;
#endif
