/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "sleep_api.h"
#include "cmsis.h"

static void SYSCLKConfig_STOP(void)
{
    ErrorStatus HSEStartUpStatus;
  
    RCC_HSEConfig(RCC_HSE_ON); // Enable HSE
  
    HSEStartUpStatus = RCC_WaitForHSEStartUp(); // Wait till HSE is ready

    if (HSEStartUpStatus == SUCCESS) {
        RCC_PLLCmd(ENABLE); // Enable PLL
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {} // Wait till PLL is ready
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); // Select PLL as system clock source
        while(RCC_GetSYSCLKSource() != 0x08) {} // Wait till PLL is used as system clock source
    }
}

void sleep(void)
{
    SCB->SCR = 0; // Normal sleep mode for ARM core
    __WFI();
}

void deepsleep(void)
{    
    // Enable PWR clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    
    // Request to enter STOP mode with regulator in low power mode
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);  
    
    // At this stage the system has resumed from STOP mode.
    // Re-configure the system clock: enable HSE, PLL and select 
    // PLL as system clock source (because HSE and PLL are disabled in STOP mode).
    SYSCLKConfig_STOP();
}
