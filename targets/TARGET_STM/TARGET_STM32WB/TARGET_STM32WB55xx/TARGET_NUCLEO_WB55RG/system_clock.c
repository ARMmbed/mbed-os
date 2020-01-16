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
  * System clock source | HSE (external 32 MHz clock)
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
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    while (LL_HSEM_1StepLock(HSEM, CFG_HW_RCC_SEMID));

    Config_HSE();

    /* Configure the main internal regulator output voltage */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Initializes the CPU, AHB and APB busses clocks */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        error("HAL_RCC_OscConfig error\n");
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
        error("HAL_RCC_ClockConfig error\n");
    }
    /** Initializes the peripherals clocks
    */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SMPS;
    PeriphClkInitStruct.SmpsClockSelection = RCC_SMPSCLKSOURCE_HSE;
    PeriphClkInitStruct.SmpsDivSelection = RCC_SMPSCLKDIV_RANGE0;

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        error("HAL_RCCEx_PeriphCLKConfig error\n");
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

    Configure_RF_Clock_Sources();

    LL_HSEM_ReleaseLock(HSEM, CFG_HW_RCC_SEMID, 0);
}
