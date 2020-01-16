/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

/**
  * This file configures the system clock as follows:
  *-----------------------------------------------------------------------------
  * System clock source | 1- USE_PLL_HSE_EXTC (external 32 MHz clock)
  *                     | 2- USE_PLL_HSE_XTAL (external 8 MHz xtal)
  *                     | 3- USE_PLL_HSI (internal 16 MHz)
  *                     | 4- USE_PLL_MSI (internal 100kHz to 48 MHz)
  *-----------------------------------------------------------------------------
  * SYSCLK(MHz)         | 32
  * AHBCLK (MHz)        | 32
  * APB1CLK (MHz)       | 32
  * APB2CLK (MHz)       | 32
  * USB capable         | NO // todo
  *-----------------------------------------------------------------------------
**/

#include "stm32wbxx.h"
#include "mbed_error.h"
#include "stm32wbxx_ll_hsem.h"
#include "stm32wbxx_ll_hsem.h"
#include "otp.h"
#include "hw_conf.h" /* Common BLE file where BLE shared resources are defined */

// Clock source is selected with CLOCK_SOURCE in json config
#define USE_PLL_HSE_EXTC 0x8 // Use external clock (not available)
#define USE_PLL_HSE_XTAL 0x4 // Use external 32 MHz xtal (X1 on board + need HW patch)
#define USE_PLL_HSI      0x2 // Use HSI 16MHz internal clock
#define USE_PLL_MSI      0x1 // Use MSI internal clock

#define DEBUG_MCO (0) // Output the MCO on PA8 for debugging (0=OFF, 1=SYSCLK, 2=HSE, 3=HSI, 4=MSI)

#if ( ((CLOCK_SOURCE) & USE_PLL_HSE_XTAL) || ((CLOCK_SOURCE) & USE_PLL_HSE_EXTC) )
uint8_t SetSysClock_PLL_HSE(uint8_t bypass);
#endif /* ((CLOCK_SOURCE) & USE_PLL_HSE_XTAL) || ((CLOCK_SOURCE) & USE_PLL_HSE_EXTC) */

#if ((CLOCK_SOURCE) & USE_PLL_HSI)
uint8_t SetSysClock_PLL_HSI(void);
#endif /* ((CLOCK_SOURCE) & USE_PLL_HSI) */

#if ((CLOCK_SOURCE) & USE_PLL_MSI)
uint8_t SetSysClock_PLL_MSI(void);
#endif /* ((CLOCK_SOURCE) & USE_PLL_MSI) */

static void Configure_RF_Clock_Sources(void)
{
    static uint8_t RF_ON = 0;

    if (!RF_ON) {
        // Reset backup domain
        if ((LL_RCC_IsActiveFlag_PINRST()) && (!LL_RCC_IsActiveFlag_SFTRST())) {
            // Write twice the value to flush the APB-AHB bridge
            // This bit shall be written in the register before writing the next one
            HAL_PWR_EnableBkUpAccess();
            HAL_PWR_EnableBkUpAccess();
            __HAL_RCC_BACKUPRESET_FORCE();
            __HAL_RCC_BACKUPRESET_RELEASE();
        }

        /**
         * Select LSE clock
         */
        LL_RCC_LSE_Enable();
        while (!LL_RCC_LSE_IsReady());

        /**
         * Select wakeup source of BLE RF
         */
        LL_RCC_SetRFWKPClockSource(LL_RCC_RFWKP_CLKSOURCE_LSE);

        /**
         * Switch OFF LSI
         */
        LL_RCC_LSI1_Disable();

        RF_ON = 1;
    }

    return;
}

static void Config_HSE(void)
{
    OTP_ID0_t *p_otp;

    /**
     * Read HSE_Tuning from OTP
     */
    p_otp = (OTP_ID0_t *) OTP_Read(0);
    if (p_otp) {
        LL_RCC_HSE_SetCapacitorTuning(p_otp->hse_tuning);
    }

    return;
}


/**
  * @brief  Configures the System clock source, PLL Multiplier and Divider factors, AHB/APBx prescalers
  * @note   This function should be called only once the RCC clock configuration
  *         is reset to the default reset state (done in SystemInit() function).
  * @param  None
  * @retval None
  */

void SetSysClock(void)
{
    while (LL_HSEM_1StepLock(HSEM, CFG_HW_RCC_SEMID));
#if ((CLOCK_SOURCE) & USE_PLL_HSE_EXTC)
    /* 1- Try to start with HSE and external clock */
    if (SetSysClock_PLL_HSE(1) == 0)
#endif
    {
#if ((CLOCK_SOURCE) & USE_PLL_HSE_XTAL)
        /* 2- If fail try to start with HSE and external xtal */
        if (SetSysClock_PLL_HSE(0) == 0)
#endif
        {
#if ((CLOCK_SOURCE) & USE_PLL_HSI)
            /* 3- If fail start with HSI clock */
            if (SetSysClock_PLL_HSI() == 0)
#endif
            {
#if ((CLOCK_SOURCE) & USE_PLL_MSI)
                /* 4- If fail start with MSI clock */
                if (SetSysClock_PLL_MSI() == 0)
#endif
                {
                    {
                        error("SetSysClock failed\n");
                    }
                }
            }
        }
    }

    Configure_RF_Clock_Sources();

    // Output clock on MCO1 pin(PA8) for debugging purpose
#if DEBUG_MCO == 1
    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1); // 64 MHz
#endif
    LL_HSEM_ReleaseLock(HSEM, CFG_HW_RCC_SEMID, 0);
}

#if (((CLOCK_SOURCE) & USE_PLL_HSE_XTAL) || ((CLOCK_SOURCE) & USE_PLL_HSE_EXTC))
/******************************************************************************/
/*            PLL (clocked by HSE) used as System clock source                */
/******************************************************************************/
uint8_t SetSysClock_PLL_HSE(uint8_t bypass)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    Config_HSE();

    /** Configure the main internal regulator output voltage
    */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        return 0; // FAIL
    }
    /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK4 | RCC_CLOCKTYPE_HCLK2
                                  | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
        return 0; // FAIL
    }
    /** Initializes the peripherals clocks
    */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SMPS;
    PeriphClkInitStruct.SmpsClockSelection = RCC_SMPSCLKSOURCE_HSE;
    PeriphClkInitStruct.SmpsDivSelection = RCC_SMPSCLKDIV_RANGE0;

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        return 0; // FAIL
    }

    /**
     * Select HSI as system clock source after Wake Up from Stop mode
     */
    LL_RCC_SetClkAfterWakeFromStop(LL_RCC_STOP_WAKEUPCLOCK_HSI);

    /**
     * Set RNG on HSI48
     */
    LL_RCC_HSI48_Enable();
    while (!LL_RCC_HSI48_IsReady());
    LL_RCC_SetCLK48ClockSource(LL_RCC_CLK48_CLKSOURCE_HSI48);

    return 1;
}

#endif /* ((CLOCK_SOURCE) & USE_PLL_HSE_XTAL) || ((CLOCK_SOURCE) & USE_PLL_HSE_EXTC) */

#if ((CLOCK_SOURCE) & USE_PLL_HSI)
/******************************************************************************/
/*            PLL (clocked by HSI) used as System clock source                */
/******************************************************************************/
uint8_t SetSysClock_PLL_HSI(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // Enable HSI oscillator and activate PLL with HSI as source
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
    RCC_OscInitStruct.HSEState            = RCC_HSE_OFF;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI; // 16 MHz
    RCC_OscInitStruct.PLL.PLLM            = RCC_PLLM_DIV2;     // 8 MHz
    RCC_OscInitStruct.PLL.PLLN            = 16;                // 128 MHz
    RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ            = RCC_PLLQ_DIV4;
    RCC_OscInitStruct.PLL.PLLR            = RCC_PLLR_DIV2;     // 64 MHz // RCC_SYSCLKSOURCE_PLLCLK

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        return 0; // FAIL
    }

    // Select PLL as system clock source and configure the clocks dividers
    RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_HCLK2 | RCC_CLOCKTYPE_HCLK4 | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK; // 64 MHz
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;         // 64 MHz
    RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV2;         // 32 MHz
    RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;         // 64 MHz
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;           // 64 MHz
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;           // 64 MHz

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) {
        return 0; // FAIL
    }

    // Output clock on MCO1 pin(PA8) for debugging purpose
#if DEBUG_MCO == 3
    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI, RCC_MCODIV_1); // 16 MHz
#endif

    return 1;
}
#endif /* ((CLOCK_SOURCE) & USE_PLL_HSI) */

#if ((CLOCK_SOURCE) & USE_PLL_MSI)
/******************************************************************************/
/*            PLL (clocked by MSI) used as System clock source                */
/******************************************************************************/
uint8_t SetSysClock_PLL_MSI(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    // RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0}; // USB todo

#if MBED_CONF_TARGET_LSE_AVAILABLE
    // Enable LSE Oscillator to automatically calibrate the MSI clock
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE;            // No PLL update
    RCC_OscInitStruct.LSEState       = RCC_LSE_ON;              // External 32.768 kHz clock on OSC_IN/OSC_OUT
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        return 0; // FAIL
    }

    /* Enable the CSS interrupt in case LSE signal is corrupted or not present */
    HAL_RCCEx_DisableLSECSS();
#endif /* MBED_CONF_TARGET_LSE_AVAILABLE */

    // Enable MSI Oscillator and activate PLL with MSI as source
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_6;    // 4 MHz
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_MSI;
    RCC_OscInitStruct.PLL.PLLM            = RCC_PLLM_DIV1;     // 4 MHz
    RCC_OscInitStruct.PLL.PLLN            = 32;                // 128 MHz
    RCC_OscInitStruct.PLL.PLLR            = RCC_PLLR_DIV2;     // 64 MHz // RCC_SYSCLKSOURCE_PLLCLK
    RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV5;
    RCC_OscInitStruct.PLL.PLLQ            = RCC_PLLQ_DIV4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        return 0; // FAIL
    }

#if MBED_CONF_TARGET_LSE_AVAILABLE
    /* Enable MSI Auto-calibration through LSE */
    HAL_RCCEx_EnableMSIPLLMode();
#endif /* MBED_CONF_TARGET_LSE_AVAILABLE */

    /* Select MSI output as USB clock source */
    // PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
    // PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_MSI; /* 48 MHz */
    // HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

    // Select PLL as system clock source and configure the clocks dividers
    RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_HCLK2 | RCC_CLOCKTYPE_HCLK4 | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK; // 64 MHz
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;         // 64 MHz
    RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV2;         // 32 MHz
    RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;         // 64 MHz
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;           // 64 MHz
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;           // 64 MHz

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) {
        return 0; // FAIL
    }


    // Output clock on MCO1 pin(PA8) for debugging purpose
#if DEBUG_MCO == 4
    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_MSI, RCC_MCODIV_1); // 4 MHz
#endif

    return 1; // OK
}
#endif /* ((CLOCK_SOURCE) & USE_PLL_MSI) */

