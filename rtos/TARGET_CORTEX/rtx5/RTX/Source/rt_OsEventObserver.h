/*
 * Copyright (c) 2013-2016 ARM Limited. All rights reserved.
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
 *
 * -----------------------------------------------------------------------------
 *
 * Project:     CMSIS-RTOS RTX
 * Title:       OS Event Observer
 *
 * -----------------------------------------------------------------------------
 */
#ifndef _RT_OS_EVENT_OBSERVER_H
#define _RT_OS_EVENT_OBSERVER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t version;
    void (*pre_start)(void);
    void *(*thread_create)(int thread_id, void *context);
    void (*thread_destroy)(void *context);
    void (*thread_switch)(void *context);
} OsEventObserver;
extern const OsEventObserver *osEventObs;

void osRegisterForOsEvents(const OsEventObserver *observer);

#ifdef __cplusplus
};
#endif

#endif

/** @}*/
