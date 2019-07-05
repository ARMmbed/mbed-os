/* mbed Microcontroller Library
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
 */
#include "cmsis.h"

int mbed_sdk_inited = 0;
extern void SetSysClock(void);

// This function is called after RAM initialization and before main.
void mbed_sdk_init()
{
#if defined(__ICACHE_PRESENT) /* STM32F7 */
    // The mbed_sdk_init can be called either during cold boot or during
    // application boot after bootloader has been executed.
    // In case the bootloader has already enabled the cache,
    // is is needed to not enable it again.
    if ((SCB->CCR & (uint32_t)SCB_CCR_IC_Msk) == 0) { // If ICache is disabled
        SCB_EnableICache();
    }
    if ((SCB->CCR & (uint32_t)SCB_CCR_DC_Msk) == 0) { // If DCache is disabled
        SCB_EnableDCache();
    }
#endif /* __ICACHE_PRESENT */

    // Update the SystemCoreClock variable.
    SystemCoreClockUpdate();
    HAL_Init();

    /* Configure the System clock source, PLL Multiplier and Divider factors,
       AHB/APBx prescalers and Flash settings */
    SetSysClock();
    SystemCoreClockUpdate();

    /* Start LSI clock for RTC */
#if DEVICE_RTC
#if !MBED_CONF_TARGET_LSE_AVAILABLE
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

    if (__HAL_RCC_GET_RTC_SOURCE() != RCC_RTCCLKSOURCE_NO_CLK) {
        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
        RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE;
        RCC_OscInitStruct.LSIState       = RCC_LSI_ON;
        if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
            error("Init : cannot initialize LSI\n");
        }
    }
#endif /* ! MBED_CONF_TARGET_LSE_AVAILABLE */
#endif /* DEVICE_RTC */

    mbed_sdk_inited = 1;
}
