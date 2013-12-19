/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2014, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
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
