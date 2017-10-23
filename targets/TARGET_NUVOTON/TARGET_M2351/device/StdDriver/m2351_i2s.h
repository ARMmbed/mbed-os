/****************************************************************************//**
 * @file     i2s.h
 * @version  V3.00
 * @brief    M2351 series I2S driver header file
 *
 * Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#ifndef __I2S_H__
#define __I2S_H__

#ifdef __cplusplus
extern "C"
{
#endif

/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup I2S_Driver I2S Driver
  @{
*/

/** @addtogroup I2S_EXPORTED_CONSTANTS I2S Exported Constants
  @{
*/
#define I2S_DATABIT_8           (0 << I2S_CTL0_DATWIDTH_Pos)       /*!< I2S data width is 8-bit  \hideinitializer */
#define I2S_DATABIT_16          (1 << I2S_CTL0_DATWIDTH_Pos)       /*!< I2S data width is 16-bit  \hideinitializer */
#define I2S_DATABIT_24          (2 << I2S_CTL0_DATWIDTH_Pos)       /*!< I2S data width is 24-bit  \hideinitializer */
#define I2S_DATABIT_32          (3 << I2S_CTL0_DATWIDTH_Pos)       /*!< I2S data width is 32-bit  \hideinitializer */

/* Audio Format */
#define I2S_ENABLE_MONO         I2S_CTL0_MONO_Msk                  /*!< Mono channel  \hideinitializer */
#define I2S_DISABLE_MONO        0                                  /*!< Stereo channel  \hideinitializer */

/* I2S Data Format */
#define I2S_FORMAT_I2S          (0 << I2S_CTL0_FORMAT_Pos)         /*!< I2S data format  \hideinitializer */
#define I2S_FORMAT_I2S_MSB      (1 << I2S_CTL0_FORMAT_Pos)         /*!< I2S MSB data format  \hideinitializer */
#define I2S_FORMAT_I2S_LSB      (2 << I2S_CTL0_FORMAT_Pos)         /*!< I2S LSB data format  \hideinitializer */
#define I2S_FORMAT_PCM          (4 << I2S_CTL0_FORMAT_Pos)         /*!< PCM data format  \hideinitializer */
#define I2S_FORMAT_PCM_MSB      (5 << I2S_CTL0_FORMAT_Pos)         /*!< PCM MSB data format  \hideinitializer */
#define I2S_FORMAT_PCM_LSB      (6 << I2S_CTL0_FORMAT_Pos)         /*!< PCM LSB data format  \hideinitializer */

/* I2S Data Format */
#define I2S_ORDER_AT_MSB        0                                  /*!< Channel data is at MSB  \hideinitializer */
#define I2S_ORDER_AT_LSB        I2S_CTL0_ORDER_Msk                 /*!< Channel data is at LSB  \hideinitializer */

/* I2S TDM Channel Number */
#define I2S_TDM_2CH            0                                   /*!< Use TDM 2 channel  \hideinitializer */
#define I2S_TDM_4CH            1                                   /*!< Use TDM 4 channel  \hideinitializer */
#define I2S_TDM_6CH            2                                   /*!< Use TDM 6 channel  \hideinitializer */
#define I2S_TDM_8CH            3                                   /*!< Use TDM 8 channel  \hideinitializer */

/* I2S TDM Channel Width */
#define I2S_TDM_WIDTH_8BIT      0                                  /*!< TDM channel witch is 8-bit  \hideinitializer */
#define I2S_TDM_WIDTH_16BIT     1                                  /*!< TDM channel witch is 16-bit  \hideinitializer */
#define I2S_TDM_WIDTH_24BIT     2                                  /*!< TDM channel witch is 24-bit  \hideinitializer */
#define I2S_TDM_WIDTH_32BIT     3                                  /*!< TDM channel witch is 32-bit  \hideinitializer */

/* I2S TDM Sync Width */
#define I2S_TDM_SYNC_ONE_BCLK       0                              /*!< TDM sync widht is one BLCK period  \hideinitializer */
#define I2S_TDM_SYNC_ONE_CHANNEL    1                              /*!< TDM sync widht is one channel period  \hideinitializer */

/* I2S Operation mode */
#define I2S_MODE_SLAVE          I2S_CTL0_SLAVE_Msk                  /*!< As slave mode  \hideinitializer */
#define I2S_MODE_MASTER         0                                   /*!< As master mode  \hideinitializer */

/* I2S FIFO Threshold */
#define I2S_FIFO_TX_LEVEL_WORD_0    0                               /*!< TX threshold is 0 word  \hideinitializer */
#define I2S_FIFO_TX_LEVEL_WORD_1    (1 << I2S_CTL1_TXTH_Pos)        /*!< TX threshold is 1 word  \hideinitializer */
#define I2S_FIFO_TX_LEVEL_WORD_2    (2 << I2S_CTL1_TXTH_Pos)        /*!< TX threshold is 2 words  \hideinitializer */
#define I2S_FIFO_TX_LEVEL_WORD_3    (3 << I2S_CTL1_TXTH_Pos)        /*!< TX threshold is 3 words  \hideinitializer */
#define I2S_FIFO_TX_LEVEL_WORD_4    (4 << I2S_CTL1_TXTH_Pos)        /*!< TX threshold is 4 words  \hideinitializer */
#define I2S_FIFO_TX_LEVEL_WORD_5    (5 << I2S_CTL1_TXTH_Pos)        /*!< TX threshold is 5 words  \hideinitializer */
#define I2S_FIFO_TX_LEVEL_WORD_6    (6 << I2S_CTL1_TXTH_Pos)        /*!< TX threshold is 6 words  \hideinitializer */
#define I2S_FIFO_TX_LEVEL_WORD_7    (7 << I2S_CTL1_TXTH_Pos)        /*!< TX threshold is 7 words  \hideinitializer */

#define I2S_FIFO_RX_LEVEL_WORD_1    0                               /*!< RX threshold is 1 word  \hideinitializer */
#define I2S_FIFO_RX_LEVEL_WORD_2    (1 << I2S_CTL1_RXTH_Pos)        /*!< RX threshold is 2 words  \hideinitializer */
#define I2S_FIFO_RX_LEVEL_WORD_3    (2 << I2S_CTL1_RXTH_Pos)        /*!< RX threshold is 3 words  \hideinitializer */
#define I2S_FIFO_RX_LEVEL_WORD_4    (3 << I2S_CTL1_RXTH_Pos)        /*!< RX threshold is 4 words  \hideinitializer */
#define I2S_FIFO_RX_LEVEL_WORD_5    (4 << I2S_CTL1_RXTH_Pos)        /*!< RX threshold is 5 words  \hideinitializer */
#define I2S_FIFO_RX_LEVEL_WORD_6    (5 << I2S_CTL1_RXTH_Pos)        /*!< RX threshold is 6 words  \hideinitializer */
#define I2S_FIFO_RX_LEVEL_WORD_7    (6 << I2S_CTL1_RXTH_Pos)        /*!< RX threshold is 7 words  \hideinitializer */
#define I2S_FIFO_RX_LEVEL_WORD_8    (7 << I2S_CTL1_RXTH_Pos)        /*!< RX threshold is 8 words  \hideinitializer */

/* I2S Record Channel */
#define I2S_MONO_RIGHT          0                                   /*!< Record mono right channel  \hideinitializer */
#define I2S_MONO_LEFT           I2S_CTL0_RXLCH_Msk                  /*!< Record mono left channel  \hideinitializer */

/* I2S Channel */
#define I2S_RIGHT               0                                  /*!< Select right channel  \hideinitializer */
#define I2S_LEFT                1                                  /*!< Select left channel  \hideinitializer */

/*@}*/ /* end of group I2S_EXPORTED_CONSTANTS */

/** @addtogroup I2S_EXPORTED_FUNCTIONS I2S Exported Functions
  @{
*/
/*---------------------------------------------------------------------------------------------------------*/
/* inline functions                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
/**
  * @brief  Enable zero cross detect function.
  * @param[in] i2s is the base address of I2S module.
  * @param[in] u32ChMask is the mask for channel number (valid value is from (1~8).
  * @return none
  * \hideinitializer
  */
static __INLINE void I2S_ENABLE_TX_ZCD(I2S_T *i2s, uint32_t u32ChMask)
{
    if((u32ChMask > 0) && (u32ChMask < 9))
        i2s->CTL1 |= (1 << (u32ChMask - 1));
}

/**
  * @brief  Disable zero cross detect function.
  * @param[in] i2s is the base address of I2S module.
  * @param[in] u32ChMask is the mask for channel number (valid value is from (1~8).
  * @return none
  * \hideinitializer
  */
static __INLINE void I2S_DISABLE_TX_ZCD(I2S_T *i2s, uint32_t u32ChMask)
{
    if((u32ChMask > 0) && (u32ChMask < 9))
        i2s->CTL1 &= ~(1 << (u32ChMask - 1));
}

/**
  * @brief  Enable I2S Tx DMA function. I2S requests DMA to transfer data to Tx FIFO.
  * @param[in] i2s is the base address of I2S module.
  * @return none
  * \hideinitializer
  */
#define I2S_ENABLE_TXDMA(i2s)  ( (i2s)->CTL0 |= I2S_CTL0_TXPDMAEN_Msk )

/**
  * @brief  Disable I2S Tx DMA function. I2S requests DMA to transfer data to Tx FIFO.
  * @param[in] i2s is the base address of I2S module.
  * @return none
  * \hideinitializer
  */
#define I2S_DISABLE_TXDMA(i2s) ( (i2s)->CTL0 &= ~I2S_CTL0_TXPDMAEN_Msk )

/**
  * @brief  Enable I2S Rx DMA function. I2S requests DMA to transfer data from Rx FIFO.
  * @param[in] i2s is the base address of I2S module.
  * @return none
  * \hideinitializer
  */
#define I2S_ENABLE_RXDMA(i2s) ( (i2s)->CTL0 |= I2S_CTL0_RXPDMAEN_Msk )

/**
  * @brief  Disable I2S Rx DMA function. I2S requests DMA to transfer data from Rx FIFO.
  * @param[in] i2s is the base address of I2S module.
  * @return none
  * \hideinitializer
  */
#define I2S_DISABLE_RXDMA(i2s) ( (i2s)->CTL0 &= ~I2S_CTL0_RXPDMAEN_Msk )

/**
  * @brief  Enable I2S Tx function .
  * @param[in] i2s is the base address of I2S module.
  * @return none
  * \hideinitializer
  */
#define I2S_ENABLE_TX(i2s) ( (i2s)->CTL0 |= I2S_CTL0_TXEN_Msk )

/**
  * @brief  Disable I2S Tx function .
  * @param[in] i2s is the base address of I2S module.
  * @return none
  * \hideinitializer
  */
#define I2S_DISABLE_TX(i2s) ( (i2s)->CTL0 &= ~I2S_CTL0_TXEN_Msk )

/**
  * @brief  Enable I2S Rx function .
  * @param[in] i2s is the base address of I2S module.
  * @return none
  * \hideinitializer
  */
#define I2S_ENABLE_RX(i2s) ( (i2s)->CTL0 |= I2S_CTL0_RXEN_Msk )

/**
  * @brief  Disable I2S Rx function .
  * @param[in] i2s is the base address of I2S module.
  * @return none
  * \hideinitializer
  */
#define I2S_DISABLE_RX(i2s) ( (i2s)->CTL0 &= ~I2S_CTL0_RXEN_Msk )

/**
  * @brief  Enable Tx Mute function .
  * @param[in] i2s is the base address of I2S module.
  * @return none
  * \hideinitializer
  */
#define I2S_ENABLE_TX_MUTE(i2s)  ( (i2s)->CTL0 |= I2S_CTL0_MUTE_Msk )

/**
  * @brief  Disable Tx Mute function .
  * @param[in] i2s is the base address of I2S module.
  * @return none
  * \hideinitializer
  */
#define I2S_DISABLE_TX_MUTE(i2s) ( (i2s)->CTL0 &= ~I2S_CTL0_MUTE_Msk )

/**
  * @brief  Clear Tx FIFO. Internal pointer is reset to FIFO start point.
  * @param[in] i2s is the base address of I2S module.
  * @return none
  * \hideinitializer
  */
#define I2S_CLR_TX_FIFO(i2s) ( (i2s)->CTL0 |= I2S_CTL0_TXFBCLR_Msk )

/**
  * @brief  Clear Rx FIFO. Internal pointer is reset to FIFO start point.
  * @param[in] i2s is the base address of I2S module.
  * @return none
  * \hideinitializer
  */
#define I2S_CLR_RX_FIFO(i2s) ( (i2s)->CTL0 |= I2S_CTL0_RXFBCLR_Msk )

/**
  * @brief  This function sets the recording source channel when mono mode is used.
  * @param[in] i2s is the base address of I2S module.
  * @param[in] u32Ch left or right channel. Valid values are:
  *                - \ref I2S_MONO_LEFT
  *                - \ref I2S_MONO_RIGHT
  * @return none
  * \hideinitializer
  */
static __INLINE void I2S_SET_MONO_RX_CHANNEL(I2S_T *i2s, uint32_t u32Ch)
{
    u32Ch == I2S_MONO_LEFT ?
    (i2s->CTL0 |= I2S_CTL0_RXLCH_Msk) :
    (i2s->CTL0 &= ~I2S_CTL0_RXLCH_Msk);
}

/**
  * @brief  Write data to I2S Tx FIFO.
  * @param[in] i2s is the base address of I2S module.
  * @param[in] u32Data: The data written to FIFO.
  * @return none
  * \hideinitializer
  */
#define I2S_WRITE_TX_FIFO(i2s, u32Data)  ( (i2s)->TXFIFO = u32Data )

/**
  * @brief  Read Rx FIFO.
  * @param[in] i2s is the base address of I2S module.
  * @return Data in Rx FIFO.
  * \hideinitializer
  */
#define I2S_READ_RX_FIFO(i2s) ( (i2s)->RXFIFO )

/**
  * @brief  This function gets the interrupt flag according to the mask parameter.
  * @param[in] i2s is the base address of I2S module.
  * @param[in] u32Mask is the mask for the all interrupt flags.
  * @return The masked bit value of interrupt flag.
  * \hideinitializer
  */
#define I2S_GET_INT_FLAG(i2s, u32Mask) ( (i2s)->STATUS0 & (u32Mask) )

/**
  * @brief  This function clears the interrupt flag according to the mask parameter.
  * @param[in] i2s is the base address of I2S module.
  * @param[in] u32Mask is the mask for the all interrupt flags.
  * @return none
  * \hideinitializer
  */
#define I2S_CLR_INT_FLAG(i2s, u32Mask) ( (i2s)->STATUS0 |= (u32Mask) )

/**
  * @brief  This function gets the zero crossing interrupt flag according to the mask parameter.
  * @param[in] i2s is the base address of I2S module.
  * @param[in] u32Mask is the mask for the all interrupt flags.
  * @return The masked bit value of interrupt flag.
  * \hideinitializer
  */
#define I2S_GET_ZC_INT_FLAG(i2s, u32Mask) ( (i2s)->STATUS1 & (u32Mask) )

/**
  * @brief  This function clears the zero crossing interrupt flag according to the mask parameter.
  * @param[in] i2s is the base address of I2S module.
  * @param[in] u32Mask is the mask for the all interrupt flags.
  * @return none
  * \hideinitializer
  */
#define I2S_CLR_ZC_INT_FLAG(i2s, u32Mask) ( (i2s)->STATUS1 |= (u32Mask) )

/**
  * @brief  Get transmit FIFO level
  * @param[in] i2s is the base address of I2S module.
  * @return FIFO level
  * \hideinitializer
  */
#define I2S_GET_TX_FIFO_LEVEL(i2s) ( (((i2s)->STATUS1 & I2S_STATUS1_TXCNT_Msk) >> I2S_STATUS1_TXCNT_Pos) & 0xF )

/**
  * @brief  Get receive FIFO level
  * @param[in] i2s is the base address of I2S module.
  * @return FIFO level
  * \hideinitializer
  */
#define I2S_GET_RX_FIFO_LEVEL(i2s) ( (((i2s)->STATUS1 & I2S_STATUS1_RXCNT_Msk) >> I2S_STATUS1_RXCNT_Pos) & 0xF )

uint32_t I2S_Open(I2S_T *i2s, uint32_t u32MasterSlave, uint32_t u32SampleRate, uint32_t u32WordWidth, uint32_t u32Channels, uint32_t u32DataFormat);
void I2S_Close(I2S_T *i2s);
void I2S_EnableInt(I2S_T *i2s, uint32_t u32Mask);
void I2S_DisableInt(I2S_T *i2s, uint32_t u32Mask);
uint32_t I2S_EnableMCLK(I2S_T *i2s, uint32_t u32BusClock);
void I2S_DisableMCLK(I2S_T *i2s);
void I2S_SetFIFO(I2S_T *i2s, uint32_t u32TxThreshold, uint32_t u32RxThreshold);
void I2S_ConfigureTDM(I2S_T *i2s, uint32_t u32ChannelWidth, uint32_t u32ChannelNum, uint32_t u32SyncWidth);

/*@}*/ /* end of group I2S_EXPORTED_FUNCTIONS */


/*@}*/ /* end of group I2S_Driver */

/*@}*/ /* end of group Standard_Driver */
#ifdef __cplusplus
}
#endif

#endif

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/

