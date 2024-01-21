/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/**
  * This file configures the system clock depending on config from targets.json:
  *-----------------------------------------------------------------------------
  * System clock source | 1- USE_PLL_HSE_EXTC (external clock)
  *                     | 2- USE_PLL_HSE_XTAL (external xtal)
  *                     | 3- USE_PLL_HSI (internal 16 MHz)
  *                     | 4- USE_PLL_MSI (internal 100kHz to 48 MHz)
  *-----------------------------------------------------------------------------
  * SYSCLK(MHz)         | 160
  * AHBCLK (MHz)        | 160
  * APB1CLK (MHz)       | 160
  * APB2CLK (MHz)       | 160
  * APB3CLK (MHz)       | 160
  * USB capable         | TODO
  *-----------------------------------------------------------------------------
**/

#include "stm32u5xx.h"
#include "mbed_error.h"

// clock source is selected with CLOCK_SOURCE in json config
#define USE_PLL_HSE_EXTC 0x8 // Use external clock (OSC_IN)
#define USE_PLL_HSE_XTAL 0x4 // Use external xtal (OSC_IN/OSC_OUT)
#define USE_PLL_HSI      0x2 // Use HSI internal clock
#define USE_PLL_MSI      0x1 // Use MSI internal clock

#define DEBUG_MCO        (0) // Output the MCO on PA8 for debugging (0=OFF, 1=SYSCLK)
// TODO , 2=HSE, 3=HSI, 4=MSI)
#define DEBUG_LSCO       (0) // Output the LSCO on PA2 for debugging (0=OFF, 1=LSE)
// TODO , 2=LSI)

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
  * @note   This function is called in mbed_sdk_init() function (targets/TARGET_STM/mbed_overrides.c)
  *         and after each deepsleep period in hal_deepsleep() (targets/TARGET_STM/sleep.c)
  * @param  None
  * @retval None
  */

MBED_WEAK void SetSysClock(void)
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
                    {
                        error("SetSysClock failed\n");
                    }
                }
            }
        }
    }

// Output clock on MCO pin(PA8) for debugging purpose
#if DEBUG_MCO == 1
    HAL_RCC_MCOConfig(RCC_MCO, RCC_MCOSOURCE_SYSCLK, RCC_MCO_NODIV); // 160 MHz 
#endif

// Output clock on LSCO pin(PA2) for debugging purpose
// LSE is set with the low power ticker
#if DEBUG_LSCO == 1
    HAL_RCCEx_EnableLSCO(RCC_LSCOSOURCE_LSE); // 32 kHz
#endif

}

#if ( ((CLOCK_SOURCE) & USE_PLL_HSE_XTAL) || ((CLOCK_SOURCE) & USE_PLL_HSE_EXTC) )
/******************************************************************************/
/*            PLL (clocked by HSE) used as System clock source                */
/******************************************************************************/
MBED_WEAK uint8_t SetSysClock_PLL_HSE(uint8_t bypass)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

#if DEVICE_USBDEVICE
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit = {0};
#endif /* DEVICE_USBDEVICE */
    
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
        return 0; // FAIL
    }

    // Enable HSE oscillator and activate PLL with HSE as source
    RCC_OscInitStruct.OscillatorType        = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI48;
    if (bypass == 0) {
        RCC_OscInitStruct.HSEState            = RCC_HSE_ON; // External xtal on OSC_IN/OSC_OUT
    } else {
        RCC_OscInitStruct.HSEState            = RCC_HSE_BYPASS; // External clock on OSC_IN
    }
#if DEVICE_USBDEVICE
    RCC_OscInitStruct.HSI48State            = RCC_HSI48_ON;
#else
    RCC_OscInitStruct.HSI48State            = RCC_HSI48_OFF;
#endif /* DEVICE_USBDEVICE */
#if HSE_VALUE==10000000UL
    RCC_OscInitStruct.PLL.PLLState          = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource         = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMBOOST         = RCC_PLLMBOOST_DIV1;
    RCC_OscInitStruct.PLL.PLLM              = 1; // VCO input clock = 10 MHz (10 MHz / 1)
#else
#error Unsupported external clock value, check HSE_VALUE define
#endif
    RCC_OscInitStruct.PLL.PLLN              = 16; // VCO output clock = 160 MHz (10 MHz * 16)
    RCC_OscInitStruct.PLL.PLLP              = 2;
    RCC_OscInitStruct.PLL.PLLQ              = 2;
    RCC_OscInitStruct.PLL.PLLR              = 1;  // PLL clock = 160 MHz
    RCC_OscInitStruct.PLL.PLLRGE            = RCC_PLLVCIRANGE_1;
    RCC_OscInitStruct.PLL.PLLFRACN          = 0;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        return 0; // FAIL
    }

#if DEVICE_USBDEVICE
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkIniRCC_PeriphClkInittStruct.UsbClockSelection    = RCC_USBCLKSOURCE_HSI48; /* 48 MHz */
    if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit) != HAL_OK) {
        return 0; // FAIL
    }
#endif /* DEVICE_USBDEVICE */

    // Select PLL clock as system clock source and configure the HCLK, PCLK1 and PCLK2 clock dividers
    RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK3);
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK; // 160 MHz
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;         // 160 MHz
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;           // 160 MHz
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;           // 160 MHz
    RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;           // 160 MHz
    
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
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
    return 0; // TODO
}
#endif /* ((CLOCK_SOURCE) & USE_PLL_HSI) */

#if ((CLOCK_SOURCE) & USE_PLL_MSI)
/******************************************************************************/
/*            PLL (clocked by MSI) used as System clock source                */
/******************************************************************************/
MBED_WEAK uint8_t SetSysClock_PLL_MSI(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWREx_DisableUCPDDeadBattery(); /* Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral */
    HAL_PWREx_EnableVddA();
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

#if MBED_CONF_TARGET_LSE_AVAILABLE
        // Enable LSE Oscillator to automatically calibrate the MSI clock
        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
        RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE; // No PLL update
        RCC_OscInitStruct.LSEState       = RCC_LSE_ON;   // External 32.768 kHz clock on OSC32_IN/OSC32_OUT
        if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
            return 0; // FAIL
        }
#endif /* MBED_CONF_TARGET_LSE_AVAILABLE */

    /* Enable MSI Oscillator and activate PLL with MSI as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48 | RCC_OSCILLATORTYPE_HSI
                                       | RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_0;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
    RCC_OscInitStruct.PLL.PLLMBOOST = RCC_PLLMBOOST_DIV4;
    RCC_OscInitStruct.PLL.PLLM = 3;
    RCC_OscInitStruct.PLL.PLLN = 10;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 2;
    RCC_OscInitStruct.PLL.PLLR = 1;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLLVCIRANGE_1;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        return 0; // FAIL
    }

#if MBED_CONF_TARGET_LSE_AVAILABLE
        /* Enable MSI Auto-calibration through LSE */
        HAL_RCCEx_EnableMSIPLLMode();
#endif /* MBED_CONF_TARGET_LSE_AVAILABLE */

#if DEVICE_USBDEVICE
        /* Select MSI output as USB clock source */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
        PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_MSI; /* 48 MHz */
        HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
#endif /* DEVICE_USBDEVICE */

    // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2
                                  | RCC_CLOCKTYPE_PCLK3;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
        return 0; // FAIL
    }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RNG;
    PeriphClkInit.RngClockSelection = RCC_RNGCLKSOURCE_HSI48;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        return 0; // FAIL
    }

    HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY);
    HAL_ICACHE_Enable();

    return 1; // OK
}
#endif /* ((CLOCK_SOURCE) & USE_PLL_MSI) */
