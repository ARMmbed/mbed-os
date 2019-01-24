/**
  ******************************************************************************
  * @file    BlueNRG1_i2c.c
  * @author  VMA Application Team
  * @version V2.1.0
  * @date    21-March-2016
  * @brief   This file provides all the I2C firmware functions.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "../../Periph_Driver/inc/BlueNRG1_i2c.h"


/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
  * @{
  */

/** @addtogroup I2C_Peripheral  I2C Peripheral
  * @{
  */ 

/** @defgroup I2C_Private_TypesDefinitions Private Types Definitions
  * @{
  */

/**
  * @}
  */

/** @defgroup I2C_Private_Defines Private Defines
  * @{
  */

/* I2C Standard mode */
#define I2C_CR_SM_STD             (0x0)

/* I2C Fast mode */
#define I2C_CR_SM_FAST            (0x1)

#define I2C_CLOCK                 (16000000)

#define I2C_SLAVE_ADDR_EXT10_SMK  (0x03FF)


/**
  * @}
  */

/** @defgroup I2C_Private_Macros Private Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup I2C_Private_Variables Private Variables
  * @{
  */
static __I uint16_t Foncycles[4] = {1, 3, 4, 6};

/**
  * @}
  */

/** @defgroup I2C_Private_FunctionPrototypes Private Function Prototypes
  * @{
  */

/**
  * @}
  */

/** @defgroup I2C_Public_Functions Public Functions
  * @{
  */


/**
* @brief  Deinitializes the I2C peripheral registers to their default reset values.
* @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
* @retval None
*/
void I2C_DeInit(I2C_Type* I2Cx)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  /* Generate a stop condition */
  I2Cx->CR_b.FS_1 = SET;

  /* Disable the selected I2C peripheral / software reset */
  I2Cx->CR_b.PE = RESET;
}


/**
  * @brief  Initializes the I2Cx peripheral according to the specified 
  *         parameters in the I2C_InitStruct.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
  * @param  I2C_InitStruct: pointer to a @ref I2C_InitType structure that
  *         contains the configuration information for the specified I2C peripheral.
* @retval   None
  */
void I2C_Init(I2C_Type* I2Cx, I2C_InitType* I2C_InitStruct)
{
  uint16_t foncycle;
  uint32_t brcnt;

  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_CLOCK_SPEED_I2C(I2C_InitStruct->I2C_ClockSpeed));
  assert_param(IS_I2C_OPERATING_MODE(I2C_InitStruct->I2C_OperatingMode));
  assert_param(IS_I2C_FILTERING(I2C_InitStruct->I2C_Filtering));
  assert_param(IS_I2C_OWN_ADDRESS1(I2C_InitStruct->I2C_OwnAddress1));
  assert_param(IS_FUNCTIONAL_STATE(I2C_InitStruct->I2C_ExtendAddress));
  
  /* Disable the selected I2C peripheral / software reset */
  I2Cx->CR_b.PE = RESET;

  /* Select the digital filtering */
  I2Cx->CR_b.FON = I2C_InitStruct->I2C_Filtering;

  /* Clock rate computation */
  foncycle = Foncycles[I2C_InitStruct->I2C_Filtering];
  brcnt = I2C_CLOCK - I2C_InitStruct->I2C_ClockSpeed * foncycle;

  /* Configure speed in standard mode */
  if (I2C_InitStruct->I2C_ClockSpeed <= 100000)
  { 
    brcnt /= (I2C_InitStruct->I2C_ClockSpeed << 1);
    
    I2Cx->CR_b.SM = I2C_CR_SM_STD;
    
    /* @16MHz 0x0003=>250ns */
    I2Cx->SCR_b.SLSU = 0x003;
  }
  /* Configure speed in fast mode */
  else if (I2C_InitStruct->I2C_ClockSpeed <= 400000)
  {
    brcnt /= (I2C_InitStruct->I2C_ClockSpeed * 3);
    
    I2Cx->CR_b.SM = I2C_CR_SM_FAST;
    
    /* @16MHz 0x0002=>125ns */
    I2Cx->SCR_b.SLSU = 0x0002;
  }

  /* Set I2C own address */
  MODIFY_REG(I2Cx->SCR, I2C_SLAVE_ADDR_EXT10_SMK, I2C_InitStruct->I2C_OwnAddress1);

  /* Configure the baud rate */
  I2Cx->BRCR_b.BRCNT = (uint16_t)brcnt;

  /* Configure the operating mode */
  I2Cx->CR_b.OM = I2C_InitStruct->I2C_OperatingMode;
  
  if(I2C_InitStruct->I2C_ExtendAddress == ENABLE) {
    I2Cx->CR_b.SAM = SET;
  }
  else {
    I2Cx->CR_b.SAM = RESET;
  }
  
  /* Enable the selected I2C peripheral */
  I2Cx->CR_b.PE = SET;
}


/**
  * @brief  Fills the I2C_InitStruct with default values.
  * @param  I2C_InitStruct: pointer to an @ref I2C_InitType structure which will be initialized.
  * @retval None
  */
void I2C_StructInit(I2C_InitType* I2C_InitStruct)
{
  /*---------------- Reset I2C init structure parameters values ----------------*/
  /* initialize the I2C_ClockSpeed member */
  I2C_InitStruct->I2C_ClockSpeed = 5000;

  /* initialize the I2C_OperatingMode member */
  I2C_InitStruct->I2C_OperatingMode = I2C_OperatingMode_Slave;

  /* initialize the I2C_Filtering member */
  I2C_InitStruct->I2C_Filtering = I2C_Filtering_Off;

  /* Initialize the I2C_OwnAddress1 member */
  I2C_InitStruct->I2C_OwnAddress1 = 0;

  /* Initialize the I2C_AddressingMode member */
  I2C_InitStruct->I2C_ExtendAddress = DISABLE;
}

/**
  * @brief  Enables or disables the specified I2C peripheral.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
  * @param  NewState: functional state @ref FunctionalState
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2C_Cmd(I2C_Type* I2Cx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE) {
    /* Enable the selected I2C peripheral */
    I2Cx->CR_b.PE = SET;
  }
  else {
    /* Disable the selected I2C peripheral */
    I2Cx->CR_b.PE = RESET;
  }
}


/**
  * @brief  Start an I2C transaction sending the frame specified.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
  * @param  tr: pointer to an @ref I2C_TransactionType structure used
  *   for this transaction.
  * @retval None.
  */
void I2C_BeginTransaction(I2C_Type* I2Cx, I2C_TransactionType* tr)
{
  
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_OPERATION(tr->Operation));
  assert_param(IS_I2C_STARTBYTE(tr->StartByte));
  assert_param(IS_I2C_ADDRESSTYPE(tr->AddressType));
  assert_param(IS_I2C_OWN_ADDRESS1(tr->Address));
  assert_param(IS_I2C_STOPCONDITION(tr->StopCondition));
  assert_param(IS_I2C_LENGTH(tr->Length));
    
  I2Cx->MCR = ((uint32_t)(tr->Operation)) | (((uint32_t)(tr->Address)) << 1) |
        ((uint32_t)(tr->StartByte)) | ((uint32_t)(tr->AddressType)) | 
        ((uint32_t)(tr->StopCondition)) | (((uint32_t)(tr->Length)) << 15);
  
}


/**
  * @brief  Enables or disables the general call feature.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
  * @param  NewState: functional state @ref FunctionalState
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2C_GeneralCallCmd(I2C_Type* I2Cx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE) {
    /* Enable generall call */
    I2Cx->CR_b.SGCM = SET;
  }
  else {
    /* Disable generall call */
    I2Cx->CR_b.SGCM = RESET;
  }
}

/**
  * @brief  Enables or disables the specified I2C interrupts.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
  * @param  I2C_IT: specifies the I2C interrupts sources to be enabled or disabled. 
  *   This parameter can be any combination of the following values:
  *     @arg I2C_IT_TXFE: Tx FIFO empty
  *     @arg I2C_IT_TXFNE: TX FIFO nearly empty
  *     @arg I2C_IT_TXFF: Tx FIFO full
  *     @arg I2C_IT_TXFOVR: Tx FIFO overflow
  *     @arg I2C_IT_RXFE: Rx FIFO empty
  *     @arg I2C_IT_RXFNF: Rx FIFO nearly full
  *     @arg I2C_IT_RXFF: Rx FIFO full
  *     @arg I2C_IT_LBR: Length number of bytes received
  *     @arg I2C_IT_RFSR: Read from slave request
  *     @arg I2C_IT_RFSE: Read from slave empty
  *     @arg I2C_IT_WTSR: Write to slave request
  *     @arg I2C_IT_MTD: Master transaction done
  *     @arg I2C_IT_STD: Slave transaction done
  *     @arg I2C_IT_SAL: Slave arbitration lost
  *     @arg I2C_IT_MAL: Master arbitration lost
  *     @arg I2C_IT_BERR: Bus error
  *     @arg I2C_IT_MTDWS: Master transaction done without stop
  *     @arg I2C_IT_TIMEOUT: Timeout or Tlow error
  * @param  NewState: functional state @ref FunctionalState
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2C_ITConfig(I2C_Type* I2Cx, uint32_t I2C_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_I2C_IT(I2C_IT));
  
  if (NewState != DISABLE) {
    /* Enable the selected I2C interrupts */
    SET_BIT(I2Cx->IMSCR, I2C_IT);
  }
  else {
    /* Disable the selected I2C interrupts */
    CLEAR_BIT(I2Cx->IMSCR, I2C_IT);
  }
}

/**
  * @brief  Fills the Tx FIFO with the specified data byte.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
  * @param  Data: Byte to be transmitted.
  * @retval None
  */
void I2C_FillTxFIFO(I2C_Type* I2Cx, uint8_t Data)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  /* Write in the DR register the data to be sent */
  I2Cx->TFR = (uint32_t)Data;
}

/**
  * @brief  Returns the most recent received data by the I2Cx peripheral.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
  * @retval uint8_t The value of the received data.
  */
uint8_t I2C_ReceiveData(I2C_Type* I2Cx)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  /* Return the data in the DR register */
  return I2Cx->RFR_b.RDATA;
}


/**
  * @brief  Returns the most recent received data by the I2Cx peripheral.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
  * @retval None
  */
void I2C_FlushTx(I2C_Type* I2Cx)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  /* Set flush tx flag */
  I2Cx->CR_b.FTX = SET;
}


/**
  * @brief  Return the status of the TX FIFO flush operation.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
  * @retval I2C_OpStatus operation status @ref I2C_OpStatus
  */
I2C_OpStatus I2C_WaitFlushTx(I2C_Type* I2Cx)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  /* Wait the flush of TX FIFO is completed */
  if(I2Cx->CR_b.FTX == SET) {
    return I2C_OP_ONGOING;
  }
  else {
    return I2C_OP_OK;
  }
}


/**
  * @brief  Returns the most recent received data by the I2Cx peripheral.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
  * @retval None
  */
void I2C_FlushRx(I2C_Type* I2Cx)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  /* Set flush rx flag */
  I2Cx->CR_b.FRX = SET;
}


/**
  * @brief  Return the status of the RX FIFO flush operation.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
  * @retval I2C_OpStatus operation status @ref I2C_OpStatus
  */
I2C_OpStatus I2C_WaitFlushRx(I2C_Type* I2Cx)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  /* Wait the flush of TX FIFO is completed */
  if(I2Cx->CR_b.FRX == SET) {
    return I2C_OP_ONGOING;
  }
  else {
    return I2C_OP_OK;
  }
}

/**
  * @brief  Sets the transmit FIFO threshold by the I2Cx peripheral.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
  * @param  TxThres: The value of the threshold 2-byte.
  * @retval None
  */
void I2C_SetTxThreshold(I2C_Type* I2Cx, uint16_t TxThres)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_TX_THRESHOLD(TxThres));
  
  /* Set the Tx FIFO Threshold */
  I2Cx->TFTR_b.THRESH_TX = TxThres;
}

/**
  * @brief  Sets the receive FIFO threshold by the I2Cx peripheral.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
  * @param  RxThres: The value of the threshold 2-byte.
  */
void I2C_SetRxThreshold(I2C_Type* I2Cx, uint16_t RxThres)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_RX_THRESHOLD(RxThres));
  
  /* Set the Rx FIFO Threshold */
  I2Cx->RFTR_b.THRESH_RX = RxThres;
}


/**
  * @brief  Generate a stop condition.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
  * @retval None
  */
void I2C_GenerateStopCondition(I2C_Type* I2Cx)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  /* Generate a stop condition */
  I2Cx->CR_b.FS_1 = SET;
}

/**
  * @brief  Set the hold time value for data transfer.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
  * @param  I2C_HoldTime: specifies the hold time value.
  * @retval None
  */
void I2C_SetHoldTime(I2C_Type* I2Cx, uint16_t I2C_HoldTime)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_HOLDTIME(I2C_HoldTime));

  I2Cx->THDDAT = I2C_HoldTime;
}

/**
  * @brief  Set the hold time value for start condition.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
  * @param  I2C_HoldTime: specifies the hold time value.
  * @retval None
  */
void I2C_SetHoldTimeStartCondition(I2C_Type* I2Cx, uint16_t I2C_HoldTime)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_HOLDTIME(I2C_HoldTime));

  /* Standard mode */
  if(I2Cx->CR_b.SM == I2C_CR_SM_STD) {
    I2Cx->THDSTA_FST_STD_b.THDSTA_STD = I2C_HoldTime;
  }
  else if(I2Cx->CR_b.SM == I2C_CR_SM_FAST) {
    I2Cx->THDSTA_FST_STD_b.THDSTA_FST = I2C_HoldTime;
  }
}

/**
  * @brief  Set the setup time value for start condition.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
  * @param  I2C_SetupTime: specifies the setup time value.
  * @retval None
  */
void I2C_SetSetupTimeStartCondition(I2C_Type* I2Cx, uint16_t I2C_SetupTime)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_SETUPTIME(I2C_SetupTime));

  /* Standard mode */
  if(I2Cx->CR_b.SM == I2C_CR_SM_STD) {
    I2Cx->TSUSTA_FST_STD_b.TSUSTA_STD = I2C_SetupTime;
  }
  else if(I2Cx->CR_b.SM == I2C_CR_SM_FAST) {
    I2Cx->TSUSTA_FST_STD_b.TSUSTA_FST = I2C_SetupTime;
  }
}

/**
  * @brief  Checks whether the specified I2C interrupt has occurred or not.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
  * @param  I2C_IT: specifies the interrupt source to check. 
  *         This parameter can be one of the following values:
  *         @arg I2C_IT_TXFE: Tx FIFO empty
  *         @arg I2C_IT_TXFNE: TX FIFO nearly empty
  *         @arg I2C_IT_TXFF: Tx FIFO full
  *         @arg I2C_IT_TXFOVR: Tx FIFO overflow
  *         @arg I2C_IT_RXFE: Rx FIFO empty
  *         @arg I2C_IT_RXFNF: Rx FIFO nearly full
  *         @arg I2C_IT_RXFF: Rx FIFO full
  *         @arg I2C_IT_LBR: Length number of bytes received
  *         @arg I2C_IT_RFSR: Read from slave request
  *         @arg I2C_IT_RFSE: Read from slave empty
  *         @arg I2C_IT_WTSR: Write to slave request
  *         @arg I2C_IT_MTD: Master transaction done
  *         @arg I2C_IT_STD: Slave transaction done
  *         @arg I2C_IT_SAL: Slave arbitration lost
  *         @arg I2C_IT_MAL: Master arbitration lost
  *         @arg I2C_IT_BERR: Bus error
  *         @arg I2C_IT_MTDWS: Master transaction done without stop
  *         @arg I2C_IT_TIMEOUT: Timeout or Tlow error
  * @retval  ITStatus: functional state @ref ITStatus
  *         This parameter can be: SET or RESET.
  */
ITStatus I2C_GetITStatus(I2C_Type* I2Cx, uint32_t I2C_IT)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_IT(I2C_IT));

  /* Check the status of the specified I2C flag */
  if (READ_BIT(I2Cx->RISR, I2C_IT) != (uint32_t)RESET) {
    /* I2C_IT is set */
    return SET;
  }
  else {
    /* I2C_IT is reset */
    return RESET;
  }
}

/**
  * @brief  Checks whether the specified I2C interrupt has occurred or not.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
  * @param  I2C_IT: specifies the interrupt source to check. 
  *         This parameter can be one of the following values:
  *         @arg I2C_IT_TXFE: Tx FIFO empty
  *         @arg I2C_IT_TXFNE: TX FIFO nearly empty
  *         @arg I2C_IT_TXFF: Tx FIFO full
  *         @arg I2C_IT_TXFOVR: Tx FIFO overflow
  *         @arg I2C_IT_RXFE: Rx FIFO empty
  *         @arg I2C_IT_RXFNF: Rx FIFO nearly full
  *         @arg I2C_IT_RXFF: Rx FIFO full
  *         @arg I2C_IT_LBR: Length number of bytes received
  *         @arg I2C_IT_RFSR: Read from slave request
  *         @arg I2C_IT_RFSE: Read from slave empty
  *         @arg I2C_IT_WTSR: Write to slave request
  *         @arg I2C_IT_MTD: Master transaction done
  *         @arg I2C_IT_STD: Slave transaction done
  *         @arg I2C_IT_SAL: Slave arbitration lost
  *         @arg I2C_IT_MAL: Master arbitration lost
  *         @arg I2C_IT_BERR: Bus error
  *         @arg I2C_IT_MTDWS: Master transaction done without stop
  *         @arg I2C_IT_TIMEOUT: Timeout or Tlow error
  * @retval  ITStatus: functional state @ref ITStatus
  *         This parameter can be: SET or RESET.
  */
ITStatus I2C_GetITPendingBit(I2C_Type* I2Cx, uint32_t I2C_IT)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_IT(I2C_IT));

  /* Check the status of the specified I2C flag */
  if (READ_BIT(I2Cx->MISR, I2C_IT) != (uint32_t)RESET) {
    /* I2C_IT is set */
    return SET;
  }
  else {
    /* I2C_IT is reset */
    return RESET;
  }
}


/**
  * @brief  Clears the I2Cx interrupt pending bits.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
  * @param  I2C_IT: specifies the interrupt pending bit to clear. 
  *         This parameter can be any combination of the following values:
  *         @arg I2C_IT_TXFE: Tx FIFO empty
  *         @arg I2C_IT_TXFNE: TX FIFO nearly empty
  *         @arg I2C_IT_TXFF: Tx FIFO full
  *         @arg I2C_IT_TXFOVR: Tx FIFO overflow
  *         @arg I2C_IT_RXFE: Rx FIFO empty
  *         @arg I2C_IT_RXFNF: Rx FIFO nearly full
  *         @arg I2C_IT_RXFF: Rx FIFO full
  *         @arg I2C_IT_LBR: Length number of bytes received
  *         @arg I2C_IT_RFSR: Read from slave request
  *         @arg I2C_IT_RFSE: Read from slave empty
  *         @arg I2C_IT_WTSR: Write to slave request
  *         @arg I2C_IT_MTD: Master transaction done
  *         @arg I2C_IT_STD: Slave transaction done
  *         @arg I2C_IT_SAL: Slave arbitration lost
  *         @arg I2C_IT_MAL: Master arbitration lost
  *         @arg I2C_IT_BERR: Bus error
  *         @arg I2C_IT_MTDWS: Master transaction done without stop
  *         @arg I2C_IT_TIMEOUT: Timeout or Tlow error
  * @retval None
  */
void I2C_ClearITPendingBit(I2C_Type* I2Cx, uint32_t I2C_IT)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_IT(I2C_IT));
  /* Clear the selected I2C flags */
  WRITE_REG(I2Cx->ICR, I2C_IT);
}


/**
  * @brief  Get the present state of the operation executed by I2C block.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
  * @retval I2C_OpStatus: the I2C operation status @ref I2C_OpStatus
  */
I2C_OpStatus I2C_GetStatus(I2C_Type* I2Cx)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  /* Read the OP status field of the status register */
  return (I2C_OpStatus)I2Cx->SR_b.STATUS;
}



/**
  * @brief  Enables or disables the I2C DMA interface.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
  * @param  I2C_DMAReq: specifies the DMA request.
  *   This parameter can be any combination of the following values:
  *     @arg I2C_DMAReq_Tx: I2C DMA transmit request
  *     @arg I2C_DMAReq_Rx: I2C DMA receive request.
  * @param  NewState: functional state @ref FunctionalState
  *   This parameter can be: ENABLE or DISABLE.   
  * @retval None
  */
void I2C_DMACmd(I2C_Type* I2Cx, uint16_t I2C_DMAReq, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_DMAREQ(I2C_DMAReq));  
  assert_param(IS_FUNCTIONAL_STATE(NewState)); 

  if (NewState != DISABLE)
  {
    /* Enable the DMA transfer */
    I2Cx->CR |= I2C_DMAReq;
  }
  else
  {
    /* Disable the DMA transfer */
    I2Cx->CR &= ~I2C_DMAReq;
  }
}

/**
  * @brief  Enables or disables the I2C TX DMA burst mode.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
  * @param  NewState: functional state @ref FunctionalState
  *   This parameter can be: ENABLE or DISABLE.   
  * @retval None
  */
void I2C_DMATxBurstMode(I2C_Type* I2Cx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState)); 

  if (NewState != DISABLE)
  {
    /* Enable the TX DMA burst mode */
    I2Cx->DMAR_b.BURST_TX = SET;
  }
  else
  {
    /* Disable the TX DMA burst mode */
    I2Cx->DMAR_b.BURST_TX = RESET;
  }
}

/**
  * @brief  Set the number of burst of data.
  *         If this number is smaller than the transaction length, only one single request are generated.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral @ref I2C_Type
  * @param  I2C_BurstSize: specifies the number of burst request.
  * @retval None
  */
void I2C_DMATxBurstSize(I2C_Type* I2Cx, uint8_t I2C_BurstSize)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_BURSTSIZE(I2C_BurstSize));  

  I2Cx->DMAR_b.DBSIZE_TX = I2C_BurstSize;
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

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
