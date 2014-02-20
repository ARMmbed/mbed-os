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

static void SetSysClock_HSI(void)
{
  __IO uint32_t StartUpCounter = 0, HSIStatus = 0;
  
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
  /* Enable HSI */
  RCC->CR |= ((uint32_t)RCC_CR_HSION);
 
  /* Wait till HSI is ready and if Time out is reached exit */
  do
  {
    HSIStatus = RCC->CR & RCC_CR_HSIRDY;
  } while((HSIStatus == 0) && (StartUpCounter != HSI_STARTUP_TIMEOUT));

  if ((RCC->CR & RCC_CR_HSIRDY) != RESET)
  {
    HSIStatus = (uint32_t)0x01;
  }
  else
  {
    HSIStatus = (uint32_t)0x00;
  }
    
  if (HSIStatus == (uint32_t)0x01)
  {
    /* Flash 0 wait state */
    FLASH->ACR &= ~FLASH_ACR_LATENCY;
    
    /* Disable Prefetch Buffer */
    FLASH->ACR &= ~FLASH_ACR_PRFTEN;

    /* Disable 64-bit access */
    FLASH->ACR &= ~FLASH_ACR_ACC64;
    
    /* Power enable */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  
    /* Select the Voltage Range 1 (1.8 V) */
    PWR->CR = PWR_CR_VOS_0;
  
    /* Wait Until the Voltage Regulator is ready */
    while((PWR->CSR & PWR_CSR_VOSF) != RESET)
    {
    }
      
    /* HCLK = SYSCLK /1*/
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
    /* PCLK2 = HCLK /1*/
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
    
    /* PCLK1 = HCLK /1*/
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;
    
    /* Select HSI as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI;

    /* Wait till HSI is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_HSI)
    {
    }
  }
  else
  {
    /* If HSI fails to start-up, the application will have wrong clock
       configuration. User can add here some code to deal with this error */
  }
}

// MCU SLEEP mode
void sleep(void)
{
    // Enable PWR clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);    

    // Request to enter SLEEP mode with regulator ON
    PWR_EnterSleepMode(PWR_Regulator_ON, PWR_SLEEPEntry_WFI);
}

// MCU STOP mode (Regulator in LP mode, LSI, HSI and HSE OFF)
void deepsleep(void)
{    
    // Enable PWR clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    
    // Enable Ultra low power mode
    PWR_UltraLowPowerCmd(ENABLE);

    // Enter Stop Mode
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);  
  
    // After wake-up from STOP reconfigure the system clock (HSI)
    SetSysClock_HSI();
      
}
