/****************************************************************************//**
 * @file     spi5.h
 * @version  V3.00
 * @brief    M2351 series SPI5 driver header file
 *
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#ifndef __SPI5_H__
#define __SPI5_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup SPI5_Driver SPI5 Driver
  @{
*/


/** @addtogroup SPI5_EXPORTED_CONSTANTS SPI5 Exported Constants
  @{
*/

#define SPI5_MODE_0        (SPI5_CTL_TXNEG_Msk)                            /*!< CLKP=0; RX_NEG=0; TX_NEG=1 */
#define SPI5_MODE_1        (SPI5_CTL_RXNEG_Msk)                            /*!< CLKP=0; RX_NEG=1; TX_NEG=0 */
#define SPI5_MODE_2        (SPI5_CTL_CLKPOL_Msk | SPI5_CTL_RXNEG_Msk)      /*!< CLKP=1; RX_NEG=1; TX_NEG=0 */
#define SPI5_MODE_3        (SPI5_CTL_CLKPOL_Msk | SPI5_CTL_TXNEG_Msk)      /*!< CLKP=1; RX_NEG=0; TX_NEG=1 */

#define SPI5_SLAVE         (SPI5_CTL_SLAVE_Msk)                            /*!< Set as slave */
#define SPI5_MASTER        (0x0UL)                                         /*!< Set as master */

#define SPI5_SS                (0x1UL)                                     /*!< Set SS */
#define SPI5_SS_ACTIVE_HIGH    (SPI5_SSCTL_SSACTPOL_Msk)                   /*!< SS active high */
#define SPI5_SS_ACTIVE_LOW     (0x0UL)                                     /*!< SS active low */

#define SPI5_IE_MASK                        (0x01UL)                       /*!< Interrupt enable mask */
#define SPI5_SSTAIEN_MASK                   (0x04UL)                       /*!< Slave 3-Wire mode start interrupt enable mask */
#define SPI5_FIFO_TXTHIEN_MASK              (0x08UL)                       /*!< FIFO TX interrupt mask */
#define SPI5_FIFO_RXTHIEN_MASK              (0x10UL)                       /*!< FIFO RX interrupt mask */
#define SPI5_FIFO_RXOVIEN_MASK              (0x20UL)                       /*!< FIFO RX overrun interrupt mask */
#define SPI5_FIFO_TIMEOUIEN_MASK            (0x40UL)                       /*!< FIFO timeout interrupt mask */


/*@}*/ /* end of group SPI5_EXPORTED_CONSTANTS */


/** @addtogroup SPI5_EXPORTED_FUNCTIONS SPI5 Exported Functions
  @{
*/

/**
  * @brief  Abort the current transfer in slave 3-wire mode.
  * @param[in]  spi is the base address of SPI5 module.
  * @return none
  * \hideinitializer
  */
#define SPI5_ABORT_3WIRE_TRANSFER(spi) ( (spi)->SSCTL |= SPI5_SSCTL_SLVABORT_Msk )

/**
  * @brief  Clear the slave 3-wire mode start interrupt flag.
  * @param[in]  spi is the base address of SPI5 module.
  * @return none
  * \hideinitializer
  */
#define SPI5_CLR_3WIRE_START_INT_FLAG(spi)  ( (spi)->STATUS = SPI5_STATUS_SLVSTAIF_Msk )

/**
  * @brief  Clear the unit transfer interrupt flag.
  * @param[in]  spi is the base address of SPI5 module.
  * @return none
  * \hideinitializer
  */
#define SPI5_CLR_UNIT_TRANS_INT_FLAG(spi) ( (spi)->STATUS = SPI5_STATUS_UNITIF_Msk )

/**
  * @brief  Disable slave 3-wire mode.
  * @param[in]  spi is the base address of SPI5 module.
  * @return none
  * \hideinitializer
  */
#define SPI5_DISABLE_3WIRE_MODE(spi) ( (spi)->SSCTL &= ~SPI5_SSCTL_SLV3WIRE_Msk )

/**
  * @brief  Enable slave 3-wire mode.
  * @param[in]  spi is the base address of SPI5 module.
  * @return none
  * \hideinitializer
  */
#define SPI5_ENABLE_3WIRE_MODE(spi) ( (spi)->SSCTL |= SPI5_SSCTL_SLV3WIRE_Msk )

/**
  * @brief  Get the count of available data in RX FIFO.
  * @param[in]  spi is the base address of SPI5 module.
  * @return The count of available data in RX FIFO.
  * \hideinitializer
  */
#define SPI5_GET_RX_FIFO_COUNT(spi) ( (((spi)->STATUS & SPI5_STATUS_RXCNT_Msk) >> SPI5_STATUS_RXCNT_Pos) & 0xF )

/**
  * @brief  Get the RX FIFO empty flag.
  * @param[in]  spi is the base address of SPI5 module.
  * @return RX FIFO flag
  * @retval 0 RX FIFO is not empty
  * @retval 1 RX FIFO is empty
  * \hideinitializer
  */
#define SPI5_GET_RX_FIFO_EMPTY_FLAG(spi) ( ((spi)->STATUS & SPI5_STATUS_RXEMPTY_Msk) == SPI5_STATUS_RXEMPTY_Msk ? 1:0 )

/**
  * @brief  Get the TX FIFO empty flag.
  * @param[in]  spi is the base address of SPI5 module.
  * @return TX FIFO flag
  * @retval 0 TX FIFO is not empty
  * @retval 1 TX FIFO is empty
  * \hideinitializer
  */
#define SPI5_GET_TX_FIFO_EMPTY_FLAG(spi) ( ((spi)->STATUS & SPI5_STATUS_TXEMPTY_Msk) == SPI5_STATUS_TXEMPTY_Msk ? 1:0 )

/**
  * @brief  Get the TX FIFO full flag.
  * @param[in]  spi is the base address of SPI5 module.
  * @return TX FIFO flag
  * @retval 0 TX FIFO is not full
  * @retval 1 TX FIFO is full
  * \hideinitializer
  */
#define SPI5_GET_TX_FIFO_FULL_FLAG(spi) ( ((spi)->STATUS & SPI5_STATUS_TXFULL_Msk) == SPI5_STATUS_TXFULL_Msk ? 1:0 )

/**
  * @brief  Get the datum read from RX FIFO.
  * @param[in]  spi is the base address of SPI5 module.
  * @return Data in RX register.
  * \hideinitializer
  */
#define SPI5_READ_RX(spi) ( (spi)->RX )

/**
  * @brief  Write datum to TX register.
  * @param[in]  spi is the base address of SPI5 module.
  * @param[in]  u32TxData is the datum which user attempt to transfer through SPI5 bus.
  * @return none
  * \hideinitializer
  */
#define SPI5_WRITE_TX(spi, u32TxData)  ( (spi)->TX = (u32TxData) )

/**
  * @brief      Set SPI5_SS pin to high state.
  * @param[in]  spi The pointer of the specified SPI5 module.
  * @return     None.
  * @details    Disable automatic slave selection function and set SPI5_SS pin to high state. Only available in Master mode.
  * \hideinitializer
  */
#define SPI5_SET_SS_HIGH(spi)   ( (spi)->SSCTL = ((spi)->SSCTL & ~(SPI5_SSCTL_AUTOSS_Msk | SPI5_SSCTL_SSACTPOL_Msk | SPI5_SS)) )

/**
  * @brief      Set SPI5_SS pin to low state.
  * @param[in]  spi The pointer of the specified SPI5 module.
  * @return     None.
  * @details    Disable automatic slave selection function and set SPI5_SS pin to low state. Only available in Master mode.
  * \hideinitializer
  */
#define SPI5_SET_SS_LOW(spi)   ( (spi)->SSCTL = ((spi)->SSCTL & ~(SPI5_SSCTL_AUTOSS_Msk | SPI5_SSCTL_SSACTPOL_Msk | SPI5_SS)) | SPI5_SS )

/**
  * @brief      Set SPI5_SS pin to high or low state.
  * @param[in]  spi The pointer of the specified SPI5 module.
  * @param[in]  ss 0 = Set SPI5_SS to low. 1 = Set SPI5_SS to high.
  * @return     None.
  * @details    Disable automatic slave selection function and set SPI5_SS pin to specified high/low state.
  *             Only available in Master mode.
  */
#define SPI5_SET_SS_LEVEL(spi, ss)   ( (spi)->SSCTL = ((spi)->SSCTL & ~(SPI5_SSCTL_AUTOSS_Msk | SPI5_SSCTL_SSACTPOL_Msk | SPI5_SSCTL_SS_Msk)) | ((ss)^1) )

/**
  * @brief Enable byte reorder function.
  * @param[in]  spi is the base address of SPI5 module.
  * @return none
  * \hideinitializer
  */
#define SPI5_ENABLE_BYTE_REORDER(spi) ( (spi)->CTL |= SPI5_CTL_REORDER_Msk )

/**
  * @brief  Disable byte reorder function.
  * @param[in]  spi is the base address of SPI5 module.
  * @return none
  * \hideinitializer
  */
#define SPI5_DISABLE_BYTE_REORDER(spi) ( (spi)->CTL &= ~SPI5_CTL_REORDER_Msk )

/**
  * @brief  Set the length of suspend interval.
  * @param[in]  spi is the base address of SPI5 module.
  * @param[in]  u32SuspCycle decides the length of suspend interval.
  * @return none
  * \hideinitializer
  */
#define SPI5_SET_SUSPEND_CYCLE(spi, u32SuspCycle) ( (spi)->CTL = ((spi)->CTL & ~SPI5_CTL_SUSPITV_Msk) | ((u32SuspCycle) << SPI5_CTL_SUSPITV_Pos) )

/**
  * @brief  Set the SPI5 transfer sequence with LSB first.
  * @param[in]  spi is the base address of SPI5 module.
  * @return none
  * \hideinitializer
  */
#define SPI5_SET_LSB_FIRST(spi) ( (spi)->CTL |= SPI5_CTL_LSB_Msk )

/**
  * @brief  Set the SPI5 transfer sequence with MSB first.
  * @param[in]  spi is the base address of SPI5 module.
  * @return none
  * \hideinitializer
  */
#define SPI5_SET_MSB_FIRST(spi) ( (spi)->CTL &= ~SPI5_CTL_LSB_Msk )

/* Declare these inline functions here to avoid MISRA C 2004 rule 8.1 error */
__STATIC_INLINE void SPI5_SET_DATA_WIDTH(SPI5_T *spi, uint32_t u32Width);

/**
  * @brief  Set the data width of a SPI5 transaction.
  * @param[in]  spi is the base address of SPI5 module.
  * @param[in]  u32Width is the data width (from 8-32 bits).
  * @return none
  * \hideinitializer
  */
__STATIC_INLINE void SPI5_SET_DATA_WIDTH(SPI5_T *spi, uint32_t u32Width)
{
    if(u32Width == 32UL)
    {
        u32Width = 0UL;
    }

    (spi)->CTL = ((spi)->CTL & ~SPI5_CTL_DWIDTH_Msk) | ((u32Width) << SPI5_CTL_DWIDTH_Pos);
}

/**
  * @brief  Get the SPI5 busy state.
  * @param[in]  spi is the base address of SPI5 module.
  * @return SPI5 busy status
  * @retval 0 SPI5 module is not busy
  * @retval 1 SPI5 module is busy
  * \hideinitializer
  */
#define SPI5_IS_BUSY(spi) ( ((spi)->CTL & SPI5_CTL_GOBUSY_Msk) == SPI5_CTL_GOBUSY_Msk ? 1:0 )

/**
  * @brief  Set the GOBUSY bit to trigger SPI5 transfer.
  * @param[in]  spi is the base address of SPI5 module.
  * @return none
  * \hideinitializer
  */
#define SPI5_TRIGGER(spi) ( (spi)->CTL |= SPI5_CTL_GOBUSY_Msk )

/**
  * @brief  Trigger RX PDMA transfer.
  * @param[in]  spi is the base address of SPI5 module.
  * @return none
  * \hideinitializer
  */
#define SPI5_TRIGGER_RX_PDMA(spi) ( (spi)->PDMACTL |= SPI5_PDMACTL_RXPDMAEN_Msk )

/**
  * @brief  Trigger TX PDMA transfer.
  * @param[in]  spi is the base address of SPI5 module.
  * @return none
  * \hideinitializer
  */
#define SPI5_TRIGGER_TX_PDMA(spi) ( (spi)->PDMACTL |= SPI5_PDMACTL_TXPDMAEN_Msk )

/**
  * @brief  Get the status register value.
  * @param[in]  spi is the base address of SPI5 module.
  * @return status value.
  * \hideinitializer
  */
#define SPI5_GET_STATUS(spi) ( (spi)->STATUS )

uint32_t SPI5_Open(SPI5_T *spi, uint32_t u32MasterSlave, uint32_t u32SPI5Mode, uint32_t u32DataWidth, uint32_t u32BusClock);
void SPI5_Close(SPI5_T *spi);
void SPI5_ClearRxFIFO(SPI5_T *spi);
void SPI5_ClearTxFIFO(SPI5_T *spi);
void SPI5_DisableAutoSS(SPI5_T *spi);
void SPI5_EnableAutoSS(SPI5_T *spi, uint32_t u32SSPinMask, uint32_t u32ActiveLevel);
uint32_t SPI5_SetBusClock(SPI5_T *spi, uint32_t u32BusClock);
void SPI5_EnableFIFO(SPI5_T *spi, uint32_t u32TxThreshold, uint32_t u32RxThreshold);
void SPI5_DisableFIFO(SPI5_T *spi);
uint32_t SPI5_GetBusClock(SPI5_T *spi);
void SPI5_EnableInt(SPI5_T *spi, uint32_t u32Mask);
void SPI5_DisableInt(SPI5_T *spi, uint32_t u32Mask);
void SPI5_EnableWakeup(SPI5_T *spi);
void SPI5_DisableWakeup(SPI5_T *spi);
/*@}*/ /* end of group SPI5_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group SPI5_Driver */

/*@}*/ /* end of group Standard_Driver */

#ifdef __cplusplus
}
#endif

#endif /* __SPI5_H__ */

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
