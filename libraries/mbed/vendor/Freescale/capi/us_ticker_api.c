/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include <stddef.h>
#include "us_ticker_api.h"
#include "PeripheralNames.h"

/******************************************************************************
 * Timer for us timing.
 * 
 * Need to have 32bit resolution, we are using the PIT (2x32bit timers) for
 * that. All the other timers have only 16bit resolution.
 * Unfortunately, the PIT does not have a prescaler, therefore it ticks at the
 * bus clock of (24)MHz.
 * To keep 32bit resolution we are chaining the 2 32bit timers together dividing
 * the final result by 24.
 * NOTE: The PIT is a countdown timer.
 ******************************************************************************/
static int us_ticker_running = 0;

static void us_ticker_init(void) {
    us_ticker_running = 1;
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;   // Clock PIT
    PIT->MCR = 0;                       // Enable PIT

    // Timer 1
    PIT->CHANNEL[1].LDVAL = 0xFFFFFFFF;
    PIT->CHANNEL[1].TCTRL = PIT_TCTRL_CHN_MASK;    // Chain to timer 0, disable Interrupts
    PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK;   // Start timer 1

    // Timer 2
    PIT->CHANNEL[0].LDVAL = 0xFFFFFFFF;
    PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TEN_MASK;    // Start timer 0, disable interrupts
}

uint32_t us_ticker_read() {
    if (!us_ticker_running)
        us_ticker_init();

    /* To use LTMR64H and LTMR64L, timer 0 and timer 1 need to be chained.
     * To obtain the correct value, first read LTMR64H and then LTMR64L.
     * LTMR64H will have the value of CVAL1 at the time of the first access,
     * LTMR64L will have the value of CVAL0 at the time of the first access,
     * therefore the application does not need to worry about carry-over effects
     * of the running counter.
     */
    uint64_t ticks;
    ticks  = (uint64_t)PIT->LTMR64H << 32;
    ticks |= (uint64_t)PIT->LTMR64L;
    ticks  = (~ticks) / 24;
    return (uint32_t)(0xFFFFFFFF & ticks);
}


/******************************************************************************
 * Timer Event
 * 
 * It schedules interrupts at given (32bit)us interval of time.
 * It is implemented used the 16bit Low Power Timer that remains powered in all
 * power modes.
 ******************************************************************************/
static void lptmr_isr(void);
static void lptmr_irq_handler(void);
static void us_ticker_set_interrupt(unsigned int timestamp);

static ticker_event_handler event_handler = NULL;
void us_ticker_set_handler(ticker_event_handler handler) {
    event_handler = handler;
}

static uint32_t us_ticker_int_counter = 0;
static uint16_t us_ticker_int_remainder = 0;

static int lptmr_inited = 0;
void lptmr_init(void) {
    lptmr_inited = 1;
    
    /* Clock the timer */
    SIM->SCGC5 |= SIM_SCGC5_LPTMR_MASK;
    
    /* Reset */
    LPTMR0->CSR = 0;
    
    /* Set interrupt handler */
    NVIC_SetVector(LPTimer_IRQn, (uint32_t)lptmr_isr);
    NVIC_EnableIRQ(LPTimer_IRQn);
    
    /* Clock at (1)MHz -> (1)tick/us */
    LPTMR0->PSR = LPTMR_PSR_PCS(3);       // OSCERCLK -> 8MHz
    LPTMR0->PSR |= LPTMR_PSR_PRESCALE(2); // divide by 8
}

static void lptmr_set(unsigned short count) {
    /* Reset */
    LPTMR0->CSR = 0;
    
    /* Set the compare register */
    LPTMR0->CMR = count;
    
    /* Enable interrupt */
    LPTMR0->CSR |= LPTMR_CSR_TIE_MASK;
    
    /* Start the timer */
    LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;
}

static void lptmr_isr(void) {
    // write 1 to TCF to clear the LPT timer compare flag
    LPTMR0->CSR |= LPTMR_CSR_TCF_MASK;
    
    if (us_ticker_int_counter > 0) {
        lptmr_set(0xFFFF);
        us_ticker_int_counter--;
    
    } else {
        if (us_ticker_int_remainder > 0) {
            lptmr_set(us_ticker_int_remainder);
            us_ticker_int_remainder = 0;
        
        } else {
            // This function is going to disable the interrupts if there are
            // no other events in the queue
            lptmr_irq_handler();
        }
    }
}

static ticker_event_t *head = NULL;

static void lptmr_irq_handler(void) {
    /* Go through all the pending TimerEvents */
    while (1) {
        if (head == NULL) {
            // There are no more TimerEvents left, so disable matches.
            LPTMR0->CSR &= ~LPTMR_CSR_TIE_MASK;
            return;
        }
        
        if ((int)(head->timestamp - us_ticker_read()) <= 0) {
            // This event was in the past:
            //      point to the following one and execute its handler
            ticker_event_t *p = head;
            head = head->next;
            if (event_handler != NULL) {
                event_handler(p->id); // NOTE: the handler can set new events
            }
        } else {
            // This event and the following ones in the list are in the future:
            //      set it as next interrupt and return
            us_ticker_set_interrupt(head->timestamp);
            return;
        }
    }
}

static void us_ticker_set_interrupt(unsigned int timestamp) {
    if (!lptmr_inited)
         lptmr_init();
    
    int delta = (int)(timestamp - us_ticker_read());
    if (delta <= 0) {
        // This event was in the past:
        lptmr_irq_handler();
        return;
    }
    
    us_ticker_int_counter   = (uint32_t)(delta >> 16);
    us_ticker_int_remainder = (uint16_t)(0xFFFF & delta);
    if (us_ticker_int_counter > 0) {
        lptmr_set(0xFFFF);
        us_ticker_int_counter--;
    } else {
        lptmr_set(us_ticker_int_remainder);
        us_ticker_int_remainder = 0;
    }
}

void us_ticker_insert_event(ticker_event_t *obj, unsigned int timestamp, uint32_t id) {
    /* disable interrupts for the duration of the function */
    __disable_irq();
    
    // initialise our data
    obj->timestamp = timestamp;
    obj->id = id;
    
    /* Go through the list until we either reach the end, or find
       an element this should come before (which is possibly the
       head). */
    ticker_event_t *prev = NULL, *p = head;
    while (p != NULL) {
        /* check if we come before p */
        if ((int)(timestamp - p->timestamp) <= 0) {
            break;
        }
        /* go to the next element */
        prev = p;
        p = p->next;
    }
    /* if prev is NULL we're at the head */
    if (prev == NULL) {
        head = obj;
        us_ticker_set_interrupt(timestamp);
    } else {
        prev->next = obj;
    }
    /* if we're at the end p will be NULL, which is correct */
    obj->next = p;
    
    __enable_irq();
}

void us_ticker_remove_event(ticker_event_t *obj) {
    __disable_irq();
    
    // remove this object from the list
    if (head == obj) {
        // first in the list, so just drop me
        head = obj->next;
        if (obj->next != NULL) {
            us_ticker_set_interrupt(head->timestamp);
        }
    } else {
        // find the object before me, then drop me
        ticker_event_t* p = head;
        while (p != NULL) {
            if (p->next == obj) {
                p->next = obj->next;
                break;
            }
            p = p->next;
        }
    }
    
    __enable_irq();
}
