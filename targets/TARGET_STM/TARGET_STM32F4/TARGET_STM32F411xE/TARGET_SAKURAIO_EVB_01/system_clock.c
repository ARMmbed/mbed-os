/* mbed Microcontroller Library
* Copyright (c) 2006-2017 ARM Limited
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
  * System clock source | 1- USE_PLL_HSE_EXTC (external 8 MHz clock) | DEVICE_USBDEVICE=1
  *                     | 2- USE_PLL_HSE_XTAL (external 8 MHz xtal)  |
  *                     | 3- USE_PLL_HSI (internal 16 MHz)           |
  *-----------------------------------------------------------------------------
  * SYSCLK(MHz)         | 100                                        | 96
  * AHBCLK (MHz)        | 100                                        | 96
  * APB1CLK (MHz)       |  50                                        | 48
  * APB2CLK (MHz)       | 100                                        | 96
  * USB capable         |  NO                                        | YES
  *-----------------------------------------------------------------------------
**/

#include "stm32f4xx.h"
#include "mbed_error.h"


// clock source is selected with CLOCK_SOURCE in json config
#define USE_PLL_HSE_EXTC     0x8  // Use external clock (ST Link MCO)
#define USE_PLL_HSE_XTAL     0x4  // Use external xtal (X3 on board - not provided by default)
#define USE_PLL_HSI          0x2  // Use HSI internal clock

#if ( ((CLOCK_SOURCE) & USE_PLL_HSE_XTAL) || ((CLOCK_SOURCE) & USE_PLL_HSE_EXTC) )
uint8_t SetSysClock_PLL_HSE(uint8_t bypass);
#endif /* ((CLOCK_SOURCE) & USE_PLL_HSE_XTAL) || ((CLOCK_SOURCE) & USE_PLL_HSE_EXTC) */

#if ((CLOCK_SOURCE) & USE_PLL_HSI)
uint8_t SetSysClock_PLL_HSI(void);
#endif /* ((CLOCK_SOURCE) & USE_PLL_HSI) */


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
                {
                    error("SetSysClock failed\n");
                }
            }
        }
    }

    /* Output clock on MCO2 pin(PC9) for debugging purpose */
    //HAL_RCC_MCOConfig(RCC_MCO2, RCC_MCO2SOURCE_SYSCLK, RCC_MCODIV_4);
}

#if ( ((CLOCK_SOURCE) & USE_PLL_HSE_XTAL) || ((CLOCK_SOURCE) & USE_PLL_HSE_EXTC) )
/******************************************************************************/
/*            PLL (clocked by HSE) used as System clock source                */
/******************************************************************************/
uint8_t SetSysClock_PLL_HSE(uint8_t bypass)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /* The voltage scaling allows optimizing the power consumption when the device is
       clocked below the maximum system frequency, to update the voltage scaling value
       regarding system frequency refer to product datasheet. */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    // Enable HSE oscillator and activate PLL with HSE as source
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSE;
    if (bypass == 0) {
        RCC_OscInitStruct.HSEState          = RCC_HSE_ON; // External 8 MHz xtal on OSC_IN/OSC_OUT
    } else {
        RCC_OscInitStruct.HSEState          = RCC_HSE_BYPASS; // External 8 MHz clock on OSC_IN
    }

    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM            = 4;             // VCO input clock = 2 MHz (8 MHz / 4)
#if (DEVICE_USBDEVICE)
    RCC_OscInitStruct.PLL.PLLN            = 192;           // VCO output clock = 384 MHz (2 MHz * 192)
#else /* DEVICE_USBDEVICE */
    RCC_OscInitStruct.PLL.PLLN            = 200;           // VCO output clock = 400 MHz (2 MHz * 200)
#endif /* DEVICE_USBDEVICE */
    RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV4; // PLLCLK = 100 MHz or 96 MHz (depending on DEVICE_USBDEVICE)
    RCC_OscInitStruct.PLL.PLLQ            = 8;             // USB clock = 48 MHz (DEVICE_USBDEVICE=1)
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        return 0; // FAIL
    }

    // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK; // 100/96 MHz
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;         // 100/96 MHz
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;           // 50/48 MHz
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;           // 100/96 MHz
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) {
        return 0; // FAIL
    }

    /* Output clock on MCO1 pin(PA8) for debugging purpose */
    //if (bypass == 0)
    //  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_2); // 4 MHz with xtal
    //else
    //  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1); // 8 MHz with external clock

    return 1; // OK
}
#endif /* ((CLOCK_SOURCE) & USE_PLL_HSE_XTAL) || ((CLOCK_SOURCE) & USE_PLL_HSE_EXTC) */

#if ((CLOCK_SOURCE) & USE_PLL_HSI)
/******************************************************************************/
/*            PLL (clocked by HSI) used as System clock source                */
/******************************************************************************/
uint8_t SetSysClock_PLL_HSI(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /* The voltage scaling allows optimizing the power consumption when the device is
       clocked below the maximum system frequency, to update the voltage scaling value
       regarding system frequency refer to product datasheet. */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    // Enable HSI oscillator and activate PLL with HSI as source
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
    RCC_OscInitStruct.HSEState            = RCC_HSE_OFF;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM            = 8;             // VCO input clock = 2 MHz (16 MHz / 8)
#if (DEVICE_USBDEVICE)
    RCC_OscInitStruct.PLL.PLLN            = 192;           // VCO output clock = 384 MHz (2 MHz * 192)
#else /* DEVICE_USBDEVICE */
    RCC_OscInitStruct.PLL.PLLN            = 200;           // VCO output clock = 400 MHz (2 MHz * 200)
#endif /* DEVICE_USBDEVICE */
    RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV4; // PLLCLK = 100 MHz or 96 MHz (depending on DEVICE_USBDEVICE)
    RCC_OscInitStruct.PLL.PLLQ            = 8;             // USB clock = 48 MHz (DEVICE_USBDEVICE=1)
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        return 0; // FAIL
    }

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
    RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) {
        return 0; // FAIL
    }

    /* Output clock on MCO1 pin(PA8) for debugging purpose */
    //HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI, RCC_MCODIV_1); // 16 MHz

    return 1; // OK
}
#endif /* ((CLOCK_SOURCE) & USE_PLL_HSI) */
