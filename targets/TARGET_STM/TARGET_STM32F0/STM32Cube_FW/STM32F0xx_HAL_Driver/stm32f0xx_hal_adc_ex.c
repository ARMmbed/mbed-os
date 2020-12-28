/**
  ******************************************************************************
  * @file    stm32f0xx_hal_adc_ex.c
  * @author  MCD Application Team
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Analog to Digital Convertor (ADC)
  *          peripheral:
  *           + Operation functions
  *             ++ Calibration (ADC automatic self-calibration)
  *          Other functions (generic functions) are available in file 
  *          "stm32f0xx_hal_adc.c".
  *
  @verbatim
  [..] 
  (@) Sections "ADC peripheral features" and "How to use this driver" are
      available in file of generic functions "stm32l1xx_hal_adc.c".
  [..]
  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
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
#include "stm32f0xx_hal.h"

/** @addtogroup STM32F0xx_HAL_Driver
  * @{
  */

/** @defgroup ADCEx ADCEx
  * @brief ADC HAL module driver
  * @{
  */

#ifdef HAL_ADC_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup ADCEx_Private_Constants ADCEx Private Constants
  * @{
  */ 
 
/* Fixed timeout values for ADC calibration, enable settling time, disable  */
  /* settling time.                                                           */
  /* Values defined to be higher than worst cases: low clock frequency,       */
  /* maximum prescaler.                                                       */
  /* Ex of profile low frequency : Clock source at 0.1 MHz, ADC clock         */
  /* prescaler 4.                                                             */
  /* Unit: ms                                                                 */
  #define ADC_DISABLE_TIMEOUT           2
  #define ADC_CALIBRATION_TIMEOUT       2U      
/**
  * @}
  */

/* Private macros -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup ADCEx_Exported_Functions ADCEx Exported Functions
  * @{
  */

/** @defgroup ADCEx_Exported_Functions_Group1 Extended Initialization/de-initialization functions 
 *  @brief    Extended Initialization and Configuration functions
 *
@verbatim
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Perform the ADC calibration. 
@endverbatim
  * @{
  */

/**
  * @brief  Perform an ADC automatic self-calibration
  *         Calibration prerequisite: ADC must be disabled (execute this
  *         function before HAL_ADC_Start() or after HAL_ADC_Stop() ).
  * @note   Calibration factor can be read after calibration, using function
  *         HAL_ADC_GetValue() (value on 7 bits: from DR[6;0]).
  * @param  hadc ADC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADCEx_Calibration_Start(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  uint32_t tickstart = 0U;
  uint32_t backup_setting_adc_dma_transfer = 0; /* Note: Variable not declared as volatile because register read is already declared as volatile */
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  /* Process locked */
  __HAL_LOCK(hadc);
  
  /* Calibration prerequisite: ADC must be disabled. */
  if (ADC_IS_ENABLE(hadc) == RESET)
  {
    /* Set ADC state */
    ADC_STATE_CLR_SET(hadc->State, 
                      HAL_ADC_STATE_REG_BUSY,
                      HAL_ADC_STATE_BUSY_INTERNAL);
    
    /* Disable ADC DMA transfer request during calibration */
    /* Note: Specificity of this STM32 serie: Calibration factor is           */
    /*       available in data register and also transfered by DMA.           */
    /*       To not insert ADC calibration factor among ADC conversion data   */
    /*       in array variable, DMA transfer must be disabled during          */
    /*       calibration.                                                     */
    backup_setting_adc_dma_transfer = READ_BIT(hadc->Instance->CFGR1, ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG);
    CLEAR_BIT(hadc->Instance->CFGR1, ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG);

    /* Start ADC calibration */
    hadc->Instance->CR |= ADC_CR_ADCAL;

    tickstart = HAL_GetTick();  

    /* Wait for calibration completion */
    while(HAL_IS_BIT_SET(hadc->Instance->CR, ADC_CR_ADCAL))
    {
      if((HAL_GetTick() - tickstart) > ADC_CALIBRATION_TIMEOUT)
      {
        /* Update ADC state machine to error */
        ADC_STATE_CLR_SET(hadc->State,
                          HAL_ADC_STATE_BUSY_INTERNAL,
                          HAL_ADC_STATE_ERROR_INTERNAL);
        
        /* Process unlocked */
        __HAL_UNLOCK(hadc);
        
        return HAL_ERROR;
      }
    }
    
    /* Restore ADC DMA transfer request after calibration */
    SET_BIT(hadc->Instance->CFGR1, backup_setting_adc_dma_transfer);

    /* Set ADC state */
    ADC_STATE_CLR_SET(hadc->State,
                      HAL_ADC_STATE_BUSY_INTERNAL,
                      HAL_ADC_STATE_READY);
  }
  else
  {
    /* Update ADC state machine to error */
    SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
    
    tmp_hal_status = HAL_ERROR;
  }
  
  /* Process unlocked */
  __HAL_UNLOCK(hadc);
  
  /* Return function status */
  return tmp_hal_status;
}

/**
  * @}
  */  

/**
  * @}
  */

#endif /* HAL_ADC_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
