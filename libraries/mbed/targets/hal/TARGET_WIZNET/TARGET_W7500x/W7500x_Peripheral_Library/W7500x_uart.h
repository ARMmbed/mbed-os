/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date    
  * @brief   This file contains all the functions prototypes for the UART 
  *          firmware library.
  ******************************************************************************
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __W7500X_UART_H
#define __W7500X_UART_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "W7500x.h"


/** 
  * @brief  UART Init Structure definition  
  */ 
  
typedef struct
{
    uint32_t UART_BaudRate;
    uint16_t UART_WordLength;
    uint16_t UART_StopBits;
    uint16_t UART_Parity;
    uint16_t UART_Mode;
    uint16_t UART_HardwareFlowControl;
} UART_InitTypeDef;

/**
  * @}
  */


/** @defgroup UART_Exported_Constants 
  * @{
  */

#define IS_UART_01_PERIPH(PERIPH)   (((PERIPH) == UART0) || ((PERIPH) == UART1))
#define IS_UART_2_PERIPH(PERIPH)    ((PERIPH) == UART2)                                    

/**
  * @}
  */



/** @addtogroup UART_Word_Length
  * @{
  */

#define UART_WordLength_8b          ((uint16_t)UART_LCR_H_WLEN(3))
#define UART_WordLength_7b          ((uint16_t)UART_LCR_H_WLEN(2))
#define UART_WordLength_6b          ((uint16_t)UART_LCR_H_WLEN(1))
#define UART_WordLength_5b          ((uint16_t)UART_LCR_H_WLEN(0))
#define IS_UART_WORD_LENGTH(LENGTH) (((LENGTH) == UART_WordLength_8b) || \
                                     ((LENGTH) == UART_WordLength_7b) || \
                                     ((LENGTH) == UART_WordLength_6b) || \
                                     ((LENGTH) == UART_WordLength_5b) )
/**
  * @}
  */


/** @addtogroup UART_Parity
  * @{
  */

#define UART_Parity_No              ((uint16_t)0x0000)
#define UART_Parity_Even            ((uint16_t)(UART_LCR_H_PEN | UART_LCR_H_EPS))
#define UART_Parity_Odd             ((uint16_t)(UART_LCR_H_PEN))
#define IS_UART_PARITY(PARITY)      (((PARITY) == UART_Parity_No)   || \
                                     ((PARITY) == UART_Parity_Even) || \
                                     ((PARITY) == UART_Parity_Odd))

/**
  * @}
  */


/** @addtogroup UART_Stop_Bits
  * @{
  */

#define UART_StopBits_1             ((uint16_t)0x0000)
#define UART_StopBits_2             ((uint16_t)(UART_LCR_H_STP2))
#define IS_UART_STOPBITS(STOPBITS)  (((STOPBITS) == UART_StopBits_1) || \
                                     ((STOPBITS) == UART_StopBits_2))
/**
  * @}
  */


/** @addtogroup UART_Mode
  * @{
  */

#define UART_Mode_Rx                ((uint16_t)(UART_CR_RXE))
#define UART_Mode_Tx                ((uint16_t)(UART_CR_TXE))
#define IS_UART_MODE(MODE)          (((MODE) == UART_Mode_Rx) || \
                                     ((MODE) == UART_Mode_Tx))

/**
  * @}
  */


/** @addtogroup UART_Hardware_Flow_Control
  * @{
  */

#define UART_HardwareFlowControl_None       ((uint16_t)0x0000)
#define UART_HardwareFlowControl_RTS        ((uint16_t)UART_CR_RTSEn)
#define UART_HardwareFlowControl_CTS        ((uint16_t)UART_CR_CTSEn)
#define UART_HardwareFlowControl_RTS_CTS    ((uint16_t)(UART_CR_RTSEn | UART_CR_CTSEn))
#define IS_UART_HARDWARE_FLOW_CONTROL(CONTROL) \
                                    (((CONTROL) == UART_HardwareFlowControl_None) || \
                                     ((CONTROL) == UART_HardwareFlowControl_RTS)  || \
                                     ((CONTROL) == UART_HardwareFlowControl_CTS)  || \
                                     ((CONTROL) == UART_HardwareFlowControl_RTS_CTS))
/**
  * @}
  */


/** @addtogroup UART_Receive Status
  * @{
  */

#define UART_RECV_STATUS_OE    ((uint16_t)0x01UL << 3) /*!< Overrun error  */
#define UART_RECV_STATUS_BE    ((uint16_t)0x01UL << 2) /*!< Break error    */
#define UART_RECV_STATUS_PE    ((uint16_t)0x01UL << 1) /*!< Parity error   */
#define UART_RECV_STATUS_FE    ((uint16_t)0x01UL << 0) /*!< Framing error  */
#define IS_UART_RECV_STATUS(FLAG)  (((FLAG) == UART_RECV_STATUS_OE) || ((FLAG) == UART_RECV_STATUS_BE) || \
                                    ((FLAG) == UART_RECV_STATUS_PE) || ((FLAG) == UART_RECV_STATUS_FE)) 
/**
  * @}
  */



/** @addtogroup UART_Flags
  * @{
  */

#define UART_FLAG_RI    ((uint16_t)0x01UL << 8) /*!< Ring indicator        */
#define UART_FLAG_TXFE  ((uint16_t)0x01UL << 7) /*!< Transmit FIFO empty   */
#define UART_FLAG_RXFF  ((uint16_t)0x01UL << 6) /*!< Receive FIFO full     */
#define UART_FLAG_TXFF  ((uint16_t)0x01UL << 5) /*!< Transmit FIFO full    */
#define UART_FLAG_RXFE  ((uint16_t)0x01UL << 4) /*!< Receive FIFO empty    */
#define UART_FLAG_BUSY  ((uint16_t)0x01UL << 3) /*!< UART busy             */
#define UART_FLAG_DCD   ((uint16_t)0x01UL << 2) /*!< Data carrier detect   */
#define UART_FLAG_DSR   ((uint16_t)0x01UL << 1) /*!< Data set ready        */
#define UART_FLAG_CTS   ((uint16_t)0x01UL << 0) /*!< Clear to send         */
#define IS_UART_FLAG(FLAG)  (((FLAG) == UART_FLAG_RI)   || ((FLAG) == UART_FLAG_TXFE) || \
                             ((FLAG) == UART_FLAG_RXFF) || ((FLAG) == UART_FLAG_TXFF) || \
                             ((FLAG) == UART_FLAG_RXFE) || ((FLAG) == UART_FLAG_BUSY) || \
                             ((FLAG) == UART_FLAG_DCD)  || ((FLAG) == UART_FLAG_DSR)  || \
                             ((FLAG) == UART_FLAG_CTS))

/**
  * @}
  */


/** @addtogroup UART_IT_Flags
  * @{
  */

#define UART_IT_FLAG_OEI        ((uint16_t)0x01UL << 10) /*!< Overrun error interrupt   */
#define UART_IT_FLAG_BEI        ((uint16_t)0x01UL <<  9) /*!< Break error interrupt     */
#define UART_IT_FLAG_PEI        ((uint16_t)0x01UL <<  8) /*!< Parity error interrupt    */
#define UART_IT_FLAG_FEI        ((uint16_t)0x01UL <<  7) /*!< Framing error interrupt   */
#define UART_IT_FLAG_RTI        ((uint16_t)0x01UL <<  6) /*!< Receive timeout interrupt */
#define UART_IT_FLAG_TXI        ((uint16_t)0x01UL <<  5) /*!< Transmit interrupt        */
#define UART_IT_FLAG_RXI        ((uint16_t)0x01UL <<  4) /*!< Receive interrupt        */
#define UART_IT_FLAG_DSRMI      ((uint16_t)0x01UL <<  3) /*!< UARTDSR modem interrupt   */
#define UART_IT_FLAG_DCDMI      ((uint16_t)0x01UL <<  2) /*!< UARTDCD modem interrupt   */
#define UART_IT_FLAG_CTSMI      ((uint16_t)0x01UL <<  1) /*!< UARTCTS modem interrupt   */
#define UART_IT_FLAG_RIMI       ((uint16_t)0x01UL <<  0) /*!< UARTRI modem  interrupt   */
#define IS_UART_IT_FLAG(FLAG)   (((FLAG) == UART_IT_FLAG_OEI)  || ((FLAG) == UART_IT_FLAG_BEI)    || \
                                 ((FLAG) == UART_IT_FLAG_PEI)  || ((FLAG) == UART_IT_FLAG_FEI)    || \
                                 ((FLAG) == UART_IT_FLAG_RTI)  || ((FLAG) == UART_IT_FLAG_TXI)    || \
                                 ((FLAG) == UART_IT_FLAG_RXI)  || ((FLAG) == UART_IT_FLAG_DSRMI)  || \
                                 ((FLAG) == UART_IT_FLAG_DCDMI)|| ((FLAG) == UART_IT_FLAG_CTSMI)  || \
                                 ((FLAG) == UART_IT_FLAG_RIMI))
/**
  * @}
  */
/** @addtogroup UART_FIFO_Level Select 
  * @{
  */

#define UART_IFLS_RXIFLSEL7_8                ((uint16_t)(UART_IFLS_RXIFLSEL(4)))
#define UART_IFLS_RXIFLSEL3_4                ((uint16_t)(UART_IFLS_RXIFLSEL(3)))
#define UART_IFLS_RXIFLSEL1_2                ((uint16_t)(UART_IFLS_RXIFLSEL(2)))
#define UART_IFLS_RXIFLSEL1_4                ((uint16_t)(UART_IFLS_RXIFLSEL(1)))
#define UART_IFLS_RXIFLSEL1_8                ((uint16_t)(UART_IFLS_RXIFLSEL(0)))
#define UART_IFLS_TXIFLSEL7_8                ((uint16_t)(UART_IFLS_TXIFLSEL(4)))
#define UART_IFLS_TXIFLSEL3_4                ((uint16_t)(UART_IFLS_TXIFLSEL(3)))
#define UART_IFLS_TXIFLSEL1_2                ((uint16_t)(UART_IFLS_TXIFLSEL(2)))
#define UART_IFLS_TXIFLSEL1_4                ((uint16_t)(UART_IFLS_TXIFLSEL(1)))
#define UART_IFLS_TXIFLSEL1_8                ((uint16_t)(UART_IFLS_TXIFLSEL(0)))

#define IS_UART_FIFO_Level(FLAG)          (((FLAG) == UART_IFLS_RXIFLSEL7_8) || ((FLAG) == UART_IFLS_RXIFLSEL3_4)|| \
                                     ((FLAG) == UART_IFLS_RXIFLSEL1_2)|| ((FLAG) == UART_IFLS_RXIFLSEL1_4)|| ((FLAG) == UART_IFLS_RXIFLSEL1_8)||\
                                     ((FLAG) == UART_IFLS_TXIFLSEL7_8)|| ((FLAG) == UART_IFLS_TXIFLSEL3_4)|| \
                                     ((FLAG) == UART_IFLS_TXIFLSEL1_2)|| ((FLAG) == UART_IFLS_TXIFLSEL1_4)||((FLAG) == UART_IFLS_TXIFLSEL1_8))

/**
  * @}
  */

/** @addtogroup S_UART_Flags
  * @{
  */
#define S_UART_STATE_RXO        ((uint16_t)0x01UL << 3) /*!< RX buffer Overrun   */
#define S_UART_STATE_TXO        ((uint16_t)0x01UL << 2) /*!< TX buffer Overrun   */
#define S_UART_STATE_RXF        ((uint16_t)0x01UL << 1) /*!< RX buffer Full      */
#define S_UART_STATE_TXF        ((uint16_t)0x01UL << 0) /*!< TX buffer Full      */
#define IS_S_UART_STATE(FLAG)   (((FLAG) == S_UART_STATE_RXO) || ((FLAG) == S_UART_STATE_TXO) || \
                                ((FLAG) == S_UART_STATE_RXF)  || ((FLAG) == S_UART_STATE_TXF))
/**
  * @}
  */


/** @addtogroup S_UART_CTRL_Flags
  * @{
  */

#define S_UART_CTRL_RXOI        ((uint16_t)0x01UL << 5) /*!< RX overrun interrupt   */
#define S_UART_CTRL_TXOI        ((uint16_t)0x01UL << 4) /*!< TX overrun interrupt   */
#define S_UART_CTRL_RXI         ((uint16_t)0x01UL << 3) /*!< RX interrupt           */
#define S_UART_CTRL_TXI         ((uint16_t)0x01UL << 2) /*!< TX interrupt           */
#define S_UART_CTRL_RX          ((uint16_t)0x01UL << 1) /*!< RX 			         */
#define S_UART_CTRL_TX          ((uint16_t)0x01UL << 0) /*!< TX            		 */


#define IS_S_UART_CTRL_FLAG(FLAG)   (((FLAG) == S_UART_CTRL_FLAG_RXOI)  || ((FLAG) == S_UART_CTRL_FLAG_TXOI) || \
                                     ((FLAG) == S_UART_CTRL_FLAG_RXI)   || ((FLAG) == S_UART_CTRL_FLAG_TXI) || \
									 ((FLAG) == S_UART_CTRL_FLAG_RX)    || ((FLAG) == S_UART_CTRL_FLAG_TX))
/**
  * @}
  */


/** @addtogroup S_UART_IT_Flags
  * @{
  */

#define S_UART_INTSTATUS_RXOI        ((uint16_t)0x01UL << 3) /*!< RX overrun interrupt   */
#define S_UART_INTSTATUS_TXOI        ((uint16_t)0x01UL << 2) /*!< TX overrun interrupt   */
#define S_UART_INTSTATUS_RXI         ((uint16_t)0x01UL << 1) /*!< RX interrupt           */
#define S_UART_INTSTATUS_TXI         ((uint16_t)0x01UL << 0) /*!< TX interrupt           */
#define IS_S_UART_INTSTATUS(FLAG)   (((FLAG) == S_UART_INTSTATUS_RXOI)  || ((FLAG) == S_UART_INTSTATUS_TXOI) || \
									 ((FLAG) == S_UART_INTSTATUS_RXI)   || ((FLAG) == S_UART_INTSTATUS_TXI))
/**
  * @}
  */


void        UART_StructInit         (UART_InitTypeDef* UART_InitStruct);

uint32_t    UART_Init               (UART_TypeDef *UARTx, UART_InitTypeDef* UART_InitStruct);
void        UART_SendData           (UART_TypeDef* UARTx, uint16_t Data);
uint16_t    UART_ReceiveData        (UART_TypeDef* UARTx);
void        UART_SendBreak          (UART_TypeDef* UARTx);
void        UART_ClearRecvStatus    (UART_TypeDef* UARTx, uint16_t UART_RECV_STATUS);
FlagStatus  UART_GetFlagStatus      (UART_TypeDef* UARTx, uint16_t UART_FLAG);
void        UART_ITConfig           (UART_TypeDef* UARTx, uint16_t UART_IT, FunctionalState NewState);
ITStatus    UART_GetITStatus        (UART_TypeDef* UARTx, uint16_t UART_IT);
void        UART_ClearITPendingBit  (UART_TypeDef* UARTx, uint16_t UART_IT);

uint8_t     UartPutc                (UART_TypeDef* UARTx, uint8_t ch);
void        UartPuts                (UART_TypeDef* UARTx, uint8_t *str);
uint8_t     UartGetc                (UART_TypeDef* UARTx);

void 		S_UART_DeInit	(void);
uint32_t 	S_UART_Init		(uint32_t baud);
void 		S_UART_SetBaud	(uint32_t baud);
void		S_UART_SetCTRL	(uint16_t S_UART_CTRL, FunctionalState NewState);
void 		S_UART_SendData	(uint16_t Data);
uint16_t 	S_UART_ReceiveData(void);

uint8_t 	S_UartPutc	(uint8_t ch);
void 		S_UartPuts	(uint8_t *str);
uint8_t 	S_UartGetc	(void);

FlagStatus	S_UART_GetFlagStatus	(uint16_t S_UART_STATE);
void		S_UART_ITConfig			(uint16_t S_UART_CTRL, FunctionalState NewState);

ITStatus 	S_UART_GetITStatus		(uint16_t S_UART_IT);
void 		S_UART_ClearITPendingBit(uint16_t S_UART_IT);


#ifdef __cplusplus
}
#endif


#endif // __W7500X_UART_H

