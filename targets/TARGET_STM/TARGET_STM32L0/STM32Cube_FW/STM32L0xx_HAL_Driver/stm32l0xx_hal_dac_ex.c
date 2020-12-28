/**
  ******************************************************************************
  * @file    stm32l0xx_hal_dac_ex.c
  * @author  MCD Application Team
  * @brief   Extended DAC HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of DAC extension peripheral:
  *           + Extended features functions
  *     
  *     
  @verbatim
  ==============================================================================
                      ##### How to use this driver #####
  ==============================================================================
    [..]          
      (+) When Dual mode is enabled (i.e DAC Channel1 and Channel2 are used simultaneously) :
          Use HAL_DACEx_DualGetValue() to get digital data to be converted and use
          HAL_DACEx_DualSetValue() to set digital value to converted simultaneously in Channel 1 and Channel 2.  
      (+) Use HAL_DACEx_TriangleWaveGenerate() to generate Triangle signal.
      (+) Use HAL_DACEx_NoiseWaveGenerate() to generate Noise signal.
  
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


#if !defined (STM32L010xB) && !defined (STM32L010x8) && !defined (STM32L010x6) && !defined (STM32L010x4) && !defined (STM32L011xx) && !defined (STM32L021xx) && !defined (STM32L031xx) && !defined (STM32L041xx) && !defined (STM32L051xx) && !defined (STM32L061xx) && !defined (STM32L071xx) && !defined (STM32L081xx)
/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

#ifdef HAL_DAC_MODULE_ENABLED
/** @addtogroup STM32L0xx_HAL_Driver
  * @{
  */

/** @addtogroup DACEx DACEx
  * @brief DAC driver modules
  * @{
  */

/** @addtogroup DACEx_Private
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#if defined (STM32L072xx) || defined (STM32L073xx) || defined (STM32L082xx) || defined (STM32L083xx)
static void DAC_DMAConvCpltCh2(DMA_HandleTypeDef *hdma);
static void DAC_DMAErrorCh2(DMA_HandleTypeDef *hdma);
static void DAC_DMAHalfConvCpltCh2(DMA_HandleTypeDef *hdma); 
#endif
static void DAC_DMAConvCpltCh1(DMA_HandleTypeDef *hdma);
static void DAC_DMAErrorCh1(DMA_HandleTypeDef *hdma);
static void DAC_DMAHalfConvCpltCh1(DMA_HandleTypeDef *hdma);
/**
  * @}
  */

/** @addtogroup DACEx_Exported_Functions
  * @{
  */

/** @addtogroup DACEx_Exported_Functions_Group1
 *  @brief    Extended features functions 
 *

  * @{
  */

#if defined (STM32L072xx) || defined (STM32L073xx) || defined (STM32L082xx) || defined (STM32L083xx)
/**
  * @brief  Returns the last data output value of the selected DAC channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval The selected DAC channel data output value.
  */
uint32_t HAL_DACEx_DualGetValue(DAC_HandleTypeDef* hdac)
{
  uint32_t tmp = 0U;
  
  tmp |= hdac->Instance->DOR1;
  
  tmp |= hdac->Instance->DOR2 << 16U;
  
  /* Returns the DAC channel data output register value */
  return tmp;
}
#endif

/**
  * @brief  Enable or disable the selected DAC channel wave generation.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC Channel1 selected
  *            @arg DAC_CHANNEL_2: DAC Channel2 selected (STM32L07x/STM32L08x only)
  * @param  Amplitude Select max triangle amplitude. 
  *          This parameter can be one of the following values:
  *            @arg DAC_TRIANGLEAMPLITUDE_1: Select max triangle amplitude of 1
  *            @arg DAC_TRIANGLEAMPLITUDE_3: Select max triangle amplitude of 3
  *            @arg DAC_TRIANGLEAMPLITUDE_7: Select max triangle amplitude of 7
  *            @arg DAC_TRIANGLEAMPLITUDE_15: Select max triangle amplitude of 15
  *            @arg DAC_TRIANGLEAMPLITUDE_31: Select max triangle amplitude of 31
  *            @arg DAC_TRIANGLEAMPLITUDE_63: Select max triangle amplitude of 63
  *            @arg DAC_TRIANGLEAMPLITUDE_127: Select max triangle amplitude of 127
  *            @arg DAC_TRIANGLEAMPLITUDE_255: Select max triangle amplitude of 255
  *            @arg DAC_TRIANGLEAMPLITUDE_511: Select max triangle amplitude of 511
  *            @arg DAC_TRIANGLEAMPLITUDE_1023: Select max triangle amplitude of 1023
  *            @arg DAC_TRIANGLEAMPLITUDE_2047: Select max triangle amplitude of 2047
  *            @arg DAC_TRIANGLEAMPLITUDE_4095: Select max triangle amplitude of 4095
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DACEx_TriangleWaveGenerate(DAC_HandleTypeDef* hdac, uint32_t Channel, uint32_t Amplitude)
{
  /* Check the parameters */
  assert_param(IS_DAC_CHANNEL(Channel));
  assert_param(IS_DAC_LFSR_UNMASK_TRIANGLE_AMPLITUDE(Amplitude));

  /* Process locked */
  __HAL_LOCK(hdac);

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_BUSY;

  /* Enable the triangle wave generation for the selected DAC channel */
  MODIFY_REG(hdac->Instance->CR, ((DAC_CR_WAVE1)|(DAC_CR_MAMP1))<<Channel, (DAC_CR_WAVE1_1 | Amplitude) << Channel);
 

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_READY;

  /* Process unlocked */
  __HAL_UNLOCK(hdac);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Enable or disable the selected DAC channel wave generation.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC. 
  * @param  Channel The selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC Channel1 selected
  *            @arg DAC_CHANNEL_2: DAC Channel2 selected (STM32L07x/STM32L08x only)
  * @param  Amplitude Unmask DAC channel LFSR for noise wave generation.
  *          This parameter can be one of the following values:
  *            @arg DAC_LFSRUNMASK_BIT0: Unmask DAC channel LFSR bit0 for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS1_0: Unmask DAC channel LFSR bit[1:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS2_0: Unmask DAC channel LFSR bit[2:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS3_0: Unmask DAC channel LFSR bit[3:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS4_0: Unmask DAC channel LFSR bit[4:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS5_0: Unmask DAC channel LFSR bit[5:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS6_0: Unmask DAC channel LFSR bit[6:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS7_0: Unmask DAC channel LFSR bit[7:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS8_0: Unmask DAC channel LFSR bit[8:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS9_0: Unmask DAC channel LFSR bit[9:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS10_0: Unmask DAC channel LFSR bit[10:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS11_0: Unmask DAC channel LFSR bit[11:0] for noise wave generation
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DACEx_NoiseWaveGenerate(DAC_HandleTypeDef* hdac, uint32_t Channel, uint32_t Amplitude)
{
  /* Check the parameters */
  assert_param(IS_DAC_CHANNEL(Channel));
  assert_param(IS_DAC_LFSR_UNMASK_TRIANGLE_AMPLITUDE(Amplitude));

  /* Process locked */
  __HAL_LOCK(hdac);

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_BUSY;

/* Enable the noise wave generation for the selected DAC channel */
  MODIFY_REG(hdac->Instance->CR, ((DAC_CR_WAVE1)|(DAC_CR_MAMP1))<<Channel, (DAC_CR_WAVE1_0 | Amplitude) << Channel);

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_READY;

  /* Process unlocked */
  __HAL_UNLOCK(hdac);

  /* Return function status */
  return HAL_OK;
}

#if defined (STM32L072xx) || defined (STM32L073xx) || defined (STM32L082xx) || defined (STM32L083xx)
/**
  * @brief  Set the specified data holding register value for dual DAC channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *               the configuration information for the specified DAC.
  * @param  Alignment Specifies the data alignment for dual channel DAC.
  *          This parameter can be one of the following values:
  *            DAC_ALIGN_8B_R: 8bit right data alignment selected
  *            DAC_ALIGN_12B_L: 12bit left data alignment selected
  *            DAC_ALIGN_12B_R: 12bit right data alignment selected
  * @param  Data1 Data for DAC Channel1 to be loaded in the selected data holding register.
  * @param  Data2 Data for DAC Channel2 to be loaded in the selected data holding register.
  * @note   In dual mode, a unique register access is required to write in both
  *          DAC channels at the same time.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DACEx_DualSetValue(DAC_HandleTypeDef* hdac, uint32_t Alignment, uint32_t Data1, uint32_t Data2)
{  
  uint32_t data = 0U, tmp = 0U;
  
  /* Check the parameters */
  assert_param(IS_DAC_ALIGN(Alignment));
  assert_param(IS_DAC_DATA(Data1));
  assert_param(IS_DAC_DATA(Data2));
  
  /* Calculate and set dual DAC data holding register value */
  if (Alignment == DAC_ALIGN_8B_R)
  {
    data = ((uint32_t)Data2 << 8U) | Data1; 
  }
  else
  {
    data = ((uint32_t)Data2 << 16U) | Data1;
  }
  
  tmp = (uint32_t)hdac->Instance;
  tmp += DAC_DHR12RD_ALIGNMENT(Alignment);

  /* Set the dual DAC selected data holding register */
  *(__IO uint32_t *)tmp = data;
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Conversion complete callback in non blocking mode for Channel2 
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */
__weak void HAL_DACEx_ConvCpltCallbackCh2(DAC_HandleTypeDef* hdac)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdac);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_DACEx_ConvCpltCallbackCh2 could be implemented in the user file
   */
}

/**
  * @brief  Conversion half DMA transfer callback in non blocking mode for Channel2 
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */
__weak void HAL_DACEx_ConvHalfCpltCallbackCh2(DAC_HandleTypeDef* hdac)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdac);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_DACEx_ConvHalfCpltCallbackCh2 could be implemented in the user file
   */
}

/**
  * @brief  Error DAC callback for Channel2.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */
__weak void HAL_DACEx_ErrorCallbackCh2(DAC_HandleTypeDef *hdac)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdac);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_DACEx_ErrorCallbackCh2 could be implemented in the user file
   */
}

/**
  * @brief  DMA underrun DAC callback for Channel2.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */
__weak void HAL_DACEx_DMAUnderrunCallbackCh2(DAC_HandleTypeDef *hdac)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdac);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_DAC_DMAUnderrunCallbackCh2 could be implemented in the user file
   */
}

/**
  * @brief  Enables DAC and starts conversion of channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC Channel1 selected
  *            @arg DAC_CHANNEL_2: DAC Channel2 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DAC_Start(DAC_HandleTypeDef* hdac, uint32_t Channel)
{
  uint32_t tmp1 = 0U, tmp2 = 0U;
  
  /* Check the parameters */
  assert_param(IS_DAC_CHANNEL(Channel));

  /* Process locked */
  __HAL_LOCK(hdac);

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_BUSY;

  /* Enable the Peripharal */
  __HAL_DAC_ENABLE(hdac, Channel);

  if(Channel == DAC_CHANNEL_1)
  {
    tmp1 = hdac->Instance->CR & DAC_CR_TEN1;
    tmp2 = hdac->Instance->CR & DAC_CR_TSEL1;
    /* Check if software trigger enabled */
    if((tmp1 ==  DAC_CR_TEN1) && (tmp2 ==  DAC_CR_TSEL1))
    {
      /* Enable the selected DAC software conversion */
      SET_BIT(hdac->Instance->SWTRIGR, DAC_SWTRIGR_SWTRIG1);
    }
  }
  else
  {
    tmp1 = hdac->Instance->CR & DAC_CR_TEN2;
    tmp2 = hdac->Instance->CR & DAC_CR_TSEL2;
    /* Check if software trigger enabled */
    if((tmp1 == DAC_CR_TEN2) && (tmp2 == DAC_CR_TSEL2))
    {
      /* Enable the selected DAC software conversion*/
      SET_BIT(hdac->Instance->SWTRIGR, DAC_SWTRIGR_SWTRIG2);
    }
  }

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_READY;

  /* Process unlocked */
  __HAL_UNLOCK(hdac);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Enables DAC and starts conversion of channel using DMA.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC Channel1 selected
  *            @arg DAC_CHANNEL_2: DAC Channel2 selected
  * @param  pData The destination peripheral Buffer address.
  * @param  Length The length of data to be transferred from memory to DAC peripheral
  * @param  Alignment Specifies the data alignment for DAC channel.
  *          This parameter can be one of the following values:
  *            @arg DAC_ALIGN_8B_R: 8bit right data alignment selected
  *            @arg DAC_ALIGN_12B_L: 12bit left data alignment selected
  *            @arg DAC_ALIGN_12B_R: 12bit right data alignment selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DAC_Start_DMA(DAC_HandleTypeDef* hdac, uint32_t Channel, uint32_t* pData, uint32_t Length, uint32_t Alignment)
{
  uint32_t tmpreg = 0U;

  /* Check the parameters */
  assert_param(IS_DAC_CHANNEL(Channel));
  assert_param(IS_DAC_ALIGN(Alignment));

  /* Process locked */
  __HAL_LOCK(hdac);

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_BUSY;

  if(Channel == DAC_CHANNEL_1)
  {
    /* Set the DMA transfer complete callback for channel1 */
    hdac->DMA_Handle1->XferCpltCallback = DAC_DMAConvCpltCh1;

    /* Set the DMA half transfer complete callback for channel1 */
    hdac->DMA_Handle1->XferHalfCpltCallback = DAC_DMAHalfConvCpltCh1;

    /* Set the DMA error callback for channel1 */
    hdac->DMA_Handle1->XferErrorCallback = DAC_DMAErrorCh1;

    /* Enable the selected DAC channel1 DMA request */
    SET_BIT(hdac->Instance->CR, DAC_CR_DMAEN1);
       
    /* Case of use of channel 1 */
    switch(Alignment)
    {
      case DAC_ALIGN_12B_R:
        /* Get DHR12R1 address */
        tmpreg = (uint32_t)&hdac->Instance->DHR12R1;
        break;
      case DAC_ALIGN_12B_L:
        /* Get DHR12L1 address */
        tmpreg = (uint32_t)&hdac->Instance->DHR12L1;
        break;
      case DAC_ALIGN_8B_R:
        /* Get DHR8R1 address */
        tmpreg = (uint32_t)&hdac->Instance->DHR8R1;
        break;
      default:
        break;
    }
    UNUSED(tmpreg);		/* avoid warning on tmpreg affectation with stupid compiler */
  }
  else
  {
    /* Set the DMA transfer complete callback for channel2 */
    hdac->DMA_Handle2->XferCpltCallback = DAC_DMAConvCpltCh2;

    /* Set the DMA half transfer complete callback for channel2 */
    hdac->DMA_Handle2->XferHalfCpltCallback = DAC_DMAHalfConvCpltCh2;
    
    /* Set the DMA error callback for channel2 */
    hdac->DMA_Handle2->XferErrorCallback = DAC_DMAErrorCh2;

    /* Enable the selected DAC channel2 DMA request */
    SET_BIT(hdac->Instance->CR, DAC_CR_DMAEN2);

    /* Case of use of channel 2 */
    switch(Alignment)
    {
      case DAC_ALIGN_12B_R:
        /* Get DHR12R2 address */
        tmpreg = (uint32_t)&hdac->Instance->DHR12R2;
        break;
      case DAC_ALIGN_12B_L:
        /* Get DHR12L2 address */
        tmpreg = (uint32_t)&hdac->Instance->DHR12L2;
        break;
      case DAC_ALIGN_8B_R:
        /* Get DHR8R2 address */
        tmpreg = (uint32_t)&hdac->Instance->DHR8R2;
        break;
      default:
        break;
    }
  }
  
  /* Enable the DMA Stream */
  if(Channel == DAC_CHANNEL_1)
  {
    /* Enable the DAC DMA underrun interrupt */
    __HAL_DAC_ENABLE_IT(hdac, DAC_IT_DMAUDR1);
    
    /* Enable the DMA Stream */
    HAL_DMA_Start_IT(hdac->DMA_Handle1, (uint32_t)pData, tmpreg, Length);
  } 
  else
  {
    /* Enable the DAC DMA underrun interrupt */
    __HAL_DAC_ENABLE_IT(hdac, DAC_IT_DMAUDR2);
    
    /* Enable the DMA Stream */
    HAL_DMA_Start_IT(hdac->DMA_Handle2, (uint32_t)pData, tmpreg, Length);
  }
  
  /* Enable the Peripharal */
  __HAL_DAC_ENABLE(hdac, Channel);
  
  /* Process Unlocked */
  __HAL_UNLOCK(hdac);
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Disables DAC and stop conversion of channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC Channel1 selected
  *            @arg DAC_CHANNEL_2: DAC Channel2 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DAC_Stop_DMA(DAC_HandleTypeDef* hdac, uint32_t Channel)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Check the parameters */
  assert_param(IS_DAC_CHANNEL(Channel));

  /* Disable the selected DAC channel DMA request */
  CLEAR_BIT(hdac->Instance->CR, (DAC_CR_DMAEN1 << Channel));

  /* Disable the Peripharal */
  __HAL_DAC_DISABLE(hdac, Channel);

  /* Disable the DMA Channel */
  /* Channel1 is used */
  if(Channel == DAC_CHANNEL_1)
  { 
    status = HAL_DMA_Abort(hdac->DMA_Handle1);
  }
  else /* Channel2 is used for */
  { 
    status = HAL_DMA_Abort(hdac->DMA_Handle2); 
  }

  /* Check if DMA Channel effectively disabled */
  if(status != HAL_OK)
  {
    /* Update DAC state machine to error */
    hdac->State = HAL_DAC_STATE_ERROR;
  }
  else
  {
    /* Change DAC state */
    hdac->State = HAL_DAC_STATE_READY;
  }

  /* Return function status */
  return status;
}

/**
  * @brief  Returns the last data output value of the selected DAC channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC Channel1 selected
  *            @arg DAC_CHANNEL_2: DAC Channel2 selected
  * @retval The selected DAC channel data output value.
  */
uint32_t HAL_DAC_GetValue(DAC_HandleTypeDef* hdac, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_DAC_CHANNEL(Channel));
  
  /* Returns the DAC channel data output register value */
  if(Channel == DAC_CHANNEL_1)
  {
    return hdac->Instance->DOR1;
  }
  else
  {
    return hdac->Instance->DOR2;
  }
}

/**
  * @brief  Handles DAC interrupt request
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */
void HAL_DAC_IRQHandler(DAC_HandleTypeDef* hdac)
{
  /* Check underrun flag of DAC channel 1 */
  if(__HAL_DAC_GET_FLAG(hdac, DAC_FLAG_DMAUDR1))
  {
    /* Change DAC state to error state */
    hdac->State = HAL_DAC_STATE_ERROR;

    /* Set DAC error code to chanel1 DMA underrun error */
    hdac->ErrorCode |= HAL_DAC_ERROR_DMAUNDERRUNCH1;

    /* Clear the underrun flag */
    __HAL_DAC_CLEAR_FLAG(hdac,DAC_FLAG_DMAUDR1);

    /* Disable the selected DAC channel1 DMA request */
    CLEAR_BIT(hdac->Instance->CR, DAC_CR_DMAEN1);

    /* Error callback */
#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)
      hdac->DMAUnderrunCallbackCh1(hdac);
#else
    HAL_DAC_DMAUnderrunCallbackCh1(hdac);
#endif
  }
  
  /* Check underrun flag of DAC channel 2 */
  if(__HAL_DAC_GET_FLAG(hdac, DAC_FLAG_DMAUDR2))
  {
    /* Change DAC state to error state */
    hdac->State = HAL_DAC_STATE_ERROR;
    
    /* Set DAC error code to channel2 DMA underrun error */
    hdac->ErrorCode |= HAL_DAC_ERROR_DMAUNDERRUNCH2;
    
    /* Clear the underrun flag */
    __HAL_DAC_CLEAR_FLAG(hdac,DAC_FLAG_DMAUDR2);
    
    /* Disable the selected DAC channel1 DMA request */
    CLEAR_BIT(hdac->Instance->CR, DAC_CR_DMAEN2);
      
    /* Error callback */ 
#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)
      hdac->DMAUnderrunCallbackCh2(hdac);
#else
    HAL_DACEx_DMAUnderrunCallbackCh2(hdac);
#endif
  }
}


/**
  * @brief  Set the specified data holding register value for DAC channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC Channel1 selected
  *            @arg DAC_CHANNEL_2: DAC Channel2 selected
  * @param  Alignment Specifies the data alignment.
  *          This parameter can be one of the following values:
  *            @arg DAC_ALIGN_8B_R: 8bit right data alignment selected
  *            @arg DAC_ALIGN_12B_L: 12bit left data alignment selected
  *            @arg DAC_ALIGN_12B_R: 12bit right data alignment selected
  * @param  Data Data to be loaded in the selected data holding register.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DAC_SetValue(DAC_HandleTypeDef* hdac, uint32_t Channel, uint32_t Alignment, uint32_t Data)
{
  __IO uint32_t tmp = 0U;

  /* Check the parameters */
  assert_param(IS_DAC_CHANNEL(Channel));
  assert_param(IS_DAC_ALIGN(Alignment));
  assert_param(IS_DAC_DATA(Data));

  tmp = (uint32_t)hdac->Instance; 
  if(Channel == DAC_CHANNEL_1)
  {
    tmp += DAC_DHR12R1_ALIGNMENT(Alignment);
  }
  else
  {
    tmp += DAC_DHR12R2_ALIGNMENT(Alignment);
  }

  /* Set the DAC channel selected data holding register */
  *(__IO uint32_t *) tmp = Data;

  /* Return function status */
  return HAL_OK;
}
#else /* All products with only one channel */

/**
  * @brief  Enables DAC and starts conversion of channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC Channel1 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DAC_Start(DAC_HandleTypeDef* hdac, uint32_t Channel)
{
  uint32_t tmp1 = 0U, tmp2 = 0U;
  
  /* Check the parameters */
  assert_param(IS_DAC_CHANNEL(Channel));

  /* Process locked */
  __HAL_LOCK(hdac);

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_BUSY;

  /* Enable the Peripharal */
  __HAL_DAC_ENABLE(hdac, Channel);

  tmp1 = hdac->Instance->CR & DAC_CR_TEN1;
  tmp2 = hdac->Instance->CR & DAC_CR_TSEL1;
  /* Check if software trigger enabled */
  if((tmp1 ==  DAC_CR_TEN1) && (tmp2 ==  DAC_CR_TSEL1))
  {
    /* Enable the selected DAC software conversion */
    SET_BIT(hdac->Instance->SWTRIGR, DAC_SWTRIGR_SWTRIG1);
  }

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_READY;

  /* Process unlocked */
  __HAL_UNLOCK(hdac);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Enables DAC and starts conversion of channel using DMA.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC Channel1 selected
  * @param  pData The destination peripheral Buffer address.
  * @param  Length The length of data to be transferred from memory to DAC peripheral
  * @param  Alignment Specifies the data alignment for DAC channel.
  *          This parameter can be one of the following values:
  *            @arg DAC_ALIGN_8B_R: 8bit right data alignment selected
  *            @arg DAC_ALIGN_12B_L: 12bit left data alignment selected
  *            @arg DAC_ALIGN_12B_R: 12bit right data alignment selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DAC_Start_DMA(DAC_HandleTypeDef* hdac, uint32_t Channel, uint32_t* pData, uint32_t Length, uint32_t Alignment)
{
  uint32_t tmpreg = 0U;

  /* Check the parameters */
  assert_param(IS_DAC_CHANNEL(Channel));
  assert_param(IS_DAC_ALIGN(Alignment));

  /* Process locked */
  __HAL_LOCK(hdac);

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_BUSY;

  /* Set the DMA transfer complete callback for channel1 */
  hdac->DMA_Handle1->XferCpltCallback = DAC_DMAConvCpltCh1;

  /* Set the DMA half transfer complete callback for channel1 */
  hdac->DMA_Handle1->XferHalfCpltCallback = DAC_DMAHalfConvCpltCh1;

  /* Set the DMA error callback for channel1 */
  hdac->DMA_Handle1->XferErrorCallback = DAC_DMAErrorCh1;

  /* Enable the selected DAC channel1 DMA request */
  SET_BIT(hdac->Instance->CR, DAC_CR_DMAEN1);
     
  /* Case of use of channel 1 */
  switch(Alignment)
  {
    case DAC_ALIGN_12B_R:
      /* Get DHR12R1 address */
      tmpreg = (uint32_t)&hdac->Instance->DHR12R1;
      break;
    case DAC_ALIGN_12B_L:
      /* Get DHR12L1 address */
      tmpreg = (uint32_t)&hdac->Instance->DHR12L1;
      break;
    case DAC_ALIGN_8B_R:
      /* Get DHR8R1 address */
      tmpreg = (uint32_t)&hdac->Instance->DHR8R1;
      break;
    default:
      break;
  }
  UNUSED(tmpreg);		/* avoid warning on tmpreg affectation with stupid compiler */
  
  /* Enable the DMA Stream */
  /* Enable the DAC DMA underrun interrupt */
  __HAL_DAC_ENABLE_IT(hdac, DAC_IT_DMAUDR1);
  
  /* Enable the DMA Stream */
  HAL_DMA_Start_IT(hdac->DMA_Handle1, (uint32_t)pData, tmpreg, Length);
  
  /* Enable the Peripharal */
  __HAL_DAC_ENABLE(hdac, Channel);
  
  /* Process Unlocked */
  __HAL_UNLOCK(hdac);
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Disables DAC and stop conversion of channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC Channel1 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DAC_Stop_DMA(DAC_HandleTypeDef* hdac, uint32_t Channel)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Check the parameters */
  assert_param(IS_DAC_CHANNEL(Channel));

  /* Disable the selected DAC channel DMA request */
  CLEAR_BIT(hdac->Instance->CR, (DAC_CR_DMAEN1 << Channel));

  /* Disable the Peripharal */
  __HAL_DAC_DISABLE(hdac, Channel);

  /* Disable the DMA Channel */
  status = HAL_DMA_Abort(hdac->DMA_Handle1);

  /* Check if DMA Channel effectively disabled */
  if(status != HAL_OK)
  {
    /* Update DAC state machine to error */
    hdac->State = HAL_DAC_STATE_ERROR;
  }
  else
  {
    /* Change DAC state */
    hdac->State = HAL_DAC_STATE_READY;
  }

  /* Return function status */
  return status;
}

/**
  * @brief  Returns the last data output value of the selected DAC channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC Channel1 selected
  * @retval The selected DAC channel data output value.
  */
uint32_t HAL_DAC_GetValue(DAC_HandleTypeDef* hdac, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_DAC_CHANNEL(Channel));
  
  /* Returns the DAC channel data output register value */
  return hdac->Instance->DOR1;
}

/**
  * @brief  Handles DAC interrupt request
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */
void HAL_DAC_IRQHandler(DAC_HandleTypeDef* hdac)
{
  /* Check underrun flag of DAC channel 1 */
  if(__HAL_DAC_GET_FLAG(hdac, DAC_FLAG_DMAUDR1))
  {
    /* Change DAC state to error state */
    hdac->State = HAL_DAC_STATE_ERROR;

    /* Set DAC error code to chanel1 DMA underrun error */
    hdac->ErrorCode |= HAL_DAC_ERROR_DMAUNDERRUNCH1;

    /* Clear the underrun flag */
    __HAL_DAC_CLEAR_FLAG(hdac,DAC_FLAG_DMAUDR1);

    /* Disable the selected DAC channel1 DMA request */
    CLEAR_BIT(hdac->Instance->CR, DAC_CR_DMAEN1);

    /* Error callback */
    HAL_DAC_DMAUnderrunCallbackCh1(hdac);
  }
}

/**
  * @brief  Set the specified data holding register value for DAC channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC Channel1 selected
  * @param  Alignment Specifies the data alignment.
  *          This parameter can be one of the following values:
  *            @arg DAC_ALIGN_8B_R: 8bit right data alignment selected
  *            @arg DAC_ALIGN_12B_L: 12bit left data alignment selected
  *            @arg DAC_ALIGN_12B_R: 12bit right data alignment selected
  * @param  Data Data to be loaded in the selected data holding register.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DAC_SetValue(DAC_HandleTypeDef* hdac, uint32_t Channel, uint32_t Alignment, uint32_t Data)
{
  __IO uint32_t tmp = 0U;

  /* Check the parameters */
  assert_param(IS_DAC_CHANNEL(Channel));
  assert_param(IS_DAC_ALIGN(Alignment));
  assert_param(IS_DAC_DATA(Data));

  tmp = (uint32_t)hdac->Instance; 
  tmp += DAC_DHR12R1_ALIGNMENT(Alignment);

  /* Set the DAC channel selected data holding register */
  *(__IO uint32_t *) tmp = Data;

  /* Return function status */
  return HAL_OK;
}

#endif  /* #if defined (STM32L072xx) || defined (STM32L073xx) || defined (STM32L082xx) || defined (STM32L083xx) */

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup DACEx_Private
  * @{
  */
#if defined (STM32L072xx) || defined (STM32L073xx) || defined (STM32L082xx) || defined (STM32L083xx)
/**
  * @brief  DMA conversion complete callback. 
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void DAC_DMAConvCpltCh2(DMA_HandleTypeDef *hdma)   
{
  DAC_HandleTypeDef* hdac = ( DAC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
  
  HAL_DACEx_ConvCpltCallbackCh2(hdac); 
  
  hdac->State= HAL_DAC_STATE_READY;
}

/**
  * @brief  DMA half transfer complete callback. 
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void DAC_DMAHalfConvCpltCh2(DMA_HandleTypeDef *hdma)   
{
    DAC_HandleTypeDef* hdac = ( DAC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
    /* Conversion complete callback */
    HAL_DACEx_ConvHalfCpltCallbackCh2(hdac); 
}

/**
  * @brief  DMA error callback 
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void DAC_DMAErrorCh2(DMA_HandleTypeDef *hdma)   
{
  DAC_HandleTypeDef* hdac = ( DAC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
    
  /* Set DAC error code to DMA error */
  hdac->ErrorCode |= HAL_DAC_ERROR_DMA;
    
  HAL_DACEx_ErrorCallbackCh2(hdac); 
    
  hdac->State= HAL_DAC_STATE_READY;
}
#endif /* STM32L072xx || STM32L073xx || STM32L082xx || STM32L083xx */

/**
  * @brief  DMA conversion complete callback. 
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void DAC_DMAConvCpltCh1(DMA_HandleTypeDef *hdma)   
{
  DAC_HandleTypeDef* hdac = ( DAC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
  
#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)
  hdac->ConvCpltCallbackCh1(hdac);
#else
  HAL_DAC_ConvCpltCallbackCh1(hdac); 
#endif
  
  hdac->State= HAL_DAC_STATE_READY;
}

/**
  * @brief  DMA half transfer complete callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void DAC_DMAHalfConvCpltCh1(DMA_HandleTypeDef *hdma)
{
  DAC_HandleTypeDef* hdac = ( DAC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;

  /* Conversion complete callback */
#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)
  hdac->ConvHalfCpltCallbackCh1(hdac);
#else
  HAL_DAC_ConvHalfCpltCallbackCh1(hdac);
#endif  
}

/**
  * @brief  DMA error callback
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void DAC_DMAErrorCh1(DMA_HandleTypeDef *hdma)
{
  DAC_HandleTypeDef* hdac = ( DAC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;

  /* Set DAC error code to DMA error */
  hdac->ErrorCode |= HAL_DAC_ERROR_DMA;

  HAL_DAC_ErrorCallbackCh1(hdac);

  hdac->State= HAL_DAC_STATE_READY;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif /* HAL_DAC_MODULE_ENABLED */
#endif /* #if !defined (STM32L010xB) && !defined (STM32L010x8) && !defined (STM32L010x6) && !defined (STM32L010x4) && !defined (STM32L011xx) && !defined (STM32L021xx) && !defined (STM32L031xx) && !defined (STM32L041xx) && !defined (STM32L051xx) && !defined (STM32L061xx) && !defined (STM32L071xx) && !defined (STM32L081xx) */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

