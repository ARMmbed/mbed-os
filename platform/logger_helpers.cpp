/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

// No logging supported in case of NDEBUG or ID based
#if !defined(NDEBUG) && !defined(MBED_ID_BASED_TRACING)

#include <stdint.h>
#include "platform/mbed_assert.h"
#include "platform/PlatformMutex.h"
#include "platform/SingletonPtr.h"
#include "platform/mbed_critical.h"
#include "platform/logger.h"
#if defined(MBED_CONF_MBED_TRACE_FEA_IPV6)
#include "mbed-client-libservice/ip6string.h"
#include "mbed-client-libservice/common_functions.h"
#endif

#define LOG_SINGLE_HELPER_STR_SIZE_     LOG_SINGLE_STR_SIZE_

static SingletonPtr<PlatformMutex> log_helper_lock;
static uint32_t log_mutex_count = 0;
static char log_helper_data[LOG_SINGLE_HELPER_STR_SIZE_];
static bool log_data_valid = false;
static char send_null[] = "";
static char send_null_str[] = "<null>";

#ifdef __cplusplus
extern "C" {
#endif

char* mbed_log_ipv6(const uint8_t* addr_ptr)
{
    if (core_util_is_isr_active() || !core_util_are_interrupts_enabled()) {
        return send_null;
    }
#if defined(MBED_CONF_MBED_TRACE_FEA_IPV6)
    MBED_STATIC_ASSERT(LOG_SINGLE_HELPER_STR_SIZE_ >= 41, "Not enough room for ipv6 string, max 41");
    mbed_log_helper_lock();

    if (NULL == addr_ptr) {
        mbed_log_helper_unlock();
        return send_null;
    }

    log_helper_data[0] = 0;
    ip6tos(addr_ptr, log_helper_data);
    log_data_valid = true;

#if defined(MBED_CONF_ZERO_BUFFER_LOGGING) || !defined(MBED_CONF_RTOS_PRESENT)
    return log_helper_data;
#endif
#endif
    return send_null;
}

char* mbed_log_ipv6_prefix(const uint8_t* prefix, uint32_t prefix_len)
{
    if (core_util_is_isr_active() || !core_util_are_interrupts_enabled()) {
        return send_null;
    }
#if defined(MBED_CONF_MBED_TRACE_FEA_IPV6)
    MBED_STATIC_ASSERT(LOG_SINGLE_HELPER_STR_SIZE_ >= 44, "Not enough room for ipv6+prefix string, max 44");
    mbed_log_helper_lock();

    if (0 == prefix_len) {
        mbed_log_helper_unlock();
        return send_null;
    }
    if (NULL == prefix) {
        mbed_log_helper_unlock();
        return send_null_str;
    }

    ip6_prefix_tos(prefix, prefix_len, log_helper_data);
    log_data_valid = true;

#if defined(MBED_CONF_ZERO_BUFFER_LOGGING) || !defined(MBED_CONF_RTOS_PRESENT)
    return log_helper_data;
#endif
#endif
    return send_null;
}

char* mbed_log_array(const uint8_t* buf, uint32_t len)
{
    if (core_util_is_isr_active() || !core_util_are_interrupts_enabled()) {
        return send_null;
    }

    mbed_log_helper_lock();
    if (0 == len) {
        mbed_log_helper_unlock();
        return send_null;
    }
    if (NULL == buf) {
        mbed_log_helper_unlock();
        return send_null_str;
    }

    char* str = log_helper_data;
    uint32_t count = 0, retVal;
    for (uint32_t i = 0; i < len; i++) {
        retVal = snprintf(str+count, (LOG_SINGLE_HELPER_STR_SIZE_-count), "%02x", *buf++);
        if (retVal <= 0) {
            break;
        }
        count += retVal;
        if (count >= LOG_SINGLE_HELPER_STR_SIZE_) {
            str[LOG_SINGLE_HELPER_STR_SIZE_-2] = '*';
            str[LOG_SINGLE_HELPER_STR_SIZE_-1] = '\0';
            break;
        }
    }
    log_data_valid = true;

#if defined(MBED_CONF_ZERO_BUFFER_LOGGING) || !defined(MBED_CONF_RTOS_PRESENT)
    return log_helper_data;
#endif
    return send_null;
}

void mbed_log_helper_lock(void) {
    log_helper_lock->lock();
    log_mutex_count++;
}

void mbed_log_helper_unlock(void) {
    log_mutex_count--;
    log_data_valid = false;
    log_helper_lock->unlock();
}

void mbed_log_helper_unlock_all(void) {
    int count = log_mutex_count;
    log_mutex_count = 0;
    log_data_valid = false;
    do {
        log_helper_lock->unlock();
    } while (--count > 0);
}

int mbed_log_valid_helper_data(void) {
    return log_data_valid;
}

char* mbed_log_get_helper_data(void) {
    return log_helper_data;
}

#ifdef __cplusplus
}
#endif

#endif
