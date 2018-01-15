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
  *-----------------------------------------------------------------
  * System clock source | 1- USE_PLL_HSE_EXTC (external 8 MHz clock)
  *                     | 2- USE_PLL_HSE_XTAL (external 8 MHz xtal)
  *                     | 3- USE_PLL_HSI (internal 8 MHz)
  *-----------------------------------------------------------------
  * SYSCLK(MHz)         | 48
  * AHBCLK (MHz)        | 48
  * APB1CLK (MHz)       | 48
  * USB capable         | NO
  *-----------------------------------------------------------------
  */

#include "stm32f0xx.h"
#include "mbed_assert.h"

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
  * @brief  Setup the microcontroller system.
  *         Initialize the default HSI clock source, vector table location and the PLL configuration is reset.
  * @param  None
  * @retval None
  */
void SystemInit(void)
{
    /* Reset the RCC clock configuration to the default reset state ------------*/
    /* Set HSION bit */
    RCC->CR |= (uint32_t)0x00000001U;

#if defined (STM32F051x8) || defined (STM32F058x8)
    /* Reset SW[1:0], HPRE[3:0], PPRE[2:0], ADCPRE and MCOSEL[2:0] bits */
    RCC->CFGR &= (uint32_t)0xF8FFB80CU;
#else
    /* Reset SW[1:0], HPRE[3:0], PPRE[2:0], ADCPRE, MCOSEL[2:0], MCOPRE[2:0] and PLLNODIV bits */
    RCC->CFGR &= (uint32_t)0x08FFB80CU;
#endif /* STM32F051x8 or STM32F058x8 */

    /* Reset HSEON, CSSON and PLLON bits */
    RCC->CR &= (uint32_t)0xFEF6FFFFU;

    /* Reset HSEBYP bit */
    RCC->CR &= (uint32_t)0xFFFBFFFFU;

    /* Reset PLLSRC, PLLXTPRE and PLLMUL[3:0] bits */
    RCC->CFGR &= (uint32_t)0xFFC0FFFFU;

    /* Reset PREDIV[3:0] bits */
    RCC->CFGR2 &= (uint32_t)0xFFFFFFF0U;

#if defined (STM32F072xB) || defined (STM32F078xx)
    /* Reset USART2SW[1:0], USART1SW[1:0], I2C1SW, CECSW, USBSW and ADCSW bits */
    RCC->CFGR3 &= (uint32_t)0xFFFCFE2CU;
#elif defined (STM32F071xB)
    /* Reset USART2SW[1:0], USART1SW[1:0], I2C1SW, CECSW and ADCSW bits */
    RCC->CFGR3 &= (uint32_t)0xFFFFCEACU;
#elif defined (STM32F091xC) || defined (STM32F098xx)
    /* Reset USART3SW[1:0], USART2SW[1:0], USART1SW[1:0], I2C1SW, CECSW and ADCSW bits */
    RCC->CFGR3 &= (uint32_t)0xFFF0FEACU;
#elif defined (STM32F030x6) || defined (STM32F030x8) || defined (STM32F031x6) || defined (STM32F038xx) || defined (STM32F030xC)
    /* Reset USART1SW[1:0], I2C1SW and ADCSW bits */
    RCC->CFGR3 &= (uint32_t)0xFFFFFEECU;
#elif defined (STM32F051x8) || defined (STM32F058xx)
    /* Reset USART1SW[1:0], I2C1SW, CECSW and ADCSW bits */
    RCC->CFGR3 &= (uint32_t)0xFFFFFEACU;
#elif defined (STM32F042x6) || defined (STM32F048xx)
    /* Reset USART1SW[1:0], I2C1SW, CECSW, USBSW and ADCSW bits */
    RCC->CFGR3 &= (uint32_t)0xFFFFFE2CU;
#elif defined (STM32F070x6) || defined (STM32F070xB)
    /* Reset USART1SW[1:0], I2C1SW, USBSW and ADCSW bits */
    RCC->CFGR3 &= (uint32_t)0xFFFFFE6CU;
    /* Set default USB clock to PLLCLK, since there is no HSI48 */
    RCC->CFGR3 |= (uint32_t)0x00000080U;
#else
#warning "No target selected"
#endif

    /* Reset HSI14 bit */
    RCC->CR2 &= (uint32_t)0xFFFFFFFEU;

    /* Disable all interrupts */
    RCC->CIR = 0x00000000U;

    /* Enable SYSCFGENR in APB2EN, needed for 1st call of NVIC_SetVector, to copy vectors from flash to ram */
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
}

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
                while(1) {
                    MBED_ASSERT(1);
                }
            }
        }
    }

    // Output clock on MCO pin(PA8) for debugging purpose
    // HAL_RCC_MCOConfig(RCC_MCO, RCC_MCOSOURCE_SYSCLK, RCC_MCO_DIV1); // 48 MHz
}

#if ( ((CLOCK_SOURCE) & USE_PLL_HSE_XTAL) || ((CLOCK_SOURCE) & USE_PLL_HSE_EXTC) )
/******************************************************************************/
/*            PLL (clocked by HSE) used as System clock source                */
/******************************************************************************/
uint8_t SetSysClock_PLL_HSE(uint8_t bypass)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    //Select HSI as system clock source to allow modification of the PLL configuration
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
        return 0; // FAIL
    }


    // Select HSE oscillator as PLL source
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    if (bypass == 0) {
        RCC_OscInitStruct.HSEState   = RCC_HSE_ON; // External 8 MHz xtal on OSC_IN/OSC_OUT
    } else {
        RCC_OscInitStruct.HSEState   = RCC_HSE_BYPASS; // External 8 MHz clock on OSC_IN only
    }
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PREDIV     = RCC_PREDIV_DIV2;
    RCC_OscInitStruct.PLL.PLLMUL     = RCC_PLL_MUL12;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        return 0; // FAIL
    }

    // Select PLL as system clock source and configure the HCLK and PCLK1 clocks dividers
    RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK; // 48 MHz
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;         // 48 MHz
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;           // 48 MHz
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
        return 0; // FAIL
    }

//  HAL_RCC_MCOConfig(RCC_MCO, RCC_MCOSOURCE_HSE, RCC_MCO_DIV2); // 8/2 = 4 MHz

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

    // Select PLLCLK = 48 MHz ((HSI 8 MHz / 2) * 12)
    RCC_OscInitStruct.OscillatorType          = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSEState                = RCC_HSE_OFF;
    RCC_OscInitStruct.LSEState                = RCC_LSE_OFF;
    RCC_OscInitStruct.HSIState                = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue     = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.HSI14State              = RCC_HSI_OFF;
    RCC_OscInitStruct.HSI14CalibrationValue   = RCC_HSI14CALIBRATION_DEFAULT;
    RCC_OscInitStruct.LSIState                = RCC_LSI_OFF;
    RCC_OscInitStruct.PLL.PLLState            = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource           = RCC_PLLSOURCE_HSI; // HSI div 2
    RCC_OscInitStruct.PLL.PREDIV              = RCC_PREDIV_DIV2;
    RCC_OscInitStruct.PLL.PLLMUL              = RCC_PLL_MUL12;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        return 0; // FAIL
    }

    // Select PLL as system clock source and configure the HCLK and PCLK1 clocks dividers
    RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK; // 48 MHz
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;         // 48 MHz
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;           // 48 MHz
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
        return 0; // FAIL
    }

    //HAL_RCC_MCOConfig(RCC_MCO, RCC_MCOSOURCE_HSI, RCC_MCO_DIV4); // 8/4 = 2 MHz

    return 1; // OK
}
#endif /* ((CLOCK_SOURCE) & USE_PLL_HSI) */
