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
#if DEVICE_SLEEP

#include "sleep_api.h"
#include "us_ticker_api.h"
#include "hal_tick.h"
#include "mbed_critical.h"
#include "mbed_error.h"

extern void rtc_synchronize(void);

/*  Wait loop - assuming tick is 1 us */
static void wait_loop(uint32_t timeout)
{
    uint32_t t1, t2, elapsed = 0;
    t1 = us_ticker_read();
    do {
        t2 = us_ticker_read();
        elapsed = (t2 > t1) ? (t2 - t1) : ((uint64_t)t2 + 0xFFFFFFFF - t1 + 1);
    } while (elapsed < timeout);
    return;
}


// On L4 platforms we've seen unstable PLL CLK configuraiton
// when DEEP SLEEP exits just few µs after being entered
// So we need to force MSI usage before setting clocks again
static void ForcePeriphOutofDeepSleep(void)
{
    uint32_t pFLatency = 0;
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

#if (TARGET_STM32L4 || TARGET_STM32L1) /* MSI used for L4 */
    /* Get the Clocks configuration according to the internal RCC registers */
    HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &pFLatency);

    // Select HSI ss system clock source as a first step
#ifdef RCC_CLOCKTYPE_PCLK2
    RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
                                        | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
#else
    RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
                                        | RCC_CLOCKTYPE_PCLK1);
#endif
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_MSI;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, pFLatency) != HAL_OK) {
        error("clock issue\r\n");
    }
#else  /* HSI used on others */
    /* Get the Clocks configuration according to the internal RCC registers */
    HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &pFLatency);

    /**Initializes the CPU, AHB and APB busses clocks
    */
#ifdef RCC_CLOCKTYPE_PCLK2
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                   | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
#else
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                   | RCC_CLOCKTYPE_PCLK1);
#endif
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, pFLatency) != HAL_OK) {
        error("clock issue");
    }
#endif // TARGET_STM32L4
}

static void ForceOscOutofDeepSleep(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* Get the Oscillators configuration according to the internal RCC registers */
    HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

#if (TARGET_STM32L4 || TARGET_STM32L1) /* MSI used for L4 */
    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_4; // Intermediate freq, 1MHz range
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        error("clock issue\r\n");
    }
#else  /* HSI used on others */
    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        error("clock issue");
    }
#endif // TARGET_STM32L4
}

/* The content of this function has been split into 2 separate functions
   so that the involved structures are not allocated on the stack in parallel.
   This will reduce the maximum stack usage in case on non-optimized / debug
   compilers settings */
static void ForceClockOutofDeepSleep(void)
{
    ForceOscOutofDeepSleep();
    ForcePeriphOutofDeepSleep();
}

void hal_sleep(void)
{
    // Disable IRQs
    core_util_critical_section_enter();

    // Request to enter SLEEP mode
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

    // Enable IRQs
    core_util_critical_section_exit();
}

void hal_deepsleep(void)
{
    // Disable IRQs
    core_util_critical_section_enter();

    uint32_t EnterTimeUS = us_ticker_read();

    // Request to enter STOP mode with regulator in low power mode
#if TARGET_STM32L4
    int pwrClockEnabled = __HAL_RCC_PWR_IS_CLK_ENABLED();
    int lowPowerModeEnabled = PWR->CR1 & PWR_CR1_LPR;

    if (!pwrClockEnabled) {
        __HAL_RCC_PWR_CLK_ENABLE();
    }
    if (lowPowerModeEnabled) {
        HAL_PWREx_DisableLowPowerRunMode();
    }

    HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);

    if (lowPowerModeEnabled) {
        HAL_PWREx_EnableLowPowerRunMode();
    }
    if (!pwrClockEnabled) {
        __HAL_RCC_PWR_CLK_DISABLE();
    }
#else /* TARGET_STM32L4 */
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
#endif /* TARGET_STM32L4 */
    // Verify Clock Out of Deep Sleep
    ForceClockOutofDeepSleep();

    // After wake-up from STOP reconfigure the PLL
    SetSysClock();

    /*  Wait for clock to be stabilized.
     *  TO DO: a better way of doing this, would be to rely on
     *  HW Flag. At least this ensures proper operation out of
     *  deep sleep */
    wait_loop(500);

    TIM_HandleTypeDef TimMasterHandle;
    TimMasterHandle.Instance = TIM_MST;
    __HAL_TIM_SET_COUNTER(&TimMasterHandle, EnterTimeUS);

#if DEVICE_RTC
    /* Wait for RTC RSF bit synchro if RTC is configured */
#if (TARGET_STM32F2) || (TARGET_STM32F4) || (TARGET_STM32F7)
    if (READ_BIT(RCC->BDCR, RCC_BDCR_RTCSEL)) {
#else /* (TARGET_STM32F2) || (TARGET_STM32F4) || (TARGET_STM32F7) */
    if (__HAL_RCC_GET_RTC_SOURCE()) {
#endif  /* (TARGET_STM32F2) || (TARGET_STM32F4) || (TARGET_STM32F7) */
        rtc_synchronize();
    }
#endif
    // Enable IRQs
    core_util_critical_section_exit();
}

#endif
