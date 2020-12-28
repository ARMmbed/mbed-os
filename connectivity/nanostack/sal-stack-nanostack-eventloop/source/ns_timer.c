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

#include "ns_types.h"
#include "ns_list.h"
#include "ns_timer.h"
#include "eventOS_callback_timer.h"
#include "platform/arm_hal_interrupt.h"
#include "platform/arm_hal_timer.h"
#include "nsdynmemLIB.h"

#ifndef NS_EXCLUDE_HIGHRES_TIMER
typedef enum ns_timer_state_e {
    NS_TIMER_ACTIVE = 0,        // Will run on the next HAL interrupt
    NS_TIMER_HOLD,              // Will run on a later HAL interrupt
    NS_TIMER_RUN_INTERRUPT,     // Running on the interrupt we're currently handling
    NS_TIMER_STOP               // Timer not scheduled ("start" not called since last callback)
} ns_timer_state_e;

typedef struct ns_timer_struct {
    int8_t ns_timer_id;
    ns_timer_state_e timer_state;
    uint16_t slots;
    uint16_t remaining_slots;
    void (*interrupt_handler)(int8_t, uint16_t);
    ns_list_link_t link;
} ns_timer_struct;

static NS_LIST_DEFINE(ns_timer_list, ns_timer_struct, link);

#define NS_TIMER_RUNNING    1
static uint8_t ns_timer_state = 0;

#ifdef ATMEGA256RFR2
#define COMPENSATION 3
#define COMPENSATION_TUNE 1
#else
#define COMPENSATION 0
#define COMPENSATION_TUNE 0
#endif

static void ns_timer_interrupt_handler(void);
static ns_timer_struct *ns_timer_get_pointer_to_timer_struct(int8_t timer_id);
static bool ns_timer_initialized = 0;

int8_t eventOS_callback_timer_register(void (*timer_interrupt_handler)(int8_t, uint16_t))
{
    int8_t retval = -1;

    if (!ns_timer_initialized) {
        /*Set interrupt handler in HAL driver*/
        platform_timer_set_cb(ns_timer_interrupt_handler);
        ns_timer_initialized = 1;
    }

    /*Find first free timer ID in timer list*/
    /*(Note use of uint8_t to avoid overflow if we reach 0x7F)*/
    for (uint8_t i = 0; i <= INT8_MAX; i++) {
        if (!ns_timer_get_pointer_to_timer_struct(i)) {
            retval = i;
            break;
        }
    }

    if (retval == -1) {
        return -1;
    }

    ns_timer_struct *new_timer = ns_dyn_mem_alloc(sizeof(ns_timer_struct));
    if (!new_timer) {
        return -1;
    }

    /*Initialise new timer*/
    new_timer->ns_timer_id = retval;
    new_timer->timer_state = NS_TIMER_STOP;
    new_timer->remaining_slots = 0;
    new_timer->interrupt_handler = timer_interrupt_handler;

    // Critical section sufficient as long as list can't be reordered from
    // interrupt, otherwise will need to cover whole routine
    platform_enter_critical();
    ns_list_add_to_end(&ns_timer_list, new_timer);
    platform_exit_critical();

    /*Return timer ID*/
    return retval;
}

int8_t eventOS_callback_timer_unregister(int8_t ns_timer_id)
{
    ns_timer_struct *current_timer;

    current_timer = ns_timer_get_pointer_to_timer_struct(ns_timer_id);
    if (!current_timer) {
        return -1;
    }

    // Critical section sufficient as long as list can't be reordered from
    // interrupt, otherwise will need to cover whole routine
    platform_enter_critical();
    ns_list_remove(&ns_timer_list, current_timer);
    platform_exit_critical();

    ns_dyn_mem_free(current_timer);
    return 0;
}


static int8_t ns_timer_start_pl_timer(uint16_t pl_timer_start_slots)
{
    /*Don't start timer with 0 slots*/
    if (!pl_timer_start_slots) {
        pl_timer_start_slots = 1;
    }

    /*Start HAL timer*/
    platform_timer_start(pl_timer_start_slots);
    /*Set HAL timer state to running*/
    ns_timer_state |= NS_TIMER_RUNNING;
    return 0;
}

int8_t ns_timer_sleep(void)
{
    int8_t ret_val = -1;
    if (ns_timer_state & NS_TIMER_RUNNING) {
        /*Start HAL timer*/
        platform_timer_disable();
        /*Set HAL timer state to running*/
        ns_timer_state &= ~NS_TIMER_RUNNING;
        ret_val = 0;
    }
    return ret_val;
}

static int8_t ns_timer_get_next_running_to(void)
{
    uint8_t hold_count = 0;
    ns_timer_struct *first_timer = NULL;

    /*Find hold-labelled timer with the least remaining slots*/
    ns_list_foreach(ns_timer_struct, current_timer, &ns_timer_list) {
        if (current_timer->timer_state == NS_TIMER_HOLD) {
            if (!first_timer || current_timer->remaining_slots < first_timer->remaining_slots) {
                first_timer = current_timer;
            }
            /*For optimisation, count the found timers*/
            hold_count++;
        }
    }

    if (!first_timer) {
        return 0;
    }

    /*If hold-labelled timer found, set it active and start the HAL driver*/
    hold_count--;
    first_timer->timer_state = NS_TIMER_ACTIVE;
    /*Compensate time spent in timer function*/
    if (first_timer->remaining_slots > COMPENSATION) {
        first_timer->remaining_slots -= COMPENSATION;
    }
    /*Start HAL timer*/
    ns_timer_start_pl_timer(first_timer->remaining_slots);

    /*Update other hold-labelled timers*/
    ns_list_foreach(ns_timer_struct, current_timer, &ns_timer_list) {
        if (hold_count == 0) { // early termination optimisation
            break;
        }
        if (current_timer->timer_state == NS_TIMER_HOLD) {
            if (current_timer->remaining_slots == first_timer->remaining_slots) {
                current_timer->timer_state = NS_TIMER_ACTIVE;
            } else {
                current_timer->remaining_slots -= first_timer->remaining_slots;
                /*Compensate time spent in timer function*/
                if (current_timer->remaining_slots > COMPENSATION) {
                    current_timer->remaining_slots -= COMPENSATION;
                }
            }
            hold_count--;
        }
    }

    return 0;
}


static ns_timer_struct *ns_timer_get_pointer_to_timer_struct(int8_t timer_id)
{
    /*Find timer with the given ID*/
    ns_list_foreach(ns_timer_struct, current_timer, &ns_timer_list) {
        if (current_timer->ns_timer_id == timer_id) {
            return current_timer;
        }
    }
    return NULL;
}

int8_t eventOS_callback_timer_start(int8_t ns_timer_id, uint16_t slots)
{
    int8_t ret_val = 0;
    uint16_t pl_timer_remaining_slots;
    ns_timer_struct *timer;
    platform_enter_critical();

    /*Find timer to be activated*/
    timer = ns_timer_get_pointer_to_timer_struct(ns_timer_id);
    if (!timer) {
        ret_val = -1;
        goto exit;
    }

    // XXX this assumes the timer currently isn't running?
    // Is event.c relying on this restarting HAL timer after ns_timer_sleep()?

    /*If any timers are active*/
    if (ns_timer_state & NS_TIMER_RUNNING) {
        /*Get remaining slots of the currently activated timeout*/
        pl_timer_remaining_slots = platform_timer_get_remaining_slots();

        /*New timeout is shorter than currently enabled timeout*/
        if (pl_timer_remaining_slots > slots) {
            /*Start HAL timer*/
            ns_timer_start_pl_timer(slots - 0);

            ns_list_foreach(ns_timer_struct, current_timer, &ns_timer_list) {
                /*Switch active timers to hold*/
                if (current_timer->timer_state == NS_TIMER_ACTIVE) {
                    current_timer->timer_state = NS_TIMER_HOLD;
                    current_timer->remaining_slots = 0;
                }
                /*Update hold-labelled timers*/
                if (current_timer->timer_state == NS_TIMER_HOLD) {
                    current_timer->remaining_slots += (pl_timer_remaining_slots - slots);
                    /*Compensate time spent in timer function*/
                    if (current_timer->remaining_slots > (COMPENSATION - COMPENSATION_TUNE)) {
                        current_timer->remaining_slots -= (COMPENSATION - COMPENSATION_TUNE);
                    }
                }
            }
            /*Mark active and start the timer*/
            timer->timer_state = NS_TIMER_ACTIVE;
            timer->slots = slots;
            timer->remaining_slots = slots;
        }

        /*New timeout is longer than currently enabled timeout*/
        else if (pl_timer_remaining_slots < slots) {
            /*Mark hold and update remaining slots*/
            timer->timer_state = NS_TIMER_HOLD;
            timer->slots = slots;
            timer->remaining_slots = (slots - pl_timer_remaining_slots);
        }
        /*New timeout is equal to currently enabled timeout*/
        else {
            /*Mark it active and it will be handled in next interrupt*/
            timer->timer_state = NS_TIMER_ACTIVE;
            timer->slots = slots;
            timer->remaining_slots = slots;
        }
    } else {
        /*No timers running*/
        timer->timer_state = NS_TIMER_HOLD;
        timer->slots = slots;
        timer->remaining_slots = slots;
        /*Start next timeout*/
        ns_timer_get_next_running_to();
    }
exit:
    platform_exit_critical();
    return ret_val;
}

static void ns_timer_interrupt_handler(void)
{
    uint8_t i = 0;

    platform_enter_critical();
    /*Clear timer running state*/
    ns_timer_state &= ~NS_TIMER_RUNNING;
    /*Mark active timers as NS_TIMER_RUN_INTERRUPT, interrupt functions are called at the end of this function*/
    ns_list_foreach(ns_timer_struct, current_timer, &ns_timer_list) {
        if (current_timer->timer_state == NS_TIMER_ACTIVE) {
            current_timer->timer_state = NS_TIMER_RUN_INTERRUPT;
            /*For optimisation, count the found timers*/
            i++;
        }
    }

    /*Start next timeout*/
    ns_timer_get_next_running_to();

    /*Call interrupt functions*/
    ns_list_foreach(ns_timer_struct, current_timer, &ns_timer_list) {
        if (i == 0) {
            break;
        }
        if (current_timer->timer_state == NS_TIMER_RUN_INTERRUPT) {
            current_timer->timer_state = NS_TIMER_STOP;
            current_timer->interrupt_handler(current_timer->ns_timer_id, current_timer->slots);
            i--;
        }
    }

    platform_exit_critical();
}

int8_t eventOS_callback_timer_stop(int8_t ns_timer_id)
{
    uint16_t pl_timer_remaining_slots;
    bool active_timer_found = false;
    ns_timer_struct *current_timer;
    ns_timer_struct *first_timer = NULL;
    int8_t retval = -1;

    platform_enter_critical();
    /*Find timer with given timer ID*/
    current_timer = ns_timer_get_pointer_to_timer_struct(ns_timer_id);
    if (!current_timer) {
        goto exit;
    }

    retval = 0;

    /*Check if already stopped*/
    if (current_timer->timer_state == NS_TIMER_STOP) {
        goto exit;
    }

    current_timer->timer_state = NS_TIMER_STOP;
    current_timer->remaining_slots = 0;

    /*Check if some timer is already active*/
    ns_list_foreach(ns_timer_struct, curr_timer, &ns_timer_list) {
        if (curr_timer->timer_state == NS_TIMER_ACTIVE) {
            active_timer_found = true;
            break;
        }
    }
    /*If no active timers found, start one*/
    if (!active_timer_found) {
        pl_timer_remaining_slots = platform_timer_get_remaining_slots();
        /*Find hold-labelled timer with the least remaining slots*/
        ns_list_foreach(ns_timer_struct, cur_timer, &ns_timer_list) {
            if (cur_timer->timer_state == NS_TIMER_HOLD) {
                cur_timer->remaining_slots += pl_timer_remaining_slots;

                if (!first_timer || cur_timer->remaining_slots < first_timer->remaining_slots) {
                    first_timer = cur_timer;
                }
            }
        }
        /*If hold-labelled timer found, set it active and start the HAL driver*/
        if (first_timer) {
            first_timer->timer_state = NS_TIMER_ACTIVE;
            /*Start HAL timer*/
            ns_timer_start_pl_timer(first_timer->remaining_slots);
            /*If some of the other hold-labelled timers have the same remaining slots as the timer_tmp, mark them active*/
            ns_list_foreach(ns_timer_struct, cur_timer, &ns_timer_list) {
                if (cur_timer->timer_state == NS_TIMER_HOLD) {
                    if (cur_timer->remaining_slots == first_timer->remaining_slots) {
                        cur_timer->timer_state = NS_TIMER_ACTIVE;
                    } else {
                        cur_timer->remaining_slots -= first_timer->remaining_slots;
                    }
                }
            }
        }
    }

exit:
    platform_exit_critical();

    return retval;
}
#endif // NS_EXCLUDE_HIGHRES_TIMER
