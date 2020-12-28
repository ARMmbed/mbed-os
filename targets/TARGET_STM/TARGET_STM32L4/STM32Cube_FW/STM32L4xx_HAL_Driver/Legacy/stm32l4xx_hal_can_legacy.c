/**
  ******************************************************************************
  * @file    stm32l4xx_hal_can.c
  * @author  MCD Application Team
  * @brief   CAN HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Controller Area Network (CAN) peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral Control functions
  *           + Peripheral State and Error functions
  *
  @verbatim
  ==============================================================================
                                 ##### User NOTE #####
  ==============================================================================
    [..]
      (#) This HAL CAN driver is deprecated, it contains some CAN Tx/Rx FIFO management limitations.
          Another HAL CAN driver version has been designed with new API's, to fix these limitations.

  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
    [..]
      (#) Enable the CAN controller interface clock using
          __HAL_RCC_CAN1_CLK_ENABLE() for CAN1.

      (#) CAN pins configuration
        (++) Enable the clock for the CAN GPIOs using the following function:
             __HAL_RCC_GPIOx_CLK_ENABLE();
        (++) Connect and configure the involved CAN pins using the
              following function HAL_GPIO_Init();

      (#) Initialize and configure the CAN using HAL_CAN_Init() function.

      (#) Transmit the desired CAN frame using HAL_CAN_Transmit() or
          HAL_CAN_Transmit_IT() function.

      (#) Receive a CAN frame using HAL_CAN_Receive() or HAL_CAN_Receive_IT() function.

     *** Polling mode IO operation ***
     =================================
     [..]
       (+) Start the CAN peripheral transmission and wait the end of this operation
           using HAL_CAN_Transmit(), at this stage user can specify the value of timeout
           according to his end application
       (+) Start the CAN peripheral reception and wait the end of this operation
           using HAL_CAN_Receive(), at this stage user can specify the value of timeout
           according to his end application

     *** Interrupt mode IO operation ***
     ===================================
     [..]
       (+) Start the CAN peripheral transmission using HAL_CAN_Transmit_IT()
       (+) Start the CAN peripheral reception using HAL_CAN_Receive_IT()
       (+) Use HAL_CAN_IRQHandler() called under the used CAN Interrupt subroutine
       (+) At CAN end of transmission HAL_CAN_TxCpltCallback() function is executed and user can
            add his own code by customization of function pointer HAL_CAN_TxCpltCallback
       (+) In case of CAN Error, HAL_CAN_ErrorCallback() function is executed and user can
            add his own code by customization of function pointer HAL_CAN_ErrorCallback

     *** CAN HAL driver macros list ***
     =============================================
     [..]
       Below the list of most used macros in CAN HAL driver.

      (+) __HAL_CAN_ENABLE_IT: Enable the specified CAN interrupts
      (+) __HAL_CAN_DISABLE_IT: Disable the specified CAN interrupts
      (+) __HAL_CAN_GET_IT_SOURCE: Check if the specified CAN interrupt source is enabled or disabled
      (+) __HAL_CAN_CLEAR_FLAG: Clear the CAN's pending flags
      (+) __HAL_CAN_GET_FLAG: Get the selected CAN's flag status

     [..]
      (@) You can refer to the CAN Legacy HAL driver header file for more useful macros

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
#include "stm32l4xx_hal.h"

/** @addtogroup STM32L4xx_HAL_Driver
  * @{
  */

#ifdef HAL_CAN_LEGACY_MODULE_ENABLED
#ifdef HAL_CAN_MODULE_ENABLED
/* Select HAL CAN module in stm32l4xx_hal_conf.h file:
   (#) HAL_CAN_MODULE_ENABLED for new HAL CAN driver fixing FIFO limitations
   (#) HAL_CAN_LEGACY_MODULE_ENABLED for legacy HAL CAN driver */
#error 'The HAL CAN driver cannot be used with its legacy, Please ensure to enable only one HAL CAN module at once in stm32l4xx_hal_conf.h file'
#endif /* HAL_CAN_MODULE_ENABLED */

// #warning 'Legacy HAL CAN driver is enabled! It can be used with known limitations, refer to the release notes. However it is recommended to use rather the new HAL CAN driver'

#if defined(CAN1)

/** @defgroup CAN CAN
  * @brief CAN driver modules
  * @{
  */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup CAN_Private_Constants CAN Private Constants
  * @{
  */
#define CAN_TIMEOUT_VALUE  10
/**
  * @}
  */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup CAN_Private_Functions CAN Private Functions
 * @{
 */
static HAL_StatusTypeDef CAN_Receive_IT(CAN_HandleTypeDef* hcan, uint8_t FIFONumber);
static HAL_StatusTypeDef CAN_Transmit_IT(CAN_HandleTypeDef* hcan);
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/** @defgroup CAN_Exported_Functions CAN Exported Functions
  * @{
  */

/** @defgroup CAN_Exported_Functions_Group1 Initialization and de-initialization functions
 *  @brief    Initialization and Configuration functions
 *
@verbatim
  ==============================================================================
              ##### Initialization and de-initialization functions #####
  ==============================================================================
    [..]  This section provides functions allowing to:
      (+) Initialize and configure the CAN.
      (+) De-initialize the CAN.

@endverbatim
  * @{
  */

/**
  * @brief  Initialize the CAN peripheral according to the specified parameters
  *         in the CAN_InitStruct structure and initialize the associated handle.
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CAN_Init(CAN_HandleTypeDef* hcan)
{
  uint32_t status = CAN_INITSTATUS_FAILED;  /* Default init status */
  uint32_t tickstart = 0;

  /* Check CAN handle */
  if(hcan == NULL)
  {
     return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_CAN_ALL_INSTANCE(hcan->Instance));
  assert_param(IS_FUNCTIONAL_STATE(hcan->Init.TTCM));
  assert_param(IS_FUNCTIONAL_STATE(hcan->Init.ABOM));
  assert_param(IS_FUNCTIONAL_STATE(hcan->Init.AWUM));
  assert_param(IS_FUNCTIONAL_STATE(hcan->Init.NART));
  assert_param(IS_FUNCTIONAL_STATE(hcan->Init.RFLM));
  assert_param(IS_FUNCTIONAL_STATE(hcan->Init.TXFP));
  assert_param(IS_CAN_MODE(hcan->Init.Mode));
  assert_param(IS_CAN_SJW(hcan->Init.SJW));
  assert_param(IS_CAN_BS1(hcan->Init.BS1));
  assert_param(IS_CAN_BS2(hcan->Init.BS2));
  assert_param(IS_CAN_PRESCALER(hcan->Init.Prescaler));

  if(hcan->State == HAL_CAN_STATE_RESET)
  {
    /* Allocate lock resource and initialize it */
    hcan->Lock = HAL_UNLOCKED;

    /* Init the low level hardware */
    HAL_CAN_MspInit(hcan);
  }

  /* Initialize the CAN state*/
  hcan->State = HAL_CAN_STATE_BUSY;

  /* Exit from sleep mode */
  hcan->Instance->MCR &= (~(uint32_t)CAN_MCR_SLEEP);

  /* Request initialisation */
  hcan->Instance->MCR |= CAN_MCR_INRQ ;

  /* Get tick */
  tickstart = HAL_GetTick();

  /* Wait the acknowledge */
  while((hcan->Instance->MSR & CAN_MSR_INAK) != CAN_MSR_INAK)
  {
    if((HAL_GetTick()-tickstart) > CAN_TIMEOUT_VALUE)
    {
      hcan->State= HAL_CAN_STATE_TIMEOUT;
      /* Process unlocked */
      __HAL_UNLOCK(hcan);
      return HAL_TIMEOUT;
    }
  }

  /* Check acknowledge */
  if ((hcan->Instance->MSR & CAN_MSR_INAK) == CAN_MSR_INAK)
  {
    /* Set the time triggered communication mode */
    if (hcan->Init.TTCM == ENABLE)
    {
      hcan->Instance->MCR |= CAN_MCR_TTCM;
    }
    else
    {
      hcan->Instance->MCR &= ~(uint32_t)CAN_MCR_TTCM;
    }

    /* Set the automatic bus-off management */
    if (hcan->Init.ABOM == ENABLE)
    {
      hcan->Instance->MCR |= CAN_MCR_ABOM;
    }
    else
    {
      hcan->Instance->MCR &= ~(uint32_t)CAN_MCR_ABOM;
    }

    /* Set the automatic wake-up mode */
    if (hcan->Init.AWUM == ENABLE)
    {
      hcan->Instance->MCR |= CAN_MCR_AWUM;
    }
    else
    {
      hcan->Instance->MCR &= ~(uint32_t)CAN_MCR_AWUM;
    }

    /* Set the no automatic retransmission */
    if (hcan->Init.NART == ENABLE)
    {
      hcan->Instance->MCR |= CAN_MCR_NART;
    }
    else
    {
      hcan->Instance->MCR &= ~(uint32_t)CAN_MCR_NART;
    }

    /* Set the receive FIFO locked mode */
    if (hcan->Init.RFLM == ENABLE)
    {
      hcan->Instance->MCR |= CAN_MCR_RFLM;
    }
    else
    {
      hcan->Instance->MCR &= ~(uint32_t)CAN_MCR_RFLM;
    }

    /* Set the transmit FIFO priority */
    if (hcan->Init.TXFP == ENABLE)
    {
      hcan->Instance->MCR |= CAN_MCR_TXFP;
    }
    else
    {
      hcan->Instance->MCR &= ~(uint32_t)CAN_MCR_TXFP;
    }

    /* Set the bit timing register */
    hcan->Instance->BTR = (uint32_t)((uint32_t)hcan->Init.Mode) | \
                ((uint32_t)hcan->Init.SJW) | \
                ((uint32_t)hcan->Init.BS1) | \
                ((uint32_t)hcan->Init.BS2) | \
               ((uint32_t)hcan->Init.Prescaler - 1);

    /* Request leave initialisation */
    hcan->Instance->MCR &= ~(uint32_t)CAN_MCR_INRQ;

    /* Get tick */
    tickstart = HAL_GetTick();

    /* Wait the acknowledge */
    while((hcan->Instance->MSR & CAN_MSR_INAK) == CAN_MSR_INAK)
    {
      if((HAL_GetTick()-tickstart) > CAN_TIMEOUT_VALUE)
      {
         hcan->State= HAL_CAN_STATE_TIMEOUT;
         /* Process unlocked */
         __HAL_UNLOCK(hcan);
         return HAL_TIMEOUT;
      }
    }

    /* Check acknowledged */
    if ((hcan->Instance->MSR & CAN_MSR_INAK) != CAN_MSR_INAK)
    {
      status = CAN_INITSTATUS_SUCCESS;
    }
  }

  if(status == CAN_INITSTATUS_SUCCESS)
  {
    /* Set CAN error code to none */
    hcan->ErrorCode = HAL_CAN_ERROR_NONE;

    /* Initialize the CAN state */
    hcan->State = HAL_CAN_STATE_READY;

    /* Return function status */
    return HAL_OK;
  }
  else
  {
    /* Initialize the CAN state */
    hcan->State = HAL_CAN_STATE_ERROR;

    /* Return function status */
    return HAL_ERROR;
  }
}

/**
  * @brief  Configure the CAN reception filter according to the specified
  *         parameters in the CAN_FilterInitStruct.
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @param  sFilterConfig: pointer to a CAN_FilterConfTypeDef structure that
  *         contains the filter configuration information.
  * @retval None
  */
HAL_StatusTypeDef HAL_CAN_ConfigFilter(CAN_HandleTypeDef* hcan, CAN_FilterConfTypeDef* sFilterConfig)
{
  uint32_t filternbrbitpos = 0;

  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcan);

  /* Check the parameters */
  assert_param(IS_CAN_FILTER_NUMBER(sFilterConfig->FilterNumber));
  assert_param(IS_CAN_FILTER_MODE(sFilterConfig->FilterMode));
  assert_param(IS_CAN_FILTER_SCALE(sFilterConfig->FilterScale));
  assert_param(IS_CAN_FILTER_FIFO(sFilterConfig->FilterFIFOAssignment));
  assert_param(IS_FUNCTIONAL_STATE(sFilterConfig->FilterActivation));
  assert_param(IS_CAN_BANKNUMBER(sFilterConfig->BankNumber));

  filternbrbitpos = ((uint32_t)1) << sFilterConfig->FilterNumber;

  /* Initialisation mode for the filter */
  CAN1->FMR |= (uint32_t)CAN_FMR_FINIT;

#if defined(CAN2)
  /* Select the start slave bank */
  CAN1->FMR &= ~((uint32_t)CAN_FMR_CAN2SB);
  CAN1->FMR |= (uint32_t)(sFilterConfig->BankNumber << 8);
#endif

  /* Filter Deactivation */
  CAN1->FA1R &= ~(uint32_t)filternbrbitpos;

  /* Filter Scale */
  if (sFilterConfig->FilterScale == CAN_FILTERSCALE_16BIT)
  {
    /* 16-bit scale for the filter */
    CAN1->FS1R &= ~(uint32_t)filternbrbitpos;

    /* First 16-bit identifier and First 16-bit mask */
    /* Or First 16-bit identifier and Second 16-bit identifier */
    CAN1->sFilterRegister[sFilterConfig->FilterNumber].FR1 =
       ((0x0000FFFF & (uint32_t)sFilterConfig->FilterMaskIdLow) << 16) |
        (0x0000FFFF & (uint32_t)sFilterConfig->FilterIdLow);

    /* Second 16-bit identifier and Second 16-bit mask */
    /* Or Third 16-bit identifier and Fourth 16-bit identifier */
    CAN1->sFilterRegister[sFilterConfig->FilterNumber].FR2 =
       ((0x0000FFFF & (uint32_t)sFilterConfig->FilterMaskIdHigh) << 16) |
        (0x0000FFFF & (uint32_t)sFilterConfig->FilterIdHigh);
  }

  if (sFilterConfig->FilterScale == CAN_FILTERSCALE_32BIT)
  {
    /* 32-bit scale for the filter */
    CAN1->FS1R |= filternbrbitpos;
    /* 32-bit identifier or First 32-bit identifier */
    CAN1->sFilterRegister[sFilterConfig->FilterNumber].FR1 =
       ((0x0000FFFF & (uint32_t)sFilterConfig->FilterIdHigh) << 16) |
        (0x0000FFFF & (uint32_t)sFilterConfig->FilterIdLow);
    /* 32-bit mask or Second 32-bit identifier */
    CAN1->sFilterRegister[sFilterConfig->FilterNumber].FR2 =
       ((0x0000FFFF & (uint32_t)sFilterConfig->FilterMaskIdHigh) << 16) |
        (0x0000FFFF & (uint32_t)sFilterConfig->FilterMaskIdLow);
  }

  /* Filter Mode */
  if (sFilterConfig->FilterMode == CAN_FILTERMODE_IDMASK)
  {
    /*Id/Mask mode for the filter*/
    CAN1->FM1R &= ~(uint32_t)filternbrbitpos;
  }
  else /* CAN_FilterInitStruct->CAN_FilterMode == CAN_FilterMode_IdList */
  {
    /*Identifier list mode for the filter*/
    CAN1->FM1R |= (uint32_t)filternbrbitpos;
  }

  /* Filter FIFO assignment */
  if (sFilterConfig->FilterFIFOAssignment == CAN_FILTER_FIFO0)
  {
    /* FIFO 0 assignation for the filter */
    CAN1->FFA1R &= ~(uint32_t)filternbrbitpos;
  }

  if (sFilterConfig->FilterFIFOAssignment == CAN_FILTER_FIFO1)
  {
    /* FIFO 1 assignation for the filter */
    CAN1->FFA1R |= (uint32_t)filternbrbitpos;
  }

  /* Filter activation */
  if (sFilterConfig->FilterActivation == ENABLE)
  {
    CAN1->FA1R |= filternbrbitpos;
  }

  /* Leave the initialisation mode for the filter */
  CAN1->FMR &= ~((uint32_t)CAN_FMR_FINIT);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  DeInitialize the CAN peripheral registers to their default reset values.
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CAN_DeInit(CAN_HandleTypeDef* hcan)
{
  /* Check CAN handle */
  if(hcan == NULL)
  {
     return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_CAN_ALL_INSTANCE(hcan->Instance));

  /* Change CAN state */
  hcan->State = HAL_CAN_STATE_BUSY;

  /* DeInit the low level hardware */
  HAL_CAN_MspDeInit(hcan);

  /* Change CAN state */
  hcan->State = HAL_CAN_STATE_RESET;

  /* Release Lock */
  __HAL_UNLOCK(hcan);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Initialize the CAN MSP.
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
__weak void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcan);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_CAN_MspInit could be implemented in the user file
   */
}

/**
  * @brief  DeInitialize the CAN MSP.
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
__weak void HAL_CAN_MspDeInit(CAN_HandleTypeDef* hcan)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcan);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_CAN_MspDeInit could be implemented in the user file
   */
}

/**
  * @}
  */

/** @defgroup CAN_Exported_Functions_Group2 Input and Output operation functions
 *  @brief    I/O operation functions
 *
@verbatim
  ==============================================================================
                      ##### IO operation functions #####
  ==============================================================================
    [..]  This section provides functions allowing to:
      (+) Transmit a CAN frame message.
      (+) Receive a CAN frame message.
      (+) Enter CAN peripheral in sleep mode.
      (+) Wake up the CAN peripheral from sleep mode.

@endverbatim
  * @{
  */

/**
  * @brief  Initiate and transmit a CAN frame message.
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @param  Timeout: Timeout duration.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CAN_Transmit(CAN_HandleTypeDef* hcan, uint32_t Timeout)
{
  uint32_t transmitmailbox = CAN_TXSTATUS_NOMAILBOX;
  uint32_t tickstart = 0;

  /* Check the parameters */
  assert_param(IS_CAN_IDTYPE(hcan->pTxMsg->IDE));
  assert_param(IS_CAN_RTR(hcan->pTxMsg->RTR));
  assert_param(IS_CAN_DLC(hcan->pTxMsg->DLC));

  /* Process locked */
  __HAL_LOCK(hcan);

  if(hcan->State == HAL_CAN_STATE_BUSY_RX)
  {
    /* Change CAN state */
    hcan->State = HAL_CAN_STATE_BUSY_TX_RX;
  }
  else
  {
    /* Change CAN state */
    hcan->State = HAL_CAN_STATE_BUSY_TX;
  }

  /* Select one empty transmit mailbox */
  if ((hcan->Instance->TSR&CAN_TSR_TME0) == CAN_TSR_TME0)
  {
    transmitmailbox = 0;
  }
  else if ((hcan->Instance->TSR&CAN_TSR_TME1) == CAN_TSR_TME1)
  {
    transmitmailbox = 1;
  }
  else if ((hcan->Instance->TSR&CAN_TSR_TME2) == CAN_TSR_TME2)
  {
    transmitmailbox = 2;
  }

  if (transmitmailbox != CAN_TXSTATUS_NOMAILBOX)
  {
    /* Set up the Id */
    hcan->Instance->sTxMailBox[transmitmailbox].TIR &= CAN_TI0R_TXRQ;
    if (hcan->pTxMsg->IDE == CAN_ID_STD)
    {
      assert_param(IS_CAN_STDID(hcan->pTxMsg->StdId));
      hcan->Instance->sTxMailBox[transmitmailbox].TIR |= ((hcan->pTxMsg->StdId << 21) | \
                                                  hcan->pTxMsg->RTR);
    }
    else
    {
      assert_param(IS_CAN_EXTID(hcan->pTxMsg->ExtId));
      hcan->Instance->sTxMailBox[transmitmailbox].TIR |= ((hcan->pTxMsg->ExtId << 3) | \
                                                  hcan->pTxMsg->IDE | \
                                                  hcan->pTxMsg->RTR);
    }

    /* Set up the DLC */
    hcan->pTxMsg->DLC &= (uint8_t)0x0000000F;
    hcan->Instance->sTxMailBox[transmitmailbox].TDTR &= (uint32_t)0xFFFFFFF0;
    hcan->Instance->sTxMailBox[transmitmailbox].TDTR |= hcan->pTxMsg->DLC;

    /* Set up the data field */
    hcan->Instance->sTxMailBox[transmitmailbox].TDLR = (((uint32_t)hcan->pTxMsg->Data[3] << 24) |
                                             ((uint32_t)hcan->pTxMsg->Data[2] << 16) |
                                             ((uint32_t)hcan->pTxMsg->Data[1] << 8) |
                                             ((uint32_t)hcan->pTxMsg->Data[0]));
    hcan->Instance->sTxMailBox[transmitmailbox].TDHR = (((uint32_t)hcan->pTxMsg->Data[7] << 24) |
                                             ((uint32_t)hcan->pTxMsg->Data[6] << 16) |
                                             ((uint32_t)hcan->pTxMsg->Data[5] << 8) |
                                             ((uint32_t)hcan->pTxMsg->Data[4]));
    /* Request transmission */
    hcan->Instance->sTxMailBox[transmitmailbox].TIR |= CAN_TI0R_TXRQ;

    /* Get tick */
    tickstart = HAL_GetTick();

    /* Check End of transmission flag */
    while(!(__HAL_CAN_TRANSMIT_STATUS(hcan, transmitmailbox)))
    {
      /* Check for the Timeout */
      if(Timeout != HAL_MAX_DELAY)
      {
        if((Timeout == 0) || ((HAL_GetTick()-tickstart) > Timeout))
        {
          hcan->State = HAL_CAN_STATE_TIMEOUT;
          /* Process unlocked */
          __HAL_UNLOCK(hcan);
          return HAL_TIMEOUT;
        }
      }
    }
    if(hcan->State == HAL_CAN_STATE_BUSY_TX_RX)
    {
      /* Change CAN state */
      hcan->State = HAL_CAN_STATE_BUSY_RX;
    }
    else
    {
      /* Change CAN state */
      hcan->State = HAL_CAN_STATE_READY;
    }

    /* Process unlocked */
    __HAL_UNLOCK(hcan);

    /* Return function status */
    return HAL_OK;
  }
  else
  {
    /* Change CAN state */
    hcan->State = HAL_CAN_STATE_ERROR;

    /* Process unlocked */
    __HAL_UNLOCK(hcan);

    /* Return function status */
    return HAL_ERROR;
  }
}

/**
  * @brief  Initiate and transmit a CAN frame message in Interrupt mode.
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CAN_Transmit_IT(CAN_HandleTypeDef* hcan)
{
  uint32_t transmitmailbox = CAN_TXSTATUS_NOMAILBOX;

  /* Check the parameters */
  assert_param(IS_CAN_IDTYPE(hcan->pTxMsg->IDE));
  assert_param(IS_CAN_RTR(hcan->pTxMsg->RTR));
  assert_param(IS_CAN_DLC(hcan->pTxMsg->DLC));

  if((hcan->State == HAL_CAN_STATE_READY) || (hcan->State == HAL_CAN_STATE_BUSY_RX))
  {
    /* Process Locked */
    __HAL_LOCK(hcan);

    /* Select one empty transmit mailbox */
    if((hcan->Instance->TSR&CAN_TSR_TME0) == CAN_TSR_TME0)
    {
      transmitmailbox = 0;
    }
    else if((hcan->Instance->TSR&CAN_TSR_TME1) == CAN_TSR_TME1)
    {
      transmitmailbox = 1;
    }
    else if((hcan->Instance->TSR&CAN_TSR_TME2) == CAN_TSR_TME2)
    {
      transmitmailbox = 2;
    }

    if(transmitmailbox != CAN_TXSTATUS_NOMAILBOX)
    {
      /* Set up the Id */
      hcan->Instance->sTxMailBox[transmitmailbox].TIR &= CAN_TI0R_TXRQ;
      if(hcan->pTxMsg->IDE == CAN_ID_STD)
      {
        assert_param(IS_CAN_STDID(hcan->pTxMsg->StdId));
        hcan->Instance->sTxMailBox[transmitmailbox].TIR |= ((hcan->pTxMsg->StdId << 21) | \
                                                  hcan->pTxMsg->RTR);
      }
      else
      {
        assert_param(IS_CAN_EXTID(hcan->pTxMsg->ExtId));
        hcan->Instance->sTxMailBox[transmitmailbox].TIR |= ((hcan->pTxMsg->ExtId << 3) | \
                                                  hcan->pTxMsg->IDE | \
                                                  hcan->pTxMsg->RTR);
      }

      /* Set up the DLC */
      hcan->pTxMsg->DLC &= (uint8_t)0x0000000F;
      hcan->Instance->sTxMailBox[transmitmailbox].TDTR &= (uint32_t)0xFFFFFFF0;
      hcan->Instance->sTxMailBox[transmitmailbox].TDTR |= hcan->pTxMsg->DLC;

      /* Set up the data field */
      hcan->Instance->sTxMailBox[transmitmailbox].TDLR = (((uint32_t)hcan->pTxMsg->Data[3] << 24) |
                                             ((uint32_t)hcan->pTxMsg->Data[2] << 16) |
                                             ((uint32_t)hcan->pTxMsg->Data[1] << 8) |
                                             ((uint32_t)hcan->pTxMsg->Data[0]));
      hcan->Instance->sTxMailBox[transmitmailbox].TDHR = (((uint32_t)hcan->pTxMsg->Data[7] << 24) |
                                             ((uint32_t)hcan->pTxMsg->Data[6] << 16) |
                                             ((uint32_t)hcan->pTxMsg->Data[5] << 8) |
                                             ((uint32_t)hcan->pTxMsg->Data[4]));

      if(hcan->State == HAL_CAN_STATE_BUSY_RX)
      {
        /* Change CAN state */
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX;
      }
      else
      {
        /* Change CAN state */
        hcan->State = HAL_CAN_STATE_BUSY_TX;
      }

      /* Set CAN error code to none */
      hcan->ErrorCode = HAL_CAN_ERROR_NONE;

      /* Process Unlocked */
      __HAL_UNLOCK(hcan);

      /* Enable interrupts: */
      /*  - Enable Error warning Interrupt */
      /*  - Enable Error passive Interrupt */
      /*  - Enable Bus-off Interrupt */
      /*  - Enable Last error code Interrupt */
      /*  - Enable Error Interrupt */
      /*  - Enable Transmit mailbox empty Interrupt */
      __HAL_CAN_ENABLE_IT(hcan, CAN_IT_EWG |
                                CAN_IT_EPV |
                                CAN_IT_BOF |
                                CAN_IT_LEC |
                                CAN_IT_ERR |
                                CAN_IT_TME  );

      /* Request transmission */
      hcan->Instance->sTxMailBox[transmitmailbox].TIR |= CAN_TI0R_TXRQ;
    }
  }
  else
  {
    return HAL_BUSY;
  }

  return HAL_OK;
}

/**
  * @brief  Receive a correct CAN frame.
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @param  FIFONumber:    FIFO number.
  * @param  Timeout:       Timeout duration.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CAN_Receive(CAN_HandleTypeDef* hcan, uint8_t FIFONumber, uint32_t Timeout)
{
  uint32_t tickstart = 0;

  /* Check the parameters */
  assert_param(IS_CAN_FIFO(FIFONumber));

  /* Process locked */
  __HAL_LOCK(hcan);

  if(hcan->State == HAL_CAN_STATE_BUSY_TX)
  {
    /* Change CAN state */
    hcan->State = HAL_CAN_STATE_BUSY_TX_RX;
  }
  else
  {
    /* Change CAN state */
    hcan->State = HAL_CAN_STATE_BUSY_RX;
  }

  /* Get tick */
  tickstart = HAL_GetTick();

  /* Check pending message */
  while(__HAL_CAN_MSG_PENDING(hcan, FIFONumber) == 0)
  {
    /* Check for the Timeout */
    if(Timeout != HAL_MAX_DELAY)
    {
      if((Timeout == 0) || ((HAL_GetTick()-tickstart) > Timeout))
      {
        hcan->State = HAL_CAN_STATE_TIMEOUT;
        /* Process unlocked */
        __HAL_UNLOCK(hcan);
        return HAL_TIMEOUT;
      }
    }
  }

  /* Get the Id */
  hcan->pRxMsg->IDE = (uint8_t)0x04 & hcan->Instance->sFIFOMailBox[FIFONumber].RIR;
  if (hcan->pRxMsg->IDE == CAN_ID_STD)
  {
    hcan->pRxMsg->StdId = (uint32_t)0x000007FF & (hcan->Instance->sFIFOMailBox[FIFONumber].RIR >> 21);
  }
  else
  {
    hcan->pRxMsg->ExtId = (uint32_t)0x1FFFFFFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RIR >> 3);
  }

  hcan->pRxMsg->RTR = (uint8_t)0x02 & hcan->Instance->sFIFOMailBox[FIFONumber].RIR;
  /* Get the DLC */
  hcan->pRxMsg->DLC = (uint8_t)0x0F & hcan->Instance->sFIFOMailBox[FIFONumber].RDTR;
  /* Get the FMI */
  hcan->pRxMsg->FMI = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDTR >> 8);
  /* Get the data field */
  hcan->pRxMsg->Data[0] = (uint8_t)0xFF & hcan->Instance->sFIFOMailBox[FIFONumber].RDLR;
  hcan->pRxMsg->Data[1] = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 8);
  hcan->pRxMsg->Data[2] = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 16);
  hcan->pRxMsg->Data[3] = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 24);
  hcan->pRxMsg->Data[4] = (uint8_t)0xFF & hcan->Instance->sFIFOMailBox[FIFONumber].RDHR;
  hcan->pRxMsg->Data[5] = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 8);
  hcan->pRxMsg->Data[6] = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 16);
  hcan->pRxMsg->Data[7] = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 24);

  /* Release the FIFO */
  if(FIFONumber == CAN_FIFO0)
  {
    /* Release FIFO0 */
    __HAL_CAN_FIFO_RELEASE(hcan, CAN_FIFO0);
  }
  else /* FIFONumber == CAN_FIFO1 */
  {
    /* Release FIFO1 */
    __HAL_CAN_FIFO_RELEASE(hcan, CAN_FIFO1);
  }

  if(hcan->State == HAL_CAN_STATE_BUSY_TX_RX)
  {
    /* Change CAN state */
    hcan->State = HAL_CAN_STATE_BUSY_TX;
  }
  else
  {
    /* Change CAN state */
    hcan->State = HAL_CAN_STATE_READY;
  }

  /* Process unlocked */
  __HAL_UNLOCK(hcan);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Receive a correct CAN frame in Interrupt mode.
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @param  FIFONumber:    FIFO number.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CAN_Receive_IT(CAN_HandleTypeDef* hcan, uint8_t FIFONumber)
{
  /* Check the parameters */
  assert_param(IS_CAN_FIFO(FIFONumber));

  if((hcan->State == HAL_CAN_STATE_READY) || (hcan->State == HAL_CAN_STATE_BUSY_TX))
  {
    /* Process locked */
    __HAL_LOCK(hcan);

    if(hcan->State == HAL_CAN_STATE_BUSY_TX)
    {
      /* Change CAN state */
      hcan->State = HAL_CAN_STATE_BUSY_TX_RX;
    }
    else
    {
      /* Change CAN state */
      hcan->State = HAL_CAN_STATE_BUSY_RX;
    }

    /* Set CAN error code to none */
    hcan->ErrorCode = HAL_CAN_ERROR_NONE;

    /* Enable interrupts: */
    /*  - Enable Error warning Interrupt */
    /*  - Enable Error passive Interrupt */
    /*  - Enable Bus-off Interrupt */
    /*  - Enable Last error code Interrupt */
    /*  - Enable Error Interrupt */
    __HAL_CAN_ENABLE_IT(hcan, CAN_IT_EWG |
                              CAN_IT_EPV |
                              CAN_IT_BOF |
                              CAN_IT_LEC |
                              CAN_IT_ERR );

    /* Process unlocked */
    __HAL_UNLOCK(hcan);

    if(FIFONumber == CAN_FIFO0)
    {
      /* Enable FIFO 0 overrun and message pending Interrupt */
      __HAL_CAN_ENABLE_IT(hcan, CAN_IT_FOV0 | CAN_IT_FMP0);
    }
    else
    {
      /* Enable FIFO 1 overrun and message pending Interrupt */
      __HAL_CAN_ENABLE_IT(hcan, CAN_IT_FOV1 | CAN_IT_FMP1);
    }

  }
  else
  {
    return HAL_BUSY;
  }

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Enter the Sleep (low power) mode.
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_CAN_Sleep(CAN_HandleTypeDef* hcan)
{
  uint32_t tickstart = 0;

  /* Process locked */
  __HAL_LOCK(hcan);

  /* Change CAN state */
  hcan->State = HAL_CAN_STATE_BUSY;

  /* Request Sleep mode */
   hcan->Instance->MCR = (((hcan->Instance->MCR) & (uint32_t)(~(uint32_t)CAN_MCR_INRQ)) | CAN_MCR_SLEEP);

  /* Sleep mode status */
  if ((hcan->Instance->MSR & (CAN_MSR_SLAK|CAN_MSR_INAK)) != CAN_MSR_SLAK)
  {
    /* Process unlocked */
    __HAL_UNLOCK(hcan);

    /* Return function status */
    return HAL_ERROR;
  }

  /* Get tick */
  tickstart = HAL_GetTick();

  /* Wait the acknowledge */
  while((hcan->Instance->MSR & (CAN_MSR_SLAK|CAN_MSR_INAK)) != CAN_MSR_SLAK)
  {
    if((HAL_GetTick()-tickstart) > CAN_TIMEOUT_VALUE)
    {
      hcan->State = HAL_CAN_STATE_TIMEOUT;
      /* Process unlocked */
      __HAL_UNLOCK(hcan);
      return HAL_TIMEOUT;
    }
  }

  /* Change CAN state */
  hcan->State = HAL_CAN_STATE_READY;

  /* Process unlocked */
  __HAL_UNLOCK(hcan);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Wake up the CAN peripheral from sleep mode (after that the CAN peripheral
  *         is in the normal mode).
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_CAN_WakeUp(CAN_HandleTypeDef* hcan)
{
  uint32_t tickstart = 0;

  /* Process locked */
  __HAL_LOCK(hcan);

  /* Change CAN state */
  hcan->State = HAL_CAN_STATE_BUSY;

  /* Wake up request */
  hcan->Instance->MCR &= ~(uint32_t)CAN_MCR_SLEEP;

  /* Get tick */
  tickstart = HAL_GetTick();

  /* Sleep mode status */
  while((hcan->Instance->MSR & CAN_MSR_SLAK) == CAN_MSR_SLAK)
  {
    if((HAL_GetTick()-tickstart) > CAN_TIMEOUT_VALUE)
    {
      hcan->State= HAL_CAN_STATE_TIMEOUT;
      /* Process unlocked */
      __HAL_UNLOCK(hcan);
      return HAL_TIMEOUT;
    }
  }
  if((hcan->Instance->MSR & CAN_MSR_SLAK) == CAN_MSR_SLAK)
  {
    /* Process unlocked */
    __HAL_UNLOCK(hcan);

    /* Return function status */
    return HAL_ERROR;
  }

  /* Change CAN state */
  hcan->State = HAL_CAN_STATE_READY;

  /* Process unlocked */
  __HAL_UNLOCK(hcan);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Handle CAN interrupt request.
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
void HAL_CAN_IRQHandler(CAN_HandleTypeDef* hcan)
{
  uint32_t errorcode = HAL_CAN_ERROR_NONE;

  /* Check Overrun flag for FIFO0 */
  if((__HAL_CAN_GET_FLAG(hcan, CAN_FLAG_FOV0))    &&
     (__HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_FOV0)))
  {
    /* Set CAN error code to FOV0 error */
    errorcode |= HAL_CAN_ERROR_FOV0;

    /* Clear FIFO0 Overrun Flag */
    __HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_FOV0);
  }

  /* Check Overrun flag for FIFO1 */
  if((__HAL_CAN_GET_FLAG(hcan, CAN_FLAG_FOV1))    &&
     (__HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_FOV1)))
  {
    /* Set CAN error code to FOV1 error */
    errorcode |= HAL_CAN_ERROR_FOV1;

    /* Clear FIFO1 Overrun Flag */
    __HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_FOV1);
  }

  /* Check End of transmission flag */
  if(__HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_TME))
  {
    if((__HAL_CAN_TRANSMIT_STATUS(hcan, CAN_TXMAILBOX_0)) ||
       (__HAL_CAN_TRANSMIT_STATUS(hcan, CAN_TXMAILBOX_1)) ||
       (__HAL_CAN_TRANSMIT_STATUS(hcan, CAN_TXMAILBOX_2)))
    {
      /* Call transmit function */
      CAN_Transmit_IT(hcan);
    }
  }

  /* Check End of reception flag for FIFO0 */
  if((__HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_FMP0)) &&
     (__HAL_CAN_MSG_PENDING(hcan, CAN_FIFO0) != 0))
  {
    /* Call receive function */
    CAN_Receive_IT(hcan, CAN_FIFO0);
  }

  /* Check End of reception flag for FIFO1 */
  if((__HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_FMP1)) &&
     (__HAL_CAN_MSG_PENDING(hcan, CAN_FIFO1) != 0))
  {
    /* Call receive function */
    CAN_Receive_IT(hcan, CAN_FIFO1);
  }

  /* Set error code in handle */
  hcan->ErrorCode |= errorcode;

  /* Check Error Warning Flag */
  if((__HAL_CAN_GET_FLAG(hcan, CAN_FLAG_EWG))    &&
     (__HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_EWG)) &&
     (__HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_ERR)))
  {
    /* Set CAN error code to EWG error */
    hcan->ErrorCode |= HAL_CAN_ERROR_EWG;
    /* No need for clear of Error Warning Flag as read-only */
  }

  /* Check Error Passive Flag */
  if((__HAL_CAN_GET_FLAG(hcan, CAN_FLAG_EPV))    &&
     (__HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_EPV)) &&
     (__HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_ERR)))
  {
    /* Set CAN error code to EPV error */
    hcan->ErrorCode |= HAL_CAN_ERROR_EPV;
    /* No need for clear of Error Passive Flag as read-only */
  }

  /* Check Bus-Off Flag */
  if((__HAL_CAN_GET_FLAG(hcan, CAN_FLAG_BOF))    &&
     (__HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_BOF)) &&
     (__HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_ERR)))
  {
    /* Set CAN error code to BOF error */
    hcan->ErrorCode |= HAL_CAN_ERROR_BOF;
    /* No need for clear of Bus-Off Flag as read-only */
  }

  /* Check Last error code Flag */
  if((!HAL_IS_BIT_CLR(hcan->Instance->ESR, CAN_ESR_LEC)) &&
     (__HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_LEC))         &&
     (__HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_ERR)))
  {
    switch(hcan->Instance->ESR & CAN_ESR_LEC)
    {
      case(CAN_ESR_LEC_0):
          /* Set CAN error code to STF error */
          hcan->ErrorCode |= HAL_CAN_ERROR_STF;
          break;
      case(CAN_ESR_LEC_1):
          /* Set CAN error code to FOR error */
          hcan->ErrorCode |= HAL_CAN_ERROR_FOR;
          break;
      case(CAN_ESR_LEC_1 | CAN_ESR_LEC_0):
          /* Set CAN error code to ACK error */
          hcan->ErrorCode |= HAL_CAN_ERROR_ACK;
          break;
      case(CAN_ESR_LEC_2):
          /* Set CAN error code to BR error */
          hcan->ErrorCode |= HAL_CAN_ERROR_BR;
          break;
      case(CAN_ESR_LEC_2 | CAN_ESR_LEC_0):
          /* Set CAN error code to BD error */
          hcan->ErrorCode |= HAL_CAN_ERROR_BD;
          break;
      case(CAN_ESR_LEC_2 | CAN_ESR_LEC_1):
          /* Set CAN error code to CRC error */
          hcan->ErrorCode |= HAL_CAN_ERROR_CRC;
          break;
      default:
          break;
    }

    /* Clear Last error code Flag */
    hcan->Instance->ESR &= ~(CAN_ESR_LEC);
  }

  /* Call the Error call Back in case of Errors */
  if(hcan->ErrorCode != HAL_CAN_ERROR_NONE)
  {
    /* Clear ERRI bit */
    SET_BIT(hcan->Instance->MSR, CAN_MSR_ERRI);

    /* Set the CAN state ready to be able to start again the process */
    hcan->State = HAL_CAN_STATE_READY;

    /* Disable interrupts: */
    /*  - Disable Error warning Interrupt */
    /*  - Disable Error passive Interrupt */
    /*  - Disable Bus-off Interrupt */
    /*  - Disable Last error code Interrupt */
    /*  - Disable Error Interrupt */
    /*  - Disable FIFO 0 message pending Interrupt */
    /*  - Disable FIFO 0 Overrun Interrupt */
    /*  - Disable FIFO 1 message pending Interrupt */
    /*  - Disable FIFO 1 Overrun Interrupt */
    /*  - Disable Transmit mailbox empty Interrupt */
    __HAL_CAN_DISABLE_IT(hcan, CAN_IT_EWG |
                               CAN_IT_EPV |
                               CAN_IT_BOF |
                               CAN_IT_LEC |
                               CAN_IT_ERR |
                               CAN_IT_FMP0|
                               CAN_IT_FOV0|
                               CAN_IT_FMP1|
                               CAN_IT_FOV1|
                               CAN_IT_TME  );

    /* Call Error callback function */
    HAL_CAN_ErrorCallback(hcan);
  }
}

/**
  * @brief  Transmission complete callback in non-blocking mode.
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
__weak void HAL_CAN_TxCpltCallback(CAN_HandleTypeDef* hcan)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcan);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_CAN_TxCpltCallback could be implemented in the user file
   */
}

/**
  * @brief  Reception complete callback in non-blocking mode.
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
__weak void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcan);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_CAN_RxCpltCallback could be implemented in the user file
   */
}

/**
  * @brief  Error CAN callback.
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
__weak void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcan);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_CAN_ErrorCallback could be implemented in the user file
   */
}

/**
  * @}
  */

/** @defgroup CAN_Exported_Functions_Group3 Peripheral State and Error functions
 *  @brief   CAN Peripheral State functions
 *
@verbatim
  ==============================================================================
            ##### Peripheral State and Error functions #####
  ==============================================================================
    [..]
    This subsection provides functions allowing to :
      (+) Check the CAN state.
      (+) Check CAN Errors detected during interrupt process.

@endverbatim
  * @{
  */

/**
  * @brief  Return the CAN handle state.
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval HAL state
  */
HAL_CAN_StateTypeDef HAL_CAN_GetState(CAN_HandleTypeDef* hcan)
{
  /* Return CAN handle state */
  return hcan->State;
}

/**
  * @brief  Return the CAN error code.
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval CAN Error Code
  */
uint32_t HAL_CAN_GetError(CAN_HandleTypeDef *hcan)
{
  return hcan->ErrorCode;
}

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup CAN_Private_Functions CAN Private Functions
 * @{
 */
/**
  * @brief  Initiate and transmit a CAN frame message.
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval HAL status
  */
static HAL_StatusTypeDef CAN_Transmit_IT(CAN_HandleTypeDef* hcan)
{
  /* Disable Transmit mailbox empty Interrupt */
  __HAL_CAN_DISABLE_IT(hcan, CAN_IT_TME);

  if(hcan->State == HAL_CAN_STATE_BUSY_TX)
  {
    /* Disable interrupts: */
    /*  - Disable Error warning Interrupt */
    /*  - Disable Error passive Interrupt */
    /*  - Disable Bus-off Interrupt */
    /*  - Disable Last error code Interrupt */
    /*  - Disable Error Interrupt */
    __HAL_CAN_DISABLE_IT(hcan, CAN_IT_EWG |
                               CAN_IT_EPV |
                               CAN_IT_BOF |
                               CAN_IT_LEC |
                               CAN_IT_ERR  );
  }

  if(hcan->State == HAL_CAN_STATE_BUSY_TX_RX)
  {
    /* Change CAN state */
    hcan->State = HAL_CAN_STATE_BUSY_RX;
  }
  else
  {
    /* Change CAN state */
    hcan->State = HAL_CAN_STATE_READY;
  }

  /* Transmission complete callback */
  HAL_CAN_TxCpltCallback(hcan);

  return HAL_OK;
}

/**
  * @brief  Receive a correct CAN frame.
  * @param  hcan:       Pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @param  FIFONumber: Specify the FIFO number
  * @retval HAL status
  */
static HAL_StatusTypeDef CAN_Receive_IT(CAN_HandleTypeDef* hcan, uint8_t FIFONumber)
{
  /* Get the Id */
  hcan->pRxMsg->IDE = (uint8_t)0x04 & hcan->Instance->sFIFOMailBox[FIFONumber].RIR;
  if (hcan->pRxMsg->IDE == CAN_ID_STD)
  {
    hcan->pRxMsg->StdId = (uint32_t)0x000007FF & (hcan->Instance->sFIFOMailBox[FIFONumber].RIR >> 21);
  }
  else
  {
    hcan->pRxMsg->ExtId = (uint32_t)0x1FFFFFFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RIR >> 3);
  }

  hcan->pRxMsg->RTR = (uint8_t)0x02 & hcan->Instance->sFIFOMailBox[FIFONumber].RIR;
  /* Get the DLC */
  hcan->pRxMsg->DLC = (uint8_t)0x0F & hcan->Instance->sFIFOMailBox[FIFONumber].RDTR;
  /* Get the FMI */
  hcan->pRxMsg->FMI = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDTR >> 8);
  /* Get the data field */
  hcan->pRxMsg->Data[0] = (uint8_t)0xFF & hcan->Instance->sFIFOMailBox[FIFONumber].RDLR;
  hcan->pRxMsg->Data[1] = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 8);
  hcan->pRxMsg->Data[2] = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 16);
  hcan->pRxMsg->Data[3] = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 24);
  hcan->pRxMsg->Data[4] = (uint8_t)0xFF & hcan->Instance->sFIFOMailBox[FIFONumber].RDHR;
  hcan->pRxMsg->Data[5] = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 8);
  hcan->pRxMsg->Data[6] = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 16);
  hcan->pRxMsg->Data[7] = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 24);
  /* Release the FIFO */
  /* Release FIFO0 */
  if (FIFONumber == CAN_FIFO0)
  {
    __HAL_CAN_FIFO_RELEASE(hcan, CAN_FIFO0);

    /* Disable FIFO 0 overrun and message pending Interrupt */
    __HAL_CAN_DISABLE_IT(hcan, CAN_IT_FOV0 | CAN_IT_FMP0);
  }
  /* Release FIFO1 */
  else /* FIFONumber == CAN_FIFO1 */
  {
    __HAL_CAN_FIFO_RELEASE(hcan, CAN_FIFO1);

    /* Disable FIFO 1 overrun and message pending Interrupt */
    __HAL_CAN_DISABLE_IT(hcan, CAN_IT_FOV1 | CAN_IT_FMP1);
  }

  if(hcan->State == HAL_CAN_STATE_BUSY_RX)
  {
    /* Disable interrupts: */
    /*  - Disable Error warning Interrupt */
    /*  - Disable Error passive Interrupt */
    /*  - Disable Bus-off Interrupt */
    /*  - Disable Last error code Interrupt */
    /*  - Disable Error Interrupt */
    __HAL_CAN_DISABLE_IT(hcan, CAN_IT_EWG |
                               CAN_IT_EPV |
                               CAN_IT_BOF |
                               CAN_IT_LEC |
                               CAN_IT_ERR  );
  }

  if(hcan->State == HAL_CAN_STATE_BUSY_TX_RX)
  {
    /* Disable CAN state */
    hcan->State = HAL_CAN_STATE_BUSY_TX;
  }
  else
  {
    /* Change CAN state */
    hcan->State = HAL_CAN_STATE_READY;
  }

  /* Receive complete callback */
  HAL_CAN_RxCpltCallback(hcan);

  /* Return function status */
  return HAL_OK;
}
/**
 * @}
 */

/**
  * @}
  */

#endif /* CAN1 */

#endif /* HAL_CAN_LEGACY_MODULE_ENABLED */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
