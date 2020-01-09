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
#include "us_ticker_data.h"
#include "mbed_critical.h"
#include "mbed_error.h"

extern void save_timer_ctx(void);
extern void restore_timer_ctx(void);
extern void SetSysClock(void);

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


static void ForcePeriphOutofDeepSleep(void)
{
    uint32_t pFLatency = 0;
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Get the Clocks configuration according to the internal RCC registers */
    HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &pFLatency);

#ifdef RCC_CLOCKTYPE_PCLK2
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                   | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
#else /* RCC_CLOCKTYPE_PCLK2 */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                   | RCC_CLOCKTYPE_PCLK1);
#endif /* RCC_CLOCKTYPE_PCLK2 */

#if defined (RCC_SYSCLKSOURCE_MSI) /* STM32Lx */
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_MSI;
#else /* defined RCC_SYSCLKSOURCE_MSI */
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
#endif /* defined RCC_SYSCLKSOURCE_MSI */

    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, pFLatency) != HAL_OK) {
        error("ForcePeriphOutofDeepSleep clock issue\r\n");
    }
}


static void ForceOscOutofDeepSleep(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* Get the Oscillators configuration according to the internal RCC registers */
    HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

#if defined (RCC_SYSCLKSOURCE_MSI) /* STM32Lx */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
#if defined RCC_MSIRANGE_11
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_11; // Highest freq, 48MHz range
#else
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6; // 4MHz range
#endif
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
#else /* defined RCC_SYSCLKSOURCE_MSI */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
#endif /* defined RCC_SYSCLKSOURCE_MSI */

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        error("ForceOscOutofDeepSleep clock issue\r\n");
    }

}


void hal_sleep(void)
{
    // Disable IRQs
    core_util_critical_section_enter();

    // Request to enter SLEEP mode
#if defined(PWR_CR1_LPR)
    // State Transitions (see 5.3 Low-power modes, Fig. 13):
    //  * (opt): Low Power Run (LPR) Mode -> Run Mode
    //  * Run Mode -> Sleep
    //  --- Wait for Interrupt --
    //  * Sleep -> Run Mode
    //  * (opt): Run Mode -> Low Power Run Mode

    // [5.4.1 Power control register 1 (PWR_CR1)]
    // LPR: When this bit is set, the regulator is switched from main mode (MR) to low-power mode (LPR).
    uint32_t lowPowerMode = LL_PWR_IsEnabledLowPowerRunMode();
    if (lowPowerMode) {
        HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
    } else {
        HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
    }
#elif defined(PWR_CR_LPDS) || defined(PWR_CR1_LPDS)
    uint32_t lowPowerMode = LL_PWR_GetRegulModeDS();
    if (lowPowerMode) {
        HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
    } else {
        HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
    }
#else
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
#endif

    // Enable IRQs
    core_util_critical_section_exit();
}

extern int serial_is_tx_ongoing(void);
extern int mbed_sdk_inited;

/*  Most of STM32 targets can have the same generic deep sleep
 *  function, but a few targets might need very specific sleep
 *  mode management, so this function is defined as WEAK.
 *  Check for alternative hal_deepsleep specific implementation
 *  in targets folders in case of doubt */
__WEAK void hal_deepsleep(void)
{
    /*  WORKAROUND:
     *  MBED serial driver does not handle deepsleep lock
     *  to prevent entering deepsleep until HW serial FIFO is empty.
     *  This is tracked in mbed issue 4408.
     *  For now, we're checking all Serial HW FIFO. If any transfer is ongoing
     *  we're not entering deep sleep and returning immediately. */
    if (serial_is_tx_ongoing()) {
        return;
    }

    // Disable IRQs
    core_util_critical_section_enter();

    save_timer_ctx();

    // Request to enter STOP mode with regulator in low power mode
#ifdef PWR_CR1_LPMS_STOP2 /* STM32L4 */
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
#elif defined(DUAL_CORE)
    int lowPowerModeEnabled = LL_PWR_GetRegulModeDS();

#if defined(CORE_CM7)
    HAL_PWREx_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI, PWR_D3_DOMAIN);
    HAL_PWREx_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI, PWR_D1_DOMAIN);

#elif defined(CORE_CM4)
    HAL_PWREx_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI, PWR_D3_DOMAIN);
    HAL_PWREx_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI, PWR_D2_DOMAIN);

#else
#error "Wrong Core selection"
#endif /* CORE_CM7 */

    if (lowPowerModeEnabled) {
        LL_PWR_SetRegulModeDS(lowPowerModeEnabled);
    }

#else /* PWR_CR1_LPMS_STOP2 */
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
#endif /* PWR_CR1_LPMS_STOP2 */

    /* Prevent HAL_GetTick() from using ticker_read_us() to read the
     * us_ticker timestamp until the us_ticker context is restored. */
    mbed_sdk_inited = 0;

    /* After wake-up from STOP reconfigure the PLL */
#if defined(DUAL_CORE)
    /* CFG_HW_STOP_MODE_SEMID is used to protect read access to STOP flag, and this avoid both core to configure clocks if both exit from stop at the same time */
    while (LL_HSEM_1StepLock(HSEM, CFG_HW_STOP_MODE_SEMID)) {
    }

    /* Clocks need to be reconfigured only if system has been in stop mode */
    if (LL_PWR_CPU_IsActiveFlag_STOP() && LL_PWR_CPU2_IsActiveFlag_STOP()) {
        /* We've seen unstable PLL CLK configuration when DEEP SLEEP exits just few µs after being entered
        *  So we need to force clock init out of Deep Sleep.
        *  This init has been split into 2 separate functions so that the involved structures are not allocated on the stack in parallel.
        *  This will reduce the maximum stack usage in case on non-optimized / debug compilers settings
        */
        while (LL_HSEM_1StepLock(HSEM, CFG_HW_RCC_SEMID)) {
        }
        ForceOscOutofDeepSleep();
        ForcePeriphOutofDeepSleep();
        SetSysClock();
        LL_HSEM_ReleaseLock(HSEM, CFG_HW_RCC_SEMID, HSEM_CR_COREID_CURRENT);
    }
#if defined(CORE_CM7)
    LL_PWR_ClearFlag_CPU();
#elif defined(CORE_CM4)
    LL_PWR_ClearFlag_CPU2();
#else
#error "Core not supported"
#endif

    LL_HSEM_ReleaseLock(HSEM, CFG_HW_STOP_MODE_SEMID, HSEM_CR_COREID_CURRENT);
#else
    /* We've seen unstable PLL CLK configuration when DEEP SLEEP exits just few µs after being entered
    *  So we need to force clock init out of Deep Sleep.
    *  This init has been split into 2 separate functions so that the involved structures are not allocated on the stack in parallel.
    *  This will reduce the maximum stack usage in case on non-optimized / debug compilers settings
    */
    ForceOscOutofDeepSleep();
    ForcePeriphOutofDeepSleep();
    SetSysClock();
#endif

    /*  Wait for clock to be stabilized.
     *  TO DO: a better way of doing this, would be to rely on
     *  HW Flag. At least this ensures proper operation out of
     *  deep sleep */
    wait_loop(500);


    restore_timer_ctx();

    /* us_ticker context restored, allow HAL_GetTick() to read the us_ticker
     * timestamp via ticker_read_us() again. */
    mbed_sdk_inited = 1;

    // Enable IRQs
    core_util_critical_section_exit();
}

#endif
