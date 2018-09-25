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
#include "stm32wbxx.h"
#include "mbed_assert.h"

// Clock source is selected with CLOCK_SOURCE in json config
#define USE_PLL_HSE_EXTC 0x8 // Use external clock (not available)
#define USE_PLL_HSE_XTAL 0x4 // Use external 32 MHz xtal (X1 on board + need HW patch)
#define USE_PLL_HSI      0x2 // Use HSI 16MHz internal clock
#define USE_PLL_MSI      0x1 // Use MSI 4MHz internal clock (default)

#define DEBUG_MCO (0) // Output the MCO on PA8 for debugging (0=OFF, 1=SYSCLK, 2=HSE, 3=HSI, 4=MSI)

#if (((CLOCK_SOURCE) & USE_PLL_HSE_XTAL) || ((CLOCK_SOURCE) & USE_PLL_HSE_EXTC))
uint8_t SetSysClock_PLL_HSE(uint8_t bypass);
#endif /* ((CLOCK_SOURCE) & USE_PLL_HSE_XTAL) || ((CLOCK_SOURCE) & USE_PLL_HSE_EXTC) */

#if ((CLOCK_SOURCE) & USE_PLL_HSI)
uint8_t SetSysClock_PLL_HSI(void);
#endif /* ((CLOCK_SOURCE) & USE_PLL_HSI) */

#if ((CLOCK_SOURCE) & USE_PLL_MSI)
uint8_t SetSysClock_PLL_MSI(void);
#endif /* ((CLOCK_SOURCE) & USE_PLL_MSI) */

void Configure_RF_Clock_Sources(void);

/**
  * @brief  Configures the System clock source, PLL Multiplier and Divider factors, AHB/APBx prescalers
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
            if (SetSysClock_PLL_HSI()==0)
#endif
            {
#if ((CLOCK_SOURCE) & USE_PLL_MSI)
                /* 4- If fail start with MSI clock */
                if (SetSysClock_PLL_MSI() == 0)
#endif
                {
                    while(1) {
                        MBED_ASSERT(1);
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
}

#if (((CLOCK_SOURCE) & USE_PLL_HSE_XTAL) || ((CLOCK_SOURCE) & USE_PLL_HSE_EXTC))
/******************************************************************************/
/*            PLL (clocked by HSE) used as System clock source                */
/******************************************************************************/
uint8_t SetSysClock_PLL_HSE(uint8_t bypass)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // Enable HSE oscillator and activate PLL with HSE as source
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    if (bypass == 0) {
        RCC_OscInitStruct.HSEState   = RCC_HSE_ON; // External 32 MHz xtal on OSC_IN/OSC_OUT
    } else {
        RCC_OscInitStruct.HSEState   = RCC_HSE_BYPASS; // External 32 MHz clock on OSC_IN
    }

    RCC_OscInitStruct.PLL.PLLState  = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM      = RCC_PLLM_DIV2;
    RCC_OscInitStruct.PLL.PLLN      = 16;
    RCC_OscInitStruct.PLL.PLLP      = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ      = RCC_PLLQ_DIV4;
    RCC_OscInitStruct.PLL.PLLR      = RCC_PLLR_DIV4;

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
#if DEBUG_MCO == 2
    if (bypass == 0)
        HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_2); // xx MHz
    else
        HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1); // xx MHz
#endif

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
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM            = RCC_PLLM_DIV2;
    RCC_OscInitStruct.PLL.PLLN            = 16;
    RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ            = RCC_PLLQ_DIV4;
    RCC_OscInitStruct.PLL.PLLR            = RCC_PLLR_DIV2;

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

    // Enable MSI Oscillator and activate PLL with MSI as source
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_6;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_MSI;
    RCC_OscInitStruct.PLL.PLLM            = RCC_PLLM_DIV1;
    RCC_OscInitStruct.PLL.PLLN            = 32;
    RCC_OscInitStruct.PLL.PLLR            = RCC_PLLR_DIV2;
    RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV5;
    RCC_OscInitStruct.PLL.PLLQ            = RCC_PLLQ_DIV4;

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

    // Calibrate MSI
    LL_PWR_EnableBkUpAccess();
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
    LL_RCC_LSE_Enable();
    while (LL_RCC_LSE_IsReady() != 1) {}
    LL_RCC_MSI_EnablePLLMode();

    // Output clock on MCO1 pin(PA8) for debugging purpose
#if DEBUG_MCO == 4
    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_MSI, RCC_MCODIV_1); // 4 MHz
#endif

    return 1; // OK
}
#endif /* ((CLOCK_SOURCE) & USE_PLL_MSI) */

void Configure_RF_Clock_Sources(void)
{
    /**
     * Select LSE clock
     */
    LL_RCC_LSE_Enable();
    while(!LL_RCC_LSE_IsReady());

    /**
     * Select wakeup source of BLE RF
     */
    LL_RCC_SetRFWKPClockSource(LL_RCC_RFWKP_CLKSOURCE_LSE);

    /**
     * Switch OFF LSI
     */
    LL_RCC_LSI1_Disable();

    /**
     * Set RNG on HSI48
     */
    LL_RCC_HSI48_Enable();
    while(!LL_RCC_HSI48_IsReady());
    LL_RCC_SetCLK48ClockSource(LL_RCC_CLK48_CLKSOURCE_HSI48);

    return;
}
