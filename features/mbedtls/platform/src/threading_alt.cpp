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

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_THREADING_C)

#include "mbedtls/threading.h"

#if defined(MBEDTLS_THREADING_ALT)
static void threading_mutex_init_mbed( mbedtls_threading_mutex_t *mutex )
{
    if( mutex == NULL )
        return;

    mutex->mutex = new rtos::Mutex();

    mutex->is_valid = true;
}

static void threading_mutex_free_mbed( mbedtls_threading_mutex_t *mutex )
{
    if( mutex == NULL || mutex->is_valid != true )
        return;

    delete mutex->mutex;

    mutex->is_valid = false;
}

static int threading_mutex_lock_mbed( mbedtls_threading_mutex_t *mutex )
{
    if( mutex == NULL || mutex->is_valid != true )
        return( MBEDTLS_ERR_THREADING_BAD_INPUT_DATA );

    if( mutex->mutex->lock() != osOK )
        return( MBEDTLS_ERR_THREADING_MUTEX_ERROR );

    return( 0 );
}

static int threading_mutex_unlock_mbed( mbedtls_threading_mutex_t *mutex )
{
    if( mutex == NULL || mutex->is_valid != true )
        return( MBEDTLS_ERR_THREADING_BAD_INPUT_DATA );

    if( mutex->mutex->unlock() != osOK )
        return( MBEDTLS_ERR_THREADING_MUTEX_ERROR );

    return( 0 );
}

void mbedtls_threading_set_mbed( void )
{
    mbedtls_threading_set_alt( threading_mutex_init_mbed,
            threading_mutex_free_mbed, threading_mutex_lock_mbed,
            threading_mutex_unlock_mbed );
}

#endif /* MBEDTLS_THREADING_ALT */

#endif /* MBEDTLS_THREADING_C */
