/**
  ******************************************************************************
  * @file    stm32f0xx_hal_rcc_ex.c
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    26-June-2015
  * @brief   Extended RCC HAL module driver
  *          This file provides firmware functions to manage the following 
  *          functionalities RCC extension peripheral:
  *           + Extended Clock Source configuration functions
  *  
  @verbatim
  ==============================================================================
                      ##### How to use this driver #####
  ==============================================================================

      For CRS, RCC Extention HAL driver can be used as follows:

      (#) In System clock config, HSI48 need to be enabled

      (#] Enable CRS clock in IP MSP init which will use CRS functions

      (#) Call CRS functions like this
          (##) Prepare synchronization configuration necessary for HSI48 calibration
              (+++) Default values can be set for frequency Error Measurement (reload and error limit)
                        and also HSI48 oscillator smooth trimming.
              (+++) Macro __HAL_RCC_CRS_CALCULATE_RELOADVALUE can be also used to calculate 
                        directly reload value with target and sychronization frequencies values
          (##) Call function HAL_RCCEx_CRSConfig which
              (+++) Reset CRS registers to their default values.
              (+++) Configure CRS registers with synchronization configuration 
              (+++) Enable automatic calibration and frequency error counter feature

          (##) A polling function is provided to wait for complete Synchronization
              (+++) Call function HAL_RCCEx_CRSWaitSynchronization()
              (+++) According to CRS status, user can decide to adjust again the calibration or continue
                        application if synchronization is OK
              
      (#) User can retrieve information related to synchronization in calling function
            HAL_RCCEx_CRSGetSynchronizationInfo()

      (#) Regarding synchronization status and synchronization information, user can try a new calibration
           in changing synchronization configuration and call again HAL_RCCEx_CRSConfig.
           Note: When the SYNC event is detected during the downcounting phase (before reaching the zero value), 
           it means that the actual frequency is lower than the target (and so, that the TRIM value should be 
           incremented), while when it is detected during the upcounting phase it means that the actual frequency 
           is higher (and that the TRIM value should be decremented).

      (#) To use IT mode, user needs to handle it in calling different macros available to do it
            (__HAL_RCC_CRS_XXX_IT). Interuptions will go through RCC Handler (RCC_IRQn/RCC_CRS_IRQHandler)
              (++) Call function HAL_RCCEx_CRSConfig()
              (++) Enable RCC_IRQn (thnaks to NVIC functions)
              (++) Enable CRS IT (__HAL_RCC_CRS_ENABLE_IT)
              (++) Implement CRS status management in RCC_CRS_IRQHandler

      (#) To force a SYNC EVENT, user can use function HAL_RCCEx_CRSSoftwareSynchronizationGenerate(). Function can be 
            called before calling HAL_RCCEx_CRSConfig (for instance in Systick handler)
            
  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
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
#include "stm32f0xx_hal.h"

/** @addtogroup STM32F0xx_HAL_Driver
  * @{
  */

#ifdef HAL_RCC_MODULE_ENABLED

/** @defgroup RCCEx RCCEx
  * @brief RCC Extension HAL module driver.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup RCCEx_Private_Constants RCCEx Private Constants
  * @{
  */
/* Bit position in register */
#define CRS_CFGR_FELIM_BITNUMBER    16
#define CRS_CR_TRIM_BITNUMBER       8
#define CRS_ISR_FECAP_BITNUMBER     16
/**
  * @}
  */
  
/* Private macro -------------------------------------------------------------*/
/** @defgroup RCCEx_Private_Macros RCCEx Private Macros
  * @{
  */
/**
  * @}
  */
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/

/** @defgroup RCCEx_Exported_Functions RCCEx Exported Functions
  * @{
  */

/** @defgroup RCCEx_Exported_Functions_Group1 Extended Peripheral Control functions 
 *  @brief  Extended RCC clocks control functions 
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
  *         (USART, RTC, I2C, CEC and USB).
  *
  * @note   Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select 
  *         the RTC clock source; in this case the Backup domain will be reset in  
  *         order to modify the RTC Clock source, as consequence RTC registers (including 
  *         the backup registers) and RCC_BDCR register are set to their reset values.
  *
  * @retval None
  */
HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)
{
  uint32_t tickstart = 0;
  uint32_t temp_reg = 0;

  /* Check the parameters */
  assert_param(IS_RCC_PERIPHCLK(PeriphClkInit->PeriphClockSelection));
  
  /*---------------------------- RTC configuration -------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_RTC) == (RCC_PERIPHCLK_RTC))
  {
    /* Reset the Backup domain only if the RTC Clock source selction is modified */ 
    if((RCC->BDCR & RCC_BDCR_RTCSEL) != (PeriphClkInit->RTCClockSelection & RCC_BDCR_RTCSEL))
    {
      /* Enable Power Clock*/
      __HAL_RCC_PWR_CLK_ENABLE();
      
      /* Enable write access to Backup domain */
      SET_BIT(PWR->CR, PWR_CR_DBP);
      
      /* Wait for Backup domain Write protection disable */
      tickstart = HAL_GetTick();
      
      while((PWR->CR & PWR_CR_DBP) == RESET)
      {
        if((HAL_GetTick() - tickstart) > RCC_DBP_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }      
      }
      
      /* Store the content of BDCR register before the reset of Backup Domain */
      temp_reg = (RCC->BDCR & ~(RCC_BDCR_RTCSEL));
      /* RTC Clock selection can be changed only if the Backup Domain is reset */
      __HAL_RCC_BACKUPRESET_FORCE();
      __HAL_RCC_BACKUPRESET_RELEASE();
      /* Restore the Content of BDCR register */
      RCC->BDCR = temp_reg;

      /* Wait for LSERDY if LSE was enabled */
      if (HAL_IS_BIT_SET(temp_reg, RCC_BDCR_LSERDY))
      {
        /* Get timeout */
        tickstart = HAL_GetTick();
      
        /* Wait till LSE is ready */  
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET)
        {
          if((HAL_GetTick() - tickstart) > RCC_LSE_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }      
        }  
      }
      __HAL_RCC_RTC_CONFIG(PeriphClkInit->RTCClockSelection); 
    }
  }
  
  /*------------------------------- USART1 Configuration ------------------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART1) == RCC_PERIPHCLK_USART1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART1CLKSOURCE(PeriphClkInit->Usart1ClockSelection));
    
    /* Configure the USART1 clock source */
    __HAL_RCC_USART1_CONFIG(PeriphClkInit->Usart1ClockSelection);
  }
  
#if defined(STM32F071xB) || defined(STM32F072xB) || defined(STM32F078xx)\
 || defined(STM32F091xC) || defined(STM32F098xx)
  /*----------------------------- USART2 Configuration --------------------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART2) == RCC_PERIPHCLK_USART2)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART2CLKSOURCE(PeriphClkInit->Usart2ClockSelection));
    
    /* Configure the USART2 clock source */
    __HAL_RCC_USART2_CONFIG(PeriphClkInit->Usart2ClockSelection);
  }
#endif /* STM32F071xB || STM32F072xB || STM32F078xx || */
       /* STM32F091xC || STM32F098xx */

#if defined(STM32F091xC) || defined(STM32F098xx)
  /*----------------------------- USART3 Configuration --------------------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART3) == RCC_PERIPHCLK_USART3)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART3CLKSOURCE(PeriphClkInit->Usart3ClockSelection));
    
    /* Configure the USART3 clock source */
    __HAL_RCC_USART3_CONFIG(PeriphClkInit->Usart3ClockSelection);
  }
#endif /* STM32F091xC || STM32F098xx */  

  /*------------------------------ I2C1 Configuration ------------------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2C1) == RCC_PERIPHCLK_I2C1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2C1CLKSOURCE(PeriphClkInit->I2c1ClockSelection));
    
    /* Configure the I2C1 clock source */
    __HAL_RCC_I2C1_CONFIG(PeriphClkInit->I2c1ClockSelection);
  }

#if defined(STM32F042x6) || defined(STM32F048xx) || defined(STM32F072xB) || defined(STM32F078xx) || defined(STM32F070xB) || defined(STM32F070x6)
  /*------------------------------ USB Configuration ------------------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USB) == RCC_PERIPHCLK_USB)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USBCLKSOURCE(PeriphClkInit->UsbClockSelection));
    
    /* Configure the USB clock source */
    __HAL_RCC_USB_CONFIG(PeriphClkInit->UsbClockSelection);
  }
#endif /* STM32F042x6 || STM32F048xx || STM32F072xB || STM32F078xx || STM32F070xB || STM32F070x6 */

#if defined(STM32F042x6) || defined(STM32F048xx)\
 || defined(STM32F051x8) || defined(STM32F058xx)\
 || defined(STM32F071xB) || defined(STM32F072xB) || defined(STM32F078xx)\
 || defined(STM32F091xC) || defined(STM32F098xx)
  /*------------------------------ CEC clock Configuration -------------------*/ 
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_CEC) == RCC_PERIPHCLK_CEC)
  {
    /* Check the parameters */
    assert_param(IS_RCC_CECCLKSOURCE(PeriphClkInit->CecClockSelection));
    
    /* Configure the CEC clock source */
    __HAL_RCC_CEC_CONFIG(PeriphClkInit->CecClockSelection);
  }
#endif /* STM32F042x6 || STM32F048xx ||                */
       /* STM32F051x8 || STM32F058xx ||                */
       /* STM32F071xB || STM32F072xB || STM32F078xx || */
       /* STM32F091xC || STM32F098xx */
  
  return HAL_OK;
}

/**
  * @brief  Get the RCC_ClkInitStruct according to the internal
  * RCC configuration registers.
  * @param  PeriphClkInit: pointer to an RCC_PeriphCLKInitTypeDef structure that
  *         returns the configuration information for the Extended Peripherals clocks
  *         (USART, RTC, I2C, CEC and USB).
  * @retval None
  */
void HAL_RCCEx_GetPeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)
{
  /* Set all possible values for the extended clock type parameter------------*/
  /* Common part first */
  PeriphClkInit->PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_I2C1   | RCC_PERIPHCLK_RTC;  
  /* Get the RTC configuration --------------------------------------------*/
  PeriphClkInit->RTCClockSelection = __HAL_RCC_GET_RTC_SOURCE();
  /* Get the USART1 configuration --------------------------------------------*/
  PeriphClkInit->Usart1ClockSelection = __HAL_RCC_GET_USART1_SOURCE();
  /* Get the I2C1 clock source -----------------------------------------------*/
  PeriphClkInit->I2c1ClockSelection = __HAL_RCC_GET_I2C1_SOURCE();

#if defined(STM32F071xB) || defined(STM32F072xB) || defined(STM32F078xx)\
 || defined(STM32F091xC) || defined(STM32F098xx)
  PeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_USART2;
  /* Get the USART2 clock source ---------------------------------------------*/
  PeriphClkInit->Usart2ClockSelection = __HAL_RCC_GET_USART2_SOURCE();
#endif /* STM32F071xB || STM32F072xB || STM32F078xx || */
       /* STM32F091xC || STM32F098xx */

#if defined(STM32F091xC) || defined(STM32F098xx)
  PeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_USART3;
  /* Get the USART3 clock source ---------------------------------------------*/
  PeriphClkInit->Usart3ClockSelection = __HAL_RCC_GET_USART3_SOURCE();
#endif /* STM32F091xC || STM32F098xx */

#if defined(STM32F042x6) || defined(STM32F048xx) || defined(STM32F072xB) || defined(STM32F078xx) || defined(STM32F070xB) || defined(STM32F070x6)
  PeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_USB;
  /* Get the USB clock source ---------------------------------------------*/
  PeriphClkInit->UsbClockSelection = __HAL_RCC_GET_USB_SOURCE();
#endif /* STM32F042x6 || STM32F048xx || STM32F072xB || STM32F078xx || STM32F070xB || STM32F070x6 */

#if defined(STM32F042x6) || defined(STM32F048xx)\
 || defined(STM32F051x8) || defined(STM32F058xx)\
 || defined(STM32F071xB) || defined(STM32F072xB) || defined(STM32F078xx)\
 || defined(STM32F091xC) || defined(STM32F098xx)
  PeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_CEC;
  /* Get the CEC clock source ------------------------------------------------*/
  PeriphClkInit->CecClockSelection = __HAL_RCC_GET_CEC_SOURCE();
#endif /* STM32F042x6 || STM32F048xx ||                */
       /* STM32F051x8 || STM32F058xx ||                */
       /* STM32F071xB || STM32F072xB || STM32F078xx || */
       /* STM32F091xC || STM32F098xx */

}

/**
  * @brief  Returns the peripheral clock frequency
  * @note   Returns 0 if peripheral clock is unknown
  * @param  PeriphClk: Peripheral clock identifier
  *         This parameter can be one of the following values:
  *            @arg RCC_PERIPHCLK_RTC     RTC peripheral clock
  *            @arg RCC_PERIPHCLK_USART1  USART1 peripheral clock
  *            @arg RCC_PERIPHCLK_USART2  USART2 peripheral clock (*)
  *            @arg RCC_PERIPHCLK_USART3  USART3 peripheral clock (*)
  *            @arg RCC_PERIPHCLK_I2C1    I2C1 peripheral clock
  *            @arg RCC_PERIPHCLK_USB     USB peripheral clock (*)
  *            @arg RCC_PERIPHCLK_CEC     CEC peripheral clock (*)
  * @note   (*) means that this peripheral is not present on all the STM32F0xx devices
  * @retval Frequency in Hz (0: means that no available frequency for the peripheral)
  */
uint32_t HAL_RCCEx_GetPeriphCLKFreq(uint32_t PeriphClk)
{
  uint32_t frequency = 0;
  uint32_t srcclk = 0;
#if defined(USB)
  uint32_t pllmull = 0, pllsource = 0, predivfactor = 0;
#endif /* USB */

  /* Check the parameters */
  assert_param(IS_RCC_PERIPHCLK(PeriphClk));
  
  switch (PeriphClk)
  {
  case RCC_PERIPHCLK_RTC:
    {
      /* Get the current RTC source */
      srcclk = __HAL_RCC_GET_RTC_SOURCE();

      /* Check if LSE is ready and if RTC clock selection is LSE */
      if ((srcclk == RCC_RTCCLKSOURCE_LSE) && (HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)))
      {
        frequency = LSE_VALUE;
      }
      /* Check if LSI is ready and if RTC clock selection is LSI */
      else if ((srcclk == RCC_RTCCLKSOURCE_LSI) && (HAL_IS_BIT_SET(RCC->CSR, RCC_CSR_LSIRDY)))
      {
        frequency = LSI_VALUE;
      }
      /* Check if HSE is ready  and if RTC clock selection is HSI_DIV32*/
      else if ((srcclk == RCC_RTCCLKSOURCE_HSE_DIV32) && (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY)))
      {
        frequency = HSE_VALUE / 32;
      }
      /* Clock not enabled for RTC*/
      else
      {
        frequency = 0;
      }
      break;
    }
  case RCC_PERIPHCLK_USART1:
    {
      /* Get the current USART1 source */
      srcclk = __HAL_RCC_GET_USART1_SOURCE();

      /* Check if USART1 clock selection is PCLK1 */
      if (srcclk == RCC_USART1CLKSOURCE_PCLK1)
      {
        frequency = HAL_RCC_GetPCLK1Freq();
      }
      /* Check if HSI is ready and if USART1 clock selection is HSI */
      else if ((srcclk == RCC_USART1CLKSOURCE_HSI) && (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)))
      {
        frequency = HSI_VALUE;
      }
      /* Check if USART1 clock selection is SYSCLK */
      else if (srcclk == RCC_USART1CLKSOURCE_SYSCLK)
      {
        frequency = HAL_RCC_GetSysClockFreq();
      }
      /* Check if LSE is ready  and if USART1 clock selection is LSE */
      else if ((srcclk == RCC_USART1CLKSOURCE_LSE) && (HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)))
      {
        frequency = LSE_VALUE;
      }
      /* Clock not enabled for USART1*/
      else
      {
        frequency = 0;
      }
      break;
    }
#if defined(RCC_CFGR3_USART2SW)
  case RCC_PERIPHCLK_USART2:
    {
      /* Get the current USART2 source */
      srcclk = __HAL_RCC_GET_USART2_SOURCE();

      /* Check if USART2 clock selection is PCLK1 */
      if (srcclk == RCC_USART2CLKSOURCE_PCLK1)
      {
        frequency = HAL_RCC_GetPCLK1Freq();
      }
      /* Check if HSI is ready and if USART2 clock selection is HSI */
      else if ((srcclk == RCC_USART2CLKSOURCE_HSI) && (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)))
      {
        frequency = HSI_VALUE;
      }
      /* Check if USART2 clock selection is SYSCLK */
      else if (srcclk == RCC_USART2CLKSOURCE_SYSCLK)
      {
        frequency = HAL_RCC_GetSysClockFreq();
      }
      /* Check if LSE is ready  and if USART2 clock selection is LSE */
      else if ((srcclk == RCC_USART2CLKSOURCE_LSE) && (HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)))
      {
        frequency = LSE_VALUE;
      }
      /* Clock not enabled for USART2*/
      else
      {
        frequency = 0;
      }
      break;
    }
#endif /* RCC_CFGR3_USART2SW */
#if defined(RCC_CFGR3_USART3SW)
  case RCC_PERIPHCLK_USART3:
    {
      /* Get the current USART3 source */
      srcclk = __HAL_RCC_GET_USART3_SOURCE();

      /* Check if USART3 clock selection is PCLK1 */
      if (srcclk == RCC_USART3CLKSOURCE_PCLK1)
      {
        frequency = HAL_RCC_GetPCLK1Freq();
      }
      /* Check if HSI is ready and if USART3 clock selection is HSI */
      else if ((srcclk == RCC_USART3CLKSOURCE_HSI) && (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)))
      {
        frequency = HSI_VALUE;
      }
      /* Check if USART3 clock selection is SYSCLK */
      else if (srcclk == RCC_USART3CLKSOURCE_SYSCLK)
      {
        frequency = HAL_RCC_GetSysClockFreq();
      }
      /* Check if LSE is ready  and if USART3 clock selection is LSE */
      else if ((srcclk == RCC_USART3CLKSOURCE_LSE) && (HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)))
      {
        frequency = LSE_VALUE;
      }
      /* Clock not enabled for USART3*/
      else
      {
        frequency = 0;
      }
      break;
    }
#endif /* RCC_CFGR3_USART3SW */
  case RCC_PERIPHCLK_I2C1:
    {
      /* Get the current I2C1 source */
      srcclk = __HAL_RCC_GET_I2C1_SOURCE();

      /* Check if HSI is ready and if I2C1 clock selection is HSI */
      if ((srcclk == RCC_I2C1CLKSOURCE_HSI) && (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)))
      {
        frequency = HSI_VALUE;
      }
      /* Check if I2C1 clock selection is SYSCLK */
      else if (srcclk == RCC_I2C1CLKSOURCE_SYSCLK)
      {
        frequency = HAL_RCC_GetSysClockFreq();
      }
      /* Clock not enabled for I2C1*/
      else
      {
        frequency = 0;
      }
      break;
    }
#if defined(USB)
  case RCC_PERIPHCLK_USB:
    {
      /* Get the current USB source */
      srcclk = __HAL_RCC_GET_USB_SOURCE();

      /* Check if PLL is ready and if USB clock selection is PLL */
      if ((srcclk == RCC_USBCLKSOURCE_PLLCLK) && (HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLLRDY)))
      {
        /* Get PLL clock source and multiplication factor ----------------------*/
        pllmull      = RCC->CFGR & RCC_CFGR_PLLMUL;
        pllsource    = RCC->CFGR & RCC_CFGR_PLLSRC;
        pllmull      = (pllmull >> RCC_CFGR_PLLMUL_BITNUMBER) + 2;
        predivfactor = (RCC->CFGR2 & RCC_CFGR2_PREDIV) + 1;

        if (pllsource == RCC_CFGR_PLLSRC_HSE_PREDIV)
        {
          /* HSE used as PLL clock source : frequency = HSE/PREDIV * PLLMUL */
          frequency = (HSE_VALUE/predivfactor) * pllmull;
        }
#if defined(RCC_CR2_HSI48ON)
        else if (pllsource == RCC_CFGR_PLLSRC_HSI48_PREDIV)
        {
          /* HSI48 used as PLL clock source : frequency = HSI48/PREDIV * PLLMUL */
          frequency = (HSI48_VALUE / predivfactor) * pllmull;
        }
#endif /* RCC_CR2_HSI48ON */
        else
        {
#if defined(STM32F042x6) || defined(STM32F048xx) || defined(STM32F078xx) || defined(STM32F072xB) || defined(STM32F070xB)
          /* HSI used as PLL clock source : frequency = HSI/PREDIV * PLLMUL */
          frequency = (HSI_VALUE / predivfactor) * pllmull;
#else
          /* HSI used as PLL clock source : frequency = HSI/2 * PLLMUL */
          frequency = (HSI_VALUE >> 1) * pllmull;
#endif /* STM32F042x6 || STM32F048xx || STM32F072xB || STM32F078xx || STM32F070xB */
        }
      }
#if defined(RCC_CR2_HSI48ON)
      /* Check if HSI48 is ready and if USB clock selection is HSI48 */
      else if ((srcclk == RCC_USBCLKSOURCE_HSI48) && (HAL_IS_BIT_SET(RCC->CR2, RCC_CR2_HSI48RDY)))
      {
        frequency = HSI48_VALUE;
      }
#endif /* RCC_CR2_HSI48ON */
      /* Clock not enabled for USB*/
      else
      {
        frequency = 0;
      }
      break;
    }
#endif /* USB */
#if defined(CEC)
  case RCC_PERIPHCLK_CEC:
    {
      /* Get the current CEC source */
      srcclk = __HAL_RCC_GET_CEC_SOURCE();

      /* Check if HSI is ready and if CEC clock selection is HSI */
      if ((srcclk == RCC_CECCLKSOURCE_HSI) && (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)))
      {
        frequency = HSI_VALUE;
      }
      /* Check if LSE is ready  and if CEC clock selection is LSE */
      else if ((srcclk == RCC_CECCLKSOURCE_LSE) && (HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)))
      {
        frequency = LSE_VALUE;
      }
      /* Clock not enabled for CEC */
      else
      {
        frequency = 0;
      }
      break;
    }
#endif /* CEC */
  default: 
    {
      break;
    }
  }
  return(frequency);
}

#if defined(STM32F042x6) || defined(STM32F048xx)\
 || defined(STM32F071xB) || defined(STM32F072xB) || defined(STM32F078xx)\
 || defined(STM32F091xC) || defined(STM32F098xx)
/**
  * @brief  Start automatic synchronization using polling mode
  * @param  pInit Pointer on RCC_CRSInitTypeDef structure
  * @retval None
  */
void HAL_RCCEx_CRSConfig(RCC_CRSInitTypeDef *pInit)
{
  /* Check the parameters */
  assert_param(IS_RCC_CRS_SYNC_DIV(pInit->Prescaler));
  assert_param(IS_RCC_CRS_SYNC_SOURCE(pInit->Source));
  assert_param(IS_RCC_CRS_SYNC_POLARITY(pInit->Polarity));
  assert_param(IS_RCC_CRS_RELOADVALUE(pInit->ReloadValue));
  assert_param(IS_RCC_CRS_ERRORLIMIT(pInit->ErrorLimitValue));
  assert_param(IS_RCC_CRS_HSI48CALIBRATION(pInit->HSI48CalibrationValue));


  /* CONFIGURATION */

  /* Before configuration, reset CRS registers to their default values*/
  __HAL_RCC_CRS_FORCE_RESET();
  __HAL_RCC_CRS_RELEASE_RESET();

  /* Configure Synchronization input */
  /* Clear SYNCDIV[2:0], SYNCSRC[1:0] & SYNCSPOL bits */
  CRS->CFGR &= ~(CRS_CFGR_SYNCDIV | CRS_CFGR_SYNCSRC | CRS_CFGR_SYNCPOL);

  /* Set the CRS_CFGR_SYNCDIV[2:0] bits according to Prescaler value */
  CRS->CFGR |= pInit->Prescaler;

  /* Set the SYNCSRC[1:0] bits according to Source value */
  CRS->CFGR |= pInit->Source;

  /* Set the SYNCSPOL bits according to Polarity value */
  CRS->CFGR |= pInit->Polarity;

  /* Configure Frequency Error Measurement */
  /* Clear RELOAD[15:0] & FELIM[7:0] bits*/
  CRS->CFGR &= ~(CRS_CFGR_RELOAD | CRS_CFGR_FELIM);

  /* Set the RELOAD[15:0] bits according to ReloadValue value */
  CRS->CFGR |= pInit->ReloadValue;

  /* Set the FELIM[7:0] bits according to ErrorLimitValue value */
  CRS->CFGR |= (pInit->ErrorLimitValue << CRS_CFGR_FELIM_BITNUMBER);

  /* Adjust HSI48 oscillator smooth trimming */
  /* Clear TRIM[5:0] bits */
  CRS->CR &= ~CRS_CR_TRIM;

  /* Set the TRIM[5:0] bits according to RCC_CRS_HSI48CalibrationValue value */
  CRS->CR |= (pInit->HSI48CalibrationValue << CRS_CR_TRIM_BITNUMBER);


  /* START AUTOMATIC SYNCHRONIZATION*/
  
  /* Enable Automatic trimming */
  __HAL_RCC_CRS_ENABLE_AUTOMATIC_CALIB();

  /* Enable Frequency error counter */
  __HAL_RCC_CRS_ENABLE_FREQ_ERROR_COUNTER();

}

/**
  * @brief  Generate the software synchronization event
  * @retval None
  */
void HAL_RCCEx_CRSSoftwareSynchronizationGenerate(void)
{
  CRS->CR |= CRS_CR_SWSYNC;
}


/**
  * @brief  Function to return synchronization info 
  * @param  pSynchroInfo Pointer on RCC_CRSSynchroInfoTypeDef structure
  * @retval None
  */
void HAL_RCCEx_CRSGetSynchronizationInfo(RCC_CRSSynchroInfoTypeDef *pSynchroInfo)
{
  /* Check the parameter */
  assert_param(pSynchroInfo != NULL);
  
  /* Get the reload value */
  pSynchroInfo->ReloadValue = (uint32_t)(CRS->CFGR & CRS_CFGR_RELOAD);
  
  /* Get HSI48 oscillator smooth trimming */
  pSynchroInfo->HSI48CalibrationValue = (uint32_t)((CRS->CR & CRS_CR_TRIM) >> CRS_CR_TRIM_BITNUMBER);

  /* Get Frequency error capture */
  pSynchroInfo->FreqErrorCapture = (uint32_t)((CRS->ISR & CRS_ISR_FECAP) >> CRS_ISR_FECAP_BITNUMBER);

  /* Get FFrequency error direction */
  pSynchroInfo->FreqErrorDirection = (uint32_t)(CRS->ISR & CRS_ISR_FEDIR);
  
  
}

/**
* @brief This function handles CRS Synchronization Timeout.
* @param Timeout: Duration of the timeout
* @note  Timeout is based on the maximum time to receive a SYNC event based on synchronization
*        frequency.
* @note    If Timeout set to HAL_MAX_DELAY, HAL_TIMEOUT will be never returned.
* @retval Combination of Synchronization status
*          This parameter can be a combination of the following values:
*            @arg RCC_CRS_TIMEOUT
*            @arg RCC_CRS_SYNCOK
*            @arg RCC_CRS_SYNCWARM
*            @arg RCC_CRS_SYNCERR
*            @arg RCC_CRS_SYNCMISS
*            @arg RCC_CRS_TRIMOV
*/
uint32_t HAL_RCCEx_CRSWaitSynchronization(uint32_t Timeout)
{
  uint32_t crsstatus = RCC_CRS_NONE;
  uint32_t tickstart = 0;
  
  /* Get timeout */
  tickstart = HAL_GetTick();
  
  /* Check that if one of CRS flags have been set */
  while(RCC_CRS_NONE == crsstatus)
  {
    if(Timeout != HAL_MAX_DELAY)
    {
      if((Timeout == 0) || ((HAL_GetTick() - tickstart) > Timeout))
      {
        crsstatus = RCC_CRS_TIMEOUT;
      }
    }
    /* Check CRS SYNCOK flag  */
    if(__HAL_RCC_CRS_GET_FLAG(RCC_CRS_FLAG_SYNCOK))
    {
      /* CRS SYNC event OK */
      crsstatus |= RCC_CRS_SYNCOK;
    
      /* Clear CRS SYNC event OK bit */
      __HAL_RCC_CRS_CLEAR_FLAG(RCC_CRS_FLAG_SYNCOK);
    }
    
    /* Check CRS SYNCWARN flag  */
    if(__HAL_RCC_CRS_GET_FLAG(RCC_CRS_FLAG_SYNCWARN))
    {
      /* CRS SYNC warning */
      crsstatus |= RCC_CRS_SYNCWARM;
    
      /* Clear CRS SYNCWARN bit */
      __HAL_RCC_CRS_CLEAR_FLAG(RCC_CRS_FLAG_SYNCWARN);
    }
    
    /* Check CRS TRIM overflow flag  */
    if(__HAL_RCC_CRS_GET_FLAG(RCC_CRS_FLAG_TRIMOVF))
    {
      /* CRS SYNC Error */
      crsstatus |= RCC_CRS_TRIMOV;
    
      /* Clear CRS Error bit */
      __HAL_RCC_CRS_CLEAR_FLAG(RCC_CRS_FLAG_TRIMOVF);
    }
    
    /* Check CRS Error flag  */
    if(__HAL_RCC_CRS_GET_FLAG(RCC_CRS_FLAG_SYNCERR))
    {
      /* CRS SYNC Error */
      crsstatus |= RCC_CRS_SYNCERR;
    
      /* Clear CRS Error bit */
      __HAL_RCC_CRS_CLEAR_FLAG(RCC_CRS_FLAG_SYNCERR);
    }
    
    /* Check CRS SYNC Missed flag  */
    if(__HAL_RCC_CRS_GET_FLAG(RCC_CRS_FLAG_SYNCMISS))
    {
      /* CRS SYNC Missed */
      crsstatus |= RCC_CRS_SYNCMISS;
    
      /* Clear CRS SYNC Missed bit */
      __HAL_RCC_CRS_CLEAR_FLAG(RCC_CRS_FLAG_SYNCMISS);
    }
    
    /* Check CRS Expected SYNC flag  */
    if(__HAL_RCC_CRS_GET_FLAG(RCC_CRS_FLAG_ESYNC))
    {
      /* frequency error counter reached a zero value */
      __HAL_RCC_CRS_CLEAR_FLAG(RCC_CRS_FLAG_ESYNC);
    }
  }
  
  return crsstatus;
}
          
#endif /* STM32F042x6 || STM32F048xx ||                */
       /* STM32F071xB || STM32F072xB || STM32F078xx || */
       /* STM32F091xC || STM32F098xx */

/**
  * @}
  */

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
