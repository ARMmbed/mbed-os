/**
  ******************************************************************************
  * @file    stm32g0xx_hal_dma_ex.c
  * @author  MCD Application Team
  * @brief   DMA Extension HAL module driver
  *         This file provides firmware functions to manage the following
  *         functionalities of the DMA Extension peripheral:
  *           + Extended features functions
  *
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
  [..]
  The DMA Extension HAL driver can be used as follows:
   (+) Configure the DMAMUX Synchronization Block using HAL_DMAEx_ConfigMuxSync function.
   (+) Configure the DMAMUX Request Generator Block using HAL_DMAEx_ConfigMuxRequestGenerator function.
       Functions HAL_DMAEx_EnableMuxRequestGenerator and HAL_DMAEx_DisableMuxRequestGenerator can then be used
       to respectively enable/disable the request generator.

   (+) To handle the DMAMUX Interrupts, the function  HAL_DMAEx_MUX_IRQHandler should be called from
       the DMAMUX IRQ handler i.e DMAMUX1_OVR_IRQHandler.
       As only one interrupt line is available for all DMAMUX channels and request generators , HAL_DMAEx_MUX_IRQHandler should be
       called with, as parameter, the appropriate DMA handle as many as used DMAs in the user project
      (exception done if a given DMA is not using the DMAMUX SYNC block neither a request generator)

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

/** @addtogroup STM32G0xx_HAL_Driver
  * @{
  */

/** @defgroup DMAEx DMAEx
  * @brief DMA Extended HAL module driver
  * @{
  */

#ifdef HAL_DMA_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/


/** @defgroup DMAEx_Exported_Functions DMAEx Exported Functions
  * @{
  */

/** @defgroup DMAEx_Exported_Functions_Group1 DMAEx Extended features functions
 *  @brief   Extended features functions
 *
@verbatim
 ===============================================================================
                #####  Extended features functions  #####
 ===============================================================================
    [..]  This section provides functions allowing to:

    (+) Configure the DMAMUX Synchronization Block using HAL_DMAEx_ConfigMuxSync function.
    (+) Configure the DMAMUX Request Generator Block using HAL_DMAEx_ConfigMuxRequestGenerator function.
       Functions HAL_DMAEx_EnableMuxRequestGenerator and HAL_DMAEx_DisableMuxRequestGenerator can then be used
       to respectively enable/disable the request generator.
    (+) Handle DMAMUX interrupts using HAL_DMAEx_MUX_IRQHandler : should be called from
        the DMAMUX IRQ handler

@endverbatim
  * @{
  */

/**
  * @brief Configure the DMAMUX synchronization parameters for a given DMA channel (instance).
  * @param hdma Pointer to a DMA_HandleTypeDef structure that contains
  *             the configuration information for the specified DMA channel.
  * @param pSyncConfig Pointer to HAL_DMA_MuxSyncConfigTypeDef contains the DMAMUX synchronization parameters
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMAEx_ConfigMuxSync(DMA_HandleTypeDef *hdma, HAL_DMA_MuxSyncConfigTypeDef *pSyncConfig)
{
  /* Check the parameters */
  assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));

  assert_param(IS_DMAMUX_SYNC_SIGNAL_ID(pSyncConfig->SyncSignalID));

  assert_param(IS_DMAMUX_SYNC_POLARITY(pSyncConfig-> SyncPolarity));
  assert_param(IS_DMAMUX_SYNC_STATE(pSyncConfig->SyncEnable));
  assert_param(IS_DMAMUX_SYNC_EVENT(pSyncConfig->EventEnable));
  assert_param(IS_DMAMUX_SYNC_REQUEST_NUMBER(pSyncConfig->RequestNumber));

  /*Check if the DMA state is ready */
  if (hdma->State == HAL_DMA_STATE_READY)
  {
    /* Process Locked */
    __HAL_LOCK(hdma);

    /* Set the new synchronization parameters (and keep the request ID filled during the Init)*/
    MODIFY_REG(hdma->DMAmuxChannel->CCR, \
               (~DMAMUX_CxCR_DMAREQ_ID), \
               (pSyncConfig->SyncSignalID | ((pSyncConfig->RequestNumber - 1U) << DMAMUX_CxCR_NBREQ_Pos) | \
                pSyncConfig->SyncPolarity | ((uint32_t)pSyncConfig->SyncEnable << DMAMUX_CxCR_SE_Pos) | \
                ((uint32_t)pSyncConfig->EventEnable << DMAMUX_CxCR_EGE_Pos)));

    /* Process UnLocked */
    __HAL_UNLOCK(hdma);

    return HAL_OK;
  }
  else
  {
    /* Set the error code to busy */
    hdma->ErrorCode = HAL_DMA_ERROR_BUSY;

    /* Return error status */
    return HAL_ERROR;
  }
}

/**
  * @brief Configure the DMAMUX request generator block used by the given DMA channel (instance).
  * @param hdma Pointer to a DMA_HandleTypeDef structure that contains
  *             the configuration information for the specified DMA channel.
  * @param pRequestGeneratorConfig Pointer to HAL_DMA_MuxRequestGeneratorConfigTypeDef
  *                                contains the request generator parameters.
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMAEx_ConfigMuxRequestGenerator(DMA_HandleTypeDef *hdma, HAL_DMA_MuxRequestGeneratorConfigTypeDef *pRequestGeneratorConfig)
{
  HAL_StatusTypeDef status;
  HAL_DMA_StateTypeDef temp_state = hdma->State;

  /* Check the parameters */
  assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));

  assert_param(IS_DMAMUX_REQUEST_GEN_SIGNAL_ID(pRequestGeneratorConfig->SignalID));

  assert_param(IS_DMAMUX_REQUEST_GEN_POLARITY(pRequestGeneratorConfig->Polarity));
  assert_param(IS_DMAMUX_REQUEST_GEN_REQUEST_NUMBER(pRequestGeneratorConfig->RequestNumber));

  /* check if the DMA state is ready
     and DMA is using a DMAMUX request generator block
  */
  if (hdma->DMAmuxRequestGen == 0U)
  {
    /* Set the error code to busy */
    hdma->ErrorCode = HAL_DMA_ERROR_PARAM;

    /* error status */
    status = HAL_ERROR;
  }
  else if (((hdma->DMAmuxRequestGen->RGCR & DMAMUX_RGxCR_GE) == 0U) && (temp_state == HAL_DMA_STATE_READY))
  {
    /* RequestGenerator must be disable prior to the configuration i.e GE bit is 0 */

    /* Process Locked */
    __HAL_LOCK(hdma);

    /* Set the request generator new parameters*/
    hdma->DMAmuxRequestGen->RGCR = pRequestGeneratorConfig->SignalID | \
                                   ((pRequestGeneratorConfig->RequestNumber - 1U) << DMAMUX_RGxCR_GNBREQ_Pos) | \
                                   pRequestGeneratorConfig->Polarity;
    /* Process UnLocked */
    __HAL_UNLOCK(hdma);

    return HAL_OK;
  }
  else
  {
    /* Set the error code to busy */
    hdma->ErrorCode = HAL_DMA_ERROR_BUSY;

    /* error status */
    status = HAL_ERROR;
  }

  return status;
}

/**
  * @brief Enable the DMAMUX request generator block used by the given DMA channel (instance).
  * @param hdma Pointer to a DMA_HandleTypeDef structure that contains
  *             the configuration information for the specified DMA channel.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMAEx_EnableMuxRequestGenerator(DMA_HandleTypeDef *hdma)
{
  /* Check the parameters */
  assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));

  /* check if the DMA state is ready
     and DMA is using a DMAMUX request generator block
  */
  if ((hdma->State != HAL_DMA_STATE_RESET) && (hdma->DMAmuxRequestGen != 0))
  {

    /* Enable the request generator*/
    hdma->DMAmuxRequestGen->RGCR |= DMAMUX_RGxCR_GE;

    return HAL_OK;
  }
  else
  {
    return HAL_ERROR;
  }
}

/**
  * @brief Disable the DMAMUX request generator block used by the given DMA channel (instance).
  * @param hdma Pointer to a DMA_HandleTypeDef structure that contains
  *             the configuration information for the specified DMA channel.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMAEx_DisableMuxRequestGenerator(DMA_HandleTypeDef *hdma)
{
  /* Check the parameters */
  assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));

  /* check if the DMA state is ready
     and DMA is using a DMAMUX request generator block
  */
  if ((hdma->State != HAL_DMA_STATE_RESET) && (hdma->DMAmuxRequestGen != 0))
  {

    /* Disable the request generator*/
    hdma->DMAmuxRequestGen->RGCR &= ~DMAMUX_RGxCR_GE;

    return HAL_OK;
  }
  else
  {
    return HAL_ERROR;
  }
}

/**
  * @brief Handles DMAMUX interrupt request.
  * @param hdma Pointer to a DMA_HandleTypeDef structure that contains
  *             the configuration information for the specified DMA channel.
  * @retval None
  */
void HAL_DMAEx_MUX_IRQHandler(DMA_HandleTypeDef *hdma)
{
  /* Check for DMAMUX Synchronization overrun */
  if ((hdma->DMAmuxChannelStatus->CSR & hdma->DMAmuxChannelStatusMask) != 0U)
  {
    /* Disable the synchro overrun interrupt */
    hdma->DMAmuxChannel->CCR &= ~DMAMUX_CxCR_SOIE;

    /* Clear the DMAMUX synchro overrun flag */
    hdma->DMAmuxChannelStatus->CFR = hdma->DMAmuxChannelStatusMask;

    /* Update error code */
    hdma->ErrorCode |= HAL_DMA_ERROR_SYNC;

    if (hdma->XferErrorCallback != NULL)
    {
      /* Transfer error callback */
      hdma->XferErrorCallback(hdma);
    }
  }

  if (hdma->DMAmuxRequestGen != 0)
  {
    /* if using a DMAMUX request generator block Check for DMAMUX request generator overrun */
    if ((hdma->DMAmuxRequestGenStatus->RGSR & hdma->DMAmuxRequestGenStatusMask) != 0U)
    {
      /* Disable the request gen overrun interrupt */
      hdma->DMAmuxRequestGen->RGCR &= ~DMAMUX_RGxCR_OIE;

      /* Clear the DMAMUX request generator overrun flag */
      hdma->DMAmuxRequestGenStatus->RGCFR = hdma->DMAmuxRequestGenStatusMask;

      /* Update error code */
      hdma->ErrorCode |= HAL_DMA_ERROR_REQGEN;

      if (hdma->XferErrorCallback != NULL)
      {
        /* Transfer error callback */
        hdma->XferErrorCallback(hdma);
      }
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_DMA_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
