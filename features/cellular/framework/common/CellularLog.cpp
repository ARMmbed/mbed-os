/*
 * Copyright (c) , Arm Limited and affiliates.
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
#include <stdarg.h>
#include <stddef.h>
#include "CellularLog.h"
#include "us_ticker_api.h"
#include "serial_api.h"
#include "rtos/Mutex.h"

extern serial_t     stdio_uart;
extern int          stdio_uart_inited;

namespace mbed {

uint32_t cellular_log_time = us_ticker_read() / 1000L;

/**
 * Local mutex object for synchronization
 */
static rtos::Mutex mutex;

/**
 * Lock provided for serial printing used by trace library
 */
static void serial_lock()
{
    mutex.lock();
}

/**
 * Releasing lock provided for serial printing used by trace library
 */
static void serial_unlock()
{
    mutex.unlock();
}

void log_init(PinName tx, PinName rx, int baud)
{
#ifdef FEATURE_COMMON_PAL
    mbed_trace_mutex_wait_function_set(serial_lock);
    mbed_trace_mutex_release_function_set(serial_unlock);
    mbed_trace_init();
#else
    serial_init(&stdio_uart, tx, rx);
    serial_baud(&stdio_uart, baud);
    stdio_uart_inited = 1;
#endif
    log_info("\r\n\r\n**************************************");
    log_info("Hellular says Cellular Hello!");
    log_info("Build time %s %s", __DATE__, __TIME__);
}
} // namespace mbed
