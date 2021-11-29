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

#include "netsocket/DTLSSocketWrapper.h"
#include "platform/Callback.h"
#include "drivers/Timer.h"
#include "events/mbed_events.h"
#include "rtos/Kernel.h"

#if defined(MBEDTLS_SSL_CLI_C)

DTLSSocketWrapper::DTLSSocketWrapper(Socket *transport, const char *hostname, control_transport control) :
    TLSSocketWrapper(transport, hostname, control)
{
    mbedtls_ssl_conf_transport(get_ssl_config(), MBEDTLS_SSL_TRANSPORT_DATAGRAM);

    // Defines MBEDTLS_SSL_CONF_SET_TIMER/GET_TIMER define global functions which should be the same for all
    // callers of mbedtls_ssl_set_timer_cb and there should be only one ssl context. If these rules don't apply,
    // these defines can't be used
#if !defined(MBEDTLS_SSL_CONF_SET_TIMER) && !defined(MBEDTLS_SSL_CONF_GET_TIMER)
    mbedtls_ssl_set_timer_cb(get_ssl_context(), this, timing_set_delay, timing_get_delay);
#endif /* !defined(MBEDTLS_SSL_CONF_SET_TIMER) && !defined(MBEDTLS_SSL_CONF_GET_TIMER) */
}

DTLSSocketWrapper::~DTLSSocketWrapper()
{
    close();
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

    auto int_duration = std::chrono::duration<uint32_t, std::milli>(int_ms);
    auto fin_duration = std::chrono::duration<uint32_t, std::milli>(fin_ms);

    context->_int_time = rtos::Kernel::Clock::now() + int_duration;
    context->_timer_event_id = mbed::mbed_event_queue()->call_in(fin_duration, context, &DTLSSocketWrapper::timer_event);
}

int DTLSSocketWrapper::timing_get_delay(void *ctx)
{
    DTLSSocketWrapper *context = static_cast<DTLSSocketWrapper *>(ctx);

    /* See documentation of "typedef int mbedtls_ssl_get_timer_t( void * ctx );" from ssl.h */

    if (context->_timer_event_id == 0) {
        return -1;
    } else if (context->_timer_expired) {
        return 2;
    } else if (context->_int_time < rtos::Kernel::Clock::now()) {
        return 1;
    } else {
        return 0;
    }
}

nsapi_error_t DTLSSocketWrapper::close()
{
    if (_timer_event_id != 0) {
        mbed::mbed_event_queue()->cancel(_timer_event_id);
        _timer_event_id = 0;
    }

    return TLSSocketWrapper::close();
}

void DTLSSocketWrapper::timer_event(void)
{
    _timer_expired = true;
    event();
}

#endif /* MBEDTLS_SSL_CLI_C */
