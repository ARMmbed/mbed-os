/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#ifndef MBEDTLS_THREADING_ALT_H
#define MBEDTLS_THREADING_ALT_H

#if defined(MBEDTLS_THREADING_ALT)

#ifdef __cplusplus
#include "Mutex.h"

typedef struct
{
    rtos::Mutex* mutex;
    char is_valid;
} mbedtls_threading_mutex_t;
#else
typedef struct
{
    void* mutex;
    char is_valid;
} mbedtls_threading_mutex_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

void mbedtls_threading_set_mbed( void );

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_THREADING_ALT */
#endif /* threading_alt.h */
