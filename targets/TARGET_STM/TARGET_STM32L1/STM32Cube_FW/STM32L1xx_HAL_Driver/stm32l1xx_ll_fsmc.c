/**
  ******************************************************************************
  * @file    stm32l1xx_ll_fsmc.c
  * @author  MCD Application Team
  * @brief   FSMC Low Layer HAL module driver.
  *
  *          This file provides firmware functions to manage the following
  *          functionalities of the Flexible Static Memory Controller (FSMC) peripheral memories:
  *           + Initialization/de-initialization functions
  *           + Peripheral Control functions
  *           + Peripheral State functions
  *
  @verbatim
  =============================================================================
                        ##### FSMC peripheral features #####
  =============================================================================
    [..] The Flexible static memory controller (FSMC) includes following memory controllers:
         (+) The NOR/PSRAM memory controller

    [..] The FSMC functional block makes the interface with synchronous and asynchronous static
         memories. Its main purposes are:
         (+) to translate AHB transactions into the appropriate external device protocol.
         (+) to meet the access time requirements of the external memory devices.

    [..] All external memories share the addresses, data and control signals with the controller.
         Each external device is accessed by means of a unique Chip Select. The FSMC performs
         only one access at a time to an external device.
         The main features of the FSMC controller are the following:
          (+) Interface with static-memory mapped devices including:
             (++) Static random access memory (SRAM).
             (++) NOR Flash memory.
             (++) PSRAM (4 memory banks).
          (+) Independent Chip Select control for each memory bank
          (+) Independent configuration for each memory bank

  @endverbatim
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
#include "stm32l1xx_hal.h"

/** @addtogroup STM32L1xx_HAL_Driver
  * @{
  */

#if defined(FSMC_BANK1)

#if defined(HAL_SRAM_MODULE_ENABLED) || defined(HAL_NOR_MODULE_ENABLED)

/** @defgroup FSMC_LL FSMC Low Layer
  * @brief FSMC driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup FSMC_LL_Private_Constants FSMC Low Layer Private Constants
  * @{
  */

/* ----------------------- FSMC registers bit mask --------------------------- */
/* --- BCR Register ---*/
/* BCR register clear mask */
#define BCR_CLEAR_MASK                 ((uint32_t)(FSMC_BCRx_FACCEN  | FSMC_BCRx_MUXEN     | \
                                                   FSMC_BCRx_MTYP    | FSMC_BCRx_MWID      | \
                                                   FSMC_BCRx_BURSTEN | FSMC_BCRx_WAITPOL   | \
                                                   FSMC_BCRx_WRAPMOD | FSMC_BCRx_WAITCFG   | \
                                                   FSMC_BCRx_WREN    | FSMC_BCRx_WAITEN    | \
                                                   FSMC_BCRx_EXTMOD  | FSMC_BCRx_ASYNCWAIT | \
                                                   FSMC_BCRx_CBURSTRW))
/* --- BTR Register ---*/
/* BTR register clear mask */
#define BTR_CLEAR_MASK                 ((uint32_t)(FSMC_BTRx_ADDSET | FSMC_BTRx_ADDHLD  |\
                                                   FSMC_BTRx_DATAST | FSMC_BTRx_BUSTURN |\
                                                   FSMC_BTRx_CLKDIV | FSMC_BTRx_DATLAT  |\
                                                   FSMC_BTRx_ACCMOD))

/* --- BWTR Register ---*/
/* BWTR register clear mask */
#define BWTR_CLEAR_MASK                ((uint32_t)(FSMC_BWTRx_ADDSET | FSMC_BWTRx_ADDHLD | \
                                                   FSMC_BWTRx_DATAST | FSMC_BWTRx_ACCMOD | \
                                                   FSMC_BWTRx_BUSTURN))

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/** @defgroup FSMC_LL_Private_Macros FSMC Low Layer Private Macros
  * @{
  */

/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup FSMC_LL_Exported_Functions FSMC Low Layer Exported Functions
  * @{
  */

/** @defgroup FSMC_NORSRAM FSMC NORSRAM Controller functions
  * @brief    NORSRAM Controller functions
  *
  @verbatim
  ==============================================================================
                   ##### How to use NORSRAM device driver #####
  ==============================================================================

  [..]
    This driver contains a set of APIs to interface with the FSMC NORSRAM banks in order
    to run the NORSRAM external devices.

    (+) FSMC NORSRAM bank reset using the function FSMC_NORSRAM_DeInit()
    (+) FSMC NORSRAM bank control configuration using the function FSMC_NORSRAM_Init()
    (+) FSMC NORSRAM bank timing configuration using the function FSMC_NORSRAM_Timing_Init()
    (+) FSMC NORSRAM bank extended timing configuration using the function
        FSMC_NORSRAM_Extended_Timing_Init()
    (+) FSMC NORSRAM bank enable/disable write operation using the functions
        FSMC_NORSRAM_WriteOperation_Enable()/FSMC_NORSRAM_WriteOperation_Disable()


@endverbatim
  * @{
  */

/** @defgroup FSMC_NORSRAM_Group1 Initialization/de-initialization functions
  * @brief    Initialization and Configuration functions
  *
  @verbatim
  ==============================================================================
              ##### Initialization and de_initialization functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to:
    (+) Initialize and configure the FSMC NORSRAM interface
    (+) De-initialize the FSMC NORSRAM interface
    (+) Configure the FSMC clock and associated GPIOs

@endverbatim
  * @{
  */

/**
  * @brief  Initialize the FSMC_NORSRAM device according to the specified
  *         control parameters in the FSMC_NORSRAM_InitTypeDef
  * @param  Device Pointer to NORSRAM device instance
  * @param  Init Pointer to NORSRAM Initialization structure
  * @retval HAL status
  */
HAL_StatusTypeDef  FSMC_NORSRAM_Init(FSMC_NORSRAM_TypeDef *Device, FSMC_NORSRAM_InitTypeDef *Init)
{
  /* Check the parameters */
  assert_param(IS_FSMC_NORSRAM_DEVICE(Device));
  assert_param(IS_FSMC_NORSRAM_BANK(Init->NSBank));
  assert_param(IS_FSMC_MUX(Init->DataAddressMux));
  assert_param(IS_FSMC_MEMORY(Init->MemoryType));
  assert_param(IS_FSMC_NORSRAM_MEMORY_WIDTH(Init->MemoryDataWidth));
  assert_param(IS_FSMC_BURSTMODE(Init->BurstAccessMode));
  assert_param(IS_FSMC_WAIT_POLARITY(Init->WaitSignalPolarity));
  assert_param(IS_FSMC_WRAP_MODE(Init->WrapMode));
  assert_param(IS_FSMC_WAIT_SIGNAL_ACTIVE(Init->WaitSignalActive));
  assert_param(IS_FSMC_WRITE_OPERATION(Init->WriteOperation));
  assert_param(IS_FSMC_WAITE_SIGNAL(Init->WaitSignal));
  assert_param(IS_FSMC_EXTENDED_MODE(Init->ExtendedMode));
  assert_param(IS_FSMC_ASYNWAIT(Init->AsynchronousWait));
  assert_param(IS_FSMC_WRITE_BURST(Init->WriteBurst));

  /* Disable NORSRAM Device */
  __FSMC_NORSRAM_DISABLE(Device, Init->NSBank);

  /* Set NORSRAM device control parameters */
  if (Init->MemoryType == FSMC_MEMORY_TYPE_NOR)
  {
    MODIFY_REG(Device->BTCR[Init->NSBank], BCR_CLEAR_MASK, (uint32_t)(FSMC_NORSRAM_FLASH_ACCESS_ENABLE
               | Init->DataAddressMux
               | Init->MemoryType
               | Init->MemoryDataWidth
               | Init->BurstAccessMode
               | Init->WaitSignalPolarity
               | Init->WrapMode
               | Init->WaitSignalActive
               | Init->WriteOperation
               | Init->WaitSignal
               | Init->ExtendedMode
               | Init->AsynchronousWait
               | Init->WriteBurst
                                                                     )
              );
  }
  else
  {
    MODIFY_REG(Device->BTCR[Init->NSBank], BCR_CLEAR_MASK, (uint32_t)(FSMC_NORSRAM_FLASH_ACCESS_DISABLE
               | Init->DataAddressMux
               | Init->MemoryType
               | Init->MemoryDataWidth
               | Init->BurstAccessMode
               | Init->WaitSignalPolarity
               | Init->WrapMode
               | Init->WaitSignalActive
               | Init->WriteOperation
               | Init->WaitSignal
               | Init->ExtendedMode
               | Init->AsynchronousWait
               | Init->WriteBurst
                                                                     )
              );
  }

  return HAL_OK;
}


/**
  * @brief  DeInitialize the FSMC_NORSRAM peripheral
  * @param  Device Pointer to NORSRAM device instance
  * @param  ExDevice Pointer to NORSRAM extended mode device instance
  * @param  Bank NORSRAM bank number
  * @retval HAL status
  */
HAL_StatusTypeDef FSMC_NORSRAM_DeInit(FSMC_NORSRAM_TypeDef *Device, FSMC_NORSRAM_EXTENDED_TypeDef *ExDevice, uint32_t Bank)
{
  /* Check the parameters */
  assert_param(IS_FSMC_NORSRAM_DEVICE(Device));
  assert_param(IS_FSMC_NORSRAM_EXTENDED_DEVICE(ExDevice));
  assert_param(IS_FSMC_NORSRAM_BANK(Bank));

  /* Disable the FSMC_NORSRAM device */
  __FSMC_NORSRAM_DISABLE(Device, Bank);

  /* De-initialize the FSMC_NORSRAM device */
  /* FSMC_NORSRAM_BANK1 */
  if (Bank == FSMC_NORSRAM_BANK1)
  {
    Device->BTCR[Bank] = 0x000030DB;
  }
  /* FSMC_NORSRAM_BANK2, FSMC_NORSRAM_BANK3 or FSMC_NORSRAM_BANK4 */
  else
  {
    Device->BTCR[Bank] = 0x000030D2;
  }

  Device->BTCR[Bank + 1] = 0x0FFFFFFF;
  ExDevice->BWTR[Bank]   = 0x0FFFFFFF;

  return HAL_OK;
}


/**
  * @brief  Initialize the FSMC_NORSRAM Timing according to the specified
  *         parameters in the FSMC_NORSRAM_TimingTypeDef
  * @param  Device Pointer to NORSRAM device instance
  * @param  Timing Pointer to NORSRAM Timing structure
  * @param  Bank NORSRAM bank number
  * @retval HAL status
  */
HAL_StatusTypeDef FSMC_NORSRAM_Timing_Init(FSMC_NORSRAM_TypeDef *Device, FSMC_NORSRAM_TimingTypeDef *Timing, uint32_t Bank)
{
  /* Check the parameters */
  assert_param(IS_FSMC_NORSRAM_DEVICE(Device));
  assert_param(IS_FSMC_ADDRESS_SETUP_TIME(Timing->AddressSetupTime));
  assert_param(IS_FSMC_ADDRESS_HOLD_TIME(Timing->AddressHoldTime));
  assert_param(IS_FSMC_DATASETUP_TIME(Timing->DataSetupTime));
  assert_param(IS_FSMC_TURNAROUND_TIME(Timing->BusTurnAroundDuration));
  assert_param(IS_FSMC_CLK_DIV(Timing->CLKDivision));
  assert_param(IS_FSMC_DATA_LATENCY(Timing->DataLatency));
  assert_param(IS_FSMC_ACCESS_MODE(Timing->AccessMode));
  assert_param(IS_FSMC_NORSRAM_BANK(Bank));

  /* Set FSMC_NORSRAM device timing parameters */
  MODIFY_REG(Device->BTCR[Bank + 1],                                                    \
             BTR_CLEAR_MASK,                                                                     \
             (uint32_t)(Timing->AddressSetupTime                                               | \
                        ((Timing->AddressHoldTime)        << POSITION_VAL(FSMC_BTRx_ADDHLD))        | \
                        ((Timing->DataSetupTime)          << POSITION_VAL(FSMC_BTRx_DATAST))        | \
                        ((Timing->BusTurnAroundDuration)  << POSITION_VAL(FSMC_BTRx_BUSTURN))       | \
                        (((Timing->CLKDivision) - 1)        << POSITION_VAL(FSMC_BTRx_CLKDIV))        | \
                        (((Timing->DataLatency) - 2)        << POSITION_VAL(FSMC_BTRx_DATLAT))        | \
                        (Timing->AccessMode)));

  return HAL_OK;
}

/**
  * @brief  Initialize the FSMC_NORSRAM Extended mode Timing according to the specified
  *         parameters in the FSMC_NORSRAM_TimingTypeDef
  * @param  Device Pointer to NORSRAM device instance
  * @param  Timing Pointer to NORSRAM Timing structure
  * @param  Bank NORSRAM bank number
  * @param  ExtendedMode FSMC Extended Mode
  *          This parameter can be one of the following values:
  *            @arg FSMC_EXTENDED_MODE_DISABLE
  *            @arg FSMC_EXTENDED_MODE_ENABLE
  * @retval HAL status
  */
HAL_StatusTypeDef  FSMC_NORSRAM_Extended_Timing_Init(FSMC_NORSRAM_EXTENDED_TypeDef *Device, FSMC_NORSRAM_TimingTypeDef *Timing, uint32_t Bank, uint32_t ExtendedMode)
{
  /* Check the parameters */
  assert_param(IS_FSMC_EXTENDED_MODE(ExtendedMode));

  /* Set NORSRAM device timing register for write configuration, if extended mode is used */
  if (ExtendedMode == FSMC_EXTENDED_MODE_ENABLE)
  {
    /* Check the parameters */
    assert_param(IS_FSMC_NORSRAM_EXTENDED_DEVICE(Device));
    assert_param(IS_FSMC_ADDRESS_SETUP_TIME(Timing->AddressSetupTime));
    assert_param(IS_FSMC_ADDRESS_HOLD_TIME(Timing->AddressHoldTime));
    assert_param(IS_FSMC_DATASETUP_TIME(Timing->DataSetupTime));
    assert_param(IS_FSMC_TURNAROUND_TIME(Timing->BusTurnAroundDuration));
    assert_param(IS_FSMC_ACCESS_MODE(Timing->AccessMode));
    assert_param(IS_FSMC_NORSRAM_BANK(Bank));

    /* Set NORSRAM device timing register for write configuration, if extended mode is used */
    MODIFY_REG(Device->BWTR[Bank],                                                  \
               BWTR_CLEAR_MASK,                                                              \
               (uint32_t)(Timing->AddressSetupTime                                         | \
                          ((Timing->AddressHoldTime)        << POSITION_VAL(FSMC_BWTRx_ADDHLD)) | \
                          ((Timing->DataSetupTime)          << POSITION_VAL(FSMC_BWTRx_DATAST)) | \
                          Timing->AccessMode                                                          | \
                          ((Timing->BusTurnAroundDuration)  << POSITION_VAL(FSMC_BWTRx_BUSTURN))));
  }
  else
  {
    Device->BWTR[Bank] = 0x0FFFFFFF;
  }

  return HAL_OK;
}


/**
  * @}
  */


/** @defgroup FSMC_NORSRAM_Group2 Control functions
 *  @brief   management functions
 *
@verbatim
  ==============================================================================
                      ##### FSMC_NORSRAM Control functions #####
  ==============================================================================
  [..]
    This subsection provides a set of functions allowing to control dynamically
    the FSMC NORSRAM interface.

@endverbatim
  * @{
  */

/**
  * @brief  Enables dynamically FSMC_NORSRAM write operation.
  * @param  Device Pointer to NORSRAM device instance
  * @param  Bank NORSRAM bank number
  * @retval HAL status
  */
HAL_StatusTypeDef FSMC_NORSRAM_WriteOperation_Enable(FSMC_NORSRAM_TypeDef *Device, uint32_t Bank)
{
  /* Check the parameters */
  assert_param(IS_FSMC_NORSRAM_DEVICE(Device));
  assert_param(IS_FSMC_NORSRAM_BANK(Bank));

  /* Enable write operation */
  SET_BIT(Device->BTCR[Bank], FSMC_WRITE_OPERATION_ENABLE);

  return HAL_OK;
}

/**
  * @brief  Disables dynamically FSMC_NORSRAM write operation.
  * @param  Device Pointer to NORSRAM device instance
  * @param  Bank NORSRAM bank number
  * @retval HAL status
  */
HAL_StatusTypeDef FSMC_NORSRAM_WriteOperation_Disable(FSMC_NORSRAM_TypeDef *Device, uint32_t Bank)
{
  /* Check the parameters */
  assert_param(IS_FSMC_NORSRAM_DEVICE(Device));
  assert_param(IS_FSMC_NORSRAM_BANK(Bank));

  /* Disable write operation */
  CLEAR_BIT(Device->BTCR[Bank], FSMC_WRITE_OPERATION_ENABLE);

  return HAL_OK;
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

/**
  * @}
  */

#endif /* defined(HAL_SRAM_MODULE_ENABLED) || defined(HAL_NOR_MODULE_ENABLED) */

#endif /* FSMC_BANK1 */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
