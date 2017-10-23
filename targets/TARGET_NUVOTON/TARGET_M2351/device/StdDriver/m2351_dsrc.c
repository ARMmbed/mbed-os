/**************************************************************************//**
 * @file     dsrc.c
 * @version  V3.00
 * @brief    Dedicated Short Range Communication Controller(DSRC) driver source file
 *
 * @note
 * Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include "M2351.h"


/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup DSRC_Driver DSRC Driver
  @{
*/

/** @addtogroup DSRC_EXPORTED_FUNCTIONS DSRC Exported Functions
  @{
*/

/**
  * @brief      Open DSRC Module
  *
  * @param[in]  dsrc        The pointer of the DSRC module
  * @param[in]  u32Mode     Decides the DSRC operating mode. Valid values are:
  *                         - \ref DSRC_MODE_FM0
  *                         - \ref DSRC_MODE_MONCHESTER
  *                         - \ref DSRC_MODE_SPI
  * @param[in]  u32crcEn    Set 0 to disable CRC engine; set 1 to enable CRC engine
  * @param[in]  u32pdmaEn   Set 0 to disable PDMA engine; set 1 to enable PDMA engine
  *
  * @return     None
  *
  * @details    This function make DSRC module be ready to transfer.
  */
void DSRC_Open(DSRC_T *dsrc, uint32_t u32Mode, uint32_t u32crcEn, uint32_t u32pdmaEn)
{
    dsrc->CTL = (dsrc->CTL & ~DSRC_MODE_Msk) | u32Mode;

    /* Enable DSRC */
    dsrc->CTL |= DSRC_CTL_DSRCEN_Msk;

    /* Enable TBP */
    DSRC_ENABLE_TBP(dsrc);

    if(u32crcEn == 1UL)
        DSRC_ENABLE_CRC(dsrc);  /* Enable CRC */
    else
        DSRC_DISABLE_CRC(dsrc); /* Disable CRC */

    if(u32pdmaEn == 1UL)
    {
        /* Enable RX PDMA */
        DSRC_ENABLE_RX_PDMA(dsrc);
        /* Enable TX PDMA */
        DSRC_ENABLE_TX_PDMA(dsrc);
    }
    else
    {
        /* Disable RX PDMA */
        dsrc->CTL &= ~(DSRC_CTL_TRDMAEN_Msk);
        /* Enable TX PDMA */
        dsrc->CTL &= ~(DSRC_CTL_TTDMAEN_Msk);
    }
}

/**
  * @brief      Close DSRC Module
  *
  * @param[in]  dsrc    The pointer of the DSRC module
  *
  * @return     None
  *
  * @details    This function is used to disable DSRC module.
  */
void DSRC_Close(DSRC_T *dsrc)
{
    dsrc->CTL &= ~DSRC_CTL_DSRCEN_Msk;
}

/**
  * @brief      Set Preamble
  *
  * @param[in]  dsrc    The pointer of the DSRC module
  * @param[in]  u32Mode Decides the premable size of DSRC module. Valid values are:
  *                     - \ref DSRC_CTL_PREAMFMT_16, the premable pattern is u32Val[15:0]
  *                     - \ref DSRC_CTL_PREAMFMT_32, the premable pattern is u32Val[31:0]
  * @param[in]  u32Val  Preamble pattern value
  *
  * @return     None
  *
  * @details    This function is used to decide the premable size and pattern of DSRC module.
  *             The valid premable pattern is selected by u32Mode setting.
  */
void DSRC_SetPreamble(DSRC_T *dsrc, uint32_t u32Mode, uint32_t u32Val)
{
    dsrc->CTL = (dsrc->CTL & ~DSRC_CTL_PREAMFMT_Msk) | (u32Mode);
    dsrc->PREAMBLE = u32Val;
}

/*@}*/ /* end of group DSRC_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group DSRC_Driver */

/*@}*/ /* end of group Standard_Driver */

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
