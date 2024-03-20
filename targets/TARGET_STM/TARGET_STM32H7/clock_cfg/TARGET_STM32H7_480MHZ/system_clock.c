/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2024 STMicroelectronics.
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
  * This file configures the system clock as follows:
  *------------------------------------------------------------------------------
  * System clock source   | 1- USE_PLL_HSE_EXTC (overdrive) | 1- USE_PLL_HSE_EXTC
  *                       | 2- USE_PLL_HSE_XTAL (overdrive) | 2- USE_PLL_HSE_XTAL
  *                       | 3- USE_PLL_HSI (overdrive)      | 3- USE_PLL_HSI
  *------------------------------------------------------------------------------
  * SYSCLK(MHz)           |            480                  |        400
  * AHBCLK (MHz)          |            240                  |        200
  * APB1CLK (MHz)         |            120                  |        100
  * APB2CLK (MHz)         |            120                  |        100
  * APB3CLK (MHz)         |            120                  |        100
  * APB4CLK (MHz)         |            120                  |        100
  * USB capable (48 MHz)  |            YES                  |        YES
  *------------------------------------------------------------------------------
  *
  * It is used for all STM32H7 family microcontrollers with a top speed of 480MHz.
  * The input clock from the external oscillator may be any frequency evenly divisible by
  * 5MHz or 2MHz, and must be between 4MHz and 50MHz.
  * 
  * Note that 480MHz is the "overdrive" mode and is basically an overclock.  It is only supported
  * under certain conditions (LDO in use) and cannot be used over the full temperature range.
  * For industrial applications it is recommended to disable overdrive. Overdrive can be enabled/
  * disabled via the "target.enable-overdrive-mode" option in mbed_app.json.
  * 
**/

#include "stm32h7xx.h"
#include "mbed_error.h"

// clock source is selected with CLOCK_SOURCE in json config
#define USE_PLL_HSE_EXTC     0x8  // Use external clock (ST Link MCO or CMOS oscillator)
#define USE_PLL_HSE_XTAL     0x4  // Use external xtal (not provided by default on nucleo boards)
#define USE_PLL_HSI          0x2  // Use HSI internal clock

#if ( ((CLOCK_SOURCE) & USE_PLL_HSE_XTAL) || ((CLOCK_SOURCE) & USE_PLL_HSE_EXTC) )
uint8_t SetSysClock_PLL_HSE(uint8_t bypass);
#endif /* ((CLOCK_SOURCE) & USE_PLL_HSE_XTAL) || ((CLOCK_SOURCE) & USE_PLL_HSE_EXTC) */

#if ((CLOCK_SOURCE) & USE_PLL_HSI)
uint8_t SetSysClock_PLL_HSI(void);
#endif /* ((CLOCK_SOURCE) & USE_PLL_HSI) */

#if MBED_CONF_TARGET_ENABLE_OVERDRIVE_MODE
#define FLASH_LATENCY FLASH_LATENCY_4
#else
#define FLASH_LATENCY FLASH_LATENCY_2
#endif

/**
  * @brief  Configures the System clock source, PLL Multiplier and Divider factors,
  *               AHB/APBx prescalers and Flash settings
  * @note   This function should be called only once the RCC clock configuration
  *         is reset to the default reset state (done in SystemInit() function).
  * @param  None
  * @retval None
  */

void SetSysClock(void)
{
#if ((CLOCK_SOURCE) & USE_PLL_HSE_EXTC)
    /* 1- Try to start with HSE and external clock (MCO from STLink PCB part) */
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
                error("SetSysClock failed\n");
            }
        }
    }

    /* Make sure that 64MHz HSI clock is selected as the PER_CLOCK source, as this
       vastly simplifies peripheral clock logic (since peripherals' input clocks will always
       be 64MHz regardless of HSE frequency)*/
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CKPER;
    PeriphClkInitStruct.CkperClockSelection = RCC_CLKPSOURCE_HSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        error("HAL_RCCEx_PeriphCLKConfig failed\n");
    }
}


#if ( ((CLOCK_SOURCE) & USE_PLL_HSE_XTAL) || ((CLOCK_SOURCE) & USE_PLL_HSE_EXTC) )
/******************************************************************************/
/*            PLL (clocked by HSE) used as System clock source                */
/******************************************************************************/
MBED_WEAK uint8_t SetSysClock_PLL_HSE(uint8_t bypass)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    /* Configure the main internal regulator output voltage */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

#if MBED_CONF_TARGET_ENABLE_OVERDRIVE_MODE
    /* Enable overdrive mode (needed to hit 480MHz).  Note that on STM32H74x/5x,
       unlike other STM32H7x devices, you have to switch to VOS1 first, then switch to VOS0. */
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

    while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
#endif

    /* Enable HSE Oscillator and activate PLL with HSE as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI48;
    if (bypass) {
        RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
    } else {
        RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    }
    RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

    if(HSE_VALUE % 2000000 == 0)
    {
        // Clock divisible by 2.  Divide down to 2MHz and then multiply up again.
        RCC_OscInitStruct.PLL.PLLM = (HSE_VALUE / 2000000); // PLL1 input clock = 2MHz
        RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1; // PLL1 input clock is between 2 and 4 MHz

#if MBED_CONF_TARGET_ENABLE_OVERDRIVE_MODE
        RCC_OscInitStruct.PLL.PLLN = 480; // PLL1 internal (VCO) clock = 960 MHz
#else
        RCC_OscInitStruct.PLL.PLLN = 400; // PLL1 internal (VCO) clock = 800 MHz
#endif
    }
    else if(HSE_VALUE % 5000000 == 0)
    {
        RCC_OscInitStruct.PLL.PLLM = (HSE_VALUE / 5000000); // PLL1 input clock = 5MHz
        RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2; // PLL1 input clock is between 4 and 8 MHz

#if MBED_CONF_TARGET_ENABLE_OVERDRIVE_MODE
        RCC_OscInitStruct.PLL.PLLN = 192; // 960 MHz
#else
        RCC_OscInitStruct.PLL.PLLN = 160; // 800 MHz
#endif
    }
    else
    {
        error("HSE_VALUE not divisible by 2MHz or 5MHz\n");
    }

    RCC_OscInitStruct.PLL.PLLP = 2;   // PLLCLK = SYSCLK = 480/400 MHz

#if MBED_CONF_TARGET_ENABLE_OVERDRIVE_MODE
    RCC_OscInitStruct.PLL.PLLQ = 96;  // PLL1Q used for FDCAN = 10 MHz
#else
    RCC_OscInitStruct.PLL.PLLQ = 80;  // PLL1Q used for FDCAN = 10 MHz
#endif

    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE; // PLL1 VCO clock is between 192 and 960 MHz
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        return 0; // FAIL
    }

    /* Select PLL as system clock source and configure bus clocks dividers */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 |
                                  RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_D3PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY) != HAL_OK) {
        return 0; // FAIL
    }

#if DEVICE_USBDEVICE
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        return 0; // FAIL
    }

    HAL_PWREx_EnableUSBVoltageDetector();
#endif /* DEVICE_USBDEVICE */

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

    /* First step voltage regulator up to full voltage */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

#if MBED_CONF_TARGET_ENABLE_OVERDRIVE_MODE
    /* Enable overdrive mode (needed to hit 480MHz).  Note that on STM32H74x/5x,
       unlike other STM32H7x devices, you have to switch to VOS1 first, then switch to VOS0. */
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

    while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
#endif

    // Enable HSI oscillator and activate PLL with HSI as source
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_CSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
    RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 8;    //PLL1 input clock = 8 MHz

#if MBED_CONF_TARGET_ENABLE_OVERDRIVE_MODE
    RCC_OscInitStruct.PLL.PLLN = 120;  // PLL1 internal (VCO) clock = 960 MHz
#else
    RCC_OscInitStruct.PLL.PLLN = 100; // PLL1 internal (VCO) clock = 800 MHz
#endif

    RCC_OscInitStruct.PLL.PLLP = 2;    // 480/400 MHz
    
#if MBED_CONF_TARGET_ENABLE_OVERDRIVE_MODE
    RCC_OscInitStruct.PLL.PLLQ = 96;  // PLL1Q used for FDCAN = 10 MHz
#else
    RCC_OscInitStruct.PLL.PLLQ = 80;  // PLL1Q used for FDCAN = 10 MHz
#endif

    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE; // PLL1 VCO clock is between 192 and 960 MHz
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3; // PLL1 input clock is between 8 and 16 MHz
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        return 0; // FAIL
    }

    /* Select PLL as system clock source and configure  bus clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
                                   RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY) != HAL_OK) {
        return 0; // FAIL
    }
    
    return 1; // OK
}
#endif /* ((CLOCK_SOURCE) & USE_PLL_HSI) */
