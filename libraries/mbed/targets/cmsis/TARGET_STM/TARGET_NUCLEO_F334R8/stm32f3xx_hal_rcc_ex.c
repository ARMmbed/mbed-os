/**
  ******************************************************************************
  * @file    stm32f3xx_hal_rcc_ex.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    18-June-2014
  * @brief   Extended RCC HAL module driver
  *          This file provides firmware functions to manage the following 
  *          functionalities RCC extension peripheral:
  *           + Extended Clock Source configuration functions
  *  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
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
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/** @addtogroup STM32F3xx_HAL_Driver
  * @{
  */

/** @defgroup RCCEx
  * @brief RCC Extended HAL module driver.
  * @{
  */

#ifdef HAL_RCC_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup RCCEx_Private_Functions
  * @{
  */

/** @defgroup RCCEx_Group1 Extended Peripheral Control functions 
  * @brief    Extended Peripheral Control functions
  *
@verbatim   
 ===============================================================================
                ##### Extended Peripheral Control functions  #####
 ===============================================================================  
    [..]
    This subsection provides a set of functions allowing to control the RCC Clocks 
    frequencies.
    [..] 
    (@) Important note: Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to
        select the RTC clock source; in this case the Backup domain will be reset in  
        order to modify the RTC Clock source, as consequence RTC registers (including 
        the backup registers) and RCC_BDCR register are set to their reset values.
      
@endverbatim
  * @{
  */

/**
  * @brief  Initializes the RCC extended peripherals clocks according to the specified
  *         parameters in the RCC_PeriphCLKInitTypeDef.
  * @param  PeriphClkInit: pointer to an RCC_PeriphCLKInitTypeDef structure that
  *         contains the configuration information for the Extended Peripherals clocks
  *         (ADC, CEC, I2C, I2S, SDADC, HRTIM, TIM, USART, RTC and USB).
  *
  * @note   Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select 
  *         the RTC clock source; in this case the Backup domain will be reset in  
  *         order to modify the RTC Clock source, as consequence RTC registers (including 
  *         the backup registers) and RCC_BDCR register are set to their reset values.
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)
{
  uint32_t tickstart = 0;
  uint32_t tmpreg = 0;
    
  /* Check the parameters */
  assert_param(IS_RCC_PERIPHCLK(PeriphClkInit->PeriphClockSelection));
  
   
  /*---------------------------- RTC configuration -------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_RTC) == (RCC_PERIPHCLK_RTC))
  {
    /* Enable Power Clock*/
    __PWR_CLK_ENABLE();
    
    /* Enable write access to Backup domain */
    SET_BIT(PWR->CR, PWR_CR_DBP);
    
    /* Wait for Backup domain Write protection disable */
    tickstart = HAL_GetTick();
    
    while((PWR->CR & PWR_CR_DBP) == RESET)
    {
      if((HAL_GetTick()-tickstart) > DBP_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      }      
    }
    
    /* Reset the Backup domain only if the RTC Clock source selction is modified */ 
    if((RCC->BDCR & RCC_BDCR_RTCSEL) != (PeriphClkInit->RTCClockSelection & RCC_BDCR_RTCSEL))
    {
      /* Store the content of BDCR register before the reset of Backup Domain */
      tmpreg = (RCC->BDCR & ~(RCC_BDCR_RTCSEL));
      /* RTC Clock selection can be changed only if the Backup Domain is reset */
      __HAL_RCC_BACKUPRESET_FORCE();
      __HAL_RCC_BACKUPRESET_RELEASE();
      /* Restore the Content of BDCR register */
      RCC->BDCR = tmpreg;
    }
    
    /* If LSE is selected as RTC clock source, wait for LSE reactivation */
    if(PeriphClkInit->RTCClockSelection == RCC_RTCCLKSOURCE_LSE)
    {
      /* Get timeout */
      tickstart = HAL_GetTick();
      
      /* Wait till LSE is ready */  
      while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET)
      {
      if((HAL_GetTick()-tickstart) > LSE_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }      
      }  
    }
    __HAL_RCC_RTC_CONFIG(PeriphClkInit->RTCClockSelection); 
  }
  
  /*------------------------------- USART1 Configuration ------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART1) == RCC_PERIPHCLK_USART1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART1CLKSOURCE(PeriphClkInit->Usart1ClockSelection));
    
    /* Configure the USART1 clock source */
    __HAL_RCC_USART1_CONFIG(PeriphClkInit->Usart1ClockSelection);
  }
  
  /*----------------------------- USART2 Configuration --------------------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART2) == RCC_PERIPHCLK_USART2)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART2CLKSOURCE(PeriphClkInit->Usart2ClockSelection));
    
    /* Configure the USART2 clock source */
    __HAL_RCC_USART2_CONFIG(PeriphClkInit->Usart2ClockSelection);
  }
  
  /*------------------------------ USART3 Configuration ------------------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART3) == RCC_PERIPHCLK_USART3)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART3CLKSOURCE(PeriphClkInit->Usart3ClockSelection));
    
    /* Configure the USART3 clock source */
    __HAL_RCC_USART3_CONFIG(PeriphClkInit->Usart3ClockSelection);
  }

  /*------------------------------ I2C1 Configuration ------------------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2C1) == RCC_PERIPHCLK_I2C1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2C1CLKSOURCE(PeriphClkInit->I2c1ClockSelection));
    
    /* Configure the I2C1 clock source */
    __HAL_RCC_I2C1_CONFIG(PeriphClkInit->I2c1ClockSelection);
  }

#if defined(STM32F302x8) || defined(STM32F302xC) || \
    defined(STM32F303xC) || defined(STM32F373xC)

  /*------------------------------ USB Configuration ------------------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USB) == RCC_PERIPHCLK_USB)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USBCLKSOURCE(PeriphClkInit->USBClockSelection));
    
    /* Configure the USB clock source */
    __HAL_RCC_USB_CONFIG(PeriphClkInit->USBClockSelection);
  }

#endif /* STM32F302x8 || STM32F302xC || */
       /* STM32F303xC || STM32F373xC    */

#if defined(STM32F301x8) || defined(STM32F302x8) || defined(STM32F318xx) || \
    defined(STM32F302xC) || defined(STM32F303xC) || defined(STM32F358xx) || \
    defined(STM32F373xC) || defined(STM32F378xx)

  /*------------------------------ I2C2 Configuration ------------------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2C2) == RCC_PERIPHCLK_I2C2)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2C2CLKSOURCE(PeriphClkInit->I2c2ClockSelection));
    
    /* Configure the I2C2 clock source */
    __HAL_RCC_I2C2_CONFIG(PeriphClkInit->I2c2ClockSelection);
  }

#endif
  
#if defined(STM32F301x8) || defined(STM32F302x8) ||  defined(STM32F318xx)

  /*------------------------------ I2C3 Configuration ------------------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2C3) == RCC_PERIPHCLK_I2C3)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2C3CLKSOURCE(PeriphClkInit->I2c3ClockSelection));
    
    /* Configure the I2C3 clock source */
    __HAL_RCC_I2C3_CONFIG(PeriphClkInit->I2c3ClockSelection);
  }

#endif /* STM32F301x8 || STM32F302x8 || STM32F318xx */
  
#if defined(STM32F302xC) || defined(STM32F303xC) || defined(STM32F358xx)

  /*------------------------------ UART4 Configuration ------------------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_UART4) == RCC_PERIPHCLK_UART4)
  {
    /* Check the parameters */
    assert_param(IS_RCC_UART4CLKSOURCE(PeriphClkInit->Uart4ClockSelection));
    
    /* Configure the UART4 clock source */
    __HAL_RCC_UART4_CONFIG(PeriphClkInit->Uart4ClockSelection);
  }

  /*------------------------------ UART5 Configuration ------------------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_UART5) == RCC_PERIPHCLK_UART5)
  {
    /* Check the parameters */
    assert_param(IS_RCC_UART5CLKSOURCE(PeriphClkInit->Uart5ClockSelection));
    
    /* Configure the UART5 clock source */
    __HAL_RCC_UART5_CONFIG(PeriphClkInit->Uart5ClockSelection);
  }

#endif /* STM32F302xC || STM32F303xC || STM32F358xx */

#if defined(STM32F301x8) || defined(STM32F302x8) || defined(STM32F318xx) || \
    defined(STM32F302xC) || defined(STM32F303xC) || defined(STM32F358xx)

  /*------------------------------ I2S Configuration ------------------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2S) == RCC_PERIPHCLK_I2S)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2SCLKSOURCE(PeriphClkInit->I2sClockSelection));
    
    /* Configure the I2S clock source */
    __HAL_RCC_I2S_CONFIG(PeriphClkInit->I2sClockSelection);
  }

#endif /* STM32F301x8 || STM32F302x8 || STM32F318xx || STM32F302xC || STM32F303xC || STM32F358xx */
  
#if defined(STM32F301x8) || defined(STM32F302x8) || defined(STM32F318xx)
      
  /*------------------------------ ADC1 clock Configuration ------------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_ADC1) == RCC_PERIPHCLK_ADC1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_ADC1PLLCLK_DIV(PeriphClkInit->Adc1ClockSelection));
    
    /* Configure the ADC1 clock source */
    __HAL_RCC_ADC1_CONFIG(PeriphClkInit->Adc1ClockSelection);
  }

#endif /* STM32F301x8 || STM32F302x8 || STM32F318xx */
  
#if defined(STM32F302xC) || defined(STM32F303xC) || defined(STM32F358xx) || \
    defined(STM32F303x8) || defined(STM32F334x8) || defined(STM32F328xx)
      
  /*------------------------------ ADC1 & ADC2 clock Configuration -------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_ADC12) == RCC_PERIPHCLK_ADC12)
  {
    /* Check the parameters */
    assert_param(IS_RCC_ADC12PLLCLK_DIV(PeriphClkInit->Adc12ClockSelection));
    
    /* Configure the ADC12 clock source */
    __HAL_RCC_ADC12_CONFIG(PeriphClkInit->Adc12ClockSelection);
  }

#endif /* STM32F302xC || STM32F303xC || STM32F358xx || */
       /* STM32F303x8 || STM32F334x8 || STM32F328xx */    
  
#if defined(STM32F303xC) || defined(STM32F358xx)

  /*------------------------------ ADC3 & ADC4 clock Configuration -------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_ADC34) == RCC_PERIPHCLK_ADC34)
  {
    /* Check the parameters */
    assert_param(IS_RCC_ADC34PLLCLK_DIV(PeriphClkInit->Adc34ClockSelection));
    
    /* Configure the ADC34 clock source */
    __HAL_RCC_ADC34_CONFIG(PeriphClkInit->Adc34ClockSelection);
  }

#endif /* STM32F303xC || STM32F358xx */

#if defined(STM32F373xC) || defined(STM32F378xx)
      
  /*------------------------------ ADC1 clock Configuration ------------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_ADC1) == RCC_PERIPHCLK_ADC1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_ADC1PCLK2_DIV(PeriphClkInit->Adc1ClockSelection));
    
    /* Configure the ADC1 clock source */
    __HAL_RCC_ADC1_CONFIG(PeriphClkInit->Adc1ClockSelection);
  }

#endif /* STM32F373xC || STM32F378xx */

#if defined(STM32F301x8) || defined(STM32F302x8) || defined(STM32F318xx) || \
    defined(STM32F302xC) || defined(STM32F303xC) || defined(STM32F358xx) || \
    defined(STM32F303x8) || defined(STM32F334x8) || defined(STM32F328xx)

  /*------------------------------ TIM1 clock Configuration ----------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_TIM1) == RCC_PERIPHCLK_TIM1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_TIM1CLKSOURCE(PeriphClkInit->Tim1ClockSelection));
    
    /* Configure the TIM1 clock source */
    __HAL_RCC_TIM1_CONFIG(PeriphClkInit->Tim1ClockSelection);
  }

#endif /* STM32F301x8 || STM32F302x8 || STM32F318xx || */
       /* STM32F302xC || STM32F303xC || STM32F358xx || */
       /* STM32F303x8 || STM32F334x8 || STM32F328xx    */

#if defined(STM32F303xC) || defined(STM32F358xx)

  /*------------------------------ TIM8 clock Configuration ----------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_TIM8) == RCC_PERIPHCLK_TIM8)
  {
    /* Check the parameters */
    assert_param(IS_RCC_TIM8CLKSOURCE(PeriphClkInit->Tim8ClockSelection));
    
    /* Configure the TIM8 clock source */
    __HAL_RCC_TIM8_CONFIG(PeriphClkInit->Tim8ClockSelection);
  }

#endif /* STM32F303xC || STM32F358xx */

#if defined(STM32F301x8) || defined(STM32F302x8) || defined(STM32F318xx)

  /*------------------------------ TIM15 clock Configuration ----------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_TIM15) == RCC_PERIPHCLK_TIM15)
  {
    /* Check the parameters */
    assert_param(IS_RCC_TIM15CLKSOURCE(PeriphClkInit->Tim15ClockSelection));
    
    /* Configure the TIM15 clock source */
    __HAL_RCC_TIM15_CONFIG(PeriphClkInit->Tim15ClockSelection);
  }

  /*------------------------------ TIM16 clock Configuration ----------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_TIM16) == RCC_PERIPHCLK_TIM16)
  {
    /* Check the parameters */
    assert_param(IS_RCC_TIM16CLKSOURCE(PeriphClkInit->Tim16ClockSelection));
    
    /* Configure the TIM16 clock source */
    __HAL_RCC_TIM16_CONFIG(PeriphClkInit->Tim16ClockSelection);
  }

  /*------------------------------ TIM17 clock Configuration ----------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_TIM17) == RCC_PERIPHCLK_TIM17)
  {
    /* Check the parameters */
    assert_param(IS_RCC_TIM17CLKSOURCE(PeriphClkInit->Tim17ClockSelection));
    
    /* Configure the TIM17 clock source */
    __HAL_RCC_TIM17_CONFIG(PeriphClkInit->Tim17ClockSelection);
  }

#endif /* STM32F301x8 || STM32F302x8 || STM32F318xx */

#if defined(STM32F334x8)

  /*------------------------------ HRTIM1 clock Configuration ----------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_HRTIM1) == RCC_PERIPHCLK_HRTIM1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_HRTIM1CLKSOURCE(PeriphClkInit->Hrtim1ClockSelection));
    
    /* Configure the HRTIM1 clock source */
    __HAL_RCC_HRTIM1_CONFIG(PeriphClkInit->Hrtim1ClockSelection);
  }

#endif /* STM32F334x8 */

#if defined(STM32F373xC) || defined(STM32F378xx)
  
  /*------------------------------ SDADC clock Configuration -------------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SDADC) == RCC_PERIPHCLK_SDADC)
  {
    /* Check the parameters */
    assert_param(IS_RCC_SDADCSYSCLK_DIV(PeriphClkInit->SdadcClockSelection));
    
    /* Configure the SDADC clock prescaler */
    __HAL_RCC_SDADC_CONFIG(PeriphClkInit->SdadcClockSelection);
  }

  /*------------------------------ CEC clock Configuration -------------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_CEC) == RCC_PERIPHCLK_CEC)
  {
    /* Check the parameters */
    assert_param(IS_RCC_CECCLKSOURCE(PeriphClkInit->CecClockSelection));
    
    /* Configure the CEC clock source */
    __HAL_RCC_CEC_CONFIG(PeriphClkInit->CecClockSelection);
  }

#endif /* STM32F373xC || STM32F378xx */
  
  return HAL_OK;
}

/**
  * @brief  Get the RCC_ClkInitStruct according to the internal
  * RCC configuration registers.
  * @param  PeriphClkInit: pointer to an RCC_PeriphCLKInitTypeDef structure that
  *         returns the configuration information for the Extended Peripherals clocks
  *         (ADC, CEC, I2C, I2S, SDADC, HRTIM, TIM, USART, RTC and USB clocks).
  * @retval None
  */
void HAL_RCCEx_GetPeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)
{
  /* Set all possible values for the extended clock type parameter------------*/
  /* Common part first */
  PeriphClkInit->PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_USART3 | \
                                        RCC_PERIPHCLK_I2C1   | RCC_PERIPHCLK_RTC;
  
  /* Get the RTC configuration --------------------------------------------*/
  PeriphClkInit->RTCClockSelection = __HAL_RCC_GET_RTC_SOURCE();
  /* Get the USART1 clock configuration --------------------------------------------*/
  PeriphClkInit->Usart1ClockSelection = __HAL_RCC_GET_USART1_SOURCE();
  /* Get the USART2 clock configuration -----------------------------------------*/
  PeriphClkInit->Usart2ClockSelection = __HAL_RCC_GET_USART2_SOURCE();
   /* Get the USART3 clock configuration -----------------------------------------*/
  PeriphClkInit->Usart3ClockSelection = __HAL_RCC_GET_USART3_SOURCE();
  /* Get the I2C1 clock configuration -----------------------------------------*/
  PeriphClkInit->I2c1ClockSelection = __HAL_RCC_GET_I2C1_SOURCE();

#if defined(STM32F302x8) || defined(STM32F302xC) || \
    defined(STM32F303xC) || defined(STM32F373xC)

  PeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_USB;
  /* Get the USB clock configuration -----------------------------------------*/
  PeriphClkInit->USBClockSelection = __HAL_RCC_GET_USB_SOURCE();

#endif /* STM32F302x8 || STM32F302xC || */
       /* STM32F303xC || STM32F373xC    */

#if defined(STM32F301x8) || defined(STM32F302x8) || defined(STM32F318xx) || \
    defined(STM32F302xC) || defined(STM32F303xC) || defined(STM32F358xx) || \
    defined(STM32F373xC) || defined(STM32F378xx)

  PeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_I2C2;
  /* Get the I2C2 clock configuration -----------------------------------------*/
  PeriphClkInit->I2c2ClockSelection = __HAL_RCC_GET_I2C2_SOURCE();

#endif /* STM32F301x8 || STM32F302x8 || STM32F318xx || STM32F302xC || STM32F303xC || STM32F358xx || */
       /* STM32F373xC || STM32F378xx */
  
#if defined(STM32F301x8) || defined(STM32F302x8) || defined(STM32F318xx)

  PeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_I2C3;
  /* Get the I2C3 clock configuration -----------------------------------------*/
  PeriphClkInit->I2c3ClockSelection = __HAL_RCC_GET_I2C3_SOURCE();

#endif /* STM32F301x8 || STM32F302x8 || STM32F318xx */
  
#if defined(STM32F302xC) || defined(STM32F303xC) || defined(STM32F358xx)

  PeriphClkInit->PeriphClockSelection |= (RCC_PERIPHCLK_UART4  | RCC_PERIPHCLK_UART5);
  /* Get the UART4 clock configuration -----------------------------------------*/
  PeriphClkInit->Uart4ClockSelection = __HAL_RCC_GET_UART4_SOURCE();
  /* Get the UART5 clock configuration -----------------------------------------*/
  PeriphClkInit->Uart5ClockSelection = __HAL_RCC_GET_UART5_SOURCE();

#endif /* STM32F302xC || STM32F303xC || STM32F358xx */

#if defined(STM32F301x8) || defined(STM32F302x8) || defined(STM32F318xx) || \
    defined(STM32F302xC) || defined(STM32F303xC) || defined(STM32F358xx)

  PeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_I2S;
  /* Get the I2S clock configuration -----------------------------------------*/
  PeriphClkInit->I2sClockSelection = __HAL_RCC_GET_I2S_SOURCE();

#endif /* STM32F301x8 || STM32F302x8 || STM32F318xx || STM32F302xC || STM32F303xC || STM32F358xx */
  
#if defined(STM32F301x8) || defined(STM32F302x8) || defined(STM32F318xx) || \
    defined(STM32F373xC) || defined(STM32F378xx)
      
  PeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_ADC1;
  /* Get the ADC1 clock configuration -----------------------------------------*/
  PeriphClkInit->Adc1ClockSelection = __HAL_RCC_GET_ADC1_SOURCE();

#endif /* STM32F301x8 || STM32F302x8 || STM32F318xx || STM32F373xC || STM32F378xx */

#if defined(STM32F302xC) || defined(STM32F303xC) || defined(STM32F358xx) || \
    defined(STM32F303x8) || defined(STM32F334x8) || defined(STM32F328xx)

  PeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_ADC12;
  /* Get the ADC1 & ADC2 clock configuration -----------------------------------------*/
  PeriphClkInit->Adc12ClockSelection = __HAL_RCC_GET_ADC12_SOURCE();

#endif /* STM32F302xC || STM32F303xC || STM32F358xx  || */
       /* STM32F303x8 || STM32F334x8 || STM32F328xx */

#if defined(STM32F303xC) || defined(STM32F358xx)

  PeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_ADC34;
   /* Get the ADC3 & ADC4 clock configuration -----------------------------------------*/
  PeriphClkInit->Adc34ClockSelection = __HAL_RCC_GET_ADC34_SOURCE();

#endif /* STM32F303xC || STM32F358xx */

#if defined(STM32F301x8) || defined(STM32F302x8) || defined(STM32F318xx) || \
    defined(STM32F302xC) || defined(STM32F303xC) || defined(STM32F358xx) || \
    defined(STM32F303x8) || defined(STM32F334x8) || defined(STM32F328xx)

  PeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_TIM1;
  /* Get the TIM1 clock configuration -----------------------------------------*/
  PeriphClkInit->Tim1ClockSelection = __HAL_RCC_GET_TIM1_SOURCE();

#endif /* STM32F301x8 || STM32F302x8 || STM32F318xx || */
       /* STM32F302xC || STM32F303xC || STM32F358xx || */
       /* STM32F303x8 || STM32F334x8 || STM32F328xx    */

#if defined(STM32F301x8) || defined(STM32F302x8) || defined(STM32F318xx)

  PeriphClkInit->PeriphClockSelection |= (RCC_PERIPHCLK_TIM15 | RCC_PERIPHCLK_TIM16 | RCC_PERIPHCLK_TIM17);
  /* Get the TIM15 clock configuration -----------------------------------------*/
  PeriphClkInit->Tim15ClockSelection = __HAL_RCC_GET_TIM15_SOURCE();
  /* Get the TIM16 clock configuration -----------------------------------------*/
  PeriphClkInit->Tim16ClockSelection = __HAL_RCC_GET_TIM16_SOURCE();
  /* Get the TIM17 clock configuration -----------------------------------------*/
  PeriphClkInit->Tim17ClockSelection = __HAL_RCC_GET_TIM17_SOURCE();

#endif /* STM32F301x8 || STM32F302x8 || STM32F318xx */

#if defined(STM32F303xC) || defined(STM32F358xx)

  PeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_TIM8;
  /* Get the TIM8 clock configuration -----------------------------------------*/
  PeriphClkInit->Tim8ClockSelection = __HAL_RCC_GET_TIM8_SOURCE();

#endif /* STM32F303xC || STM32F358xx */

#if defined(STM32F334x8)

  PeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_HRTIM1;
  /* Get the HRTIM1 clock configuration -----------------------------------------*/
  PeriphClkInit->Hrtim1ClockSelection = __HAL_RCC_GET_HRTIM1_SOURCE();

#endif /* STM32F334x8 */

#if defined(STM32F373xC) || defined(STM32F378xx)

  PeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_SDADC;
  /* Get the SDADC clock configuration -----------------------------------------*/
  PeriphClkInit->SdadcClockSelection = __HAL_RCC_GET_SDADC_SOURCE();

  PeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_CEC;
  /* Get the CEC clock configuration -----------------------------------------*/
  PeriphClkInit->CecClockSelection = __HAL_RCC_GET_CEC_SOURCE();

#endif /* STM32F373xC || STM32F378xx */

}

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_RCC_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
