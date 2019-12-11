/* mbed Microcontroller Library
* Copyright (c) 2006-2019 ARM Limited
* Copyright (c) 2006-2019 STMicroelectronics
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
  * System clock source | 1- USE_PLL_HSE_EXTC (external 8 MHz clock)
  *                     | 2- USE_PLL_HSE_XTAL (external 8 MHz xtal)
  *                     | 3- USE_PLL_HSI (internal 16 MHz)
  *                     | 4- USE_PLL_MSI (internal 100kHz to 48 MHz)
  *-----------------------------------------------------------------------------
  * SYSCLK(MHz)         | 120
  * AHBCLK (MHz)        | 120
  * APB1CLK (MHz)       | 120
  * APB2CLK (MHz)       | 120
  * USB capable         | YES
  *-----------------------------------------------------------------------------
**/

#include "stm32l4xx.h"
#include "mbed_assert.h"

// clock source is selected with CLOCK_SOURCE in json config
#define USE_PLL_HSE_EXTC 0x8 // Use external clock (ST Link MCO - not enabled by default)
#define USE_PLL_HSE_XTAL 0x4 // Use external xtal (X2 on board)
#define USE_PLL_HSI      0x2 // Use HSI internal clock
#define USE_PLL_MSI      0x1 // Use MSI internal clock

#if ( ((CLOCK_SOURCE) & USE_PLL_HSE_XTAL) || ((CLOCK_SOURCE) & USE_PLL_HSE_EXTC) )
uint8_t SetSysClock_PLL_HSE(uint8_t bypass);
#endif /* ((CLOCK_SOURCE) & USE_PLL_HSE_XTAL) || ((CLOCK_SOURCE) & USE_PLL_HSE_EXTC) */

#if ((CLOCK_SOURCE) & USE_PLL_HSI)
uint8_t SetSysClock_PLL_HSI(void);
#endif /* ((CLOCK_SOURCE) & USE_PLL_HSI) */

#if ((CLOCK_SOURCE) & USE_PLL_MSI)
uint8_t SetSysClock_PLL_MSI(void);
#endif /* ((CLOCK_SOURCE) & USE_PLL_MSI) */


/**
  * @brief  Configures the System clock source, PLL Multiplier and Divider factors,
  *               AHB/APBx prescalers and Flash settings
  * @note   This function is called in mbed_sdk_init() and hal_deepsleep() functions
  * @param  None
  * @retval None
  */
void SetSysClock(void)
{
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
                    while (1) {
                        MBED_ASSERT(1);
                    }
                }
            }
        }
    }
}

#if ( ((CLOCK_SOURCE) & USE_PLL_HSE_XTAL) || ((CLOCK_SOURCE) & USE_PLL_HSE_EXTC) )
/******************************************************************************/
/*            PLL (clocked by HSE) used as System clock source                */
/******************************************************************************/
uint8_t SetSysClock_PLL_HSE(uint8_t bypass)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST) != HAL_OK) {
        return 0; // FAIL
    }

    // Enable HSE oscillator and activate PLL with HSE as source
    RCC_OscInitStruct.OscillatorType        = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI48;
    if (bypass == 0) {
        RCC_OscInitStruct.HSEState          = RCC_HSE_ON; // External 16 MHz xtal on OSC_IN/OSC_OUT
    } else {
        RCC_OscInitStruct.HSEState          = RCC_HSE_BYPASS; // External 16 MHz clock on OSC_IN
    }
    RCC_OscInitStruct.HSIState              = RCC_HSI_OFF;
#if DEVICE_USBDEVICE
    RCC_OscInitStruct.HSI48State            = RCC_HSI48_ON;
#else
    RCC_OscInitStruct.HSI48State            = RCC_HSI48_OFF;
#endif /* DEVICE_USBDEVICE */
    RCC_OscInitStruct.PLL.PLLSource         = RCC_PLLSOURCE_HSE; // 16 MHz
    RCC_OscInitStruct.PLL.PLLState          = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLM              = 4;  // 4 MHz
    RCC_OscInitStruct.PLL.PLLN              = 60; // 240 MHz
    RCC_OscInitStruct.PLL.PLLP              = 7;
    RCC_OscInitStruct.PLL.PLLQ              = 2;
    RCC_OscInitStruct.PLL.PLLR              = 2;  // PLL clock = 120 MHz

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        return 0; // FAIL
    }

#if DEVICE_USBDEVICE
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInitStruct.UsbClockSelection    = RCC_USBCLKSOURCE_HSI48; /* 48 MHz */
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        return 0; // FAIL
    }
#endif /* DEVICE_USBDEVICE */

    // Select PLL clock as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
    RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK; // 120 MHz
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;         // 120 MHz
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;           // 120 MHz
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;           // 120 MHz
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        return 0; // FAIL
    }

    return 1; // OK
}
#endif /* ((CLOCK_SOURCE) & USE_PLL_HSE_XTAL) || ((CLOCK_SOURCE) & USE_PLL_HSE_EXTC) */

#if ((CLOCK_SOURCE) & USE_PLL_HSI)
/******************************************************************************/
/*            PLL (clocked by HSI) used as System clock source                */
/******************************************************************************/
uint8_t SetSysClock_PLL_HSI(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    // Select MSI as system clock source to allow modification of the PLL configuration
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
        return 0; // FAIL
    }

    // Enable HSI oscillator and activate PLL with HSI as source
    RCC_OscInitStruct.OscillatorType       = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI48;
    RCC_OscInitStruct.HSEState             = RCC_HSE_OFF;
    RCC_OscInitStruct.HSIState             = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue  = RCC_HSICALIBRATION_DEFAULT;
#if DEVICE_USBDEVICE
    RCC_OscInitStruct.HSI48State           = RCC_HSI48_ON;
#else
    RCC_OscInitStruct.HSI48State           = RCC_HSI48_OFF;
#endif /* DEVICE_USBDEVICE */
    RCC_OscInitStruct.PLL.PLLState         = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource        = RCC_PLLSOURCE_HSI; // 16 MHz
    RCC_OscInitStruct.PLL.PLLM             = 4;  // 4 MHz
    RCC_OscInitStruct.PLL.PLLN             = 60; // 240 MHz
    RCC_OscInitStruct.PLL.PLLP             = 7;
    RCC_OscInitStruct.PLL.PLLQ             = 2;
    RCC_OscInitStruct.PLL.PLLR             = 2;  // PLL clock = 120 MHz
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        return 0; // FAIL
    }

#if DEVICE_USBDEVICE
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInitStruct.UsbClockSelection    = RCC_USBCLKSOURCE_HSI48; /* 48 MHz */
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        return 0; // FAIL
    }
#endif /* DEVICE_USBDEVICE */

    // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
    RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK; // 120 MHz
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;         // 120 MHz
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;           // 120 MHz
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;           // 120 MHz
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        return 0; // FAIL
    }

    // Disable MSI Oscillator
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState       = RCC_MSI_OFF;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE; // No PLL update
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        return 0; // FAIL
    }

    return 1; // OK
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
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    // Enable LSE Oscillator to automatically calibrate the MSI clock
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE; // No PLL update
    RCC_OscInitStruct.LSEState       = RCC_LSE_ON; // External 32.768 kHz clock on OSC_IN/OSC_OUT
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) == HAL_OK) {
        RCC->CR |= RCC_CR_MSIPLLEN; // Enable MSI PLL-mode
    }

    HAL_RCCEx_DisableLSECSS();
    /* Enable MSI Oscillator and activate PLL with MSI as source */
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_MSI | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI48 | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
    RCC_OscInitStruct.HSEState            = RCC_HSE_OFF;
    RCC_OscInitStruct.HSIState            = RCC_HSI_OFF;
#if DEVICE_USBDEVICE
    RCC_OscInitStruct.HSI48State          = RCC_HSI48_ON;
#else
    RCC_OscInitStruct.HSI48State          = RCC_HSI48_OFF;
#endif /* DEVICE_USBDEVICE */
    RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_6; /* 4 MHz */
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_MSI;
    RCC_OscInitStruct.PLL.PLLM            = 1;    /* 4 MHz */
    RCC_OscInitStruct.PLL.PLLN            = 60;   /* 240 MHz */
    RCC_OscInitStruct.PLL.PLLP            = 7;    /* 48 MHz */
    RCC_OscInitStruct.PLL.PLLQ            = 2;    /* 120 MHz */
    RCC_OscInitStruct.PLL.PLLR            = 2;    /* 120 MHz */
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        return 0; // FAIL
    }
    /* Enable MSI Auto-calibration through LSE */
    HAL_RCCEx_EnableMSIPLLMode();

#if DEVICE_USBDEVICE
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInitStruct.UsbClockSelection    = RCC_USBCLKSOURCE_HSI48; /* 48 MHz */
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        return 0; // FAIL
    }
#endif /* DEVICE_USBDEVICE */

    // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
    RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK; /* 120 MHz */
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;         /* 120 MHz */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;           /* 120 MHz */
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;           /* 120 MHz */
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        return 0; // FAIL
    }

    return 1; // OK
}
#endif /* ((CLOCK_SOURCE) & USE_PLL_MSI) */
