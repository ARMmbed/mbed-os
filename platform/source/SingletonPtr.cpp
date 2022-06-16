/*
* Copyright (c) 2022 ARM Limited. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*
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

#ifdef MBED_CONF_RTOS_PRESENT
#include "cmsis_os2.h"
#endif

#include "SingletonPtr.h"

#ifdef MBED_CONF_RTOS_PRESENT
extern osMutexId_t singleton_mutex_id;
#endif

void singleton_lock(void)
{
#ifdef MBED_CONF_RTOS_PRESENT
    if (!singleton_mutex_id) {
        // RTOS has not booted yet so no mutex is needed
        return;
    }
    osMutexAcquire(singleton_mutex_id, osWaitForever);
#endif
}

void singleton_unlock(void)
{
#ifdef MBED_CONF_RTOS_PRESENT
    if (!singleton_mutex_id) {
        // RTOS has not booted yet so no mutex is needed
        return;
    }
    osMutexRelease(singleton_mutex_id);
#endif
}