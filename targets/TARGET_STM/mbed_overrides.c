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
#include "platform/mbed_error.h"

int mbed_sdk_inited = 0;
extern void SetSysClock(void);

#if defined(RCC_LSE_HIGHDRIVE_MODE) || defined(RCC_LSEDRIVE_HIGH)
#   define LSE_CONFIG_AVAILABLE
#endif

// set defaults for LSE drive load level
#if defined(LSE_CONFIG_AVAILABLE)

#   if defined(MBED_CONF_TARGET_LSE_DRIVE_LOAD_LEVEL)
#       define LSE_DRIVE_LOAD_LEVEL    MBED_CONF_TARGET_LSE_DRIVE_LOAD_LEVEL
#   else
#       if defined(RCC_LSE_HIGHDRIVE_MODE) // STM32F4
#           define LSE_DRIVE_LOAD_LEVEL    RCC_LSE_LOWPOWER_MODE
#       else
#           define LSE_DRIVE_LOAD_LEVEL    RCC_LSEDRIVE_LOW
#       endif
#   endif


/**
 * @brief configure the LSE crystal driver load
 * This settings ist target hardware dependend and 
 * depends on the crystal that is used for LSE clock.
 * For low power requirements, crystals with low load capacitors can be used and
 * driver setting is RCC_LSEDRIVE_LOW.
 * For higher stablity, crystals with higher load capacitys can be used and
 * driver setting is RCC_LSEDRIVE_HIGH.
 * 
 * A detailed description about this setting can be found here:
 * https://www.st.com/resource/en/application_note/cd00221665-oscillator-design-guide-for-stm8afals-stm32-mcus-and-mpus-stmicroelectronics.pdf
 * 
 * LSE maybe used later, but crystal load drive setting is necessary before 
 * enabling LSE.
 *   
 * @param None
 * @retval None
 */

static void LSEDriveConfig(void) {
    HAL_PWR_EnableBkUpAccess();
    #if defined(__HAL_RCC_LSEDRIVE_CONFIG)
        __HAL_RCC_LSEDRIVE_CONFIG(LSE_DRIVE_LOAD_LEVEL);
    #else
        HAL_RCCEx_SelectLSEMode(LSE_DRIVE_LOAD_LEVEL);
    #endif
}
#endif  // LSE_CONFIG_AVAILABLE

/**
 * @brief Setup the target board-specific configuration
 * of the microcontroller
 *
 * @note If used, this function should be implemented
 * elsewhere. This declaration is weak so it may be overridden
 * by user code.
 *
 * @param None
 * @retval None
 */
MBED_WEAK void TargetBSP_Init(void) {
    /** Do nothing */
}

#if MBED_CONF_TARGET_GPIO_RESET_AT_INIT
void GPIO_Full_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin        = GPIO_PIN_All;
    GPIO_InitStruct.Mode       = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Speed      = GPIO_SPEED_FREQ_LOW;
#if !TARGET_STM32F1
    GPIO_InitStruct.Pull       = GPIO_NOPULL;
    GPIO_InitStruct.Alternate  = 0;
#endif
#if defined(GPIOA)
    __HAL_RCC_GPIOA_CLK_ENABLE();
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    __HAL_RCC_GPIOA_CLK_DISABLE();
#endif
#if defined(GPIOB)
    __HAL_RCC_GPIOB_CLK_ENABLE();
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    __HAL_RCC_GPIOB_CLK_DISABLE();
#endif
#if defined(GPIOC)
    __HAL_RCC_GPIOC_CLK_ENABLE();
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    __HAL_RCC_GPIOC_CLK_DISABLE();
#endif
#if defined(GPIOD)
    __HAL_RCC_GPIOD_CLK_ENABLE();
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    __HAL_RCC_GPIOD_CLK_DISABLE();
#endif
#if defined(GPIOE)
    __HAL_RCC_GPIOE_CLK_ENABLE();
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    __HAL_RCC_GPIOE_CLK_DISABLE();
#endif
#if defined(GPIOF)
    __HAL_RCC_GPIOF_CLK_ENABLE();
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
    __HAL_RCC_GPIOF_CLK_DISABLE();
#endif
#if defined(GPIOG)
    __HAL_RCC_GPIOG_CLK_ENABLE();
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
    __HAL_RCC_GPIOG_CLK_DISABLE();
#endif
#if defined(GPIOH)
    __HAL_RCC_GPIOH_CLK_ENABLE();
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
    __HAL_RCC_GPIOH_CLK_DISABLE();
#endif
#if defined(GPIOI)
    __HAL_RCC_GPIOI_CLK_ENABLE();
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
    __HAL_RCC_GPIOI_CLK_DISABLE();
#endif
#if defined(GPIOJ)
    __HAL_RCC_GPIOJ_CLK_ENABLE();
    HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);
    __HAL_RCC_GPIOJ_CLK_DISABLE();
#endif
#if defined(GPIOK)
    __HAL_RCC_GPIOK_CLK_ENABLE();
    HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);
    __HAL_RCC_GPIOK_CLK_DISABLE();
#endif
}
#endif

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
#if defined(LSE_CONFIG_AVAILABLE)
    // LSE oscillator drive capability set before LSE is started
    if (!LL_RCC_LSE_IsReady()) {
        LSEDriveConfig();
    }
#endif

#if defined(MBED_CONF_TARGET_SYSTEM_POWER_SUPPLY)
#if IS_PWR_SUPPLY(MBED_CONF_TARGET_SYSTEM_POWER_SUPPLY)
    HAL_PWREx_ConfigSupply(MBED_CONF_TARGET_SYSTEM_POWER_SUPPLY);
#else
    #error system_power_supply not configured
#endif
#endif

    SetSysClock();
    SystemCoreClockUpdate();

#ifndef CM4_BOOT_BY_APPLICATION
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
#endif /* CM4_BOOT_BY_APPLICATION */
#endif /* CORE_M4 */

#else /* Single core */
    // Update the SystemCoreClock variable.
    SystemCoreClockUpdate();
    HAL_Init();

#if defined(LSE_CONFIG_AVAILABLE)
    // LSE oscillator drive capability set before LSE is started
    if (!LL_RCC_LSE_IsReady()) {
        LSEDriveConfig();
    }
#endif

#if defined(MBED_CONF_TARGET_SYSTEM_POWER_SUPPLY)
#if IS_PWR_SUPPLY(MBED_CONF_TARGET_SYSTEM_POWER_SUPPLY)
    HAL_PWREx_ConfigSupply(MBED_CONF_TARGET_SYSTEM_POWER_SUPPLY);
#else
    #error system_power_supply not configured
#endif
#endif

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

#if MBED_CONF_TARGET_GPIO_RESET_AT_INIT
    /* Reset all GPIO */
    GPIO_Full_Init();
#endif

    /* BSP initialization hook (external RAM, etc) */
    TargetBSP_Init();

    mbed_sdk_inited = 1;
}
