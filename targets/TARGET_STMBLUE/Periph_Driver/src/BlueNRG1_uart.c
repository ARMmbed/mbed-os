/**
  ******************************************************************************
  * @file    BlueNRG1_uart.c
  * @author  VMA Application Team
  * @version V2.1.0
  * @date    21-March-2016
  * @brief   This file provides all the UART firmware functions.
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
#include "../../Periph_Driver/inc/BlueNRG1_uart.h"

/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
  * @{
  */

/** @addtogroup UART_Peripheral  UART Peripheral
  * @{
  */

/** @defgroup UART_Private_TypesDefinitions Private Types Definitions
  * @{
  */

/**
  * @}
  */

/** @defgroup UART_Private_Defines Private Defines
  * @{
  */
#define UART_CLOCK       (16000000)

/* UART clock cycle for oversampling factor */
#define UART_CLOCK_CYCLE16        (16)      
#define UART_CLOCK_CYCLE8         (8)

/* Max UART RX timeout value */
#define UART_MAX_RX_TIMEOUT       (0x3FFFFF)

/**
  * @}
  */

/** @defgroup UART_Private_Macros Private Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup UART_Private_Variables Private Variables
  * @{
  */

/**
  * @}
  */

/** @defgroup UART_Private_FunctionPrototypes Private Function Prototypes
  * @{
  */

/**
  * @}
  */

/** @defgroup UART_Public_Functions Public Functions
  * @{
  */


/**
  * @brief  Enables or disables the UART peripheral.
  * @param  NewState: functional state @ref FunctionalState
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_Cmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE) {
    /* Enable the selected UART by setting the UARTEN bit in the UARTCR register */
    UART->CR_b.EN = SET;
  }
  else {
    /* Disable the selected UART by clearing the UARTEN bit in the UARTCR register */
    UART->CR_b.EN = RESET;
  }
}

/////////////ANTONIO///////////////
uint8_t UART_IsEnabled(void){
	return (uint8_t)UART->CR_b.EN;
}
/////////////ANTONIO///////////////


/**
* @brief  Deinitializes the UART peripheral registers to their default
*         reset values.
* @param  None
* @retval None
*/
void UART_DeInit(void)
{
  UART->TIMEOUT   = 0x000001FF;
  UART->LCRH_RX   = 0x00000000;
  UART->IBRD      = 0x00000000;
  UART->FBRD      = 0x00000000;
  UART->LCRH_TX   = 0x00000000;
  UART->CR        = 0x00040300;
  UART->IFLS      = 0x00000012;
  UART->IMSC      = 0x00000000;
  UART->ICR       = 0x00000000;
  UART->DMACR     = 0x00000000;
  UART->XFCR      = 0x00000000;
  UART->XON1      = 0x00000000;
  UART->XON2      = 0x00000000;
  UART->XOFF1     = 0x00000000;
  UART->XOFF2     = 0x00000000;
}


/**
  * @brief  Initializes the UART peripheral according to the specified
  *         parameters in the UART_InitStruct.
  * @param  UART_InitStruct: pointer to a @ref UART_InitType structure
  *         that contains the configuration information for the specified UART.
  * @retval None
  */
void UART_Init(UART_InitType* UART_InitStruct)
{
  uint32_t divider;
  uint16_t ibrd, fbrd;

  /* Check the parameters */
  assert_param(IS_UART_BAUDRATE(UART_InitStruct->UART_BaudRate));  
  assert_param(IS_UART_WORD_LENGTH(UART_InitStruct->UART_WordLengthTransmit));
  assert_param(IS_UART_WORD_LENGTH(UART_InitStruct->UART_WordLengthReceive));
  assert_param(IS_UART_STOPBITS(UART_InitStruct->UART_StopBits));
  assert_param(IS_UART_PARITY(UART_InitStruct->UART_Parity));
  assert_param(IS_UART_MODE(UART_InitStruct->UART_Mode));
  assert_param(IS_UART_HW_FLOW_CONTROL(UART_InitStruct->UART_HardwareFlowControl));
  assert_param(IS_FUNCTIONAL_STATE(UART_InitStruct->UART_FifoEnable));
  
  /*---------------------------- UART BaudRate Configuration -----------------------*/
  
  if (UART->CR_b.OVSFACT == 0)
    divider = (UART_CLOCK<<7) / (UART_CLOCK_CYCLE16 * UART_InitStruct->UART_BaudRate);
  else
    divider = (UART_CLOCK<<7) / (UART_CLOCK_CYCLE8 * UART_InitStruct->UART_BaudRate);
  
  ibrd = divider >> 7;
  UART->IBRD = ibrd;
  fbrd = ((divider - (ibrd <<7) + 1) >> 1);
  if (fbrd > 0x3f) {
    ibrd++;
    fbrd = (fbrd - 0x3F) & 0x3F;
  }
  UART->FBRD = fbrd;
  
  /*---------------------------- UART Word Length Configuration -----------------------*/
  UART->LCRH_TX_b.WLEN_TX = UART_InitStruct->UART_WordLengthTransmit;
  UART->LCRH_RX_b.WLEN_RX = UART_InitStruct->UART_WordLengthReceive;
  
  /*---------------------------- UART Stop Bits Configuration -----------------------*/
  UART->LCRH_TX_b.STP2_TX = UART_InitStruct->UART_StopBits;
  UART->LCRH_RX_b.STP2_RX = UART_InitStruct->UART_StopBits;
  
  /*---------------------------- UART Parity Configuration -----------------------*/
  if(UART_InitStruct->UART_Parity == UART_Parity_No) {
    UART->LCRH_TX_b.PEN_TX = RESET;
    UART->LCRH_RX_b.PEN_RX = RESET;
  }
  else if(UART_InitStruct->UART_Parity == UART_Parity_Odd) {
    UART->LCRH_TX_b.PEN_TX = SET;
    UART->LCRH_TX_b.EPS_TX = RESET;
    UART->LCRH_RX_b.PEN_RX = SET;
    UART->LCRH_RX_b.EPS_RX = RESET;
  }
  else {
    UART->LCRH_TX_b.PEN_TX = SET;
    UART->LCRH_TX_b.EPS_TX = SET;    
    UART->LCRH_RX_b.PEN_RX = SET;
    UART->LCRH_RX_b.EPS_RX = SET;    
  }

  /*---------------------------- UART Mode Configuration -----------------------*/
  UART->CR_b.RXE = UART_InitStruct->UART_Mode&1;
  UART->CR_b.TXE = (UART_InitStruct->UART_Mode>>1)&1;

  /*---------------------------- UART Hardware flow control Configuration -----------------------*/
  UART->CR_b.RTSEN = UART_InitStruct->UART_HardwareFlowControl&1;
  UART->CR_b.CTSEN = (UART_InitStruct->UART_HardwareFlowControl>>1)&1;

  /*---------------------------- UART Fifo Configuration -----------------------*/
  UART->LCRH_TX_b.FEN_TX = (uint8_t)UART_InitStruct->UART_FifoEnable;
  UART->LCRH_RX_b.FEN_RX = (uint8_t)UART_InitStruct->UART_FifoEnable;

}

/**
  * @brief  Fills each UART_InitStruct member with its default value.
  * @param  UART_InitStruct: pointer to a @ref UART_InitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void UART_StructInit(UART_InitType* UART_InitStruct)
{
  /* UART_InitStruct members default value */
  UART_InitStruct->UART_BaudRate = 115200;
  UART_InitStruct->UART_WordLengthTransmit = UART_WordLength_8b;
  UART_InitStruct->UART_WordLengthReceive = UART_WordLength_8b;
  UART_InitStruct->UART_StopBits = UART_StopBits_1;
  UART_InitStruct->UART_Parity = UART_Parity_No;
  UART_InitStruct->UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
  UART_InitStruct->UART_HardwareFlowControl = UART_HardwareFlowControl_None; 
  UART_InitStruct->UART_FifoEnable = DISABLE;
}

/**
  * @brief  Enables or disables the specified UART interrupts.
  * @param  UART_IT: specifies the UART interrupt sources to be enabled or disabled.
  *   This parameter can be a any combination of the following values:
	*   @arg UART_IT_TXFE:  Tx FIFO empty interrupt
  *   @arg UART_IT_XO:    XOFF interrupt
  *   @arg UART_IT_OE:    Overrun error interrupt
  *   @arg UART_IT_BE:    Break error interrupt
  *   @arg UART_IT_PE:    Parity error interrupt
  *   @arg UART_IT_FE:    Framing error interrupt
  *   @arg UART_IT_RT:    Receive timeout interrupt
  *   @arg UART_IT_TX:    Transmit interrupt
  *   @arg UART_IT_RX:    Receive interrupt
  *   @arg UART_IT_CTS:   CTS interrupt.
  * @param  NewState: functional state @ref FunctionalState
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_ITConfig(uint16_t UART_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_UART_CONFIG_IT(UART_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE) {
    /* Enable specified interrupts */
    SET_BIT(UART->IMSC, UART_IT);
  }
  else {
    /* Disable specified interrupts */
    CLEAR_BIT(UART->IMSC, UART_IT);
  }
}

/**
  * @brief  Transmits single data through the UART peripheral.
  * @param  Data: the data to transmit.
  * @retval None
  */
void UART_SendData(uint16_t Data)
{
  /* Check the parameters */
  assert_param(IS_UART_DATA(Data)); 
    
  /* Transmit Data */
  UART->DR = (Data & (uint16_t)0x01FF);
}

/**
  * @brief  Returns the most recent received data by the UART peripheral.
  * @param  None
  * @retval uint16_t: The received data.
  */
uint16_t UART_ReceiveData(void)
{
  /* Check the parameters */
  
  /* Receive Data */
  return (uint16_t)(UART->DR & 0x00FF);
}

/**
  * @brief  Enables or disables the break command.
  * @param  NewState: functional state @ref FunctionalState
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_BreakCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  UART->LCRH_TX_b.BRK = (uint8_t)NewState;
}

/**
  * @brief  Enables or disables the request to send command.
  * @param  NewState: functional state @ref FunctionalState
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_RequestToSendCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  UART->CR_b.RTS = (uint8_t)NewState;
}


/**
  * @brief  Checks whether the specified UART flag is set or not.
  * @param  UART_FLAG: specifies the flag to check.
  *   This parameter can be one of the following values:
  *     @arg UART_FLAG_CTS:   Clear to send
  *     @arg UART_FLAG_BUSY:  UART busy
  *     @arg UART_FLAG_RXFE:  Receive FIFO empty
  *     @arg UART_FLAG_TXFF:  Transmit FIFO full
  *     @arg UART_FLAG_RXFF:  Receive FIFO full
  *     @arg UART_FLAG_TXFE:  Transmit FIFO empty
  *     @arg UART_FLAG_FE:    Framing error
  *     @arg UART_FLAG_PE:    Parity error
  *     @arg UART_FLAG_BE:    Break error
  *     @arg UART_FLAG_OE:    Overrun error.
* @retval FlagStatus: functional state @ref FlagStatus
*         This parameter can be: SET or RESET.
  */
FlagStatus UART_GetFlagStatus(uint32_t UART_FLAG)
{
  /* Check the parameters */
  assert_param(IS_UART_FLAG(UART_FLAG));
  
  if (UART_FLAG & 0x80000000) {
    if (UART->RSR & UART_FLAG) {
      return SET;
    }
    else {
      return RESET;
    }
  }
  else {
    if (UART->FR & UART_FLAG) {
      return SET;
    }
    else {
      return RESET;
    }  
  }
  
}

/**
  * @brief  Clears the UART pending flags.
  * @param  UART_FLAG: specifies the flag to clear.
  *   This parameter can be any combination of the following values:
  *     @arg UART_FLAG_FE:  Framing error
  *     @arg UART_FLAG_PE:  Parity error
  *     @arg UART_FLAG_BE:  Break error
  *     @arg UART_FLAG_OE:  Overrun error
  * @retval None
  */
void UART_ClearFlag(uint32_t UART_FLAG)
{
  /* Check the parameters */
  assert_param(IS_UART_CLEAR_FLAG(UART_FLAG));
   
  UART->ECR = UART_FLAG;
}

/**
  * @brief  Checks whether the specified UART interrupt has occurred or not.
  * @param  UART_IT: specifies the UART interrupt source to check.
  *   This parameter can be one of the following values:
	*   @arg UART_IT_TXFE:  Tx FIFO empty interrupt
  *   @arg UART_IT_XO:    XOFF interrupt
  *   @arg UART_IT_OE:    Overrun error interrupt
  *   @arg UART_IT_BE:    Break error interrupt
  *   @arg UART_IT_PE:    Parity error interrupt
  *   @arg UART_IT_FE:    Framing error interrupt
  *   @arg UART_IT_RT:    Receive timeout interrupt
  *   @arg UART_IT_TX:    Transmit interrupt
  *   @arg UART_IT_RX:    Receive interrupt
  *   @arg UART_IT_CTS:   CTS interrupt
  * @retval  ITStatus: functional state @ref ITStatus
  *         This parameter can be: SET or RESET.
  */
ITStatus UART_GetITStatus(uint16_t UART_IT)
{
  /* Check the parameters */
  assert_param(IS_UART_CONFIG_IT(UART_IT));
   
  if ((UART->RIS & UART_IT) != (uint16_t)RESET) {
    return SET;
  }
  else {
    return RESET;
  }  
}

/**
  * @brief  Clears the UART interrupt pending bits.
  * @param  UART_IT: specifies the interrupt pending bit to clear.
  *   This parameter can be any combination of the following values:
	*   @arg UART_IT_TXFE:  Tx FIFO empty interrupt
  *   @arg UART_IT_XO:    XOFF interrupt
  *   @arg UART_IT_OE:    Overrun error interrupt
  *   @arg UART_IT_BE:    Break error interrupt
  *   @arg UART_IT_PE:    Parity error interrupt
  *   @arg UART_IT_FE:    Framing error interrupt
  *   @arg UART_IT_RT:    Receive timeout interrupt
  *   @arg UART_IT_TX:    Transmit interrupt
  *   @arg UART_IT_RX:    Receive interrupt
  *   @arg UART_IT_CTS:   CTS interrupt
  *   
  * @retval None
  */
void UART_ClearITPendingBit(uint16_t UART_IT)
{
  /* Check the parameters */
  assert_param(IS_UART_CONFIG_IT(UART_IT));
  
  UART->ICR = UART_IT;
}


/**
  * @brief  Sets the UART interrupt FIFO level.
  * @param  UART_TxFifo_Level: specifies the transmit interrupt FIFO level.
  *   This parameter can be one of the following values:
  *   @arg FIFO_LEV_1_64: interrupt when Tx FIFO becomes <= 1/64 full = 1
  *   @arg FIFO_LEV_1_32: interrupt when Tx FIFO becomes <= 1/32 full = 2
  *   @arg FIFO_LEV_1_16: interrupt when Tx FIFO becomes <= 1/16 full = 4
  *   @arg FIFO_LEV_1_8: interrupt when Tx FIFO becomes <= 1/8 full = 8
  *   @arg FIFO_LEV_1_4: interrupt when Tx FIFO becomes <= 1/4 full = 16
  *   @arg FIFO_LEV_1_2: interrupt when Tx FIFO becomes <= 1/2 full = 32
  *   @arg FIFO_LEV_3_4: interrupt when Tx FIFO becomes <= 3/4 full  = 48
  * @retval None
  */
void UART_TxFifoIrqLevelConfig(uint8_t UART_TxFifo_Level)
{
    /* Check the parameters */
  assert_param(IS_FIFO_LEV(UART_TxFifo_Level));

  UART->IFLS_b.TXIFLSEL = UART_TxFifo_Level;
}

/**
  * @brief  Sets the UART interrupt FIFO level.
  * @param  UART_RxFifo_Level: specifies the receive interrupt FIFO level.
  *   This parameter can be one of the following values:
  *   @arg FIFO_LEV_1_64: interrupt when Rx FIFO becomes >= 1/64 full = 1
  *   @arg FIFO_LEV_1_32: interrupt when Rx FIFO becomes >= 1/32 full = 2
  *   @arg FIFO_LEV_1_16: interrupt when Rx FIFO becomes >= 1/16 full = 4
  *   @arg FIFO_LEV_1_8: interrupt when Rx FIFO becomes >= 1/8 full = 8
  *   @arg FIFO_LEV_1_4: interrupt when Rx FIFO becomes >= 1/4 full = 16
  *   @arg FIFO_LEV_1_2: interrupt when Rx FIFO becomes >= 1/2 full = 32
  *   @arg FIFO_LEV_3_4: interrupt when Rx FIFO becomes >= 3/4 full = 48
  * @retval None
  */
void UART_RxFifoIrqLevelConfig(uint8_t UART_RxFifo_Level)
{
    /* Check the parameters */
  assert_param(IS_FIFO_LEV(UART_RxFifo_Level));

  UART->IFLS_b.RXIFLSEL = UART_RxFifo_Level;
}

/**
  * @brief  UART RX timeout.
  *         The RX timeout interrupt is asserted when
  *         the RX FIFO is not empty and no further data is received
  *         over a programmed timeout period.
  * @param  UART_TimeoutMS: is the timeout in milliseconds.
  * @retval None
  */
void UART_RXTimeoutConfig(uint32_t UART_TimeoutMS)
{
  uint32_t timeout_val;
  uint32_t tmp32;

  tmp32 = (UART->IBRD)<<4;
  if (UART->CR_b.OVSFACT == 0)
    timeout_val = (UART_TimeoutMS * 16 * 16000) / (((UART->FBRD)>>2) + tmp32);
  else
    timeout_val = (UART_TimeoutMS *  8 * 16000) / (((UART->FBRD)>>3) + tmp32);

  /* Set the timeout value */
  if(IS_UART_TIMEOUT(timeout_val)) {
    UART->TIMEOUT = timeout_val;
  }
  else {
    UART->TIMEOUT = UART_MAX_RX_TIMEOUT;
  }
}

/**
  * @brief  Enables or disables the UART oversampling.
  * @param  NewState: functional state @ref FunctionalState
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_Oversampling(FunctionalState NewState)
{
  assert_param(IS_FUNCTIONAL_STATE(NewState)); 
  
  if (NewState == ENABLE)
  {
    /* Enable the UART oversampling factor */
    UART->CR_b.OVSFACT = 1;
  }
  else
  {
    /* Disable the UART oversampling factor */
    UART->CR_b.OVSFACT = 0;
  }
}

/**
  * @brief  Enables or disables the UART DMA interface.
  * @param  UART_DMAReq: specifies the DMA request.
  *   This parameter can be any combination of the following values:
  *     @arg UART_DMAReq_Tx: UART DMA transmit request.
  *     @arg UART_DMAReq_Rx: UART DMA receive request.
  * @param  NewState: functional state @ref FunctionalState
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_DMACmd(uint8_t UART_DMAReq, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_UART_DMAREQ(UART_DMAReq));  
  assert_param(IS_FUNCTIONAL_STATE(NewState)); 

  if (NewState == ENABLE)
  {
    /* Enable the DMA transfer for selected requests by setting the DMAT and/or
       DMAR bits in the UART CR3 register */
    UART->DMACR |= UART_DMAReq;
  }
  else
  {
    /* Disable the DMA transfer for selected requests by clearing the DMAT and/or
       DMAR bits in the UART CR3 register */
    UART->DMACR &= (uint8_t)~UART_DMAReq;
  }
}



/**
  * @brief  Enables or disables the UART software flow control.
  * @param  NewState: functional state @ref FunctionalState
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_SwFlowControl(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE) {
    /* Enable UART software flow control */
    UART->XFCR_b.SFEN = SET;
  }
  else {
    /* Disable UART software flow control */
    UART->XFCR_b.SFEN = RESET;
  }
}


/**
  * @brief  Sets the UART flow control mode for reception.
  * @param  UART_RxSwFlowCtrlMode: specifies the software flow control used for RX.
  *   This parameter can be one of the following values:
  *   @arg NO_SW_FLOW_CTRL: No software flow control
  *   @arg SW_FLOW_CTRL_XON1_XOFF1: Software flow control XON1 and XOFF1
  *   @arg SW_FLOW_CTRL_XON2_XOFF2: Software flow control XON2 and XOFF2
  *   @arg SW_FLOW_CTRL_ALL_CHARS: Software flow control XON1 and XOFF1, XON2 and XOFF2
  * @retval None
  */
void UART_RxSwFlowControlMode(uint8_t UART_RxSwFlowCtrlMode)
{
    /* Check the parameters */
  assert_param(IS_SW_FLOW_CTRL(UART_RxSwFlowCtrlMode));

  UART->XFCR_b.SFRMOD = UART_RxSwFlowCtrlMode;
}

/**
  * @brief  Sets the UART flow control mode for transmission.
  * @param  UART_TxSwFlowCtrlMode: specifies the software flow control used for TX.
  *   This parameter can be one of the following values:
  *   @arg NO_SW_FLOW_CTRL: No software flow control
  *   @arg SW_FLOW_CTRL_XON1_XOFF1: Software flow control XON1 and XOFF1
  *   @arg SW_FLOW_CTRL_XON2_XOFF2: Software flow control XON2 and XOFF2
  *   @arg SW_FLOW_CTRL_ALL_CHARS: Software flow control XON1 and XOFF1, XON2 and XOFF2
  * @retval None
  */
void UART_TxSwFlowControlMode(uint8_t UART_TxSwFlowCtrlMode)
{
    /* Check the parameters */
  assert_param(IS_SW_FLOW_CTRL(UART_TxSwFlowCtrlMode));

  UART->XFCR_b.SFTMOD = UART_TxSwFlowCtrlMode;
}


/**
  * @brief  Enables or disables the UART XON any bit.
  * @param  NewState: functional state @ref FunctionalState
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_XonAnyBit(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE) {
    /* Enable the UART XON any bit */
    UART->XFCR_b.XONANY = SET;
  }
  else {
    /* Disable the UART XON any bit */
    UART->XFCR_b.XONANY = RESET;
  }
}


/**
  * @brief  Enables or disables the UART special character detection.
  * @param  NewState: functional state @ref FunctionalState
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_SpecialCharDetect(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE) {
    /* Enable the UART special character detection */
    UART->XFCR_b.SPECHAR = SET;
  }
  else {
    /* Disable the UART special character detection */
    UART->XFCR_b.SPECHAR = RESET;
  }
}


/**
  * @brief  Sets the UART flow control mode for transmission.
  * @param  uint8_t: specifies the flow control character.
  * @retval None
  */
void UART_Xon1Char(uint8_t UART_XonChar)
{
    /* Check the parameters */
  assert_param(IS_SW_FLOW_CTRL(UART_XonChar));

  UART->XON1 = UART_XonChar;
}

/**
  * @brief  Sets the UART flow control mode for transmission.
  * @param  uint8_t: specifies the flow control character.
  * @retval None
  */
void UART_Xon2Char(uint8_t UART_XonChar)
{
    /* Check the parameters */
  assert_param(IS_SW_FLOW_CTRL(UART_XonChar));

  UART->XON2 = UART_XonChar;
}

/**
  * @brief  Sets the UART flow control mode for transmission.
  * @param  uint8_t: specifies the flow control character.
  * @retval None
  */
void UART_Xoff1Char(uint8_t UART_XoffChar)
{
    /* Check the parameters */
  assert_param(IS_SW_FLOW_CTRL(UART_XoffChar));

  UART->XOFF1 = UART_XoffChar;
}

/**
  * @brief  Sets the UART flow control mode for transmission.
  * @param  uint8_t: specifies the flow control character.
  * @retval None
  */
void UART_Xoff2Char(uint8_t UART_XoffChar)
{
    /* Check the parameters */
  assert_param(IS_SW_FLOW_CTRL(UART_XoffChar));

  UART->XOFF2 = UART_XoffChar;
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
