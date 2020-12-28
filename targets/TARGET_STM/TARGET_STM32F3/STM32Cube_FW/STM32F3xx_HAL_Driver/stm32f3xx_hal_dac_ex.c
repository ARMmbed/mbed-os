/**
  ******************************************************************************
  * @file    stm32f3xx_hal_dac_ex.c
  * @author  MCD Application Team
  * @brief   DACEx HAL module driver.
  *          This file provides firmware functions to manage the extended 
  *          functionalities of the DAC peripheral.  
  *     
  *
  @verbatim   
  ==============================================================================
                      ##### How to use this driver #####
  ==============================================================================
    [..]          
      (+) When Dual mode is enabled (i.e. DAC Channel1 and Channel2 are used simultaneously) :
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/** @addtogroup STM32F3xx_HAL_Driver
  * @{
  */

#ifdef HAL_DAC_MODULE_ENABLED

/** @defgroup DACEx DACEx
  * @brief DAC HAL module driver
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup DACEx_Private_Functions DACEx Private Functions
  * @{
  */
static void DAC_DMAConvCpltCh1(DMA_HandleTypeDef *hdma);
static void DAC_DMAErrorCh1(DMA_HandleTypeDef *hdma);
static void DAC_DMAHalfConvCpltCh1(DMA_HandleTypeDef *hdma); 

#if defined(STM32F303xE) || defined(STM32F398xx)                         || \
    defined(STM32F303xC) || defined(STM32F358xx)                         || \
    defined(STM32F303x8) || defined(STM32F334x8) || defined(STM32F328xx) || \
    defined(STM32F373xC) || defined(STM32F378xx) 
/* DAC channel 2 is available on top of DAC channel 1U */
static void DAC_DMAConvCpltCh2(DMA_HandleTypeDef *hdma);
static void DAC_DMAErrorCh2(DMA_HandleTypeDef *hdma);
static void DAC_DMAHalfConvCpltCh2(DMA_HandleTypeDef *hdma); 
#endif /* STM32F303xE || STM32F398xx                || */
       /* STM32F303xC || STM32F358xx                || */
       /* STM32F303x8 || STM32F334x8 || STM32F328xx || */
       /* STM32F373xC || STM32F378xx                   */
/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/
/** @defgroup DACEx_Exported_Functions DACEx Exported Functions
  * @{
  */

/** @defgroup DACEx_Exported_Functions_Group3 DACEx Peripheral Control functions
 *  @brief   	Peripheral Control functions 
 *
@verbatim   
  ==============================================================================
             ##### Peripheral Control functions #####
  ==============================================================================  
    [..]  This section provides functions allowing to:
      (+) Set the specified data holding register value for DAC channel.
      (+) Set the specified data holding register value for dual DAC channel
	      (when DAC channel 2 is present in DAC 1U)
      
@endverbatim
  * @{
  */

/**
  * @brief  Set the specified data holding register value for DAC channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel. 
  * @param  Alignment Specifies the data alignment for DAC channel.
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
   
  tmp = (uint32_t) (hdac->Instance);

/* DAC 1 has 1 or 2 channels - no DAC2 */
/* DAC 1 has 2 channels 1U & 2U - DAC 2 has one channel 1U */

  if(Channel == DAC_CHANNEL_1)
  {
    tmp += DAC_DHR12R1_ALIGNMENT(Alignment);
  }
#if defined(STM32F303xE) || defined(STM32F398xx)                         || \
    defined(STM32F303xC) || defined(STM32F358xx)                         || \
    defined(STM32F303x8) || defined(STM32F334x8) || defined(STM32F328xx) || \
    defined(STM32F373xC) || defined(STM32F378xx)
  else /* channel = DAC_CHANNEL_2  */
  {
    tmp += DAC_DHR12R2_ALIGNMENT(Alignment);
  }
#endif /* STM32F303xE || STM32F398xx                || */
       /* STM32F303xC || STM32F358xx                || */
       /* STM32F303x8 || STM32F334x8 || STM32F328xx || */
       /* STM32F373xC || STM32F378xx                   */

  /* Set the DAC channel1 selected data holding register */
  *(__IO uint32_t *) tmp = Data;
  
  /* Return function status */
  return HAL_OK;
}

#if  defined(STM32F303xE) || defined(STM32F398xx)                         || \
     defined(STM32F303xC) || defined(STM32F358xx)                         || \
     defined(STM32F303x8) || defined(STM32F334x8) || defined(STM32F328xx) || \
     defined(STM32F373xC) || defined(STM32F378xx)
/* DAC channel 2 is present in DAC 1U */
/**
  * @brief  Set the specified data holding register value for dual DAC channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Alignment Specifies the data alignment for dual channel DAC.
  *          This parameter can be one of the following values:
  *            @arg DAC_ALIGN_8B_R: 8bit right data alignment selected
  *            @arg DAC_ALIGN_12B_L: 12bit left data alignment selected
  *            @arg DAC_ALIGN_12B_R: 12bit right data alignment selected
  * @param  Data2: Data for DAC Channel2 to be loaded in the selected data holding register.
  * @param  Data1: Data for DAC Channel1 to be loaded in the selected data  holding register.
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

    tmp = (uint32_t) (hdac->Instance);
    tmp += DAC_DHR12RD_ALIGNMENT(Alignment);

  /* Set the dual DAC selected data holding register */
  *(__IO uint32_t *)tmp = data;

  /* Return function status */
  return HAL_OK;
}
#endif /* STM32F303xE || STM32F398xx                || */
       /* STM32F303xC || STM32F358xx                || */
       /* STM32F303x8 || STM32F334x8 || STM32F328xx || */
       /* STM32F373xC || STM32F378xx                   */

/**
  * @}
  */

/** @defgroup DACEx_Exported_Functions_Group2 DACEx Input and Output operation functions
 *  @brief    IO operation functions 
 *
@verbatim   
  ==============================================================================
             ##### IO operation functions #####
  ==============================================================================  
    [..]  This section provides functions allowing to:
      (+) Start conversion.
      (+) Start conversion and enable DMA transfer.
      (+) Get result of conversion.
      (+) Handle DAC IRQ's.
      (+) Generate triangular-wave                   
      (+) Generate noise-wave
	  (+) Callback functions for DAC1 Channel2 (when supported)
@endverbatim
  * @{
  */

#if  defined(STM32F303xE) || defined(STM32F398xx)                         || \
     defined(STM32F303xC) || defined(STM32F358xx)                         || \
     defined(STM32F303x8) || defined(STM32F334x8) || defined(STM32F328xx) || \
     defined(STM32F373xC) || defined(STM32F378xx)

/* DAC 1 has 2 channels 1U & 2U - DAC 2 has one channel 1U */
/**
  * @brief  Enables DAC and starts conversion of channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC1 Channel1 or DAC2 Channel1 selected
  *            @arg DAC_CHANNEL_2: DAC1 Channel2 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DAC_Start(DAC_HandleTypeDef* hdac, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_DAC_CHANNEL_INSTANCE(hdac->Instance, Channel));
  
  /* Process locked */
  __HAL_LOCK(hdac);
  
  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_BUSY;
  
  /* Enable the Peripheral */
  __HAL_DAC_ENABLE(hdac, Channel);
  
  if(Channel == DAC_CHANNEL_1)
  {
    /* Check if software trigger enabled */
    if((hdac->Instance->CR & (DAC_CR_TEN1 | DAC_CR_TSEL1)) == (DAC_CR_TEN1 | DAC_CR_TSEL1))
    {
      /* Enable the selected DAC software conversion */
      SET_BIT(hdac->Instance->SWTRIGR, DAC_SWTRIGR_SWTRIG1);
    }
  }
  else
  {
    /* Check if software trigger enabled */
    if((hdac->Instance->CR & (DAC_CR_TEN2 | DAC_CR_TSEL2)) == (DAC_CR_TEN2 | DAC_CR_TSEL2))
    {
      /* Enable the selected DAC software conversion */
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
#endif /* STM32F303xE || STM32F398xx                || */
       /* STM32F303xC || STM32F358xx                || */
       /* STM32F303x8 || STM32F334x8 || STM32F328xx || */
       /* STM32F373xC || STM32F378xx                   */

#if defined(STM32F302xE) || \
    defined(STM32F302xC) || \
    defined(STM32F301x8) || defined(STM32F302x8) || defined(STM32F318xx)
/* DAC 1 has 1 channels 1U */
/**
  * @brief  Enables DAC and starts conversion of channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC1 Channel1 selected
  * @retval HAL status 
  */

HAL_StatusTypeDef HAL_DAC_Start(DAC_HandleTypeDef* hdac, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_DAC_CHANNEL_INSTANCE(hdac->Instance, Channel));
  
  /* Process locked */
  __HAL_LOCK(hdac);
  
  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_BUSY;
  
  /* Enable the Peripheral */
  __HAL_DAC_ENABLE(hdac, Channel);
  
  /* Check if software trigger enabled */
  if((hdac->Instance->CR & (DAC_CR_TEN1 | DAC_CR_TSEL1)) == (DAC_CR_TEN1 | DAC_CR_TSEL1))
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
#endif /* STM32F302xE                               || */
       /* STM32F302xC                               || */
       /* STM32F301x8 || STM32F302x8 || STM32F318xx    */

/* DAC 1 has 2 channels 1U & 2U - DAC 2 has one channel 1U */
#if defined(STM32F303xE) || defined(STM32F398xx)                         || \
    defined(STM32F303xC) || defined(STM32F358xx)                         || \
    defined(STM32F303x8) || defined(STM32F334x8) || defined(STM32F328xx) || \
    defined(STM32F373xC) || defined(STM32F378xx)
/* DAC 1 has 2 channels 1U & 2U */
/**
  * @brief  Enables DAC and starts conversion of channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC1 Channel1 selected
  *            @arg DAC_CHANNEL_2: DAC1 Channel2 selected
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
  assert_param(IS_DAC_CHANNEL_INSTANCE(hdac->Instance, Channel));
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

    /* Case of use of channel 1U */
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
   
    /* Case of use of channel 2U */
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
 
  /* Enable the DMA Channel */
  if(Channel == DAC_CHANNEL_1)
  {
    /* Enable the DAC DMA underrun interrupt */
    __HAL_DAC_ENABLE_IT(hdac, DAC_IT_DMAUDR1);

    /* Enable the DMA Channel */
    HAL_DMA_Start_IT(hdac->DMA_Handle1, (uint32_t)pData, tmpreg, Length);
  } 
  else
  {
    /* Enable the DAC DMA underrun interrupt */
    __HAL_DAC_ENABLE_IT(hdac, DAC_IT_DMAUDR2);

    /* Enable the DMA Channel */
    HAL_DMA_Start_IT(hdac->DMA_Handle2, (uint32_t)pData, tmpreg, Length);
  }
 
  /* Process Unlocked */
  __HAL_UNLOCK(hdac);

  /* Enable the Peripheral */
  __HAL_DAC_ENABLE(hdac, Channel);
  
  /* Return function status */
  return HAL_OK;
}
#endif /* STM32F303xE || STM32F398xx                || */
       /* STM32F303xC || STM32F358xx                || */
       /* STM32F303x8 || STM32F334x8 || STM32F328xx || */
       /* STM32F373xC || STM32F378xx                   */

#if defined(STM32F302xE) || \
    defined(STM32F302xC) || \
    defined(STM32F301x8) || defined(STM32F302x8) || defined(STM32F318xx)
/* DAC 1 has 1 channel (channel 1U)  */
/**
  * @brief  Enables DAC and starts conversion of channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC1 Channel1 selected
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
  assert_param(IS_DAC_CHANNEL_INSTANCE(hdac->Instance, Channel));
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
    
  /* Case of use of channel 1U */
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
  
  /* Enable the DMA Channel */
  /* Enable the DAC DMA underrun interrupt */
  __HAL_DAC_ENABLE_IT(hdac, DAC_IT_DMAUDR1);

  /* Enable the DMA Channel */
  HAL_DMA_Start_IT(hdac->DMA_Handle1, (uint32_t)pData, tmpreg, Length);
 
  /* Process Unlocked */
  __HAL_UNLOCK(hdac);

  /* Enable the Peripheral */
  __HAL_DAC_ENABLE(hdac, Channel); 
 
  /* Return function status */
  return HAL_OK;
}
#endif /* STM32F302xE                               || */
       /* STM32F302xC                               || */
       /* STM32F301x8 || STM32F302x8 || STM32F318xx    */

/* DAC 1 has 2 channels 1U & 2U - DAC 2 has one channel 1U */
#if  defined(STM32F303xE) || defined(STM32F398xx)                         || \
     defined(STM32F303xC) || defined(STM32F358xx)                         || \
     defined(STM32F303x8) || defined(STM32F334x8) || defined(STM32F328xx) || \
     defined(STM32F373xC) || defined(STM32F378xx)
/* DAC 1 has 2 channels 1U & 2U */
/**
  * @brief  Returns the last data output value of the selected DAC channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC1 Channel1 selected
  *            @arg DAC_CHANNEL_2: DAC1 Channel2 selected
  * @retval The selected DAC channel data output value.
  */
uint32_t HAL_DAC_GetValue(DAC_HandleTypeDef* hdac, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_DAC_CHANNEL_INSTANCE(hdac->Instance, Channel));
  
  /* Returns the DAC channel data output register value */
  if(Channel == DAC_CHANNEL_1) 
  {
    return hdac->Instance->DOR1;
  }
  else /* channel = DAC_CHANNEL_2  */
  {
    return hdac->Instance->DOR2;
  }
}
#endif /* STM32F303xE || STM32F398xx                || */
       /* STM32F303xC || STM32F358xx                || */
       /* STM32F303x8 || STM32F334x8 || STM32F328xx || */
       /* STM32F373xC || STM32F378xx                   */

#if defined(STM32F302xE) || \
    defined(STM32F302xC) || \
    defined(STM32F301x8) || defined(STM32F302x8) || defined(STM32F318xx)
/* DAC 1 has 1 channel (channel 1U)  */
/**
  * @brief  Returns the last data output value of the selected DAC channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  channel The selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC1 Channel1 selected
  * @retval The selected DAC channel data output value.
  */
uint32_t HAL_DAC_GetValue(DAC_HandleTypeDef* hdac, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_DAC_CHANNEL_INSTANCE(hdac->Instance, Channel));
  
  /* Returns the DAC channel data output register value */
  return hdac->Instance->DOR1;
}
#endif /* STM32F302xE                               || */
       /* STM32F302xC                               || */
       /* STM32F301x8 || STM32F302x8 || STM32F318xx    */

/**
  * @brief  Return the last data output value of the selected DAC channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval The selected DAC channel data output value.
  */
uint32_t HAL_DACEx_DualGetValue(DAC_HandleTypeDef* hdac)
{
  uint32_t tmp = 0U;

  tmp |= hdac->Instance->DOR1;

#if  defined(STM32F303xE) || defined(STM32F398xx)                         || \
     defined(STM32F303xC) || defined(STM32F358xx)                         || \
     defined(STM32F303x8) || defined(STM32F334x8) || defined(STM32F328xx) || \
     defined(STM32F373xC) || defined(STM32F378xx)
/* DAC channel 2 is present in DAC 1U */
  tmp |= hdac->Instance->DOR2 << 16U;
#endif /* STM32F303xE || STM32F398xx                || */
       /* STM32F303xC || STM32F358xx                || */
       /* STM32F303x8 || STM32F334x8 || STM32F328xx || */
       /* STM32F373xC || STM32F378xx                   */

  /* Returns the DAC channel data output register value */
  return tmp;
}

#if  defined(STM32F303xE) || defined(STM32F398xx)                         || \
     defined(STM32F303xC) || defined(STM32F358xx)                         || \
     defined(STM32F303x8) || defined(STM32F334x8) || defined(STM32F328xx) || \
     defined(STM32F373xC) || defined(STM32F378xx)
/* DAC channel 2 is available on top of DAC channel 1U */
/**
  * @brief  Handles DAC interrupt request
  *         This function uses the interruption of DMA
  *         underrun.  
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */
void HAL_DAC_IRQHandler(struct __DAC_HandleTypeDef* hdac)
{
  if(__HAL_DAC_GET_IT_SOURCE(hdac, DAC_IT_DMAUDR1))
  { 
    /* Check underrun flag of DAC channel 1U */
    if(__HAL_DAC_GET_FLAG(hdac, DAC_FLAG_DMAUDR1))
    {
      /* Change DAC state to error state */
      hdac->State = HAL_DAC_STATE_ERROR;
    
      /* Set DAC error code to chanel1 DMA underrun error */
      SET_BIT(hdac->ErrorCode, HAL_DAC_ERROR_DMAUNDERRUNCH1);
          
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
  }
  
  if(__HAL_DAC_GET_IT_SOURCE(hdac, DAC_IT_DMAUDR2))
  {
    /* Check underrun flag of DAC channel 2 */
    if (__HAL_DAC_GET_FLAG(hdac, DAC_FLAG_DMAUDR2))
    {
      /* Change DAC state to error state */
      hdac->State = HAL_DAC_STATE_ERROR;
    
      /* Set DAC error code to channel2 DMA underrun error */
      SET_BIT(hdac->ErrorCode, HAL_DAC_ERROR_DMAUNDERRUNCH2);
    
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
}
#endif /* STM32F303xE || STM32F398xx                || */
       /* STM32F303xC || STM32F358xx                || */
       /* STM32F303x8 || STM32F334x8 || STM32F328xx || */
       /* STM32F373xC || STM32F378xx                   */

#if defined(STM32F302xE) || \
    defined(STM32F302xC) || \
    defined(STM32F301x8) || defined(STM32F302x8) || defined(STM32F318xx)
/* DAC channel 2 is NOT available. Only DAC channel 1 is available */
/**
  * @brief  Handles DAC interrupt request
  *         This function uses the interruption of DMA
  *         underrun.  
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */
void HAL_DAC_IRQHandler(struct __DAC_HandleTypeDef* hdac)
{
  if(__HAL_DAC_GET_IT_SOURCE(hdac, DAC_IT_DMAUDR1))
  {  
    /* Check underrun flag of DAC channel 1U */
    if(__HAL_DAC_GET_FLAG(hdac, DAC_FLAG_DMAUDR1))
    {
      /* Change DAC state to error state */
      hdac->State = HAL_DAC_STATE_ERROR;
    
      /* Set DAC error code to chanel1 DMA underrun error */
      SET_BIT(hdac->ErrorCode, HAL_DAC_ERROR_DMAUNDERRUNCH1);
    
      /* Clear the underrun flag */
      __HAL_DAC_CLEAR_FLAG(hdac,DAC_FLAG_DMAUDR1);
    
      /* Disable the selected DAC channel1 DMA request */
      CLEAR_BIT(hdac->Instance->CR, DAC_CR_DMAEN1);
    
      /* Error callback */ 
      HAL_DAC_DMAUnderrunCallbackCh1(hdac);
    }
  }
}
#endif /* STM32F302xE                               || */
       /* STM32F302xC                               || */
       /* STM32F301x8 || STM32F302x8 || STM32F318xx    */

/**
  * @brief  Configures the selected DAC channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  sConfig DAC configuration structure.
  * @param  Channel The selected DAC channel. 
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC1 Channel1 selected
  *            @arg DAC_CHANNEL_2: DAC1 Channel2 selected  
  *            @arg DAC_CHANNEL_1: DAC2 Channel1 selected 
  * @retval HAL status
  */

HAL_StatusTypeDef HAL_DAC_ConfigChannel(DAC_HandleTypeDef* hdac, DAC_ChannelConfTypeDef* sConfig, uint32_t Channel)
{
  uint32_t tmpreg1 = 0U, tmpreg2 = 0U;

  /* Check the DAC parameters */
  assert_param(IS_DAC_TRIGGER(sConfig->DAC_Trigger));

#if defined(STM32F303x8) || defined(STM32F334x8) || defined(STM32F328xx)
  if ((hdac->Instance == DAC1) && (Channel == DAC_CHANNEL_1)) 
  {
    /* Output Buffer (BOFF1) control */
    assert_param(IS_DAC_OUTPUT_BUFFER_STATE(sConfig->DAC_OutputBuffer));    
  }
  else /* DAC1 channel 2U & DAC2 channel 1U */
  {
    /* Output Switch (OUTEN) control */
    assert_param(IS_DAC_OUTPUT_SWITCH_STATE(sConfig->DAC_OutputSwitch));    
  }    
#else
  assert_param(IS_DAC_OUTPUT_BUFFER_STATE(sConfig->DAC_OutputBuffer));    
#endif /* STM32F303x8 || STM32F334x8 || STM32F328xx || */
  assert_param(IS_DAC_CHANNEL(Channel));   
 
  /* Process locked */
  __HAL_LOCK(hdac);
  
  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_BUSY;
  
  /* Get the DAC CR value */
  tmpreg1 = hdac->Instance->CR;
  
  /* Clear BOFFx-OUTENx, TENx, TSELx, WAVEx and MAMPx bits */
  
  /* Configure for the selected DAC channel: buffer output or switch output, trigger */
  /* Set TSELx and TENx bits according to DAC_Trigger value */
  /* Set BOFFx bit according to DAC_OutputBuffer value OR */   
  /* Set OUTEN bit according to DAC_OutputSwitch value */   
#if defined(STM32F303x8) || defined(STM32F334x8) || defined(STM32F328xx)
  if ((hdac->Instance == DAC1) && (Channel == DAC_CHANNEL_1)) 
  {
    /* Output Buffer (BOFF1) control */
    tmpreg1 &= ~(((uint32_t)(DAC_CR_MAMP1 | DAC_CR_WAVE1 | DAC_CR_TSEL1 | DAC_CR_TEN1 | DAC_CR_BOFF1)) << Channel);
    tmpreg2 = (sConfig->DAC_Trigger | sConfig->DAC_OutputBuffer);    
  }
  else /* DAC1 channel 2U & DAC2 channel 1U */
  {
    /* Output Switch (OUTEN) control */
    tmpreg1 &= ~(((uint32_t)(DAC_CR_MAMP1 | DAC_CR_WAVE1 | DAC_CR_TSEL1 | DAC_CR_TEN1 | DAC_CR_OUTEN1)) << Channel);    
    tmpreg2 = (sConfig->DAC_Trigger | sConfig->DAC_OutputSwitch);    
  }    
#else
  tmpreg1 &= ~(((uint32_t)(DAC_CR_MAMP1 | DAC_CR_WAVE1 | DAC_CR_TSEL1 | DAC_CR_TEN1 | DAC_CR_BOFF1)) << Channel);
  tmpreg2 = (sConfig->DAC_Trigger | sConfig->DAC_OutputBuffer);
#endif  /* STM32F303x8 || STM32F334x8 || STM32F328xx || */
  
  /* Calculate CR register value depending on DAC_Channel */
  tmpreg1 |= tmpreg2 << Channel;
  /* Write to DAC CR */
  hdac->Instance->CR = tmpreg1;
  
  /* Disable wave generation */
  hdac->Instance->CR &= ~(DAC_CR_WAVE1 << Channel);

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_READY;
  
  /* Process unlocked */
  __HAL_UNLOCK(hdac);
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Enables or disables the selected DAC channel wave generation.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel.
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC1 Channel1 selected
  *            @arg DAC_CHANNEL_2: DAC1 Channel2 selected
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
  * @note   Wave generation is not available in DAC2.
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

  /* Enable the selected wave generation for the selected DAC channel */
  MODIFY_REG(hdac->Instance->CR, ((DAC_CR_WAVE1)|(DAC_CR_MAMP1))<<Channel, (DAC_CR_WAVE1_1 | Amplitude) << Channel);

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_READY;

  /* Process unlocked */
  __HAL_UNLOCK(hdac);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Enables or disables the selected DAC channel wave generation.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel.
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC1 Channel1 selected
  *            @arg DAC_CHANNEL_2: DAC1 Channel2 selected
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

  /* Enable the selected wave generation for the selected DAC channel */
  MODIFY_REG(hdac->Instance->CR, ((DAC_CR_WAVE1)|(DAC_CR_MAMP1))<<Channel, (DAC_CR_WAVE1_0 | Amplitude) << Channel);
  
  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_READY;

  /* Process unlocked */
  __HAL_UNLOCK(hdac);

  /* Return function status */
  return HAL_OK;
}

#if  defined(STM32F303xE) || defined(STM32F398xx)                         || \
     defined(STM32F303xC) || defined(STM32F358xx)                         || \
     defined(STM32F303x8) || defined(STM32F334x8) || defined(STM32F328xx) || \
     defined(STM32F373xC) || defined(STM32F378xx)
/* DAC channel 2 is available on top of DAC channel 1U */
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

  /* NOTE : This function Should not be modified, when the callback is needed,
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

  /* NOTE : This function Should not be modified, when the callback is needed,
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

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DACEx_ErrorCallbackCh2 could be implemented in the user file
   */
}

/**
  * @brief  DMA underrun DAC callback for channel2.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */
__weak void HAL_DACEx_DMAUnderrunCallbackCh2(DAC_HandleTypeDef *hdac)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdac);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DACEx_DMAUnderrunCallbackCh2 could be implemented in the user file
   */
}
#endif /* STM32F303xE || STM32F398xx                || */
       /* STM32F303xC || STM32F358xx                || */
       /* STM32F303x8 || STM32F334x8 || STM32F328xx || */
       /* STM32F373xC || STM32F378xx                   */
/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup DACEx_Private_Functions
  * @{
  */
  
/**
  * @brief  DMA conversion complete callback. 
  * @param  hdma pointer to DMA handle.
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
  * @param  hdma pointer to DMA handle.
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
  * @param  hdma pointer to DMA handle.
  * @retval None
  */
static void DAC_DMAErrorCh1(DMA_HandleTypeDef *hdma)   
{
  DAC_HandleTypeDef* hdac = ( DAC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
    
  /* Set DAC error code to DMA error */
  hdac->ErrorCode |= HAL_DAC_ERROR_DMA;
    
#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)
  hdac->ErrorCallbackCh1(hdac);
#else  
  HAL_DAC_ErrorCallbackCh1(hdac); 
#endif
    
  hdac->State= HAL_DAC_STATE_READY;
}

#if  defined(STM32F303xE) || defined(STM32F398xx)                         || \
     defined(STM32F303xC) || defined(STM32F358xx)                         || \
     defined(STM32F303x8) || defined(STM32F334x8) || defined(STM32F328xx) || \
     defined(STM32F373xC) || defined(STM32F378xx)
/* DAC channel 2 is available on top of DAC channel 1U */
/**
  * @brief  DMA conversion complete callback. 
  * @param  hdma pointer to DMA handle.
  * @retval None
  */
static void DAC_DMAConvCpltCh2(DMA_HandleTypeDef *hdma)   
{
  DAC_HandleTypeDef* hdac = ( DAC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
  
#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)  
  hdac->ConvCpltCallbackCh2(hdac);
#else
  HAL_DACEx_ConvCpltCallbackCh2(hdac); 
#endif
  
  hdac->State= HAL_DAC_STATE_READY;
}

/**
  * @brief  DMA half transfer complete callback. 
  * @param  hdma pointer to DMA handle.
  * @retval None
  */
static void DAC_DMAHalfConvCpltCh2(DMA_HandleTypeDef *hdma)   
{
    DAC_HandleTypeDef* hdac = ( DAC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;

    /* Conversion complete callback */
#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)  
  hdac->ConvHalfCpltCallbackCh2(hdac);
#else
    HAL_DACEx_ConvHalfCpltCallbackCh2(hdac); 
#endif
}

/**
  * @brief  DMA error callback 
  * @param  hdma pointer to DMA handle.
  * @retval None
  */
static void DAC_DMAErrorCh2(DMA_HandleTypeDef *hdma)   
{
  DAC_HandleTypeDef* hdac = ( DAC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
    
  /* Set DAC error code to DMA error */
  hdac->ErrorCode |= HAL_DAC_ERROR_DMA;
    
#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)
  hdac->ErrorCallbackCh2(hdac);
#else 
  HAL_DACEx_ErrorCallbackCh2(hdac); 
#endif
    
  hdac->State= HAL_DAC_STATE_READY;
}
#endif /* STM32F303xE || STM32F398xx                || */
       /* STM32F303xC || STM32F358xx                || */
       /* STM32F303x8 || STM32F334x8 || STM32F328xx || */
       /* STM32F373xC || STM32F378xx                   */

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_DAC_MODULE_ENABLED */
/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
