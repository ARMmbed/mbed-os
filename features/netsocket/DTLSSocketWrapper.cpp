/*
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "DTLSSocketWrapper.h"
#include "platform/Callback.h"
#include "drivers/Timer.h"
#include "events/mbed_events.h"
#include "rtos/Kernel.h"

#if defined(MBEDTLS_SSL_CLI_C)

DTLSSocketWrapper::DTLSSocketWrapper(Socket *transport, const char *hostname, control_transport control) :
    TLSSocketWrapper(transport, hostname, control),
    _int_ms_tick(0),
    _timer_event_id(0),
    _timer_expired(false)
{
    mbedtls_ssl_conf_transport(get_ssl_config(), MBEDTLS_SSL_TRANSPORT_DATAGRAM);
    mbedtls_ssl_set_timer_cb(get_ssl_context(), this, timing_set_delay, timing_get_delay);
}

void DTLSSocketWrapper::timing_set_delay(void *ctx, uint32_t int_ms, uint32_t fin_ms)
{
    DTLSSocketWrapper *context = static_cast<DTLSSocketWrapper *>(ctx);

    if (context->_timer_event_id) {
        mbed::mbed_event_queue()->cancel(context->_timer_event_id);
        context->_timer_expired = false;
    }

    if (fin_ms == 0) {
        context->_timer_event_id = 0;
        return;
    }

    context->_int_ms_tick = rtos::Kernel::get_ms_count() + int_ms;
    context->_timer_event_id = mbed::mbed_event_queue()->call_in(fin_ms, context, &DTLSSocketWrapper::timer_event);
}

int DTLSSocketWrapper::timing_get_delay(void *ctx)
{
    DTLSSocketWrapper *context = static_cast<DTLSSocketWrapper *>(ctx);

    /* See documentation of "typedef int mbedtls_ssl_get_timer_t( void * ctx );" from ssl.h */

    if (context->_timer_event_id == 0) {
        return -1;
    } else if (context->_timer_expired) {
        return 2;
    } else if (context->_int_ms_tick < rtos::Kernel::get_ms_count()) {
        return 1;
    } else {
        return 0;
    }
}

void DTLSSocketWrapper::timer_event(void)
{
    _timer_expired = true;
    event();
}

#endif /* MBEDTLS_SSL_CLI_C */
