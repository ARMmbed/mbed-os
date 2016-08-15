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
#ifndef EVENTOS_CALLBACK_TIMER_H_
#define EVENTOS_CALLBACK_TIMER_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "ns_types.h"

extern int8_t eventOS_callback_timer_register(void (*timer_interrupt_handler)(int8_t, uint16_t));
extern int8_t eventOS_callback_timer_unregister(int8_t ns_timer_id);

extern int8_t eventOS_callback_timer_stop(int8_t ns_timer_id);
extern int8_t eventOS_callback_timer_start(int8_t ns_timer_id, uint16_t slots);
#ifdef __cplusplus
}
#endif

#endif /* EVENTOS_CALLBACK_TIMER_H_ */
