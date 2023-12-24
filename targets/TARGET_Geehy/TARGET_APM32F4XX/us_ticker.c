/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
 * Copyright (c) 2023 Geehy Semiconductor
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

#include "apm32f4xx.h"
#include "us_ticker_api.h"
#include "PeripheralNames.h"
#include "hal_tick.h"
#include "apm32f4xx_tmr.h"
#include "apm32f4xx_rcm.h"

#if TICKER_TIMER_WIDTH_BIT == 16
static uint32_t time_before;
static uint32_t total_elapsed_time;
#endif

/* this variable is set to 1 at the end of mbed_sdk_init function.
the ticker_read_us() function must not be called until the mbed_sdk_init is terminated */
extern int mbed_sdk_inited;
static uint32_t ticker_timer_cnt;
static uint32_t ticker_timer_ch0cv;
static uint32_t ticker_timer_dmainten;

void ticker_timer_init(void);
#if TICKER_TIMER_WIDTH_BIT == 16
void ticker_16bits_timer_init(void);
#else
void ticker_32bits_timer_init(void);
#endif
/* get TIMER clock */
static uint32_t read_timer_clock(uint32_t timer_periph);
uint32_t ticker_tick_get(void);
void ticker_timer_data_save(void);
void ticker_timer_data_save(void);
void ticker_timer_data_restore(void);

/*!
 * @brief     Ticker init.
 *
 * @param     None
 *
 * @retval    None
 */
void ticker_timer_init(void)
{
#if TICKER_TIMER_WIDTH_BIT == 16
    ticker_16bits_timer_init();
#else
    ticker_32bits_timer_init();
#endif
}

/*!
 * @brief     get tick
 *
 * @param     None
 *
 * @retval    the tick
 */
uint32_t ticker_tick_get(void)
{
#if TICKER_TIMER_WIDTH_BIT == 16
    uint32_t time_new;
    if (mbed_sdk_inited)
    {
    /* Apply the latest time recorded just before the sdk is inited */
        time_new = ticker_read_us(get_us_ticker_data()) + time_before;
        time_before = 0;
        return (time_new / 1000);
    }
    else
    {
    /* Prevent small values from subtracting large ones
    example:
            0x0010-0xFFEE=FFFF0022 , (0xFFFF-0xFFEE+0x10+1=0x22,1 mean CNT=0 tick)
            FFFF0022 & 0xFFFF = 0022
    */
        time_new = us_ticker_read();
        total_elapsed_time += (time_new - time_before) & 0xFFFF;
        time_before = time_new;
        return (total_elapsed_time / 1000);
    }
#else // 32-bit timer
    if (mbed_sdk_inited)
    {
        return (ticker_read_us(get_us_ticker_data()) / 1000);
    }
    else
    {
        return (us_ticker_read() / 1000);
    }
#endif
}

/*!
 * @brief     Get frequency and counter bits of this ticker.
 *
 * @param     None
 *
 * @retval    the ticker_info_t
 */
const ticker_info_t *us_ticker_get_info()
{
    static const ticker_info_t ticker_info = {
        1000000,
        TICKER_TIMER_WIDTH_BIT
    };
    return &ticker_info;
}

/* config for 32bits TIMER */
#if TICKER_TIMER_WIDTH_BIT == 16
/*!
 * @brief     config the interrupt handler
 *
 * @param     None
 *
 * @retval    None
 */
void ticker_timer_irq_handler(void)
{
    if (TMR_ReadIntFlag(TICKER_TIMER, TMR_INT_CC1) != RESET)
    {
        TMR_ClearIntFlag(TICKER_TIMER, TMR_INT_CC1);
        us_ticker_irq_handler();
    }
}

/*!
 * @brief     initialize the TIMER
 *
 * @param     None
 *
 * @retval    None
 */
void ticker_16bits_timer_init(void)
{
    TMR_BaseConfig_T TMR_BaseConfigstruct;
    uint32_t timer_clk = read_timer_clock((uint32_t)TICKER_TIMER);

    /* enable ticker timer clock */
    TICKER_TIMER_RCM_CLOCK_ENABLE;

    /* TICKER_TIMER configuration */
    TMR_BaseConfigstruct.division          = (uint32_t)(timer_clk / 1000000) - 1;
    TMR_BaseConfigstruct.countMode         = TMR_COUNTER_MODE_UP;
    TMR_BaseConfigstruct.period            = 0xFFFF;
    TMR_BaseConfigstruct.clockDivision     = TMR_CLOCK_DIV_1;
    TMR_BaseConfigstruct.repetitionCounter = 0;
    TMR_ConfigTimeBase(TICKER_TIMER, &TMR_BaseConfigstruct);

    /* auto-reload preload disable */
    TMR_DisableAutoReload(TICKER_TIMER);

    /* configure TIMER channel enable state */
    TMR_EnableCCxChannel(TICKER_TIMER, TMR_CHANNEL_1);

    /* configure TIMER primary output function */
    TMR_EnablePWMOutputs(TICKER_TIMER);

    /* Output compare channel 0 interrupt for mbed timeout */
    NVIC_SetVector(TICKER_TIMER_IRQ, (uint32_t)ticker_timer_irq_handler);
    NVIC_EnableIRQ(TICKER_TIMER_IRQ);
    TMR_Enable(TICKER_TIMER);

    /* if define the FREEZE_TIMER_ON_DEBUG macro in mbed_app.json or other file,
       hold the TICKER_TIMER counter for debug when core halted
    */
#if !defined(NDEBUG) && defined(FREEZE_TIMER_ON_DEBUG) && defined(TICKER_TIMER_DEBUG_STOP)
    TICKER_TIMER_DEBUG_STOP;
#endif

    TMR_DisableInterrupt(TICKER_TIMER, TMR_INT_CC1);

    /* used by ticker_tick_get() */
    time_before = 0;
    total_elapsed_time = 0;
}
/* config for 32bits TIMER */
#else
/*!
 * @brief     config the interrupt handler
 *
 * @param     None
 *
 * @retval    None
 */
void ticker_timer_irq_handler(void)
{
    if (TMR_ReadIntFlag(TICKER_TIMER, TMR_INT_CC1) != RESET)
    {
        TMR_ClearIntFlag(TICKER_TIMER, TMR_INT_CC1);
        us_ticker_irq_handler();
    }
}

/*!
 * @brief     initialize the TIMER
 *
 * @param     None
 *
 * @retval    None
 */
void ticker_32bits_timer_init(void)
{
    TMR_BaseConfig_T TMR_BaseConfigstruct;
    uint32_t timer_clk = read_timer_clock((uint32_t)TICKER_TIMER);

    /* enable ticker timer clock */
    TICKER_TIMER_RCM_CLOCK_ENABLE;

    /* TICKER_TIMER configuration */
    TMR_BaseConfigstruct.division          = (uint32_t)(timer_clk / 1000000) - 1;
    TMR_BaseConfigstruct.countMode         = TMR_COUNTER_MODE_UP;
    TMR_BaseConfigstruct.period            = 0xFFFFFFFF;
    TMR_BaseConfigstruct.clockDivision     = TMR_CLOCK_DIV_1;
    TMR_BaseConfigstruct.repetitionCounter = 0;
    TMR_ConfigTimeBase(TICKER_TIMER, &TMR_BaseConfigstruct);

    /* auto-reload preload disable */
    TMR_DisableAutoReload(TICKER_TIMER);

    /* configure TIMER channel enable state */
    TMR_EnableCCxChannel(TICKER_TIMER, TMR_CHANNEL_1);

    /* configure TIMER primary output function */
    TMR_EnablePWMOutputs(TICKER_TIMER);

    /* Output compare channel 0 interrupt for mbed timeout */
    NVIC_SetVector(TICKER_TIMER_IRQ, (uint32_t)ticker_timer_irq_handler);
    NVIC_EnableIRQ(TICKER_TIMER_IRQ);

    TMR_Enable(TICKER_TIMER);

    /* if define the FREEZE_TIMER_ON_DEBUG macro in mbed_app.json or other file,
       hold the TICKER_TIMER counter for debug when core halted
    */
#if !defined(NDEBUG) && defined(FREEZE_TIMER_ON_DEBUG) && defined(TICKER_TIMER_DEBUG_STOP)
    TICKER_TIMER_DEBUG_STOP;
#endif

    TMR_DisableInterrupt(TICKER_TIMER, TMR_INT_CC1);
}

#endif /* 16-bit/32-bit timer init */

/*!
 * @brief     Initialize the ticker
 * Initialize or re-initialize the ticker. This resets all the
 * clocking and prescaler registers, along with disabling
 * the compare interrupt.
 *
 * @param     None
 *
 * @retval    None
 */
void us_ticker_init(void)
{
    TMR_DisableInterrupt(TICKER_TIMER, TMR_INT_CC1);
    /* configure TIMER channel enable state */
    TMR_EnableCCxChannel(TICKER_TIMER, TMR_CHANNEL_1);

    /* configure TIMER primary output function */
    TMR_EnablePWMOutputs(TICKER_TIMER);

    TMR_Enable(TICKER_TIMER);
}

/*!
 * @brief     Read the current counter
 * Read the current counter value without performing frequency conversions.
 * If no rollover has occurred, the seconds passed since us_ticker_init()
 * was called can be found by dividing the ticks returned by this function
 * by the frequency returned by ::us_ticker_get_info.
 *
 * @param     None
 *
 * @retval    The current timer's counter value in ticks
 */
uint32_t us_ticker_read()
{
    /* read TMR counter value */
    uint32_t count_value = 0U;
    count_value = TMR_ReadCounter(TICKER_TIMER);
    return (count_value);
}

/*!
 * @brief     Set interrupt for specified timestamp
 * Read the current counter value without performing frequency conversions.
 * If no rollover has occurred, the seconds passed since us_ticker_init()
 * was called can be found by dividing the ticks returned by this function
 * by the frequency returned by ::us_ticker_get_info.
 *
 * @param     timestamp The time in ticks to be set
 *
 * @note no special handling needs to be done for times in the past
 * as the common timer code will detect this and call
 * us_ticker_fire_interrupt() if this is the case
 *
 * @note calling this function with timestamp of more than the supported
 * number of bits returned by ::us_ticker_get_info results in undefined
 * behavior.
 *
 * @retval    None
 */
void us_ticker_set_interrupt(timestamp_t timestamp)
{
    /* configure TMR channel output pulse value.Only set this value when you interrupt disabled */
    TMR_ConfigCompare1(TICKER_TIMER, (uint32_t)timestamp);
    /* clear TIMER interrupt flag,enable the TIMER interrupt */
    TMR_ClearIntFlag(TICKER_TIMER, TMR_INT_CC1);
    TMR_EnableInterrupt(TICKER_TIMER, TMR_INT_CC1);
}

/*!
 * @brief     Set pending interrupt that should be fired right away.
 * The ticker should be initialized prior calling this function.
 *
 * @param     None
 *
 * @retval    None
 */
void us_ticker_fire_interrupt(void)
{
    /* clear TMR interrupt flag */
    TMR_ClearIntFlag(TICKER_TIMER, TMR_INT_CC1);
    /* channel 0 capture or compare event generation immediately,so CH0IF set for interrupt */
    TMR_GenerateEvent(TICKER_TIMER, TMR_EVENT_CH1);
    /* enable the TIMER interrupt */
    TMR_EnableInterrupt(TICKER_TIMER, TMR_INT_CC1);
}

/*!
 * @brief     Disable us ticker interrupt
 *
 * @param     None
 *
 * @retval    None
 */
void us_ticker_disable_interrupt(void)
{
    TMR_DisableInterrupt(TICKER_TIMER, TMR_INT_CC1);
}

/*!
 * @brief     Clear us ticker interrupt
 *
 * @note      must be called with interrupts disabled function
 *
 * @param     None
 *
 * @retval    None
 */
void us_ticker_clear_interrupt(void)
{
    TMR_ClearIntFlag(TICKER_TIMER, TMR_INT_CC1);
}

/*!
 * @brief     save ticker TIMER data when MCU go to deepsleep
 *
 * @param     None
 *
 * @retval    None
 */
void ticker_timer_data_save(void)
{
    ticker_timer_cnt = TMR_ReadCounter(TICKER_TIMER);
    ticker_timer_ch0cv = TMR_ReadCaputer1(TICKER_TIMER);
    ticker_timer_dmainten = TICKER_TIMER->DIEN;
}

/*!
 * @brief     restore ticker TIMER data when MCU go out deepsleep
 *
 * @param     None
 *
 * @retval    None
 */
void ticker_timer_data_restore(void)
{
    TMR_ConfigCounter(TICKER_TIMER, ticker_timer_cnt);
    TMR_ConfigCompare1(TICKER_TIMER, ticker_timer_ch0cv);
    TMR_EnableInterrupt(TICKER_TIMER, ticker_timer_dmainten);
}

/*!
 * @brief     Deinitialize the us ticker
 *
 * @note      This function stops the ticker from counting.
 *
 * @param     None
 *
 * @retval    None
 */
void us_ticker_free(void)
{
    /* configure TIMER channel enable state */
    TMR_DisableCCxChannel(TICKER_TIMER, TMR_CHANNEL_1);
    /* configure TIMER primary output function */
    TMR_DisablePWMOutputs(TICKER_TIMER);
    /* disable a TIMER */
    TMR_Disable(TICKER_TIMER);

    us_ticker_disable_interrupt();
}

/*!
 * @brief     Get TIMER clock
 *
 * @param     timer_periph  TIMER device information structrue
 *
 * @retval    TIMER clock
 */
static uint32_t read_timer_clock(uint32_t timer_periph)
{
    uint32_t timerclock;
    uint32_t NOGET = 0;
    uint32_t GET = 1;

    if (((uint32_t)TMR1 == timer_periph) || ((uint32_t)TMR8 == timer_periph) || \
         ((uint32_t)TMR9 == timer_periph) || ((uint32_t)TMR10 == timer_periph) || \
         ((uint32_t)TMR11 == timer_periph))
    {
        /* get the current APB2 TIMER clock source */
        if (RCM_APB_DIV_1 == (RCM->CFG_B.APB2PSC))
        {
            RCM_ReadPCLKFreq(&NOGET, &GET);
            timerclock = GET;
        } else
        {
            RCM_ReadPCLKFreq(&NOGET, &GET);
            timerclock = GET * 2;
        }
    }
    else
    {
        /* get the current APB1 TIMER clock source */
        if (RCM_APB_DIV_1 == (RCM->CFG_B.APB1PSC))
        {
            RCM_ReadPCLKFreq(&GET, &NOGET);
            timerclock = GET;
        }
        else
        {
            RCM_ReadPCLKFreq(&GET, &NOGET);
            timerclock = GET * 2;
        }
    }

    return timerclock;
}

