/**
  ******************************************************************************
  * @file    stm32l4xx_hal_rcc_ex.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    26-June-2015
  * @brief   Extended RCC HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities RCC extended peripheral:
  *           + Extended Peripheral Control functions
  *
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
#include "stm32l4xx_hal.h"

/** @addtogroup STM32L4xx_HAL_Driver
  * @{
  */

/** @defgroup RCCEx RCCEx
  * @brief RCC Extended HAL module driver
  * @{
  */

#ifdef HAL_RCC_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/** @defgroup RCCEx_Private_Constants RCCEx Private Constants
 * @{
 */
#define PLLSAI1_TIMEOUT_VALUE    1000U /* Timeout value fixed to 100 ms  */
#define PLLSAI2_TIMEOUT_VALUE    1000U /* Timeout value fixed to 100 ms  */
#define PLL_TIMEOUT_VALUE        100U /* Timeout value fixed to 100 ms  */

#define __LSCO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define LSCO_GPIO_PORT        GPIOA
#define LSCO_PIN              GPIO_PIN_2
/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup RCCEx_Private_Functions RCCEx Private Functions
 * @{
 */
static HAL_StatusTypeDef RCCEx_PLLSAI1_ConfigNP(RCC_PLLSAI1InitTypeDef *PllSai1);
static HAL_StatusTypeDef RCCEx_PLLSAI1_ConfigNQ(RCC_PLLSAI1InitTypeDef *PllSai1);
static HAL_StatusTypeDef RCCEx_PLLSAI1_ConfigNR(RCC_PLLSAI1InitTypeDef *PllSai1);
static HAL_StatusTypeDef RCCEx_PLLSAI2_ConfigNP(RCC_PLLSAI2InitTypeDef *PllSai2);
static HAL_StatusTypeDef RCCEx_PLLSAI2_ConfigNR(RCC_PLLSAI2InitTypeDef *PllSai2);
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/** @defgroup RCCEx_Exported_Functions RCCEx Exported Functions
  * @{
  */

/** @defgroup RCCEx_Exported_Functions_Group1 Extended Peripheral Control functions 
 *  @brief  Extended Peripheral Control functions
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
  * @brief  Initialize the RCC extended peripherals clocks according to the specified
  *         parameters in the RCC_PeriphCLKInitTypeDef.
  * @param  PeriphClkInit: pointer to an RCC_PeriphCLKInitTypeDef structure that
  *         contains the configuration information for the Extended Peripherals
  *         clocks(SAI1, SAI2, LPTIM1, LPTIM2, I2C1, I2C2, I2C3, LPUART,
  *         USART1, USART2, USART3, UART4, UART5, RTC, ADC1, DFSDM, SWPMI1 and USB).
  *
  * @note   Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select
  *         the RTC clock source: in this case the access to Backup domain is enabled.
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)
{
  uint32_t tmpreg = 0;
  uint32_t tickstart = 0;
  HAL_StatusTypeDef ret = HAL_OK;      /* Intermediate status */
  HAL_StatusTypeDef status = HAL_OK;   /* Final status */

  /* Check the parameters */
  assert_param(IS_RCC_PERIPHCLOCK(PeriphClkInit->PeriphClockSelection));

  /*-------------------------- SAI1 clock source configuration ---------------------*/
  if((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI1) == RCC_PERIPHCLK_SAI1))
  {
    switch(PeriphClkInit->Sai1ClockSelection)
    {
    case RCC_SAI1CLKSOURCE_PLL:      /* PLL is used as clock source for SAI1*/
      /* Enable SAI Clock output generated form System PLL . */
      __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLL_SAI3CLK);
      /* SAI1 clock source config set later after clock selection check */
      break;

    case RCC_SAI1CLKSOURCE_PLLSAI1: /* PLLSAI1 is used as clock source for SAI1*/
      /* PLLSAI1 parameters N & P configuration and clock output (PLLSAI1ClockOut) */
      ret = RCCEx_PLLSAI1_ConfigNP(&(PeriphClkInit->PLLSAI1));
      /* SAI1 clock source config set later after clock selection check */
      break;

    case RCC_SAI1CLKSOURCE_PLLSAI2:  /* PLLSAI2 is used as clock source for SAI1*/
      /* PLLSAI2 parameters N & P configuration and clock output (PLLSAI2ClockOut) */
      ret = RCCEx_PLLSAI2_ConfigNP(&(PeriphClkInit->PLLSAI2));
      /* SAI1 clock source config set later after clock selection check */
      break;

    case RCC_SAI1CLKSOURCE_PIN:      /* External clock is used as source of SAI1 clock*/
      /* SAI1 clock source config set later after clock selection check */
      break;

    default:
      ret = HAL_ERROR;
      break;
    }

    if(ret == HAL_OK)
    {
      /* Set the source of SAI1 clock*/
      __HAL_RCC_SAI1_CONFIG(PeriphClkInit->Sai1ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }

  /*-------------------------- SAI2 clock source configuration ---------------------*/
  if((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI2) == RCC_PERIPHCLK_SAI2))
  {
    switch(PeriphClkInit->Sai2ClockSelection)
    {
    case RCC_SAI2CLKSOURCE_PLL:      /* PLL is used as clock source for SAI2*/
      /* Enable SAI Clock output generated form System PLL . */
      __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLL_SAI3CLK);
      /* SAI2 clock source config set later after clock selection check */
      break;

    case RCC_SAI2CLKSOURCE_PLLSAI1: /* PLLSAI1 is used as clock source for SAI2*/
      /* PLLSAI1 parameters N & P configuration and clock output (PLLSAI1ClockOut) */
      ret = RCCEx_PLLSAI1_ConfigNP(&(PeriphClkInit->PLLSAI1));
      /* SAI2 clock source config set later after clock selection check */
      break;

    case RCC_SAI2CLKSOURCE_PLLSAI2:  /* PLLSAI2 is used as clock source for SAI2*/
      /* PLLSAI2 parameters N & P configuration and clock output (PLLSAI2ClockOut) */
      ret = RCCEx_PLLSAI2_ConfigNP(&(PeriphClkInit->PLLSAI2));
      /* SAI2 clock source config set later after clock selection check */
      break;

    case RCC_SAI2CLKSOURCE_PIN:      /* External clock is used as source of SAI2 clock*/
      /* SAI2 clock source config set later after clock selection check */
      break;

    default:
      ret = HAL_ERROR;
      break;
    }

    if(ret == HAL_OK)
    {
      /* Set the source of SAI2 clock*/
      __HAL_RCC_SAI2_CONFIG(PeriphClkInit->Sai2ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }

  /*-------------------------- RTC clock source configuration ----------------------*/
  if((PeriphClkInit->PeriphClockSelection & RCC_PERIPHCLK_RTC) == RCC_PERIPHCLK_RTC)
  {
    FlagStatus       pwrclkchanged = RESET;
    
    /* Check for RTC Parameters used to output RTCCLK */
    assert_param(IS_RCC_RTCCLKSOURCE(PeriphClkInit->RTCClockSelection));
    
    /* Reset the Backup domain only if the RTC Clock source selection is modified */
    if(READ_BIT(RCC->BDCR, RCC_BDCR_RTCSEL) != PeriphClkInit->RTCClockSelection)
    {
      /* Enable Power Clock */
      if(__HAL_RCC_PWR_IS_CLK_DISABLED())
      {
        __HAL_RCC_PWR_CLK_ENABLE();
        pwrclkchanged = SET;
      }
      
      /* Enable write access to Backup domain */
      SET_BIT(PWR->CR1, PWR_CR1_DBP);

      /* Wait for Backup domain Write protection disable */
      tickstart = HAL_GetTick();

      while((PWR->CR1 & PWR_CR1_DBP) == RESET)
      {
        if((HAL_GetTick() - tickstart) > RCC_DBP_TIMEOUT_VALUE)
        {
          ret = HAL_TIMEOUT;
          break;
        }
      }

      if(ret == HAL_OK)
      {
        /* Store the content of BDCR register before the reset of Backup Domain */
        tmpreg = READ_BIT(RCC->BDCR, ~(RCC_BDCR_RTCSEL));
        /* RTC Clock selection can be changed only if the Backup Domain is reset */
        __HAL_RCC_BACKUPRESET_FORCE();
        __HAL_RCC_BACKUPRESET_RELEASE();
        /* Restore the Content of BDCR register */
        RCC->BDCR = tmpreg;
      }

      /* Wait for LSE reactivation if LSE was enable prior to Backup Domain reset */
      if (HAL_IS_BIT_SET(tmpreg, RCC_BDCR_LSERDY))
      {
        /* Get Start Tick*/
        tickstart = HAL_GetTick();

        /* Wait till LSE is ready */
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET)
        {
          if((HAL_GetTick() - tickstart) > RCC_LSE_TIMEOUT_VALUE)
          {
            ret = HAL_TIMEOUT;
            break;
          }
        }
      }
      
      if(ret == HAL_OK)
      {
        /* Apply new RTC clock source selection */
        __HAL_RCC_RTC_CONFIG(PeriphClkInit->RTCClockSelection);
        
        /* Restore clock configuration if changed */
        if(pwrclkchanged == SET)
        {
          __HAL_RCC_PWR_CLK_DISABLE();
        }
      }
      else
      {
        /* set overall return value */
        status = ret;
      }
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }

  /*-------------------------- USART1 clock source configuration -------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART1) == RCC_PERIPHCLK_USART1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART1CLKSOURCE(PeriphClkInit->Usart1ClockSelection));

    /* Configure the USART1 clock source */
    __HAL_RCC_USART1_CONFIG(PeriphClkInit->Usart1ClockSelection);
  }

  /*-------------------------- USART2 clock source configuration -------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART2) == RCC_PERIPHCLK_USART2)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART2CLKSOURCE(PeriphClkInit->Usart2ClockSelection));

    /* Configure the USART2 clock source */
    __HAL_RCC_USART2_CONFIG(PeriphClkInit->Usart2ClockSelection);
  }

  /*-------------------------- USART3 clock source configuration -------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART3) == RCC_PERIPHCLK_USART3)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART3CLKSOURCE(PeriphClkInit->Usart3ClockSelection));

    /* Configure the USART3 clock source */
    __HAL_RCC_USART3_CONFIG(PeriphClkInit->Usart3ClockSelection);
  }

  /*-------------------------- UART4 clock source configuration --------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_UART4) == RCC_PERIPHCLK_UART4)
  {
    /* Check the parameters */
    assert_param(IS_RCC_UART4CLKSOURCE(PeriphClkInit->Uart4ClockSelection));

    /* Configure the UART4 clock source */
    __HAL_RCC_UART4_CONFIG(PeriphClkInit->Uart4ClockSelection);
  }

  /*-------------------------- UART5 clock source configuration --------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_UART5) == RCC_PERIPHCLK_UART5)
  {
    /* Check the parameters */
    assert_param(IS_RCC_UART5CLKSOURCE(PeriphClkInit->Uart5ClockSelection));

    /* Configure the UART5 clock source */
    __HAL_RCC_UART5_CONFIG(PeriphClkInit->Uart5ClockSelection);
  }

  /*-------------------------- LPUART1 clock source configuration ------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LPUART1) == RCC_PERIPHCLK_LPUART1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_LPUART1CLKSOURCE(PeriphClkInit->Lpuart1ClockSelection));

    /* Configure the LPUAR1 clock source */
    __HAL_RCC_LPUART1_CONFIG(PeriphClkInit->Lpuart1ClockSelection);
  }

  /*-------------------------- LPTIM1 clock source configuration -------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LPTIM1) == (RCC_PERIPHCLK_LPTIM1))
  {
    assert_param(IS_RCC_LPTIM1CLK(PeriphClkInit->Lptim1ClockSelection));
    __HAL_RCC_LPTIM1_CONFIG(PeriphClkInit->Lptim1ClockSelection);
  }

  /*-------------------------- LPTIM2 clock source configuration -------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LPTIM2) == (RCC_PERIPHCLK_LPTIM2))
  {
    assert_param(IS_RCC_LPTIM2CLK(PeriphClkInit->Lptim2ClockSelection));
    __HAL_RCC_LPTIM2_CONFIG(PeriphClkInit->Lptim2ClockSelection);
  }

  /*-------------------------- I2C1 clock source configuration ---------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2C1) == RCC_PERIPHCLK_I2C1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2C1CLKSOURCE(PeriphClkInit->I2c1ClockSelection));

    /* Configure the I2C1 clock source */
    __HAL_RCC_I2C1_CONFIG(PeriphClkInit->I2c1ClockSelection);
  }

  /*-------------------------- I2C2 clock source configuration ---------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2C2) == RCC_PERIPHCLK_I2C2)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2C2CLKSOURCE(PeriphClkInit->I2c2ClockSelection));

    /* Configure the I2C2 clock source */
    __HAL_RCC_I2C2_CONFIG(PeriphClkInit->I2c2ClockSelection);
  }

  /*-------------------------- I2C3 clock source configuration ---------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2C3) == RCC_PERIPHCLK_I2C3)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2C3CLKSOURCE(PeriphClkInit->I2c3ClockSelection));

    /* Configure the I2C3 clock source */
    __HAL_RCC_I2C3_CONFIG(PeriphClkInit->I2c3ClockSelection);
  }

#if defined(STM32L475xx) || defined(STM32L476xx) || defined(STM32L485xx) || defined(STM32L486xx)
  /*-------------------------- USB clock source configuration ----------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USB) == (RCC_PERIPHCLK_USB))
  {
    assert_param(IS_RCC_USBCLKSOURCE(PeriphClkInit->UsbClockSelection));
    __HAL_RCC_USB_CONFIG(PeriphClkInit->UsbClockSelection);

    if(PeriphClkInit->UsbClockSelection == RCC_USBCLKSOURCE_PLL)
    {
      /* Enable PLL48M1CLK output */
      __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLL_48M1CLK);
    }
    else if(PeriphClkInit->UsbClockSelection == RCC_USBCLKSOURCE_PLLSAI1)
    {
      /* PLLSAI1 parameters N & Q configuration and clock output (PLLSAI1ClockOut) */
      ret = RCCEx_PLLSAI1_ConfigNQ(&(PeriphClkInit->PLLSAI1));
      
      if(ret != HAL_OK)
      {
        /* set overall return value */
        status = ret;
      }
    }
  }
#endif /* STM32L475xx || STM32L476xx || STM32L485xx || STM32L486xx */

  /*-------------------------- SDMMC1 clock source configuration -------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SDMMC1) == (RCC_PERIPHCLK_SDMMC1))
  {
    assert_param(IS_RCC_SDMMC1CLKSOURCE(PeriphClkInit->Sdmmc1ClockSelection));
    __HAL_RCC_SDMMC1_CONFIG(PeriphClkInit->Sdmmc1ClockSelection);
    
    if(PeriphClkInit->Sdmmc1ClockSelection == RCC_SDMMC1CLKSOURCE_PLL)
    {
      /* Enable PLL48M1CLK output */
      __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLL_48M1CLK);
    }
    else if(PeriphClkInit->Sdmmc1ClockSelection == RCC_SDMMC1CLKSOURCE_PLLSAI1)
    {
      /* PLLSAI1 parameters N & Q configuration and clock output (PLLSAI1ClockOut) */
      ret = RCCEx_PLLSAI1_ConfigNQ(&(PeriphClkInit->PLLSAI1));
      
      if(ret != HAL_OK)
      {
        /* set overall return value */
        status = ret;
      }
    }
  }

  /*-------------------------- RNG clock source configuration ----------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_RNG) == (RCC_PERIPHCLK_RNG))
  {
    assert_param(IS_RCC_RNGCLKSOURCE(PeriphClkInit->RngClockSelection));
    __HAL_RCC_RNG_CONFIG(PeriphClkInit->RngClockSelection);

    if(PeriphClkInit->RngClockSelection == RCC_RNGCLKSOURCE_PLL)
    {
      /* Enable PLL48M1CLK output */
      __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLL_48M1CLK);
    }
    else if(PeriphClkInit->RngClockSelection == RCC_RNGCLKSOURCE_PLLSAI1)
    {
      /* PLLSAI1 parameters N & Q configuration and clock output (PLLSAI1ClockOut) */
      ret = RCCEx_PLLSAI1_ConfigNQ(&(PeriphClkInit->PLLSAI1));
      
      if(ret != HAL_OK)
      {
        /* set overall return value */
        status = ret;
      }
    }
  }

  /*-------------------------- ADC clock source configuration ----------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_ADC) == RCC_PERIPHCLK_ADC)
  {
    /* Check the parameters */
    assert_param(IS_RCC_ADCCLKSOURCE(PeriphClkInit->AdcClockSelection));

    /* Configure the ADC interface clock source */
    __HAL_RCC_ADC_CONFIG(PeriphClkInit->AdcClockSelection);
    
    if(PeriphClkInit->AdcClockSelection == RCC_ADCCLKSOURCE_PLLSAI1)
    {
      /* PLLSAI1 parameters N & R configuration and clock output (PLLSAI1ClockOut) */
      ret = RCCEx_PLLSAI1_ConfigNR(&(PeriphClkInit->PLLSAI1));
    }
    else if(PeriphClkInit->AdcClockSelection == RCC_ADCCLKSOURCE_PLLSAI2)
    {
      /* PLLSAI2 parameters N & R configuration and clock output (PLLSAI2ClockOut) */
      ret = RCCEx_PLLSAI2_ConfigNR(&(PeriphClkInit->PLLSAI2));
      
      if(ret != HAL_OK)
      {
        /* set overall return value */
        status = ret;
      }
    }
  }

  /*-------------------------- SWPMI1 clock source configuration -------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SWPMI1) == RCC_PERIPHCLK_SWPMI1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_SWPMI1CLKSOURCE(PeriphClkInit->Swpmi1ClockSelection));

    /* Configure the SWPMI1 clock source */
    __HAL_RCC_SWPMI1_CONFIG(PeriphClkInit->Swpmi1ClockSelection);
  }

  /*-------------------------- DFSDM clock source configuration --------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_DFSDM) == RCC_PERIPHCLK_DFSDM)
  {
    /* Check the parameters */
    assert_param(IS_RCC_DFSDMCLKSOURCE(PeriphClkInit->DfsdmClockSelection));

    /* Configure the DFSDM interface clock source */
    __HAL_RCC_DFSDM_CONFIG(PeriphClkInit->DfsdmClockSelection);
  }

  return status;
}

/**
  * @brief  Get the RCC_ClkInitStruct according to the internal RCC configuration registers.
  * @param  PeriphClkInit: pointer to an RCC_PeriphCLKInitTypeDef structure that
  *         returns the configuration information for the Extended Peripherals
  *         clocks(SAI1, SAI2, LPTIM1, LPTIM2, I2C1, I2C2, I2C3, LPUART,
  *         USART1, USART2, USART3, UART4, UART5, RTC, ADCx, DFSDMx, SWPMI1, USB, SDMMC1 and RNG).
  * @retval None
  */
void HAL_RCCEx_GetPeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)
{
  /* Set all possible values for the extended clock type parameter------------*/
#if defined(STM32L475xx) || defined(STM32L476xx) || defined(STM32L485xx) || defined(STM32L486xx)  
  PeriphClkInit->PeriphClockSelection = RCC_PERIPHCLK_USART1  | RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_USART3 | RCC_PERIPHCLK_UART4 | RCC_PERIPHCLK_UART5  | \
                                        RCC_PERIPHCLK_LPUART1 | RCC_PERIPHCLK_I2C1   | RCC_PERIPHCLK_I2C2   | RCC_PERIPHCLK_I2C3  | RCC_PERIPHCLK_LPTIM1 | \
                                        RCC_PERIPHCLK_LPTIM2  | RCC_PERIPHCLK_SAI1   | RCC_PERIPHCLK_SAI2   | RCC_PERIPHCLK_USB   | RCC_PERIPHCLK_SDMMC1 | \
                                        RCC_PERIPHCLK_RNG     | RCC_PERIPHCLK_ADC    | RCC_PERIPHCLK_SWPMI1 | RCC_PERIPHCLK_DFSDM | RCC_PERIPHCLK_RTC ;
#else
  PeriphClkInit->PeriphClockSelection = RCC_PERIPHCLK_USART1  | RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_USART3 | RCC_PERIPHCLK_UART4 | RCC_PERIPHCLK_UART5  | \
                                        RCC_PERIPHCLK_LPUART1 | RCC_PERIPHCLK_I2C1   | RCC_PERIPHCLK_I2C2   | RCC_PERIPHCLK_I2C3  | RCC_PERIPHCLK_LPTIM1 | \
                                        RCC_PERIPHCLK_LPTIM2  | RCC_PERIPHCLK_SAI1   | RCC_PERIPHCLK_SAI2                         | RCC_PERIPHCLK_SDMMC1 | \
                                        RCC_PERIPHCLK_RNG     | RCC_PERIPHCLK_ADC    | RCC_PERIPHCLK_SWPMI1 | RCC_PERIPHCLK_DFSDM | RCC_PERIPHCLK_RTC ;
#endif /* STM32L475xx || STM32L476xx || STM32L485xx || STM32L486xx */

  /* Get the PLLSAI1 Clock configuration -----------------------------------------------*/
  PeriphClkInit->PLLSAI1.PLLSAI1N = (uint32_t)((RCC->PLLSAI1CFGR & RCC_PLLSAI1CFGR_PLLSAI1N) >> POSITION_VAL(RCC_PLLSAI1CFGR_PLLSAI1N));
  PeriphClkInit->PLLSAI1.PLLSAI1P = (uint32_t)(((RCC->PLLSAI1CFGR & RCC_PLLSAI1CFGR_PLLSAI1P) >> POSITION_VAL(RCC_PLLSAI1CFGR_PLLSAI1P)) << 4)+7;
  PeriphClkInit->PLLSAI1.PLLSAI1R = (uint32_t)(((RCC->PLLSAI1CFGR & RCC_PLLSAI1CFGR_PLLSAI1R) >> POSITION_VAL(RCC_PLLSAI1CFGR_PLLSAI1R))+1)* 2;
  PeriphClkInit->PLLSAI1.PLLSAI1Q = (uint32_t)(((RCC->PLLSAI1CFGR & RCC_PLLSAI1CFGR_PLLSAI1Q) >> POSITION_VAL(RCC_PLLSAI1CFGR_PLLSAI1Q))+1)* 2;
   /* Get the PLLSAI2 Clock configuration -----------------------------------------------*/
  PeriphClkInit->PLLSAI2.PLLSAI2N = (uint32_t)((RCC->PLLSAI2CFGR & RCC_PLLSAI2CFGR_PLLSAI2N) >> POSITION_VAL(RCC_PLLSAI2CFGR_PLLSAI2N));
  PeriphClkInit->PLLSAI2.PLLSAI2P = (uint32_t)(((RCC->PLLSAI2CFGR & RCC_PLLSAI2CFGR_PLLSAI2P) >> POSITION_VAL(RCC_PLLSAI2CFGR_PLLSAI2P)) << 4)+7;
  PeriphClkInit->PLLSAI2.PLLSAI2R = (uint32_t)(((RCC->PLLSAI2CFGR & RCC_PLLSAI2CFGR_PLLSAI2R)>> POSITION_VAL(RCC_PLLSAI2CFGR_PLLSAI2R))+1)* 2;

  /* Get the USART1 clock source ---------------------------------------------*/
  PeriphClkInit->Usart1ClockSelection  = __HAL_RCC_GET_USART1_SOURCE();
  /* Get the USART2 clock source ---------------------------------------------*/
  PeriphClkInit->Usart2ClockSelection  = __HAL_RCC_GET_USART2_SOURCE();
  /* Get the USART3 clock source ---------------------------------------------*/
  PeriphClkInit->Usart3ClockSelection  = __HAL_RCC_GET_USART3_SOURCE();
  /* Get the UART4 clock source ----------------------------------------------*/
  PeriphClkInit->Uart4ClockSelection   = __HAL_RCC_GET_UART4_SOURCE();
  /* Get the UART5 clock source ----------------------------------------------*/
  PeriphClkInit->Uart5ClockSelection   = __HAL_RCC_GET_UART5_SOURCE();
  /* Get the LPUART1 clock source --------------------------------------------*/
  PeriphClkInit->Lpuart1ClockSelection = __HAL_RCC_GET_LPUART1_SOURCE();
  /* Get the I2C1 clock source -----------------------------------------------*/
  PeriphClkInit->I2c1ClockSelection    = __HAL_RCC_GET_I2C1_SOURCE();
   /* Get the I2C2 clock source ----------------------------------------------*/
  PeriphClkInit->I2c2ClockSelection    = __HAL_RCC_GET_I2C2_SOURCE();
  /* Get the I2C3 clock source -----------------------------------------------*/
  PeriphClkInit->I2c3ClockSelection    = __HAL_RCC_GET_I2C3_SOURCE();
  /* Get the LPTIM1 clock source ---------------------------------------------*/
  PeriphClkInit->Lptim1ClockSelection  = __HAL_RCC_GET_LPTIM1_SOURCE();
  /* Get the LPTIM2 clock source ---------------------------------------------*/
  PeriphClkInit->Lptim2ClockSelection  = __HAL_RCC_GET_LPTIM2_SOURCE();
  /* Get the SAI1 clock source -----------------------------------------------*/
  PeriphClkInit->Sai1ClockSelection    = __HAL_RCC_GET_SAI1_SOURCE();
  /* Get the SAI2 clock source -----------------------------------------------*/
  PeriphClkInit->Sai2ClockSelection    = __HAL_RCC_GET_SAI2_SOURCE();
  /* Get the RTC clock source ------------------------------------------------*/
  PeriphClkInit->RTCClockSelection     = __HAL_RCC_GET_RTC_SOURCE();

#if defined(STM32L475xx) || defined(STM32L476xx) || defined(STM32L485xx) || defined(STM32L486xx)
  /* Get the USB clock source ------------------------------------------------*/
  PeriphClkInit->UsbClockSelection     = __HAL_RCC_GET_USB_SOURCE();
#endif /* STM32L475xx || STM32L476xx || STM32L485xx || STM32L486xx */
  
  /* Get the SDMMC1 clock source ---------------------------------------------*/
  PeriphClkInit->Sdmmc1ClockSelection  = __HAL_RCC_GET_SDMMC1_SOURCE();
  /* Get the RNG clock source ------------------------------------------------*/
  PeriphClkInit->RngClockSelection     = __HAL_RCC_GET_RNG_SOURCE();
   /* Get the ADC clock source -----------------------------------------------*/
  PeriphClkInit->AdcClockSelection     = __HAL_RCC_GET_ADC_SOURCE();
  /* Get the SWPMI1 clock source ----------------------------------------------*/
  PeriphClkInit->Swpmi1ClockSelection  = __HAL_RCC_GET_SWPMI1_SOURCE();
  /* Get the DFSDM clock source -------------------------------------------*/
  PeriphClkInit->DfsdmClockSelection   = __HAL_RCC_GET_DFSDM_SOURCE();
}

/**
  * @brief  Return the peripheral clock frequency for peripherals with clock source from PLLSAIs 
  * @note   Return 0 if peripheral clock identifier not managed by this API
  * @param  PeriphClk: Peripheral clock identifier
  *         This parameter can be one of the following values:
  *            @arg RCC_PERIPHCLK_SAI1:  SAI1 peripheral clock
  *            @arg RCC_PERIPHCLK_SAI2:  SAI2 peripheral clock
  *            @arg RCC_PERIPHCLK_ADC:   ADC peripheral clock
  *            @arg RCC_PERIPHCLK_USB: USB, RNG, SDMMC1 peripheral clock
  * @retval Frequency in KHz
  */
uint32_t HAL_RCCEx_GetPeriphCLKFreq(uint32_t PeriphClk)
{
  uint32_t frequency = 0;
  uint32_t pllvco = 0, plln = 0;

  /* Compute PLL clock input */
  if(__HAL_RCC_GET_PLL_OSCSOURCE() == RCC_PLLSOURCE_MSI)
  {
    pllvco = (1 << ((__HAL_RCC_GET_MSI_RANGE() >> 4) - 4)) * 1000000;
  }
  else if(__HAL_RCC_GET_PLL_OSCSOURCE() == RCC_PLLSOURCE_HSI)
  {
    pllvco = HSI_VALUE;
  }
  else /* HSE source */
  {
    pllvco = HSE_VALUE;
  }

  /* f(PLL Source) / PLLM */
  pllvco = (pllvco / ((READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLM) >> 4) + 1));

  if((PeriphClk == RCC_PERIPHCLK_SAI1) || (PeriphClk == RCC_PERIPHCLK_SAI2))
  {

    if(__HAL_RCC_GET_PLLCLKOUT_CONFIG(RCC_PLL_SAI3CLK) != RESET)
    {
      /* f(PLLSAI3CLK) = f(VCO input) * PLLN / PLLP */
      plln = READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLN) >> 8;
      if(READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLP) != RESET)
      {
        frequency = (pllvco * plln) / 17;
      }
      else
      {
        frequency = (pllvco * plln) / 7;
      }
    }
    else if(__HAL_RCC_GET_PLLSAI1CLKOUT_CONFIG(RCC_PLLSAI1_SAI1CLK) != RESET)
    {
      /* f(PLLSAI1CLK) = f(VCOSAI1 input) * PLLSAI1N / PLLSAI1P */
      plln = READ_BIT(RCC->PLLSAI1CFGR, RCC_PLLSAI1CFGR_PLLSAI1N) >> 8;
      if(READ_BIT(RCC->PLLSAI1CFGR, RCC_PLLSAI1CFGR_PLLSAI1P) != RESET)
      {
        frequency = (pllvco * plln) / 17;
      }
      else
      {
        frequency = (pllvco * plln) / 7;
      }
    }
    else if(__HAL_RCC_GET_PLLSAI2CLKOUT_CONFIG(RCC_PLLSAI2_SAI2CLK) != RESET)
    {
      /* f(PLLSAI2CLK) = f(VCOSAI2 input) * PLLSAI2N / PLLSAI2P */
      plln = READ_BIT(RCC->PLLSAI2CFGR, RCC_PLLSAI2CFGR_PLLSAI2N) >> 8;
      if(READ_BIT(RCC->PLLSAI2CFGR, RCC_PLLSAI2CFGR_PLLSAI2P) != RESET)
      {
        frequency = (pllvco * plln) / 17;
      }
      else
      {
        frequency = (pllvco * plln) / 7;
      }
    }
    else
    {
      if(PeriphClk == RCC_PERIPHCLK_SAI1)
      {
        frequency = EXTERNAL_SAI1_CLOCK_VALUE;
      }
      else
      {
        frequency = EXTERNAL_SAI2_CLOCK_VALUE;
      }
    }
  }
  else if(PeriphClk == RCC_PERIPHCLK_ADC)
  {
    if(__HAL_RCC_GET_ADC_SOURCE() == RCC_ADCCLKSOURCE_SYSCLK)
    {
      frequency = HAL_RCC_GetSysClockFreq();
    }
    else if(__HAL_RCC_GET_ADC_SOURCE() == RCC_ADCCLKSOURCE_PLLSAI1)
    {
      if(__HAL_RCC_GET_PLLSAI1CLKOUT_CONFIG(RCC_PLLSAI1_ADC1CLK) != RESET)
      {
        /* f(PLLADC1CLK) = f(VCOSAI1 input) * PLLSAI1N / PLLSAI1R */
        plln = READ_BIT(RCC->PLLSAI1CFGR, RCC_PLLSAI1CFGR_PLLSAI1N) >> 8;
        frequency = (pllvco * plln) / (((READ_BIT(RCC->PLLSAI1CFGR, RCC_PLLSAI1CFGR_PLLSAI1R) >> 24) + 1) << 1);
      }
    }
    else /* RCC_ADCCLKSOURCE_PLLSAI2 */
    {
      if(__HAL_RCC_GET_PLLSAI2CLKOUT_CONFIG(RCC_PLLSAI2_ADC2CLK) != RESET)
      {
        /* f(PLLADC2CLK) = f(VCOSAI2 input) * PLLSAI2N / PLLSAI2R */
        plln = READ_BIT(RCC->PLLSAI2CFGR, RCC_PLLSAI2CFGR_PLLSAI2N) >> 8;
        frequency = (pllvco * plln) / (((READ_BIT(RCC->PLLSAI2CFGR, RCC_PLLSAI2CFGR_PLLSAI2R) >> 24) + 1) << 1);
      }
    }
  }

#if defined(STM32L475xx) || defined(STM32L476xx) || defined(STM32L485xx) || defined(STM32L486xx)
  else if(PeriphClk == RCC_PERIPHCLK_USB)
  {
    if(__HAL_RCC_GET_USB_SOURCE() == RCC_USBCLKSOURCE_MSI)
    {
      frequency = (1 << ((__HAL_RCC_GET_MSI_RANGE() >> 4) - 4)) * 1000000;
    }
    else if(__HAL_RCC_GET_USB_SOURCE() == RCC_USBCLKSOURCE_PLL)
    {
      /* f(PLL48M1CLK) = f(VCO input) * PLLN / PLLQ */
      plln = READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLN) >> 8;
      frequency = (pllvco * plln) / (((READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLQ) >> 21) + 1) << 1);
    }
    else if(__HAL_RCC_GET_USB_SOURCE() == RCC_USBCLKSOURCE_PLLSAI1)
    {
      /* f(PLL48M2CLK) = f(VCOSAI1 input) * PLLSAI1N / PLLSAI1Q */
      plln = READ_BIT(RCC->PLLSAI1CFGR, RCC_PLLSAI1CFGR_PLLSAI1N) >> 8;
      frequency = (pllvco * plln) / (((READ_BIT(RCC->PLLSAI1CFGR, RCC_PLLSAI1CFGR_PLLSAI1Q) >> 21) + 1) << 1);
    }
    else /* RCC_USBCLKSOURCE_NONE */
    {
      frequency = 0;
    }
  }
#endif /* STM32L475xx || STM32L476xx || STM32L485xx || STM32L486xx */

  return(frequency);
}

/**
  * @}
  */

/** @defgroup RCCEx_Exported_Functions_Group2 Extended clock management functions
 *  @brief  Extended clock management functions
 *
@verbatim   
 ===============================================================================
                ##### Extended clock management functions  #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to control the 
    activation or deactivation of MSI PLL-mode, PLLSAI1, PLLSAI2, LSE CSS,
    Low speed clock output and clock after wake-up from STOP mode.
@endverbatim
  * @{
  */

/**
  * @brief  Enable PLLSAI1.
  * @param  PLLSAI1Init: pointer to an RCC_PLLSAI1InitTypeDef structure that
  *         contains the configuration information for the PLLSAI1
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_EnablePLLSAI1(RCC_PLLSAI1InitTypeDef  *PLLSAI1Init)
{
  uint32_t tickstart = 0;
  HAL_StatusTypeDef status = HAL_OK;

  /* check for PLLSAI1 Parameters used to output PLLSAI1CLK */
  assert_param(IS_RCC_PLLSAI1N_VALUE(PLLSAI1Init->PLLSAI1N));
  assert_param(IS_RCC_PLLSAI1P_VALUE(PLLSAI1Init->PLLSAI1P));
  assert_param(IS_RCC_PLLSAI1Q_VALUE(PLLSAI1Init->PLLSAI1Q));
  assert_param(IS_RCC_PLLSAI1R_VALUE(PLLSAI1Init->PLLSAI1R));
  assert_param(IS_RCC_PLLSAI1CLOCKOUT_VALUE(PLLSAI1Init->PLLSAI1ClockOut));

  /* Disable the PLLSAI1 */
  __HAL_RCC_PLLSAI1_DISABLE();

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Wait till PLLSAI1 is ready to be updated */
  while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLSAI1RDY) != RESET)
  {
    if((HAL_GetTick() - tickstart) > PLLSAI1_TIMEOUT_VALUE)
    {
      status = HAL_TIMEOUT;
      break;
    }
  }

  if(status == HAL_OK)    
  {
    /* Configure the PLLSAI1 Multiplication factor N */
    /* Configure the PLLSAI1 Division factors P, Q and R */
    __HAL_RCC_PLLSAI1_CONFIG(PLLSAI1Init->PLLSAI1N, PLLSAI1Init->PLLSAI1P, PLLSAI1Init->PLLSAI1Q, PLLSAI1Init->PLLSAI1R);
    /* Configure the PLLSAI1 Clock output(s) */    
    __HAL_RCC_PLLSAI1CLKOUT_ENABLE(PLLSAI1Init->PLLSAI1ClockOut);

    /* Enable the PLLSAI1 again by setting PLLSAI1ON to 1*/
    __HAL_RCC_PLLSAI1_ENABLE();

    /* Get Start Tick*/
    tickstart = HAL_GetTick();

    /* Wait till PLLSAI1 is ready */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLSAI1RDY) == RESET)
    {
      if((HAL_GetTick() - tickstart) > PLLSAI1_TIMEOUT_VALUE)
      {
        status = HAL_TIMEOUT;
        break;
      }
    }
  }
  
  return status;
}

/**
  * @brief  Disable PLLSAI1.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_DisablePLLSAI1(void)
{
  uint32_t tickstart = 0;
  HAL_StatusTypeDef status = HAL_OK;

  /* Disable the PLLSAI1 */
  __HAL_RCC_PLLSAI1_DISABLE();

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Wait till PLLSAI1 is ready */
  while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLSAI1RDY) != RESET)
  {
    if((HAL_GetTick() - tickstart) > PLLSAI1_TIMEOUT_VALUE)
    {
      status = HAL_TIMEOUT;
      break;
    }
  }
  
  return status;
}

/**
  * @brief  Enable PLLSAI2.
  * @param  PLLSAI2Init: pointer to an RCC_PLLSAI2InitTypeDef structure that
  *         contains the configuration information for the PLLSAI2
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_EnablePLLSAI2(RCC_PLLSAI2InitTypeDef  *PLLSAI2Init)
{
  uint32_t tickstart = 0;
  HAL_StatusTypeDef status = HAL_OK;

  /* check for PLLSAI2 Parameters used to output PLLSAI2CLK */
  assert_param(IS_RCC_PLLSAI2N_VALUE(PLLSAI2Init->PLLSAI2N));
  assert_param(IS_RCC_PLLSAI2P_VALUE(PLLSAI2Init->PLLSAI2P));
  assert_param(IS_RCC_PLLSAI2R_VALUE(PLLSAI2Init->PLLSAI2R));
  assert_param(IS_RCC_PLLSAI2CLOCKOUT_VALUE(PLLSAI2Init->PLLSAI2ClockOut));

  /* Disable the PLLSAI2 */
  __HAL_RCC_PLLSAI2_DISABLE();

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Wait till PLLSAI2 is ready to be updated */
  while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLSAI2RDY) != RESET)
  {
    if((HAL_GetTick() - tickstart) > PLLSAI2_TIMEOUT_VALUE)
    {
      status = HAL_TIMEOUT;
      break;
    }
  }

  if(status == HAL_OK)    
  {
    /* Configure the PLLSAI2 Multiplication factor N */
    /* Configure the PLLSAI2 Division factors P and R */
    __HAL_RCC_PLLSAI2_CONFIG(PLLSAI2Init->PLLSAI2N, PLLSAI2Init->PLLSAI2P, PLLSAI2Init->PLLSAI2R);
    /* Configure the PLLSAI2 Clock output(s) */    
    __HAL_RCC_PLLSAI2CLKOUT_ENABLE(PLLSAI2Init->PLLSAI2ClockOut);

    /* Enable the PLLSAI2 again by setting PLLSAI2ON to 1*/
    __HAL_RCC_PLLSAI2_ENABLE();

    /* Get Start Tick*/
    tickstart = HAL_GetTick();

    /* Wait till PLLSAI2 is ready */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLSAI2RDY) == RESET)
    {
      if((HAL_GetTick() - tickstart) > PLLSAI2_TIMEOUT_VALUE)
      {
        status = HAL_TIMEOUT;
        break;
      }
    }
  }
  
  return status;
}

/**
  * @brief  Disable PLLISAI2.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_DisablePLLSAI2(void)
{
  uint32_t tickstart = 0;
  HAL_StatusTypeDef status = HAL_OK;

  /* Disable the PLLSAI2 */
  __HAL_RCC_PLLSAI2_DISABLE();

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Wait till PLLSAI2 is ready */
  while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLSAI2RDY) != RESET)
  {
    if((HAL_GetTick() - tickstart) > PLLSAI2_TIMEOUT_VALUE)
    {
      status = HAL_TIMEOUT;
      break;
    }
  }
  
  return status;
}

/**
  * @brief  Configure the oscillator clock source for wakeup from Stop and CSS backup clock.
  * @param  WakeUpClk: Wakeup clock 
  *         This parameter can be one of the following values:
  *            @arg RCC_STOP_WAKEUPCLOCK_MSI: MSI oscillator selection
  *            @arg RCC_STOP_WAKEUPCLOCK_HSI: HSI oscillator selection
  * @note   This function shall not be called after the Clock Security System on HSE has been
  *         enabled.
  * @retval None
  */
void HAL_RCCEx_WakeUpStopCLKConfig(uint32_t WakeUpClk)
{
  assert_param(IS_RCC_STOP_WAKEUPCLOCK(WakeUpClk));
  
  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(WakeUpClk);
}

/**
  * @brief  Configure the MSI range after standby mode.
  * @note   After Standby its frequency can be selected between 4 possible values (1, 2, 4 or 8 MHz).
  * @param  MSIRange: MSI range 
  *         This parameter can be one of the following values:
  *            @arg RCC_MSIRANGE_4: Range 4 around 1 MHz
  *            @arg RCC_MSIRANGE_5: Range 5 around 2 MHz
  *            @arg RCC_MSIRANGE_6: Range 6 around 4 MHz (reset value)
  *            @arg RCC_MSIRANGE_7: Range 7 around 8 MHz
  * @retval None
  */
void HAL_RCCEx_StandbyMSIRangeConfig(uint32_t MSIRange)
{
  assert_param(IS_RCC_MSI_STANDBY_CLOCK_RANGE(MSIRange));
  
  __HAL_RCC_MSI_STANDBY_RANGE_CONFIG(MSIRange);
}

/**
  * @brief  Enable the LSE Clock Security System.
  * @note   Prior to enable the LSE Clock Security System, LSE oscillator is to be enabled
  *         with HAL_RCC_OscConfig() and the LSE oscillator clock is to be selected as RTC
  *         clock with HAL_RCCEx_PeriphCLKConfig().
  * @retval None
  */
void HAL_RCCEx_EnableLSECSS(void)
{
  SET_BIT(RCC->BDCR, RCC_BDCR_LSECSSON) ;
}

/**
  * @brief  Disable the LSE Clock Security System.
  * @note   LSE Clock Security System can only be disabled after a LSE failure detection.
  * @retval None
  */
void HAL_RCCEx_DisableLSECSS(void)
{
  CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSECSSON) ;
}

/**
  * @brief  Select the Low Speed clock source to output on LSCO pin (PA2).
  * @param  LSCOSource: specifies the Low Speed clock source to output.
  *          This parameter can be one of the following values:
  *            @arg RCC_LSCOSOURCE_LSI: LSI clock selected as LSCO source
  *            @arg RCC_LSCOSOURCE_LSE: LSE clock selected as LSCO source
  * @retval None
  */
void HAL_RCCEx_EnableLSCO(uint32_t LSCOSource)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  FlagStatus       pwrclkchanged = RESET;
  FlagStatus       backupchanged = RESET;
  
  /* Check the parameters */
  assert_param(IS_RCC_LSCOSOURCE(LSCOSource));

  /* LSCO Pin Clock Enable */
  __LSCO_CLK_ENABLE();

  /* Configue the LSCO pin in analog mode */
  GPIO_InitStruct.Pin = LSCO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(LSCO_GPIO_PORT, &GPIO_InitStruct);

  /* Update LSCOSEL clock source in Backup Domain control register */
  if(__HAL_RCC_PWR_IS_CLK_DISABLED())
  {
    __HAL_RCC_PWR_CLK_ENABLE();
    pwrclkchanged = SET;
  }
  if(HAL_IS_BIT_CLR(PWR->CR1, PWR_CR1_DBP))
  {
    HAL_PWR_EnableBkUpAccess();
    backupchanged = SET;
  }
  
  MODIFY_REG(RCC->BDCR, RCC_BDCR_LSCOSEL | RCC_BDCR_LSCOEN, LSCOSource | RCC_BDCR_LSCOEN);
  
  if(backupchanged == SET)
  {
    HAL_PWR_DisableBkUpAccess();
  }
  if(pwrclkchanged == SET)
  {
    __HAL_RCC_PWR_CLK_DISABLE();
  }
}

/**
  * @brief  Disable the Low Speed clock output.
  * @retval None
  */
void HAL_RCCEx_DisableLSCO(void)
{
  FlagStatus       pwrclkchanged = RESET;
  FlagStatus       backupchanged = RESET;
  
  /* Update LSCOEN bit in Backup Domain control register */
  if(__HAL_RCC_PWR_IS_CLK_DISABLED())
  {
    __HAL_RCC_PWR_CLK_ENABLE();
    pwrclkchanged = SET;
  }
  if(HAL_IS_BIT_CLR(PWR->CR1, PWR_CR1_DBP))
  {
    /* Enable access to the backup domain */
    HAL_PWR_EnableBkUpAccess();
    backupchanged = SET;
  }
  
  CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSCOEN);
  
  /* Restore previous configuration */
  if(backupchanged == SET)
  {
    /* Disable access to the backup domain */
    HAL_PWR_DisableBkUpAccess();
  }
  if(pwrclkchanged == SET)
  {
    __HAL_RCC_PWR_CLK_DISABLE();
  }
}

/**
  * @brief  Enable the PLL-mode of the MSI.
  * @note   Prior to enable the PLL-mode of the MSI for automatic hardware
  *         calibration LSE oscillator is to be enabled with HAL_RCC_OscConfig().
  * @retval None
  */
void HAL_RCCEx_EnableMSIPLLMode(void)
{
  SET_BIT(RCC->CR, RCC_CR_MSIPLLEN) ;
}

/**
  * @brief  Disable the PLL-mode of the MSI.
  * @note   PLL-mode of the MSI is automatically reset when LSE oscillator is disabled.
  * @retval None
  */
void HAL_RCCEx_DisableMSIPLLMode(void)
{
  CLEAR_BIT(RCC->CR, RCC_CR_MSIPLLEN) ;
}

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup RCCEx_Private_Functions
 * @{
 */

/**
  * @brief  Configure the parameters N & P of PLLSAI1 and enable PLLSAI1 output clock(s).
  * @param  PllSai1: pointer to an RCC_PLLSAI1InitTypeDef structure that
  *         contains the configuration parameters N & P as well as PLLSAI1 output clock(s)
  *
  * @note   PLLSAI1 is temporary disable to apply new parameters
  *
  * @retval HAL status
  */
static HAL_StatusTypeDef RCCEx_PLLSAI1_ConfigNP(RCC_PLLSAI1InitTypeDef *PllSai1)
{
  uint32_t tickstart = 0;
  HAL_StatusTypeDef status = HAL_OK;

  /* check for PLLSAI1 Parameters used to output PLLSAI1CLK */
  assert_param(IS_RCC_PLLSAI1N_VALUE(PllSai1->PLLSAI1N));
  assert_param(IS_RCC_PLLSAI1P_VALUE(PllSai1->PLLSAI1P));
  assert_param(IS_RCC_PLLSAI1CLOCKOUT_VALUE(PllSai1->PLLSAI1ClockOut));

  /* Disable the PLLSAI1 */
  __HAL_RCC_PLLSAI1_DISABLE();

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Wait till PLLSAI1 is ready to be updated */
  while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLSAI1RDY) != RESET)
  {
    if((HAL_GetTick() - tickstart) > PLLSAI1_TIMEOUT_VALUE)
    {
      status = HAL_TIMEOUT;
      break;
    }
  }

  if(status == HAL_OK)    
  {
    /* Configure the PLLSAI1 Multiplication factor N */
    __HAL_RCC_PLLSAI1_MULN_CONFIG(PllSai1->PLLSAI1N);
    /* Configure the PLLSAI1 Division factor P */
    __HAL_RCC_PLLSAI1_DIVP_CONFIG(PllSai1->PLLSAI1P);
    /* Configure the PLLSAI1 Clock output(s) */
    __HAL_RCC_PLLSAI1CLKOUT_ENABLE(PllSai1->PLLSAI1ClockOut);

    /* Enable the PLLSAI1 again by setting PLLSAI1ON to 1*/
    __HAL_RCC_PLLSAI1_ENABLE();

    /* Get Start Tick*/
    tickstart = HAL_GetTick();

    /* Wait till PLLSAI1 is ready */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLSAI1RDY) == RESET)
    {
      if((HAL_GetTick() - tickstart) > PLLSAI1_TIMEOUT_VALUE)
      {
        status = HAL_TIMEOUT;
        break;
      }
    }
  }
  
  return status;
}

/**
  * @brief  Configure the parameters N & Q of PLLSAI1 and enable PLLSAI1 output clock(s).
  * @param  PllSai1: pointer to an RCC_PLLSAI1InitTypeDef structure that
  *         contains the configuration parameters N & Q as well as PLLSAI1 output clock(s)
  *
  * @note   PLLSAI1 is temporary disable to apply new parameters
  *
  * @retval HAL status
  */
static HAL_StatusTypeDef RCCEx_PLLSAI1_ConfigNQ(RCC_PLLSAI1InitTypeDef *PllSai1)
{
  uint32_t tickstart = 0;
  HAL_StatusTypeDef status = HAL_OK;

  /* check for PLLSAI1 Parameters used to output PLLSAI1CLK */
  assert_param(IS_RCC_PLLSAI1N_VALUE(PllSai1->PLLSAI1N));
  assert_param(IS_RCC_PLLSAI1Q_VALUE(PllSai1->PLLSAI1Q));
  assert_param(IS_RCC_PLLSAI1CLOCKOUT_VALUE(PllSai1->PLLSAI1ClockOut));

  /* Disable the PLLSAI1 */
  __HAL_RCC_PLLSAI1_DISABLE();

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Wait till PLLSAI1 is ready to be updated */
  while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLSAI1RDY) != RESET)
  {
    if((HAL_GetTick() - tickstart) > PLLSAI1_TIMEOUT_VALUE)
    {
      status = HAL_TIMEOUT;
      break;
    }
  }

  if(status == HAL_OK)    
  {
    /* Configure the PLLSAI1 Multiplication factor N */
    __HAL_RCC_PLLSAI1_MULN_CONFIG(PllSai1->PLLSAI1N);
    /* Configure the PLLSAI1 Division factor Q */
    __HAL_RCC_PLLSAI1_DIVQ_CONFIG(PllSai1->PLLSAI1Q);
    /* Configure the PLLSAI1 Clock output(s) */
    __HAL_RCC_PLLSAI1CLKOUT_ENABLE(PllSai1->PLLSAI1ClockOut);

    /* Enable the PLLSAI1 again by setting PLLSAI1ON to 1*/
    __HAL_RCC_PLLSAI1_ENABLE();

    /* Get Start Tick*/
    tickstart = HAL_GetTick();

    /* Wait till PLLSAI1 is ready */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLSAI1RDY) == RESET)
    {
      if((HAL_GetTick() - tickstart) > PLLSAI1_TIMEOUT_VALUE)
      {
        status = HAL_TIMEOUT;
        break;
      }
    }
  }
  
  return status;
}

/**
  * @brief  Configure the parameters N & R of PLLSAI1 and enable PLLSAI1 output clock(s).
  * @param  PllSai1: pointer to an RCC_PLLSAI1InitTypeDef structure that
  *         contains the configuration parameters N & R as well as PLLSAI1 output clock(s)
  *
  * @note   PLLSAI1 is temporary disable to apply new parameters
  *
  * @retval HAL status
  */
static HAL_StatusTypeDef RCCEx_PLLSAI1_ConfigNR(RCC_PLLSAI1InitTypeDef *PllSai1)
{
  uint32_t tickstart = 0;
  HAL_StatusTypeDef status = HAL_OK;

  /* check for PLLSAI1 Parameters used to output PLLSAI1CLK */
  assert_param(IS_RCC_PLLSAI1N_VALUE(PllSai1->PLLSAI1N));
  assert_param(IS_RCC_PLLSAI1R_VALUE(PllSai1->PLLSAI1R));
  assert_param(IS_RCC_PLLSAI1CLOCKOUT_VALUE(PllSai1->PLLSAI1ClockOut));

  /* Disable the PLLSAI1 */
  __HAL_RCC_PLLSAI1_DISABLE();

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Wait till PLLSAI1 is ready to be updated */
  while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLSAI1RDY) != RESET)
  {
    if((HAL_GetTick() - tickstart) > PLLSAI1_TIMEOUT_VALUE)
    {
      status = HAL_TIMEOUT;
      break;
    }
  }

  if(status == HAL_OK)    
  {
    /* Configure the PLLSAI1 Multiplication factor N */
    __HAL_RCC_PLLSAI1_MULN_CONFIG(PllSai1->PLLSAI1N);
    /* Configure the PLLSAI1 Division factor R */
    __HAL_RCC_PLLSAI1_DIVR_CONFIG(PllSai1->PLLSAI1R);
    /* Configure the PLLSAI1 Clock output(s) */
    __HAL_RCC_PLLSAI1CLKOUT_ENABLE(PllSai1->PLLSAI1ClockOut);

    /* Enable the PLLSAI1 again by setting PLLSAI1ON to 1*/
    __HAL_RCC_PLLSAI1_ENABLE();

    /* Get Start Tick*/
    tickstart = HAL_GetTick();

    /* Wait till PLLSAI1 is ready */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLSAI1RDY) == RESET)
    {
      if((HAL_GetTick() - tickstart) > PLLSAI1_TIMEOUT_VALUE)
      {
        status = HAL_TIMEOUT;
        break;
      }
    }
  }
  
  return status;
}

/**
  * @brief  Configure the parameters N & P of PLLSAI2 and enable PLLSAI2 output clock(s).
  * @param  PllSai2: pointer to an RCC_PLLSAI2InitTypeDef structure that
  *         contains the configuration parameters N & P as well as PLLSAI2 output clock(s)
  *
  * @note   PLLSAI2 is temporary disable to apply new parameters
  *
  * @retval HAL status
  */
static HAL_StatusTypeDef RCCEx_PLLSAI2_ConfigNP(RCC_PLLSAI2InitTypeDef *PllSai2)
{
  uint32_t tickstart = 0;
  HAL_StatusTypeDef status = HAL_OK;

  /* check for PLLSAI2 Parameters */
  assert_param(IS_RCC_PLLSAI2N_VALUE(PllSai2->PLLSAI2N));
  assert_param(IS_RCC_PLLSAI2P_VALUE(PllSai2->PLLSAI2P));
  assert_param(IS_RCC_PLLSAI2CLOCKOUT_VALUE(PllSai2->PLLSAI2ClockOut));

  /* Disable the PLLSAI2 */
  __HAL_RCC_PLLSAI2_DISABLE();

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Wait till PLLSAI2 is ready */
  while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLSAI2RDY) != RESET)
  {
    if((HAL_GetTick() - tickstart) > PLLSAI2_TIMEOUT_VALUE)
    {
      status = HAL_TIMEOUT;
      break;
    }
  }

  if(status == HAL_OK)    
  {
    /* Configure the PLLSAI2 Multiplication factor N */
    __HAL_RCC_PLLSAI2_MULN_CONFIG(PllSai2->PLLSAI2N);
    /* Configure the PLLSAI2 Division factor P */
    __HAL_RCC_PLLSAI2_DIVP_CONFIG(PllSai2->PLLSAI2P);
    /* Configure the PLLSAI2 Clock output(s) */
    __HAL_RCC_PLLSAI2CLKOUT_ENABLE(PllSai2->PLLSAI2ClockOut);

    /* Enable the PLLSAI2 again by setting PLLSAI2ON to 1*/
    __HAL_RCC_PLLSAI2_ENABLE();

    /* Get Start Tick*/
    tickstart = HAL_GetTick();

    /* Wait till PLLSAI2 is ready */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLSAI2RDY) == RESET)
    {
      if((HAL_GetTick() - tickstart) > PLLSAI2_TIMEOUT_VALUE)
      {
        status = HAL_TIMEOUT;
        break;
      }
    }
  }
  
  return status;
}

/**
  * @brief  Configure the parameters N & R of PLLSAI2 and enable PLLSAI2 output clock(s).
  * @param  PllSai2: pointer to an RCC_PLLSAI2InitTypeDef structure that
  *         contains the configuration parameters N & R as well as PLLSAI2 output clock(s)
  *
  * @note   PLLSAI2 is temporary disable to apply new parameters
  *
  * @retval HAL status
  */
static HAL_StatusTypeDef RCCEx_PLLSAI2_ConfigNR(RCC_PLLSAI2InitTypeDef *PllSai2)
{
  uint32_t tickstart = 0;
  HAL_StatusTypeDef status = HAL_OK;

  /* check for PLLSAI2 Parameters */
  assert_param(IS_RCC_PLLSAI2N_VALUE(PllSai2->PLLSAI2N));
  assert_param(IS_RCC_PLLSAI2R_VALUE(PllSai2->PLLSAI2R));
  assert_param(IS_RCC_PLLSAI2CLOCKOUT_VALUE(PllSai2->PLLSAI2ClockOut));

  /* Disable the PLLSAI2 */
  __HAL_RCC_PLLSAI2_DISABLE();

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Wait till PLLSAI2 is ready */
  while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLSAI2RDY) != RESET)
  {
    if((HAL_GetTick() - tickstart) > PLLSAI2_TIMEOUT_VALUE)
    {
      status = HAL_TIMEOUT;
      break;
    }
  }

  if(status == HAL_OK)    
  {
    /* Configure the PLLSAI2 Multiplication factor N */
    __HAL_RCC_PLLSAI2_MULN_CONFIG(PllSai2->PLLSAI2N);
    /* Configure the PLLSAI2 Division factor R */
    __HAL_RCC_PLLSAI2_DIVR_CONFIG(PllSai2->PLLSAI2R);
    /* Configure the PLLSAI2 Clock output(s) */
    __HAL_RCC_PLLSAI2CLKOUT_ENABLE(PllSai2->PLLSAI2ClockOut);

    /* Enable the PLLSAI2 again by setting PLLSAI2ON to 1*/
    __HAL_RCC_PLLSAI2_ENABLE();

    /* Get Start Tick*/
    tickstart = HAL_GetTick();

    /* Wait till PLLSAI2 is ready */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLSAI2RDY) == RESET)
    {
      if((HAL_GetTick() - tickstart) > PLLSAI2_TIMEOUT_VALUE)
      {
        status = HAL_TIMEOUT;
        break;
      }
    }
  }
  
  return status;
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

