/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2016, STMicroelectronics
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

#include "cmsis.h"
#include "us_ticker_api.h"
#include "sleep_api.h"
#include "rtc_api_hal.h"
#include "hal_tick.h"
#include "mbed_critical.h"

extern void HAL_SuspendTick(void);
extern void HAL_ResumeTick(void);

void hal_sleep(void)
{
    // Disable IRQs
    core_util_critical_section_enter();

    // Stop HAL tick to avoid to exit sleep in 1ms
    HAL_SuspendTick();
    // Request to enter SLEEP mode
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
    // Restart HAL tick
    HAL_ResumeTick();

    // Enable IRQs
    core_util_critical_section_exit();
}

void hal_deepsleep(void)
{
    // Disable IRQs
    core_util_critical_section_enter();

    // Stop HAL tick
    HAL_SuspendTick();
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

    // Restart HAL tick
    HAL_ResumeTick();

    // Enable IRQs
    core_util_critical_section_exit();

    // After wake-up from STOP reconfigure the PLL
    SetSysClock();

    TIM_HandleTypeDef TimMasterHandle;
    TimMasterHandle.Instance = TIM_MST;
    __HAL_TIM_SET_COUNTER(&TimMasterHandle, EnterTimeUS);

#if DEVICE_LOWPOWERTIMER
    rtc_synchronize();
#endif
}

#endif
