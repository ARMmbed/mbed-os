/* mbed Microcontroller Library
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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

#ifndef MBED_EXT_TIMER_API_EXT_H
#define MBED_EXT_TIMER_API_EXT_H

#include "device.h"

typedef void (*gtimer_irq_handler)(uint32_t id);

typedef struct gtimer_s gtimer_t;
enum { 
    TIMER0 = 2,    // GTimer 2, share with PWM_3
    TIMER1 = 3,    // GTimer 3, share with PWM_0
    TIMER2 = 4,    // GTimer 4, share with PWM_1
    TIMER3 = 5,    // GTimer 5, share with PWM_2
    TIMER4 = 0,    // GTimer 0, share with software-RTC functions

    GTIMER_MAX = 5    
};

void gtimer_init (gtimer_t *obj, uint32_t tid);
void gtimer_deinit (gtimer_t *obj);
uint32_t gtimer_read_tick (gtimer_t *obj);
uint64_t gtimer_read_us (gtimer_t *obj);
void gtimer_reload (gtimer_t *obj, uint32_t duration_us);
void gtimer_start (gtimer_t *obj);
void gtimer_start_one_shout (gtimer_t *obj, uint32_t duration_us, void* handler, uint32_t hid);
void gtimer_start_periodical (gtimer_t *obj, uint32_t duration_us, void* handler, uint32_t hid);
void gtimer_stop (gtimer_t *obj);

#endif

