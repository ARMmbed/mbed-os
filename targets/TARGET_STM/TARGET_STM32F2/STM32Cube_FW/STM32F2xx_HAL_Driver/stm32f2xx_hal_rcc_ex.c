/**
  ******************************************************************************
  * @file    stm32f2xx_hal_rcc_ex.c
  * @author  MCD Application Team
  * @brief   Extension RCC HAL module driver.
  *          This file provides firmware functions to manage the following 
  *          functionalities RCC extension peripheral:
  *           + Extended Peripheral Control functions
  *  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal.h"

/** @addtogroup STM32F2xx_HAL_Driver
  * @{
  */

/** @defgroup RCCEx RCCEx
  * @brief RCCEx HAL module driver
  * @{
  */

#ifdef HAL_RCC_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup RCCEx_Private_Constants
  * @{
  */
/**
  * @}
  */ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @defgroup RCCEx_Exported_Functions RCCEx Exported Functions
  *  @{
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
  * @brief  Initializes the RCC extended peripherals clocks according to the specified parameters in the
  *         RCC_PeriphCLKInitTypeDef.
  * @param  PeriphClkInit pointer to an RCC_PeriphCLKInitTypeDef structure that
  *         contains the configuration information for the Extended Peripherals clocks(I2S and RTC clocks).
  *         
  * @note   A caution to be taken when HAL_RCCEx_PeriphCLKConfig() is used to select RTC clock selection, in this case 
  *         the Reset of Backup domain will be applied in order to modify the RTC Clock source as consequence all backup 
  *        domain (RTC and RCC_BDCR register expect BKPSRAM) will be reset
  *              
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)
{
  uint32_t tickstart = 0U;
  uint32_t tmpreg1 = 0U;
    
  /* Check the parameters */
  assert_param(IS_RCC_PERIPHCLOCK(PeriphClkInit->PeriphClockSelection));
  
  /*---------------------------- I2S configuration ---------------------------*/
  if(((PeriphClkInit->PeriphClockSelection & RCC_PERIPHCLK_I2S) == (RCC_PERIPHCLK_I2S))|| \
     (PeriphClkInit->PeriphClockSelection == RCC_PERIPHCLK_PLLI2S))
  {
    /* check for Parameters */
    assert_param(IS_RCC_PLLI2SR_VALUE(PeriphClkInit->PLLI2S.PLLI2SR));
    assert_param(IS_RCC_PLLI2SN_VALUE(PeriphClkInit->PLLI2S.PLLI2SN));
      
    /* Disable the PLLI2S */
    __HAL_RCC_PLLI2S_DISABLE();
    /* Get tick */
    tickstart = HAL_GetTick();
    /* Wait till PLLI2S is disabled */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY)  != RESET)
    {
      if((HAL_GetTick() - tickstart ) > PLLI2S_TIMEOUT_VALUE)
      {
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;
      } 
    }
    /* Configure the PLLI2S division factors */
    /* PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) * (PLLI2SN/PLLM) */
    /* I2SCLK = f(PLLI2S clock output) = f(VCO clock) / PLLI2SR */
    __HAL_RCC_PLLI2S_CONFIG(PeriphClkInit->PLLI2S.PLLI2SN , PeriphClkInit->PLLI2S.PLLI2SR);
    
    /* Enable the PLLI2S */
    __HAL_RCC_PLLI2S_ENABLE();
    /* Get tick */
    tickstart = HAL_GetTick();
    /* Wait till PLLI2S is ready */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY)  == RESET)
    {
      if((HAL_GetTick() - tickstart ) > PLLI2S_TIMEOUT_VALUE)
      {
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;
      }
    }
  }
  /*--------------------------------------------------------------------------*/
  
  /*---------------------------- RTC configuration ---------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_RTC) == (RCC_PERIPHCLK_RTC))
  {
    /* Check for RTC Parameters used to output RTCCLK */
    assert_param(IS_RCC_RTCCLKSOURCE(PeriphClkInit->RTCClockSelection));
    
    /* Enable Power Clock*/
    __HAL_RCC_PWR_CLK_ENABLE();
    
    /* Enable write access to Backup domain */
    PWR->CR |= PWR_CR_DBP;
    
    /* Get tick */
    tickstart = HAL_GetTick();
    
    while((PWR->CR & PWR_CR_DBP) == RESET)
    {
      if((HAL_GetTick() - tickstart ) > RCC_DBP_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      }
    }
    /* Reset the Backup domain only if the RTC Clock source selection is modified from reset value */ 
    tmpreg1 = (RCC->BDCR & RCC_BDCR_RTCSEL);
    if((tmpreg1 != 0x00000000U) && ((tmpreg1) != (PeriphClkInit->RTCClockSelection & RCC_BDCR_RTCSEL)))
    {
      /* Store the content of BDCR register before the reset of Backup Domain */
      tmpreg1 = (RCC->BDCR & ~(RCC_BDCR_RTCSEL));
      /* RTC Clock selection can be changed only if the Backup Domain is reset */
      __HAL_RCC_BACKUPRESET_FORCE();
      __HAL_RCC_BACKUPRESET_RELEASE();
      /* Restore the Content of BDCR register */
      RCC->BDCR = tmpreg1;

      /* Wait for LSE reactivation if LSE was enable prior to Backup Domain reset */
      if(HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSEON))
      {
        /* Get tick */
        tickstart = HAL_GetTick();
        
        /* Wait till LSE is ready */
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET)
        {
          if((HAL_GetTick() - tickstart ) > RCC_LSE_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
    }
    __HAL_RCC_RTC_CONFIG(PeriphClkInit->RTCClockSelection);
  }
  /*--------------------------------------------------------------------------*/

  return HAL_OK;
}

/**
  * @brief  Configures the RCC_OscInitStruct according to the internal 
  * RCC configuration registers.
  * @param  PeriphClkInit pointer to an RCC_PeriphCLKInitTypeDef structure that 
  * will be configured.
  * @retval None
  */
void HAL_RCCEx_GetPeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)
{
  uint32_t tempreg;
  
  /* Set all possible values for the extended clock type parameter------------*/
  PeriphClkInit->PeriphClockSelection = RCC_PERIPHCLK_I2S | RCC_PERIPHCLK_RTC;
  
  /* Get the PLLI2S Clock configuration --------------------------------------*/
  PeriphClkInit->PLLI2S.PLLI2SN = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> POSITION_VAL(RCC_PLLI2SCFGR_PLLI2SN));
  PeriphClkInit->PLLI2S.PLLI2SR = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> POSITION_VAL(RCC_PLLI2SCFGR_PLLI2SR));
  
  /* Get the RTC Clock configuration -----------------------------------------------*/
  tempreg = (RCC->CFGR & RCC_CFGR_RTCPRE);
  PeriphClkInit->RTCClockSelection = (uint32_t)((tempreg) | (RCC->BDCR & RCC_BDCR_RTCSEL));
  
}
/**
  * @}
  */

/** @defgroup RCCEx_Exported_Functions_Group2 Extended Clock management functions
 *  @brief  Extended Clock management functions
 *
@verbatim   
 ===============================================================================
                ##### Extended clock management functions  #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to control the 
    activation or deactivation of PLLI2S.
@endverbatim
  * @{
  */

/**
  * @brief  Enable PLLI2S.
  * @param  PLLI2SInit  pointer to an RCC_PLLI2SInitTypeDef structure that
  *         contains the configuration information for the PLLI2S
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_EnablePLLI2S(RCC_PLLI2SInitTypeDef  *PLLI2SInit)
{
  uint32_t tickstart;

  /* Check for parameters */
  assert_param(IS_RCC_PLLI2SN_VALUE(PLLI2SInit->PLLI2SN));
  assert_param(IS_RCC_PLLI2SR_VALUE(PLLI2SInit->PLLI2SR));

  /* Disable the PLLI2S */
  __HAL_RCC_PLLI2S_DISABLE();

  /* Wait till PLLI2S is disabled */
  tickstart = HAL_GetTick();
  while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY) != RESET)
  {
    if((HAL_GetTick() - tickstart ) > PLLI2S_TIMEOUT_VALUE)
    {
      /* return in case of Timeout detected */
      return HAL_TIMEOUT;
    }
  }

  /* Configure the PLLI2S division factors */
  /* PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) x PLLI2SN */
  /* I2SRCLK = PLLI2S_VCO / PLLI2SR */
  __HAL_RCC_PLLI2S_CONFIG(PLLI2SInit->PLLI2SN, PLLI2SInit->PLLI2SR);

  /* Enable the PLLI2S */
  __HAL_RCC_PLLI2S_ENABLE();

  /* Wait till PLLI2S is ready */
  tickstart = HAL_GetTick();
  while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY) == RESET)
  {
    if((HAL_GetTick() - tickstart ) > PLLI2S_TIMEOUT_VALUE)
    {
      /* return in case of Timeout detected */
      return HAL_TIMEOUT;
    }
  }

 return HAL_OK;
}

/**
  * @brief  Disable PLLI2S.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_DisablePLLI2S(void)
{
  uint32_t tickstart;

  /* Disable the PLLI2S */
  __HAL_RCC_PLLI2S_DISABLE();

  /* Wait till PLLI2S is disabled */
  tickstart = HAL_GetTick();
  while(READ_BIT(RCC->CR, RCC_CR_PLLI2SRDY) != RESET)
  {
    if((HAL_GetTick() - tickstart) > PLLI2S_TIMEOUT_VALUE)
    {
      /* return in case of Timeout detected */
      return HAL_TIMEOUT;
    }
  }

  return HAL_OK;
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
