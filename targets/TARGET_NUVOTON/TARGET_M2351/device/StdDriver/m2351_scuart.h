/**************************************************************************//**
 * @file     scuart.h
 * @version  V3.00
 * @brief    Smart Card UART(SCUART) driver header file
 *
 * @note
 * Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __SCUART_H__
#define __SCUART_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup SCUART_Driver SCUART Driver
  @{
*/

/** @addtogroup SCUART_EXPORTED_CONSTANTS SCUART Exported Constants
  @{
*/
#define SCUART_CHAR_LEN_5     (0x3UL << SC_UARTCTL_WLS_Pos)     /*!< Set SCUART word length to 5 bits \hideinitializer */
#define SCUART_CHAR_LEN_6     (0x2UL << SC_UARTCTL_WLS_Pos)     /*!< Set SCUART word length to 6 bits \hideinitializer */
#define SCUART_CHAR_LEN_7     (0x1UL << SC_UARTCTL_WLS_Pos)     /*!< Set SCUART word length to 7 bits \hideinitializer */
#define SCUART_CHAR_LEN_8     (0UL)                             /*!< Set SCUART word length to 8 bits \hideinitializer */

#define SCUART_PARITY_NONE    (SC_UARTCTL_PBOFF_Msk)            /*!< Set SCUART transfer with no parity   \hideinitializer */
#define SCUART_PARITY_ODD     (SC_UARTCTL_OPE_Msk)              /*!< Set SCUART transfer with odd parity  \hideinitializer */
#define SCUART_PARITY_EVEN    (0UL)                             /*!< Set SCUART transfer with even parity \hideinitializer */

#define SCUART_STOP_BIT_1     (SC_CTL_NSB_Msk)                  /*!< Set SCUART transfer with one stop bit  \hideinitializer */
#define SCUART_STOP_BIT_2     (0UL)                             /*!< Set SCUART transfer with two stop bits \hideinitializer */

/*@}*/ /* end of group SCUART_EXPORTED_CONSTANTS */


/** @addtogroup SCUART_EXPORTED_FUNCTIONS SCUART Exported Functions
  @{
*/

/* Tx Macros */
/**
  * @brief      Write Data to Tx Data Register
  *
  * @param[in]  sc      The pointer of smartcard module.
  * @param[in]  u8Data  Data byte to transmit.
  *
  * @return     None
  *
  * @details    By writing data to DAT register, the SC will send out an 8-bit data.
  * \hideinitializer
  */
#define SCUART_WRITE(sc, u8Data)    ((sc)->DAT = (u8Data))

/**
  * @brief      Get Tx FIFO Empty Flag Status
  *
  * @param[in]  sc      The pointer of smartcard module.
  *
  * @retval     0                       Transmit FIFO is not empty
  * @retval     SC_STATUS_TXEMPTY_Msk   Transmit FIFO is empty
  *
  * @details    When the last byte of Tx buffer has been transferred to Transmitter Shift Register, hardware sets TXEMPTY (SC_STATUS[9]) high.
  *             It will be cleared when writing data into DAT (SC_DAT[7:0]).
  * \hideinitializer
  */
#define SCUART_GET_TX_EMPTY(sc)     ((sc)->STATUS & SC_STATUS_TXEMPTY_Msk)

/**
  * @brief      Get Tx FIFO Full Flag Status
  *
  * @param[in]  sc      The pointer of smartcard module.
  *
  * @retval     0                       Transmit FIFO is not full
  * @retval     SC_STATUS_TXFULL_Msk    Transmit FIFO is full
  *
  * @details    TXFULL (SC_STATUS[10]) is set when Tx buffer counts equals to 4, otherwise is cleared by hardware.
  * \hideinitializer
  */
#define SCUART_GET_TX_FULL(sc)      ((sc)->STATUS & SC_STATUS_TXFULL_Msk)

/**
  * @brief      Wait specified Smartcard port Transmission Complete
  *
  * @param[in]  sc      The pointer of smartcard module.
  *
  * @return     None
  *
  * @details    TXACT (SC_STATUS[31]) is cleared automatically when Tx transfer is finished or the last byte transmission has completed.
  *
  * @note       This macro blocks until transmit complete.
  * \hideinitializer
  */
#define SCUART_WAIT_TX_EMPTY(sc)    while((sc)->STATUS & SC_STATUS_TXACT_Msk)

/**
  * @brief      Check Tx FIFO Full Status
  *
  * @param[in]  sc      The pointer of smartcard module.
  *
  * @retval     0       Transmit FIFO is not full
  * @retval     1       Transmit FIFO is full
  *
  * @details    TXFULL (SC_STATUS[10]) indicates Tx buffer full or not.
  *             This bit is set when Tx buffer counts equals to 4, otherwise is cleared by hardware.
  * \hideinitializer
  */
#define SCUART_IS_TX_FULL(sc)       (((sc)->STATUS & SC_STATUS_TXFULL_Msk)? 1 : 0)

/**
  * @brief      Check Tx Transmit Complete Status
  *
  * @param[in]  sc      The pointer of smartcard module.
  *
  * @retval     0       Transmit is not complete
  * @retval     1       Transmit complete
  *
  * @details    TXACT (SC_STATUS[31]) indicates Tx Transmit is complete or not.
  * \hideinitializer
  */
#define SCUART_IS_TX_EMPTY(sc)      (((sc)->STATUS & SC_STATUS_TXACT_Msk)? 0 : 1)

/**
  * @brief      Check Tx FIFO Empty Status
  *
  * @param[in]  sc      The pointer of smartcard module.
  *
  * @retval     0       Transmit FIFO is not empty
  * @retval     1       Transmit FIFO is empty
  *
  * @details    TXEMPTY (SC_STATUS[9]) is set by hardware when the last byte of Tx buffer has been transferred to Transmitter Shift Register.
  * \hideinitializer
  */
#define SCUART_IS_TX_FIFO_EMPTY(sc) (((sc)->STATUS & SC_STATUS_TXEMPTY_Msk)? 1 : 0)

/**
  * @brief      Check specified Smartcard port Transmission Status
  *
  * @param[in]  sc      The pointer of smartcard module.
  *
  * @retval     0       Transmit is completed
  * @retval     1       Transmit is active
  *
  * @details    TXACT (SC_STATUS[31]) is set by hardware when Tx transfer is in active and the STOP bit of the last byte has been transmitted.
  * \hideinitializer
  */
#define SCUART_IS_TX_ACTIVE(sc)     (((sc)->STATUS & SC_STATUS_TXACT_Msk)? 1 : 0)


/* RX Macros */
/**
  * @brief      Read Rx Data Register
  *
  * @param[in]  sc      The pointer of smartcard module.
  *
  * @return     The oldest data byte in RX FIFO
  *
  * @details    By reading DAT register, the SC will return an 8-bit received data.
  * \hideinitializer
  */
#define SCUART_READ(sc)         ((sc)->DAT)

/**
  * @brief      Get Rx FIFO Empty Flag Status
  *
  * @param[in]  sc      The pointer of smartcard module.
  *
  * @retval     0                       Receive FIFO is not empty
  * @retval     SC_STATUS_RXEMPTY_Msk   Receive FIFO is empty
  *
  * @details    When the last byte of Rx buffer has been read by CPU, hardware sets RXEMPTY (SC_STATUS[1]) high.
  *             It will be cleared when SC receives any new data.
  * \hideinitializer
  */
#define SCUART_GET_RX_EMPTY(sc) ((sc)->STATUS & SC_STATUS_RXEMPTY_Msk)

/**
  * @brief      Get Rx FIFO Full Flag Status
  *
  * @param[in]  sc      The pointer of smartcard module.
  *
  * @retval     0                       Receive FIFO is not full
  * @retval     SC_STATUS_TXFULL_Msk    Receive FIFO is full
  *
  * @details    RXFULL (SC_STATUS[2]) is set when Rx buffer counts equals to 4, otherwise it is cleared by hardware.
  * \hideinitializer
  */
#define SCUART_GET_RX_FULL(sc)  ((sc)->STATUS & SC_STATUS_RXFULL_Msk)

/**
  * @brief      Check Rx FIFO Reach FIFO Trigger Level Status
  *
  * @param[in]  sc      The pointer of smartcard module.
  *
  * @retval     0       The number of bytes in Rx FIFO is less than trigger level.
  * @retval     1       The number of bytes in Rx FIFO equals or larger than trigger level.
  *
  * @details    RDAIF (SC_INTSTS[0]) is used for received data reaching trigger level RXTRGLV (SC_CTL[7:6]) interrupt status flag.
  *
  * @note       If receive trigger level is \b not 1 byte, this macro return 0 does not necessary indicates there is no data in FIFO.
  * \hideinitializer
  */
#define SCUART_IS_RX_READY(sc)  (((sc)->INTSTS & SC_INTSTS_RDAIF_Msk)? 1 : 0)

/**
  * @brief      Check Rx FIFO Full Status
  *
  * @param[in]  sc      The pointer of smartcard module.
  *
  * @retval     0       Receive FIFO is not full
  * @retval     1       Receive FIFO is full
  *
  * @details    RXFULLF( SC_STATUS[2]) is set when Rx buffer counts equals to 4, otherwise it is cleared by hardware.
  * \hideinitializer
  */
#define SCUART_IS_RX_FULL(sc)   (((sc)->STATUS & SC_STATUS_RXFULL_Msk)? 1 : 0)


/* Interrupt Macros */
/**
  * @brief      Enable specified SCUART Interrupt Function
  *
  * @param[in]  sc      The pointer of smartcard module.
  * @param[in]  u32Mask Interrupt masks to enable, a combination of following bits.
  *                         - \ref SC_INTEN_RXTOIEN_Msk
  *                         - \ref SC_INTEN_TERRIEN_Msk
  *                         - \ref SC_INTEN_TBEIEN_Msk
  *                         - \ref SC_INTEN_RDAIEN_Msk
  *
  * @return     None
  *
  * @details    The macro is used to enable receiver buffer time-out interrupt, transfer error interrupt,
  *             transmit buffer empty interrupt or receive data reach trigger level interrupt.
  * \hideinitializer
  */
#define SCUART_ENABLE_INT(sc, u32Mask)  ((sc)->INTEN |= (u32Mask))

/**
  * @brief      Disable specified SCUART Interrupt Function
  *
  * @param[in]  sc      The pointer of smartcard module.
  * @param[in]  u32Mask Interrupt masks to disable, a combination of following bits.
  *                         - \ref SC_INTEN_RXTOIEN_Msk
  *                         - \ref SC_INTEN_TERRIEN_Msk
  *                         - \ref SC_INTEN_TBEIEN_Msk
  *                         - \ref SC_INTEN_RDAIEN_Msk
  *
  * @return     None
  *
  * @details    The macro is used to disable receiver buffer time-out interrupt, transfer error interrupt,
  *             transmit buffer empty interrupt or receive data reach trigger level interrupt.
  * \hideinitializer
  */
#define SCUART_DISABLE_INT(sc, u32Mask) ((sc)->INTEN &= ~(u32Mask))

/**
  * @brief      Get specified SCUART Interrupt Status
  *
  * @param[in]  sc      The pointer of smartcard module.
  * @param[in]  u32Type Interrupt status to check, could be one of following value:
  *                         - \ref SC_INTSTS_RBTOIF_Msk
  *                         - \ref SC_INTSTS_TERRIF_Msk
  *                         - \ref SC_INTSTS_TBEIF_Msk
  *                         - \ref SC_INTSTS_RDAIF_Msk
  *
  * @retval     0       Specified interrupt does not happened.
  * @retval     1       Specified interrupt happened.
  *
  * @details    The macro is used to get receiver buffer time-out interrupt status, transfer error interrupt status,
  *             transmit buffer empty interrupt status or receive data reach interrupt status.
  * \hideinitializer
  */
#define SCUART_GET_INT_FLAG(sc, u32Type)    (((sc)->INTSTS & (u32Type))? 1 : 0)

/**
  * @brief      Clear specified SCUART Interrupt Flag
  *
  * @param[in]  sc      The pointer of smartcard module.
  * @param[in]  u32Type Interrupt flag to clear, could be the combination of following values:
  *                         - \ref SC_INTSTS_TERRIF_Msk
  *
  * @return     None
  *
  * @details    The macro is used to clear transfer error interrupt flag.
  * \hideinitializer
  */
#define SCUART_CLR_INT_FLAG(sc, u32Type)    ((sc)->INTSTS = (u32Type))

/**
  * @brief      Get SCUART Receiver Error Flag Status
  *
  * @param[in]  sc  The pointer of smartcard module.
  *
  * @retval     SC_STATUS_PEF_Msk   Rx parity error
  * @retval     SC_STATUS_FEF_Msk   Rx frame error
  * @retval     SC_STATUS_BEF_Msk   Rx break error
  *
  * @details    The macro is used to get receiver parity error status, frame error status or break error status.
  * \hideinitializer
  */
#define SCUART_GET_ERR_FLAG(sc)             ((sc)->STATUS & (SC_STATUS_PEF_Msk | SC_STATUS_FEF_Msk | SC_STATUS_BEF_Msk))

/**
  * @brief      Clear specified Receiver Error Flag
  *
  * @param[in]  sc      The pointer of smartcard module.
  * @param[in]  u32Mask Receiver error flag to clear, combination following values:
  *                         - \ref SC_STATUS_PEF_Msk
  *                         - \ref SC_STATUS_FEF_Msk
  *                         - \ref SC_STATUS_BEF_Msk
  *
  * @return     None
  *
  * @details    The macro is used to clear receiver parity error flag, frame error flag or break error flag.
  * \hideinitializer
  */
#define SCUART_CLR_ERR_FLAG(sc, u32Mask)    ((sc)->STATUS = (u32Mask))

void SCUART_Close(SC_T* sc);
uint32_t SCUART_Open(SC_T* sc, uint32_t u32Baudrate);
uint32_t SCUART_Read(SC_T* sc, uint8_t pu8RxBuf[], uint32_t u32ReadBytes);
uint32_t SCUART_SetLineConfig(SC_T* sc, uint32_t u32Baudrate, uint32_t u32DataWidth, uint32_t u32Parity, uint32_t u32StopBits);
void SCUART_SetTimeoutCnt(SC_T* sc, uint32_t u32TOC);
void SCUART_Write(SC_T* sc, uint8_t pu8TxBuf[], uint32_t u32WriteBytes);

/*@}*/ /* end of group SCUART_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group SCUART_Driver */

/*@}*/ /* end of group Standard_Driver */

#ifdef __cplusplus
}
#endif

#endif //__SCUART_H__

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
