#ifndef DTLSSOCKETWRAPPER_H
#define DTLSSOCKETWRAPPER_H

#include "TLSSocketWrapper.h"

// This class requires Mbed TLS SSL/TLS client code
#if defined(MBEDTLS_SSL_CLI_C)

class DTLSSocketWrapper : public TLSSocketWrapper {
public:
    DTLSSocketWrapper(Socket *transport, const char *hostname = NULL, control_transport control = TRANSPORT_CONNECT_AND_CLOSE);
private:
    static void timing_set_delay(void *ctx, uint32_t int_ms, uint32_t fin_ms);
    static int timing_get_delay(void *ctx);
    void timer_event();
    int _timer_event_id;
    bool _timer_expired:1;
};

#endif
#endif
