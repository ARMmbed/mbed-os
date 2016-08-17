/* mbed Microcontroller Library
 * Copyright (c) 2006-2012 ARM Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "rtos_idle.h"

#if DEVICE_LOWPOWERTIMER && DEVICE_SLEEP
#include "ticker_api.h"
#include "critical.h"
#include "lp_ticker_api.h"
#include "cmsis_os.h"
#include "sleep_api.h"

static ticker_event_t delay_event;
static volatile bool sleep_ended;
extern uint32_t os_clockrate;

void sleep_handler(uint32_t id)
{
    sleep_ended = true;
}
#endif /* DEVICE_LOWPOWERTIMER && DEVICE_SLEEP */

static void default_idle_hook(void)
{
#if DEVICE_LOWPOWERTIMER && DEVICE_SLEEP
    const ticker_data_t *lp_ticker_data = get_lp_ticker_data();
    uint32_t delay_ticks = os_suspend();
    uint32_t delay_us;
    sleep_ended = false;

    ticker_set_handler(lp_ticker_data, sleep_handler);

    ticker_remove_event(lp_ticker_data, &delay_event);
    delay_us = lp_ticker_read() + delay_ticks * os_clockrate;
    ticker_insert_event(lp_ticker_data, &delay_event, delay_us, (uint32_t)&delay_event);

    while(!sleep_ended) {
        sleep();
    }

    os_resume(delay_ticks);

#else /* DEVICE_LOWPOWERTIMER && DEVICE_SLEEP */
    /* Sleep: ideally, we should put the chip to sleep.
     Unfortunately, this usually requires disconnecting the interface chip (debugger).
     This can be done, but it would break the local file system.
    */
    // sleep();
#endif /* DEVICE_LOWPOWERTIMER && DEVICE_SLEEP */
}
static void (*idle_hook_fptr)(void) = &default_idle_hook;

void rtos_attach_idle_hook(void (*fptr)(void))
{
    //Attach the specified idle hook, or the default idle hook in case of a NULL pointer
    if (fptr != NULL) {
        idle_hook_fptr = fptr;
    } else {
        idle_hook_fptr = default_idle_hook;
    }
}

void rtos_idle_loop(void)
{
    //Continuously call the idle hook function pointer
    while (1) {
        idle_hook_fptr();
    }
}
