/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

 #pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define CPU_CLOCK_HZ              (100000000 )
#define HSE_SOURCE                ( RCC_HSE_ON )
#define AHB_CLOCK_DIVIDER         ( RCC_SYSCLK_Div1 )
#define APB1_CLOCK_DIVIDER        ( RCC_HCLK_Div2 )
#define APB2_CLOCK_DIVIDER        ( RCC_HCLK_Div1 )
#define PLL_SOURCE                ( RCC_PLLSource_HSE )
#define PLL_M_CONSTANT            ( 26 )
#define PLL_N_CONSTANT            ( 200 )
#define PLL_P_CONSTANT            ( 2 )
#define PLL_Q_CONSTANT            ( 4 )
#define PLL_R_CONSTANT            ( 2 )
#define SYSTEM_CLOCK_SOURCE       ( RCC_SYSCLKSource_PLLCLK )
#define SYSTICK_CLOCK_SOURCE      ( SysTick_CLKSource_HCLK )
#define INT_FLASH_WAIT_STATE      ( FLASH_Latency_3 )
#define PLATFORM_STM32_VOLTAGE_2V7_TO_3V6


/******************************************************
 *  Wi-Fi Options
 ******************************************************/

/*  GPIO pins are used to bootstrap Wi-Fi to SDIO or gSPI mode */
//#define WICED_WIFI_USE_GPIO_FOR_BOOTSTRAP_0
//#define WICED_WIFI_USE_GPIO_FOR_BOOTSTRAP_1

/*  Wi-Fi GPIO0 pin is used for out-of-band interrupt */
#define WICED_WIFI_OOB_IRQ_GPIO_PIN  ( 0 )

/*  Wi-Fi power pin is present */
//#define WICED_USE_WIFI_POWER_PIN

/*  Wi-Fi reset pin is present */
#define WICED_USE_WIFI_RESET_PIN

/*  WLAN Powersave Clock Source
 *  The WLAN sleep clock can be driven from one of two sources:
 *  1. MCO (MCU Clock Output) - default
 *     NOTE: Versions of BCM943362WCD4 up to and including P200 require a hardware patch to enable this mode
 *     - Connect STM32F205RGT6 pin 41 (PA8) to pin 44 (PA11)
 *  2. WLAN 32K internal oscillator (30% inaccuracy)
 *     - Comment the following directive : WICED_USE_WIFI_32K_CLOCK_MCO
 */
#define WICED_USE_WIFI_32K_CLOCK_MCO

#ifdef __cplusplus
} /* extern "C" */
#endif
