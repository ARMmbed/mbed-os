/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
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
#ifndef _PL_NANO_TIMER_SYS_H_
#define _PL_NANO_TIMER_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize system timer
 * */
extern void timer_sys_init(void);

extern uint32_t timer_get_runtime_ticks(void);
int8_t timer_sys_wakeup(void);
void timer_sys_disable(void);

/**
 * System Timer update and synch after sleep
 *
 * \param ticks Time in 10 ms resolution
 *
 * \return none
 *
 * */
void system_timer_tick_update(uint32_t ticks);

#ifdef __cplusplus
}
#endif

#endif /*_PL_NANO_TIMER_SYS_H_*/
