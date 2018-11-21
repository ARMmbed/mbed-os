/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
 *
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
#include "gd32f30x.h"
#include "us_ticker_api.h"
#include "PeripheralNames.h"
#include "hal_tick.h"

#if TICKER_TIMER_WIDTH_BIT == 16
uint32_t time_before;
uint32_t total_elapsed_time;
#endif

/* this variable is set to 1 at the end of mbed_sdk_init function.
the ticker_read_us() function must not be called until the mbed_sdk_init is terminated */
extern int mbed_sdk_inited;
uint32_t ticker_timer_cnt;
uint32_t ticker_timer_ch0cv;
uint32_t ticker_timer_dmainten;

void ticker_timer_init(void);
#if TICKER_TIMER_WIDTH_BIT == 16
void ticker_16bits_timer_init(void);
#else
void ticker_32bits_timer_init(void);
#endif
void ticker_timer_irq_handler(void);
/* get TIMER clock */
static uint32_t timer_get_clock(uint32_t timer_periph);
uint32_t ticker_tick_get(void);
void ticker_timer_data_save(void);
void ticker_timer_data_save(void);
void ticker_timer_data_restore(void);

void ticker_timer_init(void)
{
#if TICKER_TIMER_WIDTH_BIT == 16
    ticker_16bits_timer_init();
#else
    ticker_32bits_timer_init();
#endif
}

/** get tick
 *
 * @return the tick
 */
uint32_t ticker_tick_get(void)
{
#if TICKER_TIMER_WIDTH_BIT == 16
    uint32_t new_time;
    if (mbed_sdk_inited) {
        /* Apply the latest time recorded just before the sdk is inited */
        new_time = ticker_read_us(get_us_ticker_data()) + time_before;
        time_before = 0;
        return (new_time / 1000);
    } else {
        /* Prevent small values from subtracting large ones
        example:
                0x0010-0xFFEE=FFFF0022 , (0xFFFF-0xFFEE+0x10+1=0x22,1 mean CNT=0 tick)
                FFFF0022 & 0xFFFF = 0022
        */
        new_time = us_ticker_read();
        total_elapsed_time += (new_time - time_before) & 0xFFFF;
        time_before = new_time;
        return (total_elapsed_time / 1000);
    }
#else // 32-bit timer
    if (mbed_sdk_inited) {
        return (ticker_read_us(get_us_ticker_data()) / 1000);
    } else {
        return (us_ticker_read() / 1000);
    }
#endif
}

/** Get frequency and counter bits of this ticker.
 */
const ticker_info_t *us_ticker_get_info()
{
    static const ticker_info_t info = {
        1000000,
        TICKER_TIMER_WIDTH_BIT
    };
    return &info;
}



/* config for 32bits TIMER */
#if TICKER_TIMER_WIDTH_BIT == 16
/** config the interrupt handler
 */
void ticker_timer_irq_handler(void)
{
    if (SET == timer_interrupt_flag_get(TICKER_TIMER, TIMER_INT_FLAG_CH0)) {
        timer_interrupt_flag_clear(TICKER_TIMER, TIMER_INT_FLAG_CH0);
        us_ticker_irq_handler();
    }
}

/** initialize the TIMER
 */
void ticker_16bits_timer_init(void)
{
    timer_parameter_struct timer_initpara;
    uint32_t timer_clk = timer_get_clock(TICKER_TIMER);

    /* enable ticker timer clock */
    TICKER_TIMER_RCU_CLOCK_ENABLE;

    /* reset ticker timer peripheral */
    TICKER_TIMER_RESET_ENABLE;
    TICKER_TIMER_RESET_DISABLE;

    /* TICKER_TIMER configuration */
    timer_initpara.prescaler         = (uint32_t)(timer_clk / 1000000) - 1;;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 0xFFFF;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TICKER_TIMER, &timer_initpara);

    /* auto-reload preload disable */
    timer_auto_reload_shadow_disable(TICKER_TIMER);

    /* configure TIMER channel enable state */
    timer_channel_output_state_config(TICKER_TIMER, TIMER_CH_0, TIMER_CCX_ENABLE);

    /* configure TIMER primary output function */
    timer_primary_output_config(TICKER_TIMER, ENABLE);

    timer_enable(TICKER_TIMER);

    /* Output compare channel 0 interrupt for mbed timeout */
    NVIC_SetVector(TICKER_TIMER_IRQ, (uint32_t)ticker_timer_irq_handler);
    NVIC_EnableIRQ(TICKER_TIMER_IRQ);

    /* if define the FREEZE_TIMER_ON_DEBUG macro in mbed_app.json or other file,
       hold the TICKER_TIMER counter for debug when core halted
    */
#if !defined(NDEBUG) && defined(FREEZE_TIMER_ON_DEBUG) && defined(TICKER_TIMER_DEBUG_STOP)
    TICKER_TIMER_DEBUG_STOP;
#endif

    timer_interrupt_disable(TICKER_TIMER, TIMER_INT_CH0);

    /* used by ticker_tick_get() */
    time_before = 0;
    total_elapsed_time = 0;
}
/* config for 32bits TIMER */
#else
/** config the interrupt handler
 */
void ticker_timer_irq_handler(void)
{
    if (SET == timer_interrupt_flag_get(TICKER_TIMER, TIMER_INT_FLAG_CH0)) {
        timer_interrupt_flag_clear(TICKER_TIMER, TIMER_INT_FLAG_CH0);
        us_ticker_irq_handler();
    }
}

/** initialize the TIMER
 */
void ticker_32bits_timer_init(void)
{
    timer_parameter_struct timer_initpara;
    uint32_t timer_clk = timer_get_clock(TICKER_TIMER);

    /* enable ticker timer clock */
    TICKER_TIMER_RCU_CLOCK_ENABLE;

    /* reset ticker timer peripheral */
    TICKER_TIMER_RESET_ENABLE;
    TICKER_TIMER_RESET_DISABLE;

    /* TICKER_TIMER configuration */
    timer_initpara.prescaler         = (uint32_t)(timer_clk / 1000000) - 1;;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 0xFFFFFFFF;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TICKER_TIMER, &timer_initpara);

    /* auto-reload preload disable */
    timer_auto_reload_shadow_disable(TICKER_TIMER);

    /* configure TIMER channel enable state */
    timer_channel_output_state_config(TICKER_TIMER, TIMER_CH_0, TIMER_CCX_ENABLE);

    /* configure TIMER primary output function */
    timer_primary_output_config(TICKER_TIMER, ENABLE);

    timer_enable(TICKER_TIMER);

    /* Output compare channel 0 interrupt for mbed timeout */
    NVIC_SetVector(TICKER_TIMER_IRQ, (uint32_t)ticker_timer_irq_handler);
    NVIC_EnableIRQ(TICKER_TIMER_IRQ);

    /* if define the FREEZE_TIMER_ON_DEBUG macro in mbed_app.json or other file,
       hold the TICKER_TIMER counter for debug when core halted
    */
#if !defined(NDEBUG) && defined(FREEZE_TIMER_ON_DEBUG) && defined(TICKER_TIMER_DEBUG_STOP)
    TICKER_TIMER_DEBUG_STOP;
#endif

    timer_interrupt_disable(TICKER_TIMER, TIMER_INT_CH0);
}

#endif /* 16-bit/32-bit timer init */

/** Initialize the ticker
 *
 * Initialize or re-initialize the ticker. This resets all the
 * clocking and prescaler registers, along with disabling
 * the compare interrupt.
 *
 * @note Initialization properties tested by ::ticker_init_test
 */
void us_ticker_init(void)
{
    /* TIMER is already initialized in ticker_timer_init() */
    /* disable the TIMER interrupt */
    timer_interrupt_disable(TICKER_TIMER, TIMER_INT_CH0);
    /* configure TIMER channel enable state */
    timer_channel_output_state_config(TICKER_TIMER, TIMER_CH_0, TIMER_CCX_ENABLE);

    /* configure TIMER primary output function */
    timer_primary_output_config(TICKER_TIMER, ENABLE);

    timer_enable(TICKER_TIMER);

}

/** Read the current counter
 *
 * Read the current counter value without performing frequency conversions.
 * If no rollover has occurred, the seconds passed since us_ticker_init()
 * was called can be found by dividing the ticks returned by this function
 * by the frequency returned by ::us_ticker_get_info.
 *
 * @return The current timer's counter value in ticks
 */
uint32_t us_ticker_read()
{
    /* read TIMER counter value */
    uint32_t count_value = 0U;
    count_value = TIMER_CNT(TICKER_TIMER);
    return (count_value);
}

/** Set interrupt for specified timestamp
 *
 * @param timestamp The time in ticks to be set
 *
 * @note no special handling needs to be done for times in the past
 * as the common timer code will detect this and call
 * us_ticker_fire_interrupt() if this is the case
 *
 * @note calling this function with timestamp of more than the supported
 * number of bits returned by ::us_ticker_get_info results in undefined
 * behavior.
 */
void us_ticker_set_interrupt(timestamp_t timestamp)
{
    /* configure TIMER channel output pulse value.Only set this value when you interrupt disabled */
    timer_channel_output_pulse_value_config(TICKER_TIMER, TIMER_CH_0, (uint32_t)timestamp);
    /* clear TIMER interrupt flag,enable the TIMER interrupt */
    timer_interrupt_flag_clear(TICKER_TIMER, TIMER_INT_FLAG_CH0);
    timer_interrupt_enable(TICKER_TIMER, TIMER_INT_CH0);
}

/** Set pending interrupt that should be fired right away.
 *
 * The ticker should be initialized prior calling this function.
 */
void us_ticker_fire_interrupt(void)
{
    /* clear TIMER interrupt flag */
    timer_interrupt_flag_clear(TICKER_TIMER, TIMER_INT_FLAG_CH0);
    /* channel 0 capture or compare event generation immediately,so CH0IF set for interrupt */
    timer_event_software_generate(TICKER_TIMER, TIMER_EVENT_SRC_CH0G);
    /* enable the TIMER interrupt */
    timer_interrupt_enable(TICKER_TIMER, TIMER_INT_CH0);
}

/** Disable us ticker interrupt
 */
void us_ticker_disable_interrupt(void)
{
    /* disable the TIMER interrupt */
    timer_interrupt_disable(TICKER_TIMER, TIMER_INT_CH0);
}

/** Clear us ticker interrupt
 *  note: must be called with interrupts disabled function
 */
void us_ticker_clear_interrupt(void)
{
    /* clear TIMER interrupt flag */
    timer_interrupt_flag_clear(TICKER_TIMER, TIMER_INT_FLAG_CH0);
}

/** save ticker TIMER data when MCU go to deepsleep
*/
void ticker_timer_data_save(void)
{
    ticker_timer_cnt = TIMER_CNT(TICKER_TIMER);
    ticker_timer_ch0cv = TIMER_CH0CV(TICKER_TIMER);
    ticker_timer_dmainten = TIMER_DMAINTEN(TICKER_TIMER);
}

/** restore ticker TIMER data when MCU go out deepsleep
*/
void ticker_timer_data_restore(void)
{
    TIMER_CNT(TICKER_TIMER) = ticker_timer_cnt;
    TIMER_CH0CV(TICKER_TIMER) = ticker_timer_ch0cv;
    TIMER_DMAINTEN(TICKER_TIMER) = ticker_timer_dmainten;
}

/** Deinitialize the us ticker
 *
 * Powerdown the us ticker in preparation for sleep, powerdown, or reset.
 *
 * After this function is called, no other ticker functions should be called
 * except us_ticker_init(), calling any function other than init is undefined.
 *
 * @note This function stops the ticker from counting.
 */
void us_ticker_free(void)
{
    /* configure TIMER channel enable state */
    timer_channel_output_state_config(TICKER_TIMER, TIMER_CH_0, TIMER_CCX_DISABLE);
    /* configure TIMER primary output function */
    timer_primary_output_config(TICKER_TIMER, DISABLE);
    /* disable a TIMER */
    timer_disable(TICKER_TIMER);

    us_ticker_disable_interrupt();
}

/** get TIMER clock
 * @param timer_dev: TIMER device information structrue
                  the structure is not necessary to be reconfigured after structrue initialization,
                  the structure parameters altering is automatically configured by core
 * @return TIMER clock
*/
static uint32_t timer_get_clock(uint32_t timer_periph)
{
    uint32_t timerclk;

    if ((TIMER0 == timer_periph) || (TIMER7 == timer_periph) ||
            (TIMER8 == timer_periph) || (TIMER9 == timer_periph) || (TIMER10 == timer_periph)) {
        /* get the current APB2 TIMER clock source */
        if (RCU_APB2_CKAHB_DIV1 == (RCU_CFG0 & RCU_CFG0_APB2PSC)) {
            timerclk = rcu_clock_freq_get(CK_APB2);
        } else {
            timerclk = rcu_clock_freq_get(CK_APB2) * 2;
        }
    } else {
        /* get the current APB1 TIMER clock source */
        if (RCU_APB1_CKAHB_DIV1 == (RCU_CFG0 & RCU_CFG0_APB1PSC)) {
            timerclk = rcu_clock_freq_get(CK_APB1);
        } else {
            timerclk = rcu_clock_freq_get(CK_APB1) * 2;
        }
    }

    return timerclk;
}
