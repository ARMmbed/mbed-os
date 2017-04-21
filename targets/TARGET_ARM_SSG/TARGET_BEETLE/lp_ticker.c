/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cmsis.h"
#include "device.h"
#include "objects.h"
#include "lp_ticker_api.h"

/* Private lp_ticker data */
/* lp_ticker initialize */
static uint32_t lp_ticker_initialized = 0;
/* lp_ticker reload value */
static uint32_t lp_ticker_reload = 0x0; /* Max Value */
/* Store Overflow Delta */
static uint32_t lp_ticker_overflows_delta = 0;
/* lp_ticker Overflow limit */
static uint32_t lp_ticker_overflow_limit = 0;

#if DEVICE_LOWPOWERTIMER
/**
 * Interrupt Handler
 */
void __lp_ticker_irq_handler(void)
{
    if (DualTimer_GetIRQInfo(DUALTIMER0) == SINGLETIMER2) {
        DualTimer_ClearInterrupt(DUALTIMER0);
        /*
         * For each overflow event adds the timer max represented value to
         * the delta. This allows the lp_ticker to keep track of the elapsed
         * time:
         * elapsed_time = (num_overflow * overflow_limit) + current_time
         */
        lp_ticker_overflows_delta += lp_ticker_overflow_limit;
    } else {
        lp_ticker_irq_handler();
    }
}

/**
 * Initialize the low power ticker
 */
void lp_ticker_init(void)
{
    uint32_t lp_ticker_irqn = 0;
    /* Verify if lp_ticker has been already Initialized */
    if (lp_ticker_initialized == 1)
    {
        return;
    }
    lp_ticker_initialized = 1;

    /* Dualtimer Initialize */
    DualTimer_Initialize(DUALTIMER0, lp_ticker_reload);
    /* Dualtimer Enable */
    DualTimer_Enable(DUALTIMER0, DUALTIMER_COUNT_32
            //| DUALTIMER_PERIODIC
            );
    /* DualTimer get IRQn */
    lp_ticker_irqn = DualTimer_GetIRQn(DUALTIMER0);
    /* Enable lp_ticker IRQ */
    NVIC_SetVector((IRQn_Type)lp_ticker_irqn,
                (uint32_t)__lp_ticker_irq_handler);
    NVIC_EnableIRQ((IRQn_Type)lp_ticker_irqn);

    /* DualTimer set interrupt on SINGLETIMER2 */
    DualTimer_SetInterrupt_2(DUALTIMER0, DUALTIMER_DEFAULT_RELOAD,
                DUALTIMER_COUNT_32 | DUALTIMER_PERIODIC);

    /*
     * Set lp_ticker Overflow limit. The lp_ticker overflow limit is required
     * to calculated the return value of the lp_ticker read function in us
     * on 32bit.
     * A 32bit us value cannot be represented directly in the Dual Timer Load
     * register if it is greater than (0xFFFFFFFF ticks)/DUALTIMER_DIVIDER_US.
     */
    lp_ticker_overflow_limit = DualTimer_GetReloadValue(DUALTIMER0,
                SINGLETIMER2);
}

/**
 * Read the current counter
 * @return: The current timer's counter value in microseconds
 */
uint32_t lp_ticker_read(void)
{
    uint32_t microseconds = 0;

    /* Verify if lp_ticker has not been Initialized */
    if (lp_ticker_initialized == 0)
        lp_ticker_init();

    /* Read Timer Value */
    microseconds = lp_ticker_overflows_delta + DualTimer_Read_2(DUALTIMER0);

    return microseconds;
}

/**
 * Set interrupt for specified timestamp
 * timestamp: The time in microseconds to be set
 */
void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    int32_t delta = 0;

    /* Verify if lp_ticker has been not Initialized */
    if (lp_ticker_initialized == 0)
        lp_ticker_init();

    /* Calculate the delta */
    delta = (int32_t)(timestamp - lp_ticker_read());
    /* Check if the event was in the past */
    if (delta <= 0) {
        /* This event was in the past */
        DualTimer_SetInterrupt_1(DUALTIMER0, 0,
                DUALTIMER_COUNT_32 | DUALTIMER_ONESHOT);
        return;
    }

    /* Enable interrupt on SingleTimer1 */
    DualTimer_SetInterrupt_1(DUALTIMER0, delta,
            DUALTIMER_COUNT_32 | DUALTIMER_ONESHOT);
}

/**
 * Disable low power ticker interrupt
 */
void lp_ticker_disable_interrupt(void)
{
    /* Disable Interrupt */
    DualTimer_DisableInterrupt(DUALTIMER0,
                               SINGLETIMER1);
}

/**
 * Clear the low power ticker interrupt
 */
void lp_ticker_clear_interrupt(void)
{
    /* Clear Interrupt */
    DualTimer_ClearInterrupt(DUALTIMER0);
}

#endif
