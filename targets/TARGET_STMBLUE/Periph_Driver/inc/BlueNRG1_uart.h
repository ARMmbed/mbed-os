/**
  ******************************************************************************
  * @file    BlueNRG1_uart.h
  * @author  VMA Application Team
  * @version V2.1.0
  * @date    21-March-2016
  * @brief   This file contains all the functions prototypes for the UART 
  *          firmware library.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BLUENRG1_UART_H
#define BLUENRG1_UART_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "BlueNRG_x_device.h"

/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
  * @{
  */

/** @addtogroup UART_Peripheral  UART Peripheral
  * @{
  */ 

/** @defgroup UART_Exported_Types Exported Types
  * @{
  */ 

/** 
  * @brief Structure definition of UART initialization
  */ 
typedef struct
{
  uint32_t UART_BaudRate;             /*!< This member configures the UART communication baud rate. */

  uint8_t UART_WordLengthTransmit;    /*!< Specifies the number of data bits transmitted in a frame.
                                           This parameter can be a value of @ref UART_Word_Length */
																					 
  uint8_t UART_WordLengthReceive;     /*!< Specifies the number of data bits received in a frame.
                                           This parameter can be a value of @ref UART_Word_Length */

  uint8_t UART_StopBits;              /*!< Specifies the number of stop bits transmitted.
                                           This parameter can be a value of @ref UART_Stop_Bits */

  uint8_t UART_Parity;                /*!< Specifies the parity mode.
                                           This parameter can be a value of @ref UART_Parity */
 
  uint8_t UART_Mode;                  /*!< Specifies whether the Receive or Transmit mode is enabled or disabled.
                                           This parameter can be a value of @ref UART_Mode */

  uint8_t UART_HardwareFlowControl;   /*!< Specifies whether the hardware flow control mode is enabled
                                           or disabled.
                                           This parameter can be a value of @ref UART_Hardware_Flow_Control */
                                           
  FunctionalState UART_FifoEnable;    /*!< Specifies whether the FIFO is enabled or disabled.
                                           This parameter can be a value of @ref UART_Fifo_Enable */
} UART_InitType;


/**
  * @}
  */ 

/** @defgroup UART_Exported_Constants Exported Constants
  * @{
  */

/** @defgroup UART_Word_Length_Definition Word Length Definition
  * @{
  */ 

#define UART_WordLength_5b                  ((uint8_t)0)  /*!< 5-bit word length */
#define UART_WordLength_6b                  ((uint8_t)1)  /*!< 6-bit word length */
#define UART_WordLength_7b                  ((uint8_t)2)  /*!< 7-bit word length */
#define UART_WordLength_8b                  ((uint8_t)3)  /*!< 8-bit word length */

/* This macro checks if LENGTH is a valid UART word length value */
#define IS_UART_WORD_LENGTH(LENGTH) (((LENGTH) == UART_WordLength_5b) || \
                                     ((LENGTH) == UART_WordLength_6b) || \
                                     ((LENGTH) == UART_WordLength_7b) || \
                                     ((LENGTH) == UART_WordLength_8b))
/**
  * @}
  */ 

/** @defgroup UART_Stop_Bits_Definition Stop Bits Definition
  * @{
  */ 
  
#define UART_StopBits_1                     ((uint8_t)0)  /*!< 1 stop bit */
#define UART_StopBits_2                     ((uint8_t)1)  /*!< 2 stop bits */

/* This macro checks if STOPBITS is a valid UART stop bits value */
#define IS_UART_STOPBITS(STOPBITS) (((STOPBITS) == UART_StopBits_1) || \
                                     ((STOPBITS) == UART_StopBits_2))
/**
  * @}
  */ 

/** @defgroup UART_Parity_Definition Parity Definition
  * @{
  */ 
  
#define UART_Parity_No                      ((uint8_t)0)  /*!< No parity */
#define UART_Parity_Even                    ((uint8_t)1)  /*!< Even parity */
#define UART_Parity_Odd                     ((uint8_t)2)  /*!< Odd parity */

/* This macro checks if PARITY is a valid UART parity value */
#define IS_UART_PARITY(PARITY) (((PARITY) == UART_Parity_No) || \
                                 ((PARITY) == UART_Parity_Even) || \
                                 ((PARITY) == UART_Parity_Odd))
/**
  * @}
  */ 

/** @defgroup UART_Mode_Definition Mode Definition
  * @{
  */ 
  
#define UART_Mode_Rx                        ((uint8_t)0x01)  /*!< Enables Rx */
#define UART_Mode_Tx                        ((uint8_t)0x02)  /*!< Enables Tx */

/* This macro checks if MODE is a valid UART mode value */
#define IS_UART_MODE(MODE) ( (MODE)<= 3)

/**
  * @}
  */ 

/** @defgroup UART_Hardware_Flow_Control_Definition Hardware Flow Control Definition
  * @{
  */ 
#define UART_HardwareFlowControl_None       ((uint8_t)0x00)  /*!< Disable the hardware flow control */
#define UART_HardwareFlowControl_RTS        ((uint8_t)0x01)  /*!< Enables the RTS hardware flow control */
#define UART_HardwareFlowControl_CTS        ((uint8_t)0x02)  /*!< Enables the CTS hardware flow control */
#define UART_HardwareFlowControl_RTS_CTS    ((uint8_t)0x03)  /*!< Enables the RTS and CTS hardware flow control */

/* This macro checks if CONTROL is a valid UART hardware flow control value */
#define IS_UART_HW_FLOW_CONTROL(CONTROL) ( (CONTROL) <= 3 )

/**
  * @}
  */ 


/** @defgroup UART_Interrupt_Definition Interrupt Definition
  * @{
  */

#define UART_IT_TXFE                ((uint16_t)0x1000)  /*!< Tx FIFO empty interrupt */
#define UART_IT_XO                  ((uint16_t)0x0800)  /*!< XOFF interrupt */
#define UART_IT_OE                  ((uint16_t)0x0400)  /*!< Overrun error interrupt */
#define UART_IT_BE                  ((uint16_t)0x0200)  /*!< Break error interrupt */
#define UART_IT_PE                  ((uint16_t)0x0100)  /*!< Parity error interrupt */
#define UART_IT_FE                  ((uint16_t)0x0080)  /*!< Framing error interrupt */
#define UART_IT_RT                  ((uint16_t)0x0040)  /*!< Receive error interrupt */
#define UART_IT_TX                  ((uint16_t)0x0020)  /*!< Transmit interrupt */
#define UART_IT_RX                  ((uint16_t)0x0010)  /*!< Receive interrupt */
#define UART_IT_CTS                 ((uint16_t)0x0002)  /*!< CTS interrupt */

/* This macro checks if IT is a valid combination of UART interrupt value */
#define IS_UART_CONFIG_IT(IT) ((((IT) & (uint16_t)~0x1FF2) == 0x00) && ((IT) != (uint16_t)0x0000))



/**
  * @}
  */


/** @defgroup UART_Flags_Definition Flags Definition
  * @{
  */

#define UART_FLAG_CTS                       ((uint32_t)0x00000001)  /*!< Clear to send flag */
#define UART_FLAG_BUSY                      ((uint32_t)0x00000008)  /*!< Busy flag */
#define UART_FLAG_RXFE                      ((uint32_t)0x00000010)  /*!< Receive FIFO empty flag */
#define UART_FLAG_TXFF                      ((uint32_t)0x00000020)  /*!< Transmit FIFO full flag */
#define UART_FLAG_RXFF                      ((uint32_t)0x00000040)  /*!< Receive FIFO full flag */
#define UART_FLAG_TXFE                      ((uint32_t)0x00000080)  /*!< Transmit FIFO empty flag */

#define UART_FLAG_FE                        ((uint32_t)0x80000001)  /*!< Framing error flag */
#define UART_FLAG_PE                        ((uint32_t)0x80000002)  /*!< Parity error flag */
#define UART_FLAG_BE                        ((uint32_t)0x80000004)  /*!< Break error flag */
#define UART_FLAG_OE                        ((uint32_t)0x80000008)  /*!< Overrun error flag */

/* This macro checks if FLAG is a valid UART flag */
#define IS_UART_FLAG(FLAG) (((FLAG) == UART_FLAG_CTS) || ((FLAG) == UART_FLAG_BUSY) || \
                             ((FLAG) == UART_FLAG_RXFE) || ((FLAG) == UART_FLAG_TXFF) || \
                             ((FLAG) == UART_FLAG_RXFF) || ((FLAG) == UART_FLAG_TXFE) || \
                             ((FLAG) == UART_FLAG_OE) || ((FLAG) == UART_FLAG_FE) || \
                             ((FLAG) == UART_FLAG_PE) || ((FLAG) == UART_FLAG_BE) )

/* This macro checks if FLAG is a valid UART clearable flag */
#define IS_UART_CLEAR_FLAG(FLAG) (((FLAG) == UART_FLAG_FE) || ((FLAG) == UART_FLAG_PE) || \
                             ((FLAG) == UART_FLAG_BE) || ((FLAG) == UART_FLAG_OE)) 

/* This macro checks if BAUDRATE is a valid UART baudrate */
#define IS_UART_BAUDRATE(BAUDRATE) (((BAUDRATE) > 0) && ((BAUDRATE) <= 1000000))

/* This macro checks if DATA is a valid UART data */
#define IS_UART_DATA(DATA) ((DATA) <= 0x1FF)

/**
  * @}
  */

/** @defgroup UART_Fifo_Level_Definition FIFO Level Definition
  * @{
  */
  
#define FIFO_LEV_1_64       ((uint8_t)0)       /*!< FIFO threshold 1/64 */
#define FIFO_LEV_1_32       ((uint8_t)1)       /*!< FIFO threshold 1/32 */
#define FIFO_LEV_1_16       ((uint8_t)2)       /*!< FIFO threshold 1/16 */
#define FIFO_LEV_1_8        ((uint8_t)3)       /*!< FIFO threshold 1/8 */
#define FIFO_LEV_1_4        ((uint8_t)4)       /*!< FIFO threshold 1/4 */
#define FIFO_LEV_1_2        ((uint8_t)5)       /*!< FIFO threshold 1/2 */
#define FIFO_LEV_3_4        ((uint8_t)6)       /*!< FIFO threshold 3/4 */

/* This macro checks if TX_FIFO_LEV is a valid Tx FIFO level */
#define IS_FIFO_LEV(FIFO_LEV) ( ((FIFO_LEV) == FIFO_LEV_1_64) || \
                                ((FIFO_LEV) == FIFO_LEV_1_32) || \
                                ((FIFO_LEV) == FIFO_LEV_1_16) || \
                                ((FIFO_LEV) == FIFO_LEV_1_8) || \
                                ((FIFO_LEV) == FIFO_LEV_1_4) || \
                                ((FIFO_LEV) == FIFO_LEV_1_2) || \
                                ((FIFO_LEV) == FIFO_LEV_3_4))

/**
  * @}
  */

/** @defgroup UART_Software_Flow_Control_Mode_Definition Software Flow Control Mode Definition
  * @{
  */
#define NO_SW_FLOW_CTRL                       ((uint8_t)0)       /*!< No software flow control */
#define SW_FLOW_CTRL_XON1_XOFF1               ((uint8_t)1)       /*!< Software flow control XON1 and XOFF1 */
#define SW_FLOW_CTRL_XON2_XOFF2               ((uint8_t)2)       /*!< Software flow control XON2 and XOFF2 */
#define SW_FLOW_CTRL_ALL_CHARS                ((uint8_t)3)       /*!< Software flow control XON1 and XOFF1, XON2 and XOFF2 */

/* This macro checks if SW_FLOW_CTRL is a valid software flow control value */
#define IS_SW_FLOW_CTRL(SW_FLOW_CTRL) ( ((SW_FLOW_CTRL) == NO_SW_FLOW_CTRL) || \
                                ((SW_FLOW_CTRL) == SW_FLOW_CTRL_XON1_XOFF1) || \
                                ((SW_FLOW_CTRL) == SW_FLOW_CTRL_XON2_XOFF2) || \
                                ((SW_FLOW_CTRL) == SW_FLOW_CTRL_ALL_CHARS))
/**
  * @}
  */

/** @defgroup UART_DMA_Requests_Definition DMA Requests Definition
  * @{
  */

#define UART_DMAReq_Tx                      ((uint8_t)0x02)
#define UART_DMAReq_Rx                      ((uint8_t)0x01)

/* This macro checks if DMAREQ is a valid DMA request value */
#define IS_UART_DMAREQ(DMAREQ) ((DMAREQ)<(uint8_t)4 && (DMAREQ)!=(uint8_t)0 )

/**
  * @}
  */ 

/** @defgroup UART_Timeout_Definition DMA Timeout
  * @{
  */

/* This macro checks if VAL is a valid timeout value */
#define IS_UART_TIMEOUT(VAL) ((VAL) < 0x400000 )

/**
  * @}
  */ 

/**
  * @}
  */ 

/** @defgroup UART_Exported_Macros Exported Macros
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup UART_Exported_Functions Exported Functions
  * @{
  */

void UART_DeInit(void);
void UART_Init(UART_InitType* UART_InitStruct);
void UART_StructInit(UART_InitType* UART_InitStruct);
void UART_Cmd(FunctionalState NewState);
/////////////ANTONIO///////////////
uint8_t UART_IsEnabled(void);
/////////////ANTONIO///////////////
void UART_ITConfig(uint16_t UART_IT, FunctionalState NewState);
void UART_SendData(uint16_t Data);
uint16_t UART_ReceiveData(void);
void UART_BreakCmd(FunctionalState NewState);
void UART_RequestToSendCmd(FunctionalState NewState);
FlagStatus UART_GetFlagStatus(uint32_t UART_FLAG);
void UART_ClearFlag(uint32_t UART_FLAG);
ITStatus UART_GetITStatus(uint16_t UART_IT);
void UART_ClearITPendingBit(uint16_t UART_IT);
void UART_TxFifoIrqLevelConfig(uint8_t UART_TxFifo_Level);
void UART_RxFifoIrqLevelConfig(uint8_t UART_RxFifo_Level);
void UART_RXTimeoutConfig(uint32_t UART_TimeoutMS);
void UART_Oversampling(FunctionalState NewState);
void UART_SwFlowControl(FunctionalState NewState);
void UART_RxSwFlowControlMode(uint8_t UART_RxSwFlowCtrlMode);
void UART_TxSwFlowControlMode(uint8_t UART_TxSwFlowCtrlMode);
void UART_XonAnyBit(FunctionalState NewState);
void UART_SpecialCharDetect(FunctionalState NewState);
void UART_Xon1Char(uint8_t UART_XonChar);
void UART_Xon2Char(uint8_t UART_XonChar);
void UART_Xoff1Char(uint8_t UART_XoffChar);
void UART_Xoff2Char(uint8_t UART_XoffChar);

void UART_DMACmd(uint8_t UART_DMAReq, FunctionalState NewState);

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif

#endif /* BLUENRG1_UART_H */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
