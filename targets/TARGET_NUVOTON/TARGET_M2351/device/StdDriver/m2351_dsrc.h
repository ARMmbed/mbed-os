/**************************************************************************//**
 * @file     dsrc.h
 * @version  V3.00
 * @brief    Dedicated Short Range Communication Controller(DSRC) driver header file
 *
 * @note
 * Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __DSRC_H__
#define __DSRC_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup DSRC_Driver DSRC Driver
  @{
*/

/** @addtogroup DSRC_EXPORTED_CONSTANTS DSRC Exported Constants
  @{
*/
#define DSRC_MODE_FM0               (DSRC_CTL_CODECEN_Msk)                          /*!< DSRC operates at FM0 mode */
#define DSRC_MODE_MONCHESTER        (DSRC_CTL_CODECEN_Msk|DSRC_CTL_CODECFMT_Msk)    /*!< DSRC operates at MONCHESTER mode */
#define DSRC_MODE_SPI               (0)                                             /*!< DSRC operates at SPI mode */
#define DSRC_MODE_Msk               (DSRC_CTL_CODECEN_Msk|DSRC_CTL_CODECFMT_Msk)    /*!< Mask setting of DSRC operates mode */

#define DSRC_CTL_WKPOL_RISING       (0x0UL << DSRC_CTL_WKPOL_Pos)       /*!< DSRC wakeup pin event at rising edge */
#define DSRC_CTL_WKPOL_FALLING      (0x1UL << DSRC_CTL_WKPOL_Pos)       /*!< DSRC wakeup pin event at falling edge */
#define DSRC_CTL_WKPOL_BOTH         (0x2UL << DSRC_CTL_WKPOL_Pos)       /*!< DSRC wakeup pin event at rising or falling edge */

#define DSRC_CTL_PREAMFMT_16        (0x0UL << DSRC_CTL_PREAMFMT_Pos)    /*!< DSRC preamble pattern size is 16-bit */
#define DSRC_CTL_PREAMFMT_32        (0x1UL << DSRC_CTL_PREAMFMT_Pos)    /*!< DSRC preamble pattern size is 32-bit */

/*@}*/ /* end of group DSRC_EXPORTED_CONSTANTS */


/** @addtogroup DSRC_EXPORTED_FUNCTIONS DSRC Exported Functions
  @{
*/

/**
  * @brief      Enable DSRC Interrupt
  *
  * @param[in]  dsrc        The pointer of the DSRC module
  * @param[in]  u32Mask     Interrupt mask to be enabled. A combination of
  *                             - \ref DSRC_INTEN_CRCCORIE_Msk
  *                             - \ref DSRC_INTEN_CRCERRIE_Msk
  *                             - \ref DSRC_INTEN_STRFRMIE_Msk
  *                             - \ref DSRC_INTEN_STPFRMIE_Msk
  *                             - \ref DSRC_INTEN_RXDATERRIE_Msk
  *                             - \ref DSRC_INTEN_BRATERRIE_Msk
  *                             - \ref DSRC_INTEN_TTBPDIE_Msk
  *                             - \ref DSRC_INTEN_RTBPDIE_Msk
  *                             - \ref DSRC_INTEN_TXDONEIE_Msk
  *                             - \ref DSRC_INTEN_T2TOIE_Msk
  *                             - \ref DSRC_INTEN_T3TOIE_Msk
  *                             - \ref DSRC_INTEN_T4TOIE_Msk
  *
  * @return     None
  */
#define DSRC_ENABLE_INT(dsrc, u32Mask)  ((dsrc)->INTEN |= (u32Mask))

/**
  * @brief      Disable DSRC Interrupt
  *
  * @param[in]  dsrc        The pointer of the DSRC module
  * @param[in]  u32Mask     Interrupt mask to be disabled. A combination of
  *                             - \ref DSRC_INTEN_CRCCORIE_Msk
  *                             - \ref DSRC_INTEN_CRCERRIE_Msk
  *                             - \ref DSRC_INTEN_STRFRMIE_Msk
  *                             - \ref DSRC_INTEN_STPFRMIE_Msk
  *                             - \ref DSRC_INTEN_RXDATERRIE_Msk
  *                             - \ref DSRC_INTEN_BRATERRIE_Msk
  *                             - \ref DSRC_INTEN_TTBPDIE_Msk
  *                             - \ref DSRC_INTEN_RTBPDIE_Msk
  *                             - \ref DSRC_INTEN_TXDONEIE_Msk
  *                             - \ref DSRC_INTEN_T2TOIE_Msk
  *                             - \ref DSRC_INTEN_T3TOIE_Msk
  *                             - \ref DSRC_INTEN_T4TOIE_Msk
  *
  * @return     None
  */
#define DSRC_DISABLE_INT(dsrc, u32Mask) ((dsrc)->INTEN &= ~(u32Mask))

/**
  * @brief      Check Specify Interrupt Enabled Status
  *
  * @param[in]  dsrc        The pointer of the DSRC module
  * @param[in]  u32Mask     Interrupt mask selected. A combination of
  *                             - \ref DSRC_INTEN_CRCCORIE_Msk
  *                             - \ref DSRC_INTEN_CRCERRIE_Msk
  *                             - \ref DSRC_INTEN_STRFRMIE_Msk
  *                             - \ref DSRC_INTEN_STPFRMIE_Msk
  *                             - \ref DSRC_INTEN_RXDATERRIE_Msk
  *                             - \ref DSRC_INTEN_BRATERRIE_Msk
  *                             - \ref DSRC_INTEN_TTBPDIE_Msk
  *                             - \ref DSRC_INTEN_RTBPDIE_Msk
  *                             - \ref DSRC_INTEN_TXDONEIE_Msk
  *                             - \ref DSRC_INTEN_T2TOIE_Msk
  *                             - \ref DSRC_INTEN_T3TOIE_Msk
  *                             - \ref DSRC_INTEN_T4TOIE_Msk

  * @retval     0       Specified interrupts disabled
  * @retval     1       Specified interrupts enabled
  */
#define DSRC_GET_INTEN_STS(dsrc, u32Mask)   (((dsrc)->INTEN & (u32Mask))== (u32Mask) ? 1:0)

/**
  * @brief      Get Specified Interrupt Flag Status
  *
  * @param[in]  dsrc        The pointer of the DSRC module
  * @param[in]  u32Mask     Interrupt flags mask to be check. A combination of
  *                             - \ref DSRC_STATUS_CRCCOR_Msk
  *                             - \ref DSRC_STATUS_CRCERR_Msk
  *                             - \ref DSRC_STATUS_STRFRM_Msk
  *                             - \ref DSRC_STATUS_STPFRM_Msk
  *                             - \ref DSRC_STATUS_RXDATERR_Msk
  *                             - \ref DSRC_STATUS_BRATERR_Msk
  *                             - \ref DSRC_STATUS_TTBPDONE_Msk
  *                             - \ref DSRC_STATUS_RTBPDONE_Msk
  *                             - \ref DSRC_STATUS_TXFINISH_Msk
  *                             - \ref DSRC_STATUS_TTBPFULL_Msk
  *                             - \ref DSRC_STATUS_T2TO_Msk
  *                             - \ref DSRC_STATUS_T3TO_Msk
  *                             - \ref DSRC_STATUS_T4TO_Msk
  *                             - \ref DSRC_STATUS_CRC0_OK_Msk
  *                             - \ref DSRC_STATUS_CRC1_OK_Msk
  *                             - \ref DSRC_STATUS_EPWKF_Msk
  *
  * @retval     0   No specified interrupts
  * @retval     1   Specified interrupts occurred
  */
#define DSRC_GET_INT_FLAG(dsrc, u32Mask)    (((dsrc)->STATUS & (u32Mask))== (u32Mask) ? 1:0)


/**
  * @brief      Clear DSRC related interrupts specified by u32Mask parameter.
  *
  * @param[in]  dsrc        The pointer of the DSRC module
  * @param[in]  u32Mask     Interrupt flags mask to be cleared. A combination of
  *                             - \ref DSRC_STATUS_CRCCOR_Msk
  *                             - \ref DSRC_STATUS_CRCERR_Msk
  *                             - \ref DSRC_STATUS_STRFRM_Msk
  *                             - \ref DSRC_STATUS_STPFRM_Msk
  *                             - \ref DSRC_STATUS_RXDATERR_Msk
  *                             - \ref DSRC_STATUS_BRATERR_Msk
  *                             - \ref DSRC_STATUS_TTBPDONE_Msk
  *                             - \ref DSRC_STATUS_RTBPDONE_Msk
  *                             - \ref DSRC_STATUS_TXFINISH_Msk
  *                             - \ref DSRC_STATUS_TTBPFULL_Msk
  *                             - \ref DSRC_STATUS_T2TO_Msk
  *                             - \ref DSRC_STATUS_T3TO_Msk
  *                             - \ref DSRC_STATUS_T4TO_Msk
  *                             - \ref DSRC_STATUS_CRC0_OK_Msk
  *                             - \ref DSRC_STATUS_CRC1_OK_Msk
  *                             - \ref DSRC_STATUS_EPWKF_Msk
  *
  * @return     None
  */
#define DSRC_CLR_INT_FLAG(dsrc, u32Mask)    ((dsrc)->STATUS = (u32Mask))

/**
  * @brief      Enable TBP Receive DMA
  *
  * @param[in]  dsrc    The pointer of the DSRC module
  *
  * @return     None
  */
#define DSRC_ENABLE_RX_PDMA(dsrc)       ((dsrc)->CTL |= DSRC_CTL_TRDMAEN_Msk)

/**
  * @brief      Enable TBP Transmit DMA
  *
  * @param[in]  dsrc    The pointer of the DSRC module
  *
  * @return     None
  */
#define DSRC_ENABLE_TX_PDMA(dsrc)       ((dsrc)->CTL |= DSRC_CTL_TTDMAEN_Msk)

/**
  * @brief      Force DSRC in RX state
  *
  * @param[in]  dsrc    The pointer of the DSRC module
  *
  * @return     None
  */
#define DSRC_FORCE_RX(dsrc)             ((dsrc)->CTL |= DSRC_CTL_FORCERX_Msk)

/**
  * @brief      Force DSRC in TX state
  *
  * @param[in]  dsrc    The pointer of the DSRC module
  *
  * @return     None
  */
#define DSRC_FORCE_TX(dsrc)             ((dsrc)->CTL |= DSRC_CTL_FORCETX_Msk)

/**
  * @brief      Set DSRC RX On
  *
  * @param[in]  dsrc    The pointer of the DSRC module
  *
  * @return     None
  *
  * @note       This function can be work well only if SWRXEN (DSRC_CTL[1] Software Control RX_ON Enable Bit) is enabled.
  */
#define DSRC_RXON(dsrc)                 ((dsrc)->CTL |= DSRC_CTL_RXON_Msk)

/**
  * @brief      Set DSRC RX Off
  *
  * @param[in]  dsrc    The pointer of the DSRC module
  *
  * @return     None
  */
#define DSRC_RXOFF(dsrc)                ((dsrc)->CTL &= ~(DSRC_CTL_RXON_Msk))

/**
  * @brief      Set Wakeup Pin Trigger type
  *
  * @param[in]  dsrc    The pointer of the DSRC module
  * @param[in]  type    Decides which trigger type for DSRC wakeup pin, valid parameters are:
  *                         - \ref DSRC_CTL_WKPOL_RISING
  *                         - \ref DSRC_CTL_WKPOL_FALLING
  *                         - \ref DSRC_CTL_WKPOL_BOTH
  *
  * @return     None
  */
#define DSRC_WKPOL_TRIGGER(dsrc, type)  ((dsrc)->CTL = ((dsrc)->CTL & ~(DSRC_CTL_WKPOL_Msk)) | (type))

/**
  * @brief      Set DSRC Timer2 Timeout Period
  *
  * @param[in]  dsrc        The pointer of the DSRC module
  * @param[in]  prescale    Prescale counter, it is should be 0~255
  * @param[in]  compared    Timer compared value, it is should be 2~0xFFFFFF
  *
  * @return     None
  */
#define DSRC_SET_TMR2_TIMEOUT(dsrc, prescale, compared) ((dsrc)->TMR2 = ((prescale)<<DSRC_TMR2_PSC_Pos)|(compared))

/**
  * @brief      Set DSRC Timer3 Timeout Period
  *
  * @param[in]  dsrc        The pointer of the DSRC module
  * @param[in]  prescale    Prescale counter, it is should be 0~255
  * @param[in]  compared    Timer compared value, it is should be 2~0xFFFFFF
  *
  * @return     None
  */
#define DSRC_SET_TMR3_TIMEOUT(dsrc, prescale, compared) ((dsrc)->TMR3 = ((prescale)<<DSRC_TMR3_PSC_Pos)|(compared))

/**
  * @brief      Set DSRC Timer4 Timeout Period
  *
  * @param[in]  dsrc        The pointer of the DSRC module
  * @param[in]  prescale    Prescale counter, it is should be 0~255
  * @param[in]  compared    Timer compared value, it is should be 2~0xFFFFFF
  *
  * @return     None
  */
#define DSRC_SET_TMR4_TIMEOUT(dsrc, prescale, compared) ((dsrc)->TMR4 = ((prescale)<<DSRC_TMR4_PSC_Pos)|(compared))

/**
  * @brief      Set CRC Format
  *
  * @param[in]  dsrc        The pointer of the DSRC module
  * @param[in]  u32CRCMSB   Decides CRC generation start data format. 0 means LSB; 1 means MSB
  * @param[in]  u32CRCREV   Decides CRC checksum reverse. 0 to disable; 1 to enable
  * @param[in]  u32CRCFMT   Decides 1'complement for CRC checksum. 0 to disable; 1 to enable
  * @param[in]  u32CRCBSWAP Decides received CRC value byte swap. 0 to disable; 1 to enable
  *
  * @return     None
  */
#define DSRC_SET_CRC_FMT(dsrc, u32CRCMSB, u32CRCREV, u32CRCFMT, u32CRCBSWAP) \
            ( (dsrc)->CTL = ((dsrc)->CTL & ~(DSRC_CTL_CRCMSB_Msk|DSRC_CTL_CHKSREV_Msk|DSRC_CTL_CHKSFMT_Msk|DSRC_CTL_CRCBSWAP_Msk)) | \
                            ( ((u32CRCMSB)<<DSRC_CTL_CRCMSB_Pos) | ((u32CRCREV)<<DSRC_CTL_CHKSREV_Pos) | \
                              ((u32CRCFMT)<<DSRC_CTL_CHKSFMT_Pos) | ((u32CRCBSWAP)<<DSRC_CTL_CRCBSWAP_Pos)) )

/**
  * @brief      Set Bit Rate Accuracy Range
  *
  * @param[in]  dsrc        The pointer of the DSRC module
  * @param[in]  u32Val      Bit rate accuracy range, valid value is between 0x0 ~ 0x4.
  *
  * @return     None
  *
  * @note       This function is only available for FM0 CODEC, and bit rate accuracy range are shown in following table:
  * -
  * |    |        | DSRC clock 4MHz |     |        | DSRC clock 12MHz|
  * | :--|:------ |:--------------- | :-- |:------ |:--------------- |
  * | 0  | >75%   | 204 ~ 341 kH    |  0  | >75%   | 204 ~ 341 kHz   |
  * | 1  | >75%   | 204 ~ 341 kH    |  1  | >80%   | 213 ~ 320 kHz   |
  * | 2  | >87.5% | 227 ~ 292 kHz   |  2  | >84%   | 220 ~ 304 kHz   |
  * | 3  | >87.5% | 227 ~ 292 kHz   |  3  | >87.5% | 227 ~ 292 kHz   |
  * | 4  | X      |   Reserved      |  4  | >71%   | 198 ~ 355 kHz   |
  */
#define DSRC_BITRATE_ACC_CTL(dsrc, u32Val)  ((dsrc)->CTL = ((dsrc)->CTL & ~(DSRC_CTL_BRATEACC_Msk)) | ((u32Val)<<DSRC_CTL_BRATEACC_Pos))

/**
  * @brief      Set FM0 CODEC Deglitch
  *
  * @param[in]  dsrc        The pointer of the DSRC module
  * @param[in]  u32Val      Select FM0 CODEC deglitch setting, valid value is between 0x0 ~ 0x5 as
  *                             0 = disable to the codec deglitch selection.
  *                             1 = Filter the glitches that the width is 0.25us or less.
  *                             2 = Filter the glitches that the width is 0.50us or less.
  *                             3 = Filter the glitches that the width is 0.75us or less.
  *                             4 = Filter the glitches that the width is 1.00us or less.
  *                             5 = Filter the glitches that the width is 1.25us or less.
  *
  * @return     None
  */
#define DSRC_SET_DEGLITCH(dsrc, u32Val) ((dsrc)->CTL2 = ((dsrc)->CTL2 & ~DSRC_CTL2_CODECDEGSEL_Msk) | ((u32Val)<<DSRC_CTL2_CODECDEGSEL_Pos))


/**
  * @brief      Enable DRSC CRC
  *
  * @param[in]  dsrc        The pointer of the DSRC module
  *
  * @return     None
  */
#define DSRC_ENABLE_CRC(dsrc)   ((dsrc)->CTL |= DSRC_CTL_CRCEN_Msk)

/**
  * @brief      Disable DRSC CRC
  *
  * @param[in]  dsrc        The pointer of the DSRC module
  *
  * @return     None
  */
#define DSRC_DISABLE_CRC(dsrc)  ((dsrc)->CTL &= ~DSRC_CTL_CRCEN_Msk)

/**
  * @brief      Enable DSRC TPB
  *
  * @param[in]  dsrc        The pointer of the DSRC module
  *
  * @return     None
  */
#define DSRC_ENABLE_TBP(dsrc)   ((dsrc)->CTL |= DSRC_CTL_TBPEN_Msk)

/**
  * @brief      Disable DSRC TPB
  *
  * @param[in]  dsrc        The pointer of the DSRC module
  *
  * @return     None
  */
#define DSRC_DISABLE_TBP(dsrc)  ((dsrc)->CTL &= ~DSRC_CTL_TBPEN_Msk)


/**
  * @brief      Enable Software Control RX_ON
  *
  * @param[in]  dsrc        The pointer of the DSRC module
  *
  * @return     None
  */
#define DSRC_ENABLE_SWRX(dsrc)  ((dsrc)->CTL |= DSRC_CTL_SWRXEN_Msk)

/**
  * @brief      Disable Software Control RX_ON
  *
  * @param[in]  dsrc        The pointer of the DSRC module
  *
  * @return     None
  */
#define DSRC_DISABLE_SWRX(dsrc) ((dsrc)->CTL &= ~DSRC_CTL_SWRXEN_Msk)

/**
  * @brief  Set CRC Seed Mode.
  * @param[in]  dsrc        The pointer of the DSRC module
  * @param[in]  u32Val      Select transmit CRC seed initial value. 0 is : Seed0 1: Seed1
  *                         0 is CRCSEED0 (DSRC_CRCSEED[15:0]), 1 CRCSEED0 (DSRC_CRCSEED[31:15]).
  *
  * @return     None
  */
#define DSRC_SET_CRC_SEED(dsrc, u32Val) ((dsrc)->CTL = ((dsrc)->CTL & ~DSRC_CTL_SEEDM_Msk)| ((u32Val)<<DSRC_CTL_SEEDM_Pos))


void DSRC_Open(DSRC_T *dsrc, uint32_t u32Mode, uint32_t u32crcEn, uint32_t u32pdmaEn);
void DSRC_SetPreamble(DSRC_T *dsrc, uint32_t u32Mode, uint32_t u32Val);
void DSRC_Close(DSRC_T *dsrc);

/*@}*/ /* end of group DSRC_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group DSRC_Driver */

/*@}*/ /* end of group Standard_Driver */

#ifdef __cplusplus
}
#endif

#endif /* __DSRC_H__ */

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
