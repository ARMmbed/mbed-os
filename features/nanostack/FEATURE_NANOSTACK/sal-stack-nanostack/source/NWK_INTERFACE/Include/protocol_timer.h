/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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

#ifndef PROTOCOL_TIMER_H_
#define PROTOCOL_TIMER_H_

typedef enum {
    PROTOCOL_TIMER_MLE_TIM_UPDATE = 0,
    PROTOCOL_TIMER_STACK_TIM,
    PROTOCOL_TIMER_TCP_TIM,
    PROTOCOL_TIMER_MULTICAST_TIM,
    PROTOCOL_TIMER_BOOTSTRAP_TIM,
    PROTOCOL_TIMER_MAX
} protocol_timer_id_t;

typedef struct {
    void (*fptr)(uint16_t);
    uint16_t ticks;
    uint16_t orderedTime;
    uint16_t time_drifts;
} protocol_timer_t;

extern int protocol_timer_init(void);
extern void protocol_timer_event_lock_free(void);
extern void protocol_timer_cb(uint16_t ticks);
extern void protocol_timer_interrupt(int8_t timer_id, uint16_t slots);
extern void protocol_timer_start(protocol_timer_id_t id, void (*passed_fptr)(uint16_t), uint32_t time_ms);
extern void protocol_timer_stop(protocol_timer_id_t id);
extern void protocol_timer_sleep_balance(uint32_t time_in_ms);

#endif /* PROTOCOL_TIMER_H_ */
