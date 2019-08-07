/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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
#ifndef PLATFORM_MUTEX_H
#define PLATFORM_MUTEX_H

#include "platform/NonCopyable.h"

/** \ingroup mbed-os-public */
/** \addtogroup platform-public-api */
/** @{*/

/** \defgroup platform_PlatformMutex PlatformMutex class
 * @{
 */

/** The PlatformMutex class is used to synchronize the execution of threads.
 *
 * Mbed drivers use the PlatformMutex class instead of rtos::Mutex.
 * This enables the use of drivers when the Mbed OS is compiled without the RTOS.
 *
 * @note
 * - When the RTOS is present, the PlatformMutex becomes a typedef for rtos::Mutex.
 * - When the RTOS is absent, all methods are defined as noop.
 */

#ifdef MBED_CONF_RTOS_API_PRESENT

// rtos::Mutex is itself a dummy class if the RTOS API is present, but not the RTOS
#include "rtos/Mutex.h"
typedef rtos::Mutex PlatformMutex;

#else

class PlatformMutex: private mbed::NonCopyable<PlatformMutex> {
public:
    /** Create a PlatformMutex object.
     *
     * @note When the RTOS is present, this is an alias for rtos::Mutex::Mutex().
     */
    PlatformMutex()
    {
    }

    /** PlatformMutex destructor.
     *
     * @note When the RTOS is present, this is an alias for rtos::Mutex::~Mutex().
     */
    ~PlatformMutex()
    {
    }

    /** Wait until a PlatformMutex becomes available.
     *
     * @note
     * - When the RTOS is present, this is an alias for rtos::Mutex::lock().
     * - When the RTOS is absent, this is a noop.
     */
    void lock()
    {
    }

    /** Unlock a PlatformMutex that the same thread has previously locked.
     *
     * @note
     * - When the RTOS is present, this is an alias for rtos::Mutex::unlock().
     * - When the RTOS is absent, this is a noop.
     */
    void unlock()
    {
    }
};

#endif

#endif

/**@}*/

/**@}*/
