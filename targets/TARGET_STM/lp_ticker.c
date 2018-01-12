/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2017, STMicroelectronics
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

#if DEVICE_LOWPOWERTIMER

#include "rtc_api_hal.h"

#if MBED_CONF_TARGET_LOWPOWERTIMER_LPTIM

LPTIM_HandleTypeDef LptimHandle;

volatile uint32_t lp_SlaveCounter = 0;
volatile uint32_t lp_oc_int_part = 0;
volatile uint16_t lp_TickPeriod_us;
volatile uint8_t  lp_Fired = 0;

static void LPTIM1_IRQHandler(void);
static void (*irq_handler)(void);


void lp_ticker_init(void)
{
    /* Check if LPTIM is already configured */
#if (TARGET_STM32L0)
    if (READ_BIT(RCC->APB1ENR, RCC_APB1ENR_LPTIM1EN) != RESET) {
        return;
    }
#else
    if (__HAL_RCC_LPTIM1_IS_CLK_ENABLED()) {
        return;
    }
#endif

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

    /*  Prescaler impact:
            tick period = Prescaler division factor / LPTIM clock
            Example with LPTIM clock = 32768 Hz LSE
                Prescaler = LPTIM_PRESCALER_DIV1   => lp_TickPeriod_us = 31us  =>   2s with 16b timer
                Prescaler = LPTIM_PRESCALER_DIV2   => lp_TickPeriod_us = 61us  =>   4s with 16b timer
                Prescaler = LPTIM_PRESCALER_DIV4   => lp_TickPeriod_us = 122us =>   8s with 16b timer
                Prescaler = LPTIM_PRESCALER_DIV8   => lp_TickPeriod_us = 244us =>  16s with 16b timer
                Prescaler = LPTIM_PRESCALER_DIV16  => lp_TickPeriod_us = 488us =>  32s with 16b timer
                Prescaler = LPTIM_PRESCALER_DIV32  => lp_TickPeriod_us = 976us =>  64s with 16b timer
                Prescaler = LPTIM_PRESCALER_DIV64  => lp_TickPeriod_us = 1.9ms => 128s with 16b timer
                Prescaler = LPTIM_PRESCALER_DIV128 => lp_TickPeriod_us = 3.9ms => 256s with 16b timer
    */
    LptimHandle.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV2;
    lp_TickPeriod_us = 2 * 1000000 / RTC_CLOCK;

    LptimHandle.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
    LptimHandle.Init.OutputPolarity = LPTIM_OUTPUTPOLARITY_HIGH;
    LptimHandle.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
    LptimHandle.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
#if (TARGET_STM32L4)
    LptimHandle.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
    LptimHandle.Init.Input2Source = LPTIM_INPUT2SOURCE_GPIO;
#endif /* TARGET_STM32L4 */

    if (HAL_LPTIM_Init(&LptimHandle) != HAL_OK) {
        error("HAL_LPTIM_Init ERROR\n");
        return;
    }

    NVIC_SetVector(LPTIM1_IRQn, (uint32_t)LPTIM1_IRQHandler);
    NVIC_EnableIRQ(LPTIM1_IRQn);

#if !(TARGET_STM32L4)
    /* EXTI lines are not configured by default */
    __HAL_LPTIM_WAKEUPTIMER_EXTI_ENABLE_IT();
    __HAL_LPTIM_WAKEUPTIMER_EXTI_ENABLE_RISING_EDGE();
#endif

    __HAL_LPTIM_ENABLE_IT(&LptimHandle, LPTIM_IT_ARRM);
    __HAL_LPTIM_ENABLE_IT(&LptimHandle, LPTIM_IT_CMPM);
    __HAL_LPTIM_ENABLE_IT(&LptimHandle, LPTIM_IT_CMPOK);
    HAL_LPTIM_Counter_Start(&LptimHandle, 0xFFFF);
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

            if (lp_oc_int_part > 0) {
                lp_oc_int_part--;
            } else {
                if (irq_handler) {
                    irq_handler();
                }
            }
        }
    }

    /* Compare write interrupt */
    if (__HAL_LPTIM_GET_FLAG(&LptimHandle, LPTIM_FLAG_CMPOK) != RESET) {
        if (__HAL_LPTIM_GET_IT_SOURCE(&LptimHandle, LPTIM_IT_CMPOK) != RESET) {
            /* Clear Compare write flag */
            __HAL_LPTIM_CLEAR_FLAG(&LptimHandle, LPTIM_FLAG_CMPOK);
        }
    }

    /* Autoreload match interrupt */
    if (__HAL_LPTIM_GET_FLAG(&LptimHandle, LPTIM_FLAG_ARRM) != RESET) {
        if (__HAL_LPTIM_GET_IT_SOURCE(&LptimHandle, LPTIM_IT_ARRM) != RESET) {
            /* Clear Autoreload match flag */
            __HAL_LPTIM_CLEAR_FLAG(&LptimHandle, LPTIM_FLAG_ARRM);
            lp_SlaveCounter++;
        }
    }

#if !(TARGET_STM32L4)
    __HAL_LPTIM_WAKEUPTIMER_EXTI_CLEAR_FLAG();
#endif
}


uint32_t lp_ticker_read_TickCounter(void)
{
    uint16_t cntH_old, cntH, cntL;

    LptimHandle.Instance = LPTIM1;

    /* same algo as us_ticker_read in us_ticker_16b.c */
    do {
        cntH_old = lp_SlaveCounter;
        if (__HAL_LPTIM_GET_FLAG(&LptimHandle, LPTIM_FLAG_ARRM) == SET) {
            cntH_old += 1;
        }
        cntL = LPTIM1->CNT;
        cntH = lp_SlaveCounter;
        if (__HAL_LPTIM_GET_FLAG(&LptimHandle, LPTIM_FLAG_ARRM) == SET) {
            cntH += 1;
        }
    } while (cntH_old != cntH);
    uint32_t lp_time = (uint32_t)(cntH << 16 | cntL);
    return lp_time;
}

uint32_t lp_ticker_read(void)
{
    lp_ticker_init();
    return lp_ticker_read_TickCounter() * (uint32_t)lp_TickPeriod_us;
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    // Disable IRQs
    core_util_critical_section_enter();

    uint32_t timestamp_TickCounter = timestamp / (uint32_t)lp_TickPeriod_us;

    LptimHandle.Instance = LPTIM1;
    irq_handler = (void (*)(void))lp_ticker_irq_handler;

    __HAL_LPTIM_CLEAR_FLAG(&LptimHandle, LPTIM_FLAG_CMPOK);
    __HAL_LPTIM_CLEAR_FLAG(&LptimHandle, LPTIM_FLAG_CMPM);
    __HAL_LPTIM_COMPARE_SET(&LptimHandle, timestamp_TickCounter & 0xFFFF);

    /* CMPOK is set by hardware to inform application that the APB bus write operation to the LPTIM_CMP register has been successfully completed */
    while (__HAL_LPTIM_GET_FLAG(&LptimHandle, LPTIM_FLAG_CMPOK) == RESET) {
    }

    /* same algo as us_ticker_set_interrupt in us_ticker_16b.c */
    uint32_t current_time_TickCounter = lp_ticker_read_TickCounter();
    uint32_t delta = timestamp_TickCounter - current_time_TickCounter;
    lp_oc_int_part = (delta - 1) >> 16;
    if ( ((delta - 1) & 0xFFFF) >= 0x8000 &&
            __HAL_LPTIM_GET_FLAG(&LptimHandle, LPTIM_FLAG_CMPM) == SET ) {
        ++lp_oc_int_part;
    }

    // Enable IRQs
    core_util_critical_section_exit();
}

void lp_ticker_fire_interrupt(void)
{
    lp_Fired = 1;
    NVIC_SetPendingIRQ(LPTIM1_IRQn);
}

void lp_ticker_disable_interrupt(void)
{
    LptimHandle.Instance = LPTIM1;
    __HAL_LPTIM_DISABLE_IT(&LptimHandle, LPTIM_IT_CMPM);
}

void lp_ticker_clear_interrupt(void)
{
    LptimHandle.Instance = LPTIM1;
    __HAL_LPTIM_CLEAR_FLAG(&LptimHandle, LPTIM_FLAG_CMPM);
}

#else /* MBED_CONF_TARGET_LOWPOWERTIMER_LPTIM */

void lp_ticker_init(void)
{
    rtc_init();
}

uint32_t lp_ticker_read(void)
{
    uint32_t usecs = rtc_read_us();
    return usecs;
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t delta;

    delta = timestamp - lp_ticker_read();
    rtc_set_wake_up_timer(delta);
}

void lp_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(RTC_WKUP_IRQn);
}

void lp_ticker_disable_interrupt(void)
{
    rtc_deactivate_wake_up_timer();
}

void lp_ticker_clear_interrupt(void)
{
    NVIC_ClearPendingIRQ(RTC_WKUP_IRQn);
}

#endif /* MBED_CONF_TARGET_LOWPOWERTIMER_LPTIM */

#endif /* DEVICE_LOWPOWERTIMER */
