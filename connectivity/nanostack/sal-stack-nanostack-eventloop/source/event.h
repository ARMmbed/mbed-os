/*
 * Copyright (c) 2017 ARM Limited. All rights reserved.
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
#ifndef NS_EVENT_H_
#define NS_EVENT_H_

#ifdef __cplusplus
extern "C" {
#endif


bool event_tasklet_handler_id_valid(uint8_t tasklet_id);
void eventOS_event_send_timer_allocated(arm_event_storage_t *event);

// This requires lock to be held
arm_event_storage_t *eventOS_event_find_by_id_critical(uint8_t tasklet_id, uint8_t event_id);

#ifdef __cplusplus
}
#endif

#endif /*NS_EVENT_H_*/
