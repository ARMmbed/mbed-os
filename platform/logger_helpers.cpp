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

#include <stdint.h>
#include "platform/mbed_assert.h"
#include "platform/logger.h"
#if defined(MBED_CONF_MBED_TRACE_FEA_IPV6)
#include "mbed-client-libservice/ip6string.h"
#include "mbed-client-libservice/common_functions.h"
#endif

#if defined(MBED_CONF_ZERO_BUFFER_LOGGING) || !defined(MBED_CONF_RTOS_PRESENT)
#include "platform/PlatformMutex.h"
static SingletonPtr<PlatformMutex> log_helper_lock;
#else
#include "rtos/Semaphore.h"
using namespace rtos;
static Semaphore log_helper_lock(1,1);
#endif

#if !defined(NDEBUG)
static char log_helper_data[MBED_CONF_MAX_LOG_STR_SIZE];
static bool log_data_valid = false;
static char send_null[2] = "";
#endif

#ifdef __cplusplus
extern "C" {
#endif


char* mbed_log_ipv6(const uint8_t* addr_ptr)
{
#if !defined(NDEBUG) && defined(MBED_CONF_MBED_TRACE_FEA_IPV6)
    MBED_STATIC_ASSERT(MBED_CONF_MAX_LOG_STR_SIZE >= 41, "Not enough room for ipv6 string, max 41");
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
#else
    return send_null;
#endif

#else
    return NULL;
#endif
}

char* mbed_log_ipv6_prefix(const uint8_t* prefix, uint32_t prefix_len)
{
#if !defined(NDEBUG) && defined(MBED_CONF_MBED_TRACE_FEA_IPV6)
    MBED_STATIC_ASSERT(MBED_CONF_MAX_LOG_STR_SIZE >= 44, "Not enough room for ipv6+prefix string, max 44");
    mbed_log_helper_lock();

    if ((NULL == prefix) || (0 == prefix_len)) {
        mbed_log_helper_unlock();
        return send_null;
    }

    ip6_prefix_tos(prefix, prefix_len, log_helper_data);
    log_data_valid = true;

#if defined(MBED_CONF_ZERO_BUFFER_LOGGING) || !defined(MBED_CONF_RTOS_PRESENT)
    return log_helper_data;
#else
    return send_null;
#endif

#else
    return NULL;
#endif
}

char* mbed_log_array(const uint8_t* buf, uint32_t len)
{
#if !defined(NDEBUG)
    mbed_log_helper_lock();
    if ((NULL == buf) || (0 == len)) {
        mbed_log_helper_unlock();
        return send_null;
    }

    char* str = log_helper_data;
    uint8_t count = 0, retVal;
    for (uint32_t i = 0; i < len; i++) {
        retVal = snprintf(str+count, (MBED_CONF_MAX_LOG_STR_SIZE-count), "%02x", *buf++);
        if (retVal <= 0) {
            break;
        }
        count += retVal;
        if (count >= MBED_CONF_MAX_LOG_STR_SIZE) {
            str[MBED_CONF_MAX_LOG_STR_SIZE-2] = '*';
            str[MBED_CONF_MAX_LOG_STR_SIZE-1] = '\0';
            break;
        }
    }
    log_data_valid = true;

#if defined(MBED_CONF_ZERO_BUFFER_LOGGING) || !defined(MBED_CONF_RTOS_PRESENT)
    return log_helper_data;
#else
    return send_null;
#endif

#else
    return NULL;
#endif
}

void mbed_log_helper_lock(void) {
#if !defined(NDEBUG)
#if defined(MBED_CONF_RTOS_PRESENT)
    int retVal = log_helper_lock.wait();
    MBED_ASSERT(retVal != -1);
#else
    log_helper_lock.lock();
#endif
#endif
}

void mbed_log_helper_unlock(void) {
#if !defined(NDEBUG)
#if defined(MBED_CONF_RTOS_PRESENT)
    int retVal = log_helper_lock.release();
    MBED_ASSERT(retVal == osOK);
#else
    log_helper_lock.unlock();
#endif
    log_data_valid = false;
#endif
}

int mbed_log_valid_helper_data(void) {
#if !defined(NDEBUG)
    return log_data_valid;
#else
    return false;
#endif
}

char* mbed_log_get_helper_data(void) {
#if !defined(NDEBUG)
    return log_helper_data;
#else
    return NULL;
#endif
}

#ifdef __cplusplus
}
#endif
