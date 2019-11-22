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
#include "objects.h"

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

#if defined(DUAL_CORE)
    /* HW semaphore Clock enable*/
    __HAL_RCC_HSEM_CLK_ENABLE();

#if defined(CORE_CM4)
    __HAL_RCC_FLASH_C2_ALLOCATE();

    /* Check wether CM4 boot in parallel with CM7. If CM4 was gated but CM7 trigger the CM4 boot. No need to wait for synchronization.
       otherwise wait for CM7, which is in charge of sytem clock configuration */
    if (!LL_RCC_IsCM4BootForced()) {
        /* CM4 boots at the same time than CM7. It is necessary to synchronize with CM7, by mean of HSEM, that CM7 finishes its initialization.  */

        /* Activate HSEM notification for Cortex-M4*/
        LL_HSEM_EnableIT_C2IER(HSEM, CFG_HW_STOP_MODE_MASK_SEMID);

        /*
        * Domain D2 goes to STOP mode (Cortex-M4 in deep-sleep) waiting for
        * Cortex-M7 to perform system initialization (system clock config,
        * external memory configuration.. )
        */

        /* Select the domain Power Down DeepSleep */
        LL_PWR_SetRegulModeDS(LL_PWR_REGU_DSMODE_MAIN);
        /* Keep DSTOP mode when D2 domain enters Deepsleep */
        LL_PWR_CPU_SetD2PowerMode(LL_PWR_CPU_MODE_D2STOP);
        LL_PWR_CPU2_SetD2PowerMode(LL_PWR_CPU2_MODE_D2STOP);
        /* Set SLEEPDEEP bit of Cortex System Control Register */
        LL_LPM_EnableDeepSleep();

        /* Ensure that all instructions done before entering STOP mode */
        __DSB();
        __ISB();
        /* Request Wait For Event */
        __WFE();

        /* Reset SLEEPDEEP bit of Cortex System Control Register,
        * the following LL API Clear SLEEPDEEP bit of Cortex
        * System Control Register
        */
        LL_LPM_EnableSleep();

        /* Clear HSEM flag */
        LL_HSEM_DisableIT_C2IER(HSEM, CFG_HW_STOP_MODE_MASK_SEMID);
        LL_HSEM_ClearFlag_C2ICR(HSEM, CFG_HW_STOP_MODE_MASK_SEMID);
    }

    // Update the SystemCoreClock variable.
    SystemCoreClockUpdate();
    HAL_Init();
#else
    /* CORE_M7 */
    // Update the SystemCoreClock variable.
    SystemCoreClockUpdate();
    HAL_Init();

    /* Configure the System clock source, PLL Multiplier and Divider factors,
       AHB/APBx prescalers and Flash settings */
    SetSysClock();
    SystemCoreClockUpdate();

    /* Check wether CM4 boot in parallel with CM7. If CM4 was gated but CM7 trigger the CM4 boot. No need to wait for synchronization.
       otherwise CM7 should wakeup CM4 when system clocks initialization is done.  */
    if (READ_BIT(SYSCFG->UR1, SYSCFG_UR1_BCM4)) {
        LL_HSEM_1StepLock(HSEM, CFG_HW_STOP_MODE_SEMID);
        /*Release HSEM in order to notify the CPU2(CM4)*/
        LL_HSEM_ReleaseLock(HSEM, CFG_HW_STOP_MODE_SEMID, 0);
    } else {
        LL_RCC_ForceCM4Boot();
    }
    /* wait until CPU2 wakes up from stop mode */
    while (LL_RCC_D2CK_IsReady() == 0);
#endif /* CORE_M4 */
#else /* Single core */
    // Update the SystemCoreClock variable.
    SystemCoreClockUpdate();
    HAL_Init();

    /* Configure the System clock source, PLL Multiplier and Divider factors,
       AHB/APBx prescalers and Flash settings */
    SetSysClock();
    SystemCoreClockUpdate();
#endif /* DUAL_CORE */

    /* Start LSI clock for RTC */
#if DEVICE_RTC
#if !MBED_CONF_TARGET_LSE_AVAILABLE
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

    if (__HAL_RCC_GET_RTC_SOURCE() != RCC_RTCCLKSOURCE_NO_CLK) {
#if TARGET_STM32WB
        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI1;
#else
        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
#endif
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
