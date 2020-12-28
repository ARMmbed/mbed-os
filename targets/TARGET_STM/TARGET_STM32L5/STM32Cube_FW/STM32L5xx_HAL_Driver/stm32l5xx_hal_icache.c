/**
  ******************************************************************************
  * @file    stm32l5xx_hal_icache.c
  * @author  MCD Application Team
  * @brief   ICACHE HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Instruction Cache (ICACHE).
  *           + Initialization and Configuration
  *           + Invalidate functions
  *           + Monitoring management
  *           + Memory address remap management
  @verbatim
  ==============================================================================
                        ##### ICACHE main features #####
  ==============================================================================
  [..]
    The Instruction Cache (ICACHE) is introduced on C-AHB code bus of
    Cortex-M33 processor to improve performance when fetching instruction
    and data from both internal and external memories. It allows close to
    zero wait states performance.

    (+) The ICACHE provides two performance counters (Hit and Miss),
        cache invalidate maintenance operation, error management and TrustZone
        security support.

    (+) The ICACHE provides additionnaly the possibility to remap input address
        falling into up to four memory regions (used to remap aliased code in
        external memories to the internal Code region, for execution)

  ===============================================================================
                        ##### How to use this driver #####
  ===============================================================================
  [..]
     The ICACHE HAL driver can be used as follows:

    (#) Enable and disable the Instruction Cache with respectively
        @ref HAL_ICACHE_Enable() and @ref HAL_ICACHE_Disable()

    (#) Configure the Instruction Cache mode with @ref HAL_ICACHE_ConfigAssociativityMode()

    (#) Initiate the cache maintenance invalidation procedure with either
        @ref HAL_ICACHE_Invalidate() (blocking mode) or @ref HAL_ICACHE_Invalidate_IT()
        (interrupt mode). When interrupt mode is used, the callback function
        @ref HAL_ICACHE_InvalidateCompleteCallback() is called when the invalidate
        procedure is complete. The function @ref HAL_ICACHE_WaitForInvalidateComplete()
        may be called to wait for the end of the invalidate procedure automatically
        initiated when disabling the Instruction Cache with @ref HAL_ICACHE_Disable()

    (#) Use the performance monitoring counters for Hit and Miss with the following
        functions: @ref HAL_ICACHE_Monitor_Start(), @ref HAL_ICACHE_Monitor_Stop(),
        @ref HAL_ICACHE_Monitor_Reset(), @ref HAL_ICACHE_Monitor_GetHitValue() and
        @ref HAL_ICACHE_Monitor_GetMissValue()

    (#) Enable and disable up to four regions to remap input address from external
        memories to the internal Code region for execution with
        @ref HAL_ICACHE_EnableRemapRegion() and @ref HAL_ICACHE_DisableRemapRegion()

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "stm32l5xx_hal.h"

/** @addtogroup STM32L5xx_HAL_Driver
  * @{
  */

/** @defgroup ICACHE ICACHE
  * @brief HAL ICACHE module driver
  * @{
  */
#ifdef HAL_ICACHE_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @addtogroup ICACHE_Private_Constants ICACHE Private Constants
  * @{
  */
#define ICACHE_INVALIDATE_TIMEOUT_VALUE        1U   /* 1ms */
#define ICACHE_DISABLE_TIMEOUT_VALUE           1U   /* 1ms */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/** @defgroup ICACHE_Exported_Functions ICACHE Exported Functions
  * @{
  */

/** @defgroup ICACHE_Exported_Functions_Group1 Initialization and control functions
  * @brief    Initialization and control functions
  *
  @verbatim
  ==============================================================================
            ##### Initialization and control functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to initialize and control the
    Instruction Cache (mode, invalidate procedure, performance counters).
  @endverbatim
  * @{
  */

/**
  * @brief  Configure the Instruction Cache cache associativity mode selection.
  * @param  AssociativityMode  Associativity mode selection
  *         This parameter can be one of the following values:
  *            @arg ICACHE_1WAY   1-way cache (direct mapped cache)
  *            @arg ICACHE_2WAYS  2-ways set associative cache (default)
  * @retval HAL status (HAL_OK/HAL_ERROR)
  */
HAL_StatusTypeDef HAL_ICACHE_ConfigAssociativityMode(uint32_t AssociativityMode)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Check the parameters */
  assert_param(IS_ICACHE_ASSOCIATIVITY_MODE(AssociativityMode));

  /* Check cache is not enabled */
  if (READ_BIT(ICACHE->CR, ICACHE_CR_EN) != 0U)
  {
    status = HAL_ERROR;
  }
  else
  {
    MODIFY_REG(ICACHE->CR, ICACHE_CR_WAYSEL, AssociativityMode);
  }

  return status;
}

/**
  * @brief  DeInitialize the Instruction Cache.
  * @retval HAL status (HAL_OK/HAL_TIMEOUT)
  */
HAL_StatusTypeDef HAL_ICACHE_DeInit(void)
{
  HAL_StatusTypeDef status;

  /* Disable cache with reset value for 2-ways set associative mode */
  WRITE_REG(ICACHE->CR, ICACHE_CR_WAYSEL);

  /* Stop monitor and reset monitor values */
  (void)HAL_ICACHE_Monitor_Stop(ICACHE_MONITOR_HIT_MISS);
  (void)HAL_ICACHE_Monitor_Reset(ICACHE_MONITOR_HIT_MISS);

  /* No remapped regions */
  (void)HAL_ICACHE_DisableRemapRegion(ICACHE_REGION_0);
  (void)HAL_ICACHE_DisableRemapRegion(ICACHE_REGION_1);
  (void)HAL_ICACHE_DisableRemapRegion(ICACHE_REGION_2);
  (void)HAL_ICACHE_DisableRemapRegion(ICACHE_REGION_3);

  /* Wait for end of invalidate cache procedure */
  status = HAL_ICACHE_WaitForInvalidateComplete();

  /* Clear any pending flags */
  WRITE_REG(ICACHE->FCR, ICACHE_FCR_CBSYENDF | ICACHE_FCR_CERRF);

  return status;
}

/**
  * @brief  Enable the Instruction Cache.
  * @note   This function always returns HAL_OK even if there is any ongoing
  *         cache operation. The Instruction Cache is bypassed until the
  *         cache operation completes.
  * @retval HAL status (HAL_OK)
  */
HAL_StatusTypeDef HAL_ICACHE_Enable(void)
{
  SET_BIT(ICACHE->CR, ICACHE_CR_EN);

  return HAL_OK;
}

/**
  * @brief  Disable the Instruction Cache.
  * @note   This function waits for the cache being disabled but
  *         not for the end of the automatic cache invalidation procedure.
  * @retval HAL status (HAL_OK/HAL_TIMEOUT)
  */
HAL_StatusTypeDef HAL_ICACHE_Disable(void)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t tickstart;

  /* Reset BSYENDF before to disable the instruction cache */
  /* that starts a cache invalidation procedure */
  CLEAR_BIT(ICACHE->FCR, ICACHE_FCR_CBSYENDF);

  CLEAR_BIT(ICACHE->CR, ICACHE_CR_EN);

  /* Get tick */
  tickstart = HAL_GetTick();

  /* Wait for instruction cache being disabled */
  while (READ_BIT(ICACHE->CR, ICACHE_CR_EN) != 0U)
  {
    if ((HAL_GetTick() - tickstart) > ICACHE_DISABLE_TIMEOUT_VALUE)
    {
      status = HAL_TIMEOUT;
      break;
    }
  }

  return status;
}

/**
  * @brief  Invalidate the Instruction Cache.
  * @note   This function waits for the end of cache invalidation procedure
  *         and clears the associated BSYENDF flag.
  * @retval HAL status (HAL_OK/HAL_ERROR/HAL_TIMEOUT)
  */
HAL_StatusTypeDef HAL_ICACHE_Invalidate(void)
{
  HAL_StatusTypeDef status;

  /* Check no ongoing operation */
  if (READ_BIT(ICACHE->SR, ICACHE_SR_BUSYF) != 0U)
  {
    status = HAL_ERROR;
  }
  else
  {
    /* Make sure BSYENDF is reset before to start cache invalidation */
    CLEAR_BIT(ICACHE->FCR, ICACHE_FCR_CBSYENDF);

    /* Launch cache invalidation */
    SET_BIT(ICACHE->CR, ICACHE_CR_CACHEINV);

    status = HAL_ICACHE_WaitForInvalidateComplete();
  }

  return status;
}

/**
  * @brief  Invalidate the Instruction Cache with interrupt.
  * @note   This function launches cache invalidation and returns.
  *         User application shall resort to interrupt generation to check
  *         the end of the cache invalidation with the BSYENDF flag and the
  *         HAL_ICACHE_InvalidateCompleteCallback() callback.
  * @retval HAL status (HAL_OK/HAL_ERROR)
  */
HAL_StatusTypeDef HAL_ICACHE_Invalidate_IT(void)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Check no ongoing operation */
  if (READ_BIT(ICACHE->SR, ICACHE_SR_BUSYF) != 0U)
  {
    status = HAL_ERROR;
  }
  else
  {
    /* Make sure BSYENDF is reset */
    WRITE_REG(ICACHE->FCR, ICACHE_FCR_CBSYENDF);

    /* Enable end of cache invalidation interrupt */
    SET_BIT(ICACHE->IER, ICACHE_IER_BSYENDIE);

    /* Launch cache invalidation */
    SET_BIT(ICACHE->CR, ICACHE_CR_CACHEINV);
  }

  return status;
}

/**
  * @brief Wait for the end of the Instruction Cache invalidate procedure.
  * @note This function checks and clears the BSYENDF flag when set.
  * @retval HAL status (HAL_OK/HAL_TIMEOUT)
  */
HAL_StatusTypeDef HAL_ICACHE_WaitForInvalidateComplete(void)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t tickstart;

  /* Check if ongoing invalidation operation */
  if (READ_BIT(ICACHE->SR, ICACHE_SR_BUSYF) != 0U)
  {
    /* Get tick */
    tickstart = HAL_GetTick();

    /* Wait for end of cache invalidation */
    while (READ_BIT(ICACHE->SR, ICACHE_SR_BSYENDF) == 0U)
    {
      if ((HAL_GetTick() - tickstart) > ICACHE_INVALIDATE_TIMEOUT_VALUE)
      {
        status = HAL_TIMEOUT;
        break;
      }
    }
  }

  /* Clear BSYENDF */
  WRITE_REG(ICACHE->FCR, ICACHE_FCR_CBSYENDF);

  return status;
}


/**
  * @brief  Start the Instruction Cache performance monitoring.
  * @param  MonitorType  Monitoring type
  *         This parameter can be one of the following values:
  *            @arg ICACHE_MONITOR_HIT_MISS   Hit & Miss monitoring
  *            @arg ICACHE_MONITOR_HIT        Hit monitoring
  *            @arg ICACHE_MONITOR_MISS       Miss monitoring
  * @retval HAL status (HAL_OK)
  */
HAL_StatusTypeDef HAL_ICACHE_Monitor_Start(uint32_t MonitorType)
{
  /* Check the parameters */
  assert_param(IS_ICACHE_MONITOR_TYPE(MonitorType));

  SET_BIT(ICACHE->CR, MonitorType);

  return HAL_OK;
}

/**
  * @brief  Stop the Instruction Cache performance monitoring.
  * @note   Stopping the monitoring does not reset the values.
  * @param  MonitorType  Monitoring type
  *         This parameter can be one of the following values:
  *            @arg ICACHE_MONITOR_HIT_MISS   Hit & Miss monitoring
  *            @arg ICACHE_MONITOR_HIT        Hit monitoring
  *            @arg ICACHE_MONITOR_MISS       Miss monitoring
  * @retval HAL status (HAL_OK)
  */
HAL_StatusTypeDef HAL_ICACHE_Monitor_Stop(uint32_t MonitorType)
{
  /* Check the parameters */
  assert_param(IS_ICACHE_MONITOR_TYPE(MonitorType));

  CLEAR_BIT(ICACHE->CR, MonitorType);

  return HAL_OK;
}

/**
  * @brief  Reset the Instruction Cache performance monitoring values.
  * @param  MonitorType  Monitoring type
  *         This parameter can be one of the following values:
  *            @arg ICACHE_MONITOR_HIT_MISS   Hit & Miss monitoring
  *            @arg ICACHE_MONITOR_HIT        Hit monitoring
  *            @arg ICACHE_MONITOR_MISS       Miss monitoring
  * @retval HAL status (HAL_OK)
  */
HAL_StatusTypeDef HAL_ICACHE_Monitor_Reset(uint32_t MonitorType)
{
  /* Check the parameters */
  assert_param(IS_ICACHE_MONITOR_TYPE(MonitorType));

  /* Force/Release reset */
  SET_BIT(ICACHE->CR, (MonitorType << 2U));
  CLEAR_BIT(ICACHE->CR, (MonitorType << 2U));

  return HAL_OK;
}

/**
  * @brief  Get the Instruction Cache performance Hit monitoring value.
  * @note   Upon reaching the 32-bit maximum value, monitor does not wrap.
  * @retval Hit monitoring value
  */
uint32_t HAL_ICACHE_Monitor_GetHitValue(void)
{
  return (ICACHE->HMONR);
}

/**
  * @brief  Get the Instruction Cache performance Miss monitoring value.
  * @note   Upon reaching the 32-bit maximum value, monitor does not wrap.
  * @retval Miss monitoring value
  */
uint32_t HAL_ICACHE_Monitor_GetMissValue(void)
{
  return (ICACHE->MMONR);
}

/**
  * @}
  */

/** @defgroup ICACHE_Exported_Functions_Group2 IRQ and callback functions
  * @brief    IRQ and callback functions
  *
  @verbatim
  ==============================================================================
            ##### IRQ and callback functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to handle ICACHE global interrupt
    and the associated callback functions.
  @endverbatim
  * @{
  */

/**
  * @brief Handle the Instruction Cache interrupt request.
  * @note This function should be called under the ICACHE_IRQHandler().
  * @note This function respectively disables the interrupt and clears the
  *       flag of any pending flag before calling the associated user callback.
  * @retval None
  */
void HAL_ICACHE_IRQHandler(void)
{
  /* Get current interrupt flags and interrupt sources value */
  uint32_t itflags   = READ_REG(ICACHE->SR);
  uint32_t itsources = READ_REG(ICACHE->IER);

  /* Check Instruction cache Error interrupt flag  */
  if (((itflags & itsources) & ICACHE_FLAG_ERROR) != 0U)
  {
    /* Disable error interrupt */
    CLEAR_BIT(ICACHE->IER, ICACHE_IER_ERRIE);

    /* Clear ICACHE error pending flag */
    WRITE_REG(ICACHE->FCR, ICACHE_FCR_CERRF);

    /* Instruction cache error interrupt user callback */
    HAL_ICACHE_ErrorCallback();
  }

  /* Check Instruction cache BusyEnd interrupt flag  */
  if (((itflags & itsources) & ICACHE_FLAG_BUSYEND) != 0U)
  {
    /* Disable end of cache invalidation interrupt */
    CLEAR_BIT(ICACHE->IER, ICACHE_IER_BSYENDIE);

    /* Clear ICACHE busyend pending flag */
    WRITE_REG(ICACHE->FCR, ICACHE_FCR_CBSYENDF);

    /* Instruction cache busyend interrupt user callback */
    HAL_ICACHE_InvalidateCompleteCallback();
  }
}

/**
  * @brief  Cache invalidation complete callback.
  */
__weak void HAL_ICACHE_InvalidateCompleteCallback(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_ICACHE_InvalidateCompleteCallback() should be implemented in the user file
   */
}

/**
  * @brief  Error callback.
  */
__weak void HAL_ICACHE_ErrorCallback(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_ICACHE_ErrorCallback() should be implemented in the user file
   */
}

/**
  * @}
  */

/** @defgroup ICACHE_Exported_Functions_Group3 Memory remapped regions functions
  * @brief    Memory remapped regions functions
  *
  @verbatim
  ==============================================================================
            ##### Memory remapped regions functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to manage the remapping of
    external memories to internal Code for execution.
  @endverbatim
  * @{
  */

/**
  * @brief  Configure and enable a region for memory remapping.
  * @note   The Instruction Cache and the region must be disabled.
  * @param  Region   Region number
                     This parameter can be a value of @arg @ref ICACHE_Region
  * @param  sRegionConfig  Structure of ICACHE region configuration parameters
  * @retval HAL status (HAL_OK/HAL_ERROR)
  */
HAL_StatusTypeDef  HAL_ICACHE_EnableRemapRegion(uint32_t Region, ICACHE_RegionConfigTypeDef *sRegionConfig)
{
  const uint16_t ICacheRemapSizeAddressTab[] =
  {
    0x00FFU, 0x07FFU, /* Region 2MB:   BaseAddress size 8 bits, RemapAddress size 11 bits */
    0x00FEU, 0x07FEU, /* Region 4MB:   BaseAddress size 7 bits, RemapAddress size 10 bits */
    0x00FCU, 0x07FCU, /* Region 8MB:   BaseAddress size 6 bits, RemapAddress size 9 bits  */
    0x00F8U, 0x07F8U, /* Region 16MB:  BaseAddress size 5 bits, RemapAddress size 8 bits  */
    0x00F0U, 0x07F0U, /* Region 32MB:  BaseAddress size 4 bits, RemapAddress size 7 bits  */
    0x00E0U, 0x07E0U, /* Region 64MB:  BaseAddress size 3 bits, RemapAddress size 6 bits  */
    0x00C0U, 0x07C0U  /* Region 128MB: BaseAddress size 2 bits, RemapAddress size 5 bits  */
  };

  HAL_StatusTypeDef status = HAL_OK;
  __IO uint32_t *reg;
  uint32_t value;

  /* Check the parameters */
  assert_param(IS_ICACHE_REGION_NUMBER(Region));
  assert_param(IS_ICACHE_REGION_SIZE(sRegionConfig->Size));
  assert_param(IS_ICACHE_REGION_TRAFFIC_ROUTE(sRegionConfig->TrafficRoute));
  assert_param(IS_ICACHE_REGION_OUTPUT_BURST_TYPE(sRegionConfig->OutputBurstType));

  /* Check cache is not enabled */
  if (READ_BIT(ICACHE->CR, ICACHE_CR_EN) != 0U)
  {
    status = HAL_ERROR;
  }
  else
  {
    /* Get region control register address */
    reg = &(ICACHE->CRR0) + (1U * Region);

    /* Check region is not already enabled */
    if ((*reg & ICACHE_CRRx_REN) != 0U)
    {
      status = HAL_ERROR;
    }
    else
    {
      value  = ((sRegionConfig->BaseAddress & 0x1FFFFFFFU) >> 21U) & ICacheRemapSizeAddressTab[(sRegionConfig->Size - 1U) * 2U];
      value |= ((sRegionConfig->RemapAddress >> 5U) & ((uint32_t)(ICacheRemapSizeAddressTab[((sRegionConfig->Size - 1U) * 2U) + 1U]) << ICACHE_CRRx_REMAPADDR_Pos));
      value |= (sRegionConfig->Size << ICACHE_CRRx_RSIZE_Pos) | sRegionConfig->TrafficRoute | sRegionConfig->OutputBurstType;

      *reg = (value | ICACHE_CRRx_REN);
    }
  }

  return status;
}

/**
  * @brief  Disable the memory remapping for a predefined region.
  * @param  Region   Region number
                     This parameter can be a value of @arg @ref ICACHE_Region
  * @retval HAL status (HAL_OK/HAL_ERROR)
  */
HAL_StatusTypeDef  HAL_ICACHE_DisableRemapRegion(uint32_t Region)
{
  HAL_StatusTypeDef status = HAL_OK;
  __IO uint32_t *reg;

  /* Check the parameters */
  assert_param(IS_ICACHE_REGION_NUMBER(Region));

  /* Check cache is not enabled */
  if (READ_BIT(ICACHE->CR, ICACHE_CR_EN) != 0U)
  {
    status = HAL_ERROR;
  }
  else
  {
    /* Get region control register address */
    reg = &(ICACHE->CRR0) + (1U * Region);

    *reg &= ~ICACHE_CRRx_REN;
  }

  return status;
}


/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_ICACHE_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
