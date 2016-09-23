/** @file cfstore_os.h
 *
 * mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
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
 *
 * Header file for OS platform specific code.
 */
#ifndef __CFSTORE_OS_H
#define __CFSTORE_OS_H

#include "cmsis_os.h"
#include "configuration_store.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * portable simple locking types
 */
#ifdef __MBED__

typedef struct cfstore_os_mutex_t {
    osMutexId _osMutexId;
    osMutexDef_t _osMutexDef;
    int32_t _mutex_data[4];
} cfstore_os_mutex_t;


typedef struct cfstore_os_semaphore_t {
    osSemaphoreId id;
    uint32_t os_semaphore_cb[2];
    osSemaphoreDef_t os_semaphore_def;
} cfstore_os_semaphore_t;


#else /* TARGET_LIKE_X86_LINUX_NATIVE */

typedef struct cfstore_os_mutex_t {
    uint32_t count;
} cfstore_os_mutex_t;

typedef struct cfstore_os_semaphore_t {
    uint32_t count;
} cfstore_os_semaphore_t;

#endif /* __MBED__ */

void cfstore_os_mutex_init(cfstore_os_mutex_t* lock);
void cfstore_os_mutex_lock(cfstore_os_mutex_t* lock);
void cfstore_os_mutex_unlock(cfstore_os_mutex_t* lock);
void cfstore_os_semaphore_init(cfstore_os_semaphore_t *sem);
void cfstore_os_semaphore_lock(cfstore_os_semaphore_t *sem);
void cfstore_os_semaphore_unlock(cfstore_os_semaphore_t *sem);


int32_t cfstore_os_init(void);
extern ARM_CFSTORE_DRIVER cfstore_os_mbed_driver;


#ifdef __cplusplus
}
#endif

#endif /* __CFSTORE_OS_H */
