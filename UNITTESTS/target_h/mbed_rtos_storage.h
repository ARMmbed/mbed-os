/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#ifndef MBED_RTOS_STORAGE_H
#define MBED_RTOS_STORAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cmsis_os2.h"

typedef osMutexId_t mbed_rtos_storage_mutex_t;
typedef osSemaphoreId_t mbed_rtos_storage_semaphore_t;
typedef osThreadId_t mbed_rtos_storage_thread_t;
typedef osThreadId_t osThreadId;
typedef osMemoryPoolId_t mbed_rtos_storage_mem_pool_t;
typedef osMessageQueueId_t mbed_rtos_storage_msg_queue_t;
typedef osEventFlagsId_t mbed_rtos_storage_event_flags_t;
typedef void *mbed_rtos_storage_message_t;
typedef osTimerId_t mbed_rtos_storage_timer_t;
typedef osStatus_t osStatus;

#ifdef __cplusplus
}
#endif

#endif
