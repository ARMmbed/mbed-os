/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef M2MCONFIG_H
#define M2MCONFIG_H

#include <stdlib.h>
#include <cstring>
#include <cstdio>
#include "mbed-client/m2mstring.h"
using namespace m2m;

#ifdef YOTTA_CFG_RECONNECTION_COUNT
#define MBED_CLIENT_RECONNECTION_COUNT YOTTA_CFG_RECONNECTION_COUNT
#elif defined MBED_CONF_MBED_CLIENT_RECONNECTION_COUNT
#define MBED_CLIENT_RECONNECTION_COUNT MBED_CONF_MBED_CLIENT_RECONNECTION_COUNT
#else
#define MBED_CLIENT_RECONNECTION_COUNT 3
#endif

#ifdef YOTTA_CFG_RECONNECTION_INTERVAL
#define MBED_CLIENT_RECONNECTION_INTERVAL YOTTA_CFG_RECONNECTION_INTERVAL
#elif defined MBED_CONF_MBED_CLIENT_RECONNECTION_INTERVAL
#define MBED_CLIENT_RECONNECTION_INTERVAL MBED_CONF_MBED_CLIENT_RECONNECTION_INTERVAL
#else
#define MBED_CLIENT_RECONNECTION_INTERVAL 5
#endif

#ifdef YOTTA_CFG_TCP_KEEPALIVE_TIME
#define MBED_CLIENT_TCP_KEEPALIVE_TIME YOTTA_CFG_TCP_KEEPALIVE_TIME
#elif defined MBED_CONF_MBED_CLIENT_TCP_KEEPALIVE_TIME
#define MBED_CLIENT_TCP_KEEPALIVE_TIME MBED_CONF_MBED_CLIENT_TCP_KEEPALIVE_TIME
#else
#define MBED_CLIENT_TCP_KEEPALIVE_TIME 300
#endif

#ifdef YOTTA_CFG_DISABLE_BOOTSTRAP_FEATURE
#define MBED_CLIENT_DISABLE_BOOTSTRAP_FEATURE YOTTA_CFG_DISABLE_BOOTSTRAP_FEATURE
#elif defined MBED_CONF_MBED_CLIENT_DISABLE_BOOTSTRAP_FEATURE
#define MBED_CLIENT_DISABLE_BOOTSTRAP_FEATURE MBED_CONF_MBED_CLIENT_DISABLE_BOOTSTRAP_FEATURE
#endif

#if defined (__ICCARM__)
#define m2m_deprecated
#else
#define m2m_deprecated __attribute__ ((deprecated))
#endif

// This is valid for mbed-client-mbedtls
// For other SSL implementation there
// can be other

/*
*\brief A callback function for Random number
* required by the mbed-client-mbedtls module.
*/
typedef uint32_t (*random_number_cb)(void) ;

/*
*\brief An entropy structure for mbedtls entropy source.
* \param entropy_source_ptr Entropy function.
* \param p_source  Function data.
* \param threshold Minimum required from source before entropy is released
*                  (with mbedtls_entropy_func()) (in bytes).
* \param strong    MBEDTLS_ENTROPY_SOURCE_STRONG = 1 or
*                  MBEDTSL_ENTROPY_SOURCE_WEAK = 0.
*                  At least one strong source needs to be added.
*                  Weaker sources (such as the cycle counter) can be used as
*                  a complement.
*/
typedef struct mbedtls_entropy {
    int     (*entropy_source_ptr)(void *, unsigned char *,size_t , size_t *);
    void    *p_source;
    size_t  threshold;
    int     strong;
}entropy_cb;


#endif // M2MCONFIG_H
