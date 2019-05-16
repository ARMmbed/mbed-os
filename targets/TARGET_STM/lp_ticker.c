/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2018, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */

#if DEVICE_LPTICKER

/***********************************************************************/
/* lpticker_lptim config is 1 in json config file                      */
/* LPTICKER is based on LPTIM feature from ST drivers. RTC is not used */
#if MBED_CONF_TARGET_LPTICKER_LPTIM

#include "lp_ticker_api.h"
#include "mbed_error.h"

#if !defined(LPTICKER_DELAY_TICKS) || (LPTICKER_DELAY_TICKS < 3)
#warning "lpticker_delay_ticks value should be set to 3"
#endif

LPTIM_HandleTypeDef LptimHandle;

const ticker_info_t *lp_ticker_get_info()
{
    static const ticker_info_t info = {
#if MBED_CONF_TARGET_LSE_AVAILABLE
        LSE_VALUE / MBED_CONF_TARGET_LPTICKER_LPTIM_CLOCK,
#else
        LSI_VALUE / MBED_CONF_TARGET_LPTICKER_LPTIM_CLOCK,
#endif
        16
    };
    return &info;
}

volatile uint8_t  lp_Fired = 0;

static int LPTICKER_inited = 0;

static void LPTIM1_IRQHandler(void);
static void (*irq_handler)(void);

void lp_ticker_init(void)
{
    /* Check if LPTIM is already configured */
    if (LPTICKER_inited) {
        lp_ticker_disable_interrupt();
        return;
    }
    LPTICKER_inited = 1;

    RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct = {0};
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

#if MBED_CONF_TARGET_LSE_AVAILABLE

    /* Enable LSE clock */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

    /* Select the LSE clock as LPTIM peripheral clock */
    RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
#if (TARGET_STM32L0)
    RCC_PeriphCLKInitStruct.LptimClockSelection = RCC_LPTIM1CLKSOURCE_LSE;
#else
    RCC_PeriphCLKInitStruct.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSE;
#endif

#else /* MBED_CONF_TARGET_LSE_AVAILABLE */

    /* Enable LSI clock */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

    /* Select the LSI clock as LPTIM peripheral clock */
    RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
#if (TARGET_STM32L0)
    RCC_PeriphCLKInitStruct.LptimClockSelection = RCC_LPTIM1CLKSOURCE_LSI;
#else
    RCC_PeriphCLKInitStruct.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSI;
#endif

#endif /* MBED_CONF_TARGET_LSE_AVAILABLE */

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        error("HAL_RCC_OscConfig ERROR\n");
        return;
    }

    if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct) != HAL_OK) {
        error("HAL_RCCEx_PeriphCLKConfig ERROR\n");
        return;
    }

    __HAL_RCC_LPTIM1_CLK_ENABLE();
    __HAL_RCC_LPTIM1_FORCE_RESET();
    __HAL_RCC_LPTIM1_RELEASE_RESET();

    /* Initialize the LPTIM peripheral */
    LptimHandle.Instance = LPTIM1;
    LptimHandle.State = HAL_LPTIM_STATE_RESET;
    LptimHandle.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
#if defined(MBED_CONF_TARGET_LPTICKER_LPTIM_CLOCK)
#if (MBED_CONF_TARGET_LPTICKER_LPTIM_CLOCK == 4)
    LptimHandle.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV4;
#elif (MBED_CONF_TARGET_LPTICKER_LPTIM_CLOCK == 2)
    LptimHandle.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV2;
#else
    LptimHandle.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
#endif
#else
    LptimHandle.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
#endif /* MBED_CONF_TARGET_LPTICKER_LPTIM_CLOCK */

    LptimHandle.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
#if defined (LPTIM_ACTIVEEDGE_FALLING)
    LptimHandle.Init.Trigger.ActiveEdge = LPTIM_ACTIVEEDGE_FALLING;
#endif
#if defined (LPTIM_TRIGSAMPLETIME_DIRECTTRANSITION)
    LptimHandle.Init.Trigger.SampleTime = LPTIM_TRIGSAMPLETIME_DIRECTTRANSITION;
#endif

    LptimHandle.Init.OutputPolarity = LPTIM_OUTPUTPOLARITY_HIGH;
    LptimHandle.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
    LptimHandle.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
#if defined (LPTIM_INPUT1SOURCE_GPIO) /* STM32L4 */
    LptimHandle.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
    LptimHandle.Init.Input2Source = LPTIM_INPUT2SOURCE_GPIO;
#endif /* LPTIM_INPUT1SOURCE_GPIO */

    if (HAL_LPTIM_Init(&LptimHandle) != HAL_OK) {
        error("HAL_LPTIM_Init ERROR\n");
        return;
    }

    NVIC_SetVector(LPTIM1_IRQn, (uint32_t)LPTIM1_IRQHandler);

#if defined (__HAL_LPTIM_WAKEUPTIMER_EXTI_ENABLE_IT)
    /* EXTI lines are not configured by default */
    __HAL_LPTIM_WAKEUPTIMER_EXTI_ENABLE_IT();
    __HAL_LPTIM_WAKEUPTIMER_EXTI_ENABLE_RISING_EDGE();
#endif

    __HAL_LPTIM_ENABLE_IT(&LptimHandle, LPTIM_IT_CMPM);
    HAL_LPTIM_Counter_Start(&LptimHandle, 0xFFFF);

    /* Need to write a compare value in order to get LPTIM_FLAG_CMPOK in set_interrupt */
    __HAL_LPTIM_CLEAR_FLAG(&LptimHandle, LPTIM_FLAG_CMPOK);
    __HAL_LPTIM_COMPARE_SET(&LptimHandle, 0);
    while (__HAL_LPTIM_GET_FLAG(&LptimHandle, LPTIM_FLAG_CMPOK) == RESET) {
    }
}

static void LPTIM1_IRQHandler(void)
{
    LptimHandle.Instance = LPTIM1;

    if (lp_Fired) {
        lp_Fired = 0;
        if (irq_handler) {
            irq_handler();
        }
    }

    /* Compare match interrupt */
    if (__HAL_LPTIM_GET_FLAG(&LptimHandle, LPTIM_FLAG_CMPM) != RESET) {
        if (__HAL_LPTIM_GET_IT_SOURCE(&LptimHandle, LPTIM_IT_CMPM) != RESET) {
            /* Clear Compare match flag */
            __HAL_LPTIM_CLEAR_FLAG(&LptimHandle, LPTIM_FLAG_CMPM);

            if (irq_handler) {
                irq_handler();
            }
        }
    }

#if defined (__HAL_LPTIM_WAKEUPTIMER_EXTI_CLEAR_FLAG)
    /* EXTI lines are not configured by default */
    __HAL_LPTIM_WAKEUPTIMER_EXTI_CLEAR_FLAG();
#endif
}

uint32_t lp_ticker_read(void)
{
    uint32_t lp_time = LPTIM1->CNT;
    /* Reading the LPTIM_CNT register may return unreliable values.
    It is necessary to perform two consecutive read accesses and verify that the two returned values are identical */
    while (lp_time != LPTIM1->CNT) {
        lp_time = LPTIM1->CNT;
    }
    return lp_time;
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    LptimHandle.Instance = LPTIM1;
    irq_handler = (void (*)(void))lp_ticker_irq_handler;

    /* CMPOK is set by hardware to inform application that the APB bus write operation to the LPTIM_CMP register has been successfully completed */
    /* Any successive write before the CMPOK flag be set, will lead to unpredictable results */
    /* LPTICKER_DELAY_TICKS value prevents OS to call this set interrupt function before CMPOK */
    MBED_ASSERT(__HAL_LPTIM_GET_FLAG(&LptimHandle, LPTIM_FLAG_CMPOK) == SET);
    __HAL_LPTIM_CLEAR_FLAG(&LptimHandle, LPTIM_FLAG_CMPOK);
    __HAL_LPTIM_COMPARE_SET(&LptimHandle, timestamp);

    lp_ticker_clear_interrupt();

    NVIC_EnableIRQ(LPTIM1_IRQn);
}

void lp_ticker_fire_interrupt(void)
{
    lp_Fired = 1;
    irq_handler = (void (*)(void))lp_ticker_irq_handler;
    NVIC_SetPendingIRQ(LPTIM1_IRQn);
    NVIC_EnableIRQ(LPTIM1_IRQn);
}

void lp_ticker_disable_interrupt(void)
{
    NVIC_DisableIRQ(LPTIM1_IRQn);
    LptimHandle.Instance = LPTIM1;
}

void lp_ticker_clear_interrupt(void)
{
    LptimHandle.Instance = LPTIM1;
    __HAL_LPTIM_CLEAR_FLAG(&LptimHandle, LPTIM_FLAG_CMPM);
    NVIC_ClearPendingIRQ(LPTIM1_IRQn);
}

void lp_ticker_free(void)
{
    lp_ticker_disable_interrupt();
}

/*****************************************************************/
/* lpticker_lptim config is 0 or not defined in json config file */
/* LPTICKER is based on RTC wake up feature from ST drivers      */
#else /* MBED_CONF_TARGET_LPTICKER_LPTIM */

#include "rtc_api_hal.h"

const ticker_info_t *lp_ticker_get_info()
{
    static const ticker_info_t info = {
        RTC_CLOCK / 4, // RTC_WAKEUPCLOCK_RTCCLK_DIV4
        32
    };
    return &info;
}

void lp_ticker_init(void)
{
    rtc_init();
    lp_ticker_disable_interrupt();
}

uint32_t lp_ticker_read(void)
{
    return rtc_read_lp();
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    rtc_set_wake_up_timer(timestamp);
}

void lp_ticker_fire_interrupt(void)
{
    rtc_fire_interrupt();
}

void lp_ticker_disable_interrupt(void)
{
    rtc_deactivate_wake_up_timer();
}

void lp_ticker_clear_interrupt(void)
{
    lp_ticker_disable_interrupt();
}

void lp_ticker_free(void)
{
    lp_ticker_disable_interrupt();
}

#endif /* MBED_CONF_TARGET_LPTICKER_LPTIM */

#endif /* DEVICE_LPTICKER */
