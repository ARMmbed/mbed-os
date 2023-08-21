/**
  ******************************************************************************
  * @file    stm32wlxx_hal_adc_ex.c
  * @author  MCD Application Team
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the Analog to Digital Converter (ADC)
  *          peripheral:
  *           + Peripheral Control functions
  *          Other functions (generic functions) are available in file
  *          "stm32wlxx_hal_adc.c".
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @verbatim
  [..]
  (@) Sections "ADC peripheral features" and "How to use this driver" are
      available in file of generic functions "stm32wlxx_hal_adc.c".
  [..]
  @endverbatim
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32wlxx_hal.h"

/** @addtogroup STM32WLxx_HAL_Driver
  * @{
  */

/** @defgroup ADCEx ADCEx
  * @brief ADC Extended HAL module driver
  * @{
  */

#ifdef HAL_ADC_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/** @defgroup ADCEx_Private_Constants ADC Extended Private Constants
  * @{
  */

/* Fixed timeout value for ADC calibration.                                   */
/* Values defined to be higher than worst cases: maximum ratio between ADC    */
/* and CPU clock frequencies.                                                 */
/* Example of profile low frequency : ADC frequency at 31.25kHz (ADC clock    */
/* source PLL 8MHz, ADC clock prescaler 256), CPU frequency 48MHz.            */
/* Calibration time max = 116 / fADC (refer to datasheet)                     */
/*                      = 178 176 CPU cycles                                  */
#define ADC_CALIBRATION_TIMEOUT         (178176UL)   /*!< ADC calibration time-out value (unit: CPU cycles) */
#define ADC_DISABLE_TIMEOUT             (2UL)

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup ADCEx_Exported_Functions ADC Extended Exported Functions
  * @{
  */

/** @defgroup ADCEx_Exported_Functions_Group1 Extended Input and Output operation functions
  * @brief    Extended IO operation functions
  *
@verbatim
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:

      (+) Perform the ADC self-calibration.
      (+) Get calibration factors.
      (+) Set calibration factors.

@endverbatim
  * @{
  */

/**
  * @brief  Perform an ADC automatic self-calibration
  *         Calibration prerequisite: ADC must be disabled (execute this
  *         function before HAL_ADC_Start() or after HAL_ADC_Stop() ).
  * @note   Calibration factor can be read after calibration, using function
  *         HAL_ADC_GetValue() (value on 7 bits: from DR[6;0]).
  * @param  hadc       ADC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADCEx_Calibration_Start(ADC_HandleTypeDef *hadc)
{
  HAL_StatusTypeDef tmp_hal_status;
  __IO uint32_t wait_loop_index = 0UL;
  uint32_t backup_setting_cfgr1;
  uint32_t calibration_index;
  uint32_t calibration_factor_accumulated = 0;
  uint32_t tickstart;

  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  __HAL_LOCK(hadc);

  /* Calibration prerequisite: ADC must be disabled. */

  /* Disable the ADC (if not already disabled) */
  tmp_hal_status = ADC_Disable(hadc);

  /* Check if ADC is effectively disabled */
  if (LL_ADC_IsEnabled(hadc->Instance) == 0UL)
  {
    /* Set ADC state */
    ADC_STATE_CLR_SET(hadc->State,
                      HAL_ADC_STATE_REG_BUSY,
                      HAL_ADC_STATE_BUSY_INTERNAL);

    /* Manage settings impacting calibration                                  */
    /* - Disable ADC mode auto power-off                                      */
    /* - Disable ADC DMA transfer request during calibration                  */
    /* Note: Specificity of this STM32 series: Calibration factor is          */
    /*       available in data register and also transferred by DMA.          */
    /*       To not insert ADC calibration factor among ADC conversion data   */
    /*       in array variable, DMA transfer must be disabled during          */
    /*       calibration.                                                     */
    backup_setting_cfgr1 = READ_BIT(hadc->Instance->CFGR1, ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG | ADC_CFGR1_AUTOFF);
    CLEAR_BIT(hadc->Instance->CFGR1, ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG | ADC_CFGR1_AUTOFF);

    /* ADC calibration procedure */
    /* Note: Perform an averaging of 8 calibrations for optimized accuracy */
    for (calibration_index = 0UL; calibration_index < 8UL; calibration_index++)
    {
      /* Start ADC calibration */
      LL_ADC_StartCalibration(hadc->Instance);

      /* Wait for calibration completion */
      while (LL_ADC_IsCalibrationOnGoing(hadc->Instance) != 0UL)
      {
        wait_loop_index++;
        if (wait_loop_index >= ADC_CALIBRATION_TIMEOUT)
        {
          /* Update ADC state machine to error */
          ADC_STATE_CLR_SET(hadc->State,
                            HAL_ADC_STATE_BUSY_INTERNAL,
                            HAL_ADC_STATE_ERROR_INTERNAL);

          __HAL_UNLOCK(hadc);

          return HAL_ERROR;
        }
      }

      calibration_factor_accumulated += LL_ADC_GetCalibrationFactor(hadc->Instance);
    }
    /* Compute average */
    calibration_factor_accumulated /= calibration_index;
    /* Apply calibration factor */
    LL_ADC_Enable(hadc->Instance);
    LL_ADC_SetCalibrationFactor(hadc->Instance, calibration_factor_accumulated);
    LL_ADC_Disable(hadc->Instance);

    /* Wait for ADC effectively disabled before changing configuration */
    /* Get tick count */
    tickstart = HAL_GetTick();

    while (LL_ADC_IsEnabled(hadc->Instance) != 0UL)
    {
      if ((HAL_GetTick() - tickstart) > ADC_DISABLE_TIMEOUT)
      {
        /* New check to avoid false timeout detection in case of preemption */
        if (LL_ADC_IsEnabled(hadc->Instance) != 0UL)
        {
          /* Update ADC state machine to error */
          SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);

          /* Set ADC error code to ADC peripheral internal error */
          SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);

          return HAL_ERROR;
        }
      }
    }

    /* Restore configuration after calibration */
    SET_BIT(hadc->Instance->CFGR1, backup_setting_cfgr1);

    /* Set ADC state */
    ADC_STATE_CLR_SET(hadc->State,
                      HAL_ADC_STATE_BUSY_INTERNAL,
                      HAL_ADC_STATE_READY);
  }
  else
  {
    SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);

    /* Note: No need to update variable "tmp_hal_status" here: already set    */
    /*       to state "HAL_ERROR" by function disabling the ADC.              */
  }

  __HAL_UNLOCK(hadc);

  return tmp_hal_status;
}

/**
  * @brief  Get the calibration factor.
  * @param hadc ADC handle.
  * @retval Calibration value.
  */
uint32_t HAL_ADCEx_Calibration_GetValue(ADC_HandleTypeDef *hadc)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  /* Return the selected ADC calibration value */
  return ((hadc->Instance->CALFACT) & 0x0000007FU);
}

/**
  * @brief  Set the calibration factor to overwrite automatic conversion result.
  *         ADC must be enabled and no conversion is ongoing.
  * @param hadc ADC handle
  * @param CalibrationFactor Calibration factor (coded on 7 bits maximum)
  * @retval HAL state
  */
HAL_StatusTypeDef HAL_ADCEx_Calibration_SetValue(ADC_HandleTypeDef *hadc, uint32_t CalibrationFactor)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  uint32_t tmp_adc_is_conversion_on_going_regular;

  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_CALFACT(CalibrationFactor));

  __HAL_LOCK(hadc);

  /* Verification of hardware constraints before modifying the calibration    */
  /* factors register: ADC must be enabled, no conversion on going.           */
  tmp_adc_is_conversion_on_going_regular = LL_ADC_REG_IsConversionOngoing(hadc->Instance);

  if ((LL_ADC_IsEnabled(hadc->Instance) != 0UL)
      && (tmp_adc_is_conversion_on_going_regular == 0UL)
     )
  {
    hadc->Instance->CALFACT &= ~ADC_CALFACT_CALFACT;
    hadc->Instance->CALFACT |= CalibrationFactor;
  }
  else
  {
    /* Update ADC state machine */
    SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
    /* Update ADC error code */
    SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);

    /* Update ADC state machine to error */
    tmp_hal_status = HAL_ERROR;
  }

  __HAL_UNLOCK(hadc);

  return tmp_hal_status;
}

/**
  * @brief  Analog watchdog 2 callback in non-blocking mode.
  * @param hadc ADC handle
  * @retval None
  */
__weak void HAL_ADCEx_LevelOutOfWindow2Callback(ADC_HandleTypeDef *hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADCEx_LevelOutOfWindow2Callback must be implemented in the user file.
  */
}

/**
  * @brief  Analog watchdog 3 callback in non-blocking mode.
  * @param hadc ADC handle
  * @retval None
  */
__weak void HAL_ADCEx_LevelOutOfWindow3Callback(ADC_HandleTypeDef *hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADCEx_LevelOutOfWindow3Callback must be implemented in the user file.
  */
}


/**
  * @brief  End Of Sampling callback in non-blocking mode.
  * @param hadc ADC handle
  * @retval None
  */
__weak void HAL_ADCEx_EndOfSamplingCallback(ADC_HandleTypeDef *hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADCEx_EndOfSamplingCallback must be implemented in the user file.
  */
}

/**
  * @brief  ADC channel configuration ready callback in non-blocking mode.
  * @param hadc ADC handle
  * @retval None
  */
__weak void HAL_ADCEx_ChannelConfigReadyCallback(ADC_HandleTypeDef *hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADCEx_ChannelConfigReadyCallback must be implemented in the user file.
  */
}

/**
  * @}
  */

/**
  * @brief  Disable ADC voltage regulator.
  * @note   Disabling voltage regulator allows to save power. This operation can
  *         be carried out only when ADC is disabled.
  * @note   To enable again the voltage regulator, the user is expected to
  *         resort to HAL_ADC_Init() API.
  * @param hadc ADC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADCEx_DisableVoltageRegulator(ADC_HandleTypeDef *hadc)
{
  HAL_StatusTypeDef tmp_hal_status;

  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  /* Setting of this feature is conditioned to ADC state: ADC must be ADC disabled */
  if (LL_ADC_IsEnabled(hadc->Instance) == 0UL)
  {
    LL_ADC_DisableInternalRegulator(hadc->Instance);
    tmp_hal_status = HAL_OK;
  }
  else
  {
    tmp_hal_status = HAL_ERROR;
  }

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
