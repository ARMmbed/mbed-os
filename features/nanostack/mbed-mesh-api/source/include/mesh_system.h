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

#ifndef __INCLUDE_MESH_SYSTEM__
#define __INCLUDE_MESH_SYSTEM__
#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Event type for connecting
 */
#define APPL_EVENT_CONNECT 0x01

/*
 * \brief Send application connect event to receiver tasklet to
 * ensure that connection is made in right tasklet.
 */
void mesh_system_send_connect_event(uint8_t receiver);

/*
 * \brief Initialize mesh system.
 * Memory pool, timers, traces and support are initialized.
 */
void mesh_system_init(void);

#ifdef __cplusplus
}
#endif

#include "nanostack-event-loop/eventOS_scheduler.h"

#define nanostack_lock()            eventOS_scheduler_mutex_wait()
#define nanostack_unlock()          eventOS_scheduler_mutex_release()
#define nanostack_assert_locked()   //MBED_ASSERT(eventOS_scheduler_mutex_is_owner())

#endif /* __INCLUDE_MESH_SYSTEM__ */
