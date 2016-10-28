/**
******************************************************************************
* @file pad.c
* @brief PAD management support code
* @internal
* @author ON Semiconductor
* $Rev: 2848 $
* $Date: 2014-04-01 22:48:18 +0530 (Tue, 01 Apr 2014) $
******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
*
* THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
* ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
* INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
* @endinternal
*
* @ingroup pad
*
* @details
*/

/*************************************************************************************************
*                                                                                                *
*  Header files                                                                                  *
*                                                                                                *
*************************************************************************************************/

#include "memory_map.h"
#include "pad_map.h"
#include "clock.h"

/*************************************************************************************************
*                                                                                                *
*  Symbolic Constants                                                                            *
*                                                                                                *
*************************************************************************************************/
#define PAD_CTRL_OP_DRIVE_STRENGTH_MASK    (uint32_t)0x1C
#define PAD_NUM_OF_IO                      17
#define PAD_OP_DRIVE_STRGTH_MAX            7
#define PAD_OP_DRIVE_TYPE_MAX              1
#define PAD_OP_PULL_TYPE_MAX               3

#define PAD_REG_ADRS_BYTE_SIZE             4

#define PAD_OP_DRIVE_STRGTH_BIT_POS        2
#define PAD_OP_DRIVE_TYPE_BIT_POS          5
#define PAD_OP_PULL_TYPE_BIT_POS           0

/*************************************************************************************************
*                                                                                                *
*  Global variables                                                                              *
*                                                                                                *
*************************************************************************************************/

/* Peripheral PAD register mutex */
/* sem_pt GlobMutexPadReg; */

/*************************************************************************************************
*                                                                                                *
*  Functions                                                                                     *
*                                                                                                *
*************************************************************************************************/

/** Find description at pad.h */
void fPadInit()
{
    /** - Enable the clock for PAD peripheral device */
    CLOCK_ENABLE(CLOCK_PAD);

    /** - Set pad parameters, output drive strength, pull piece control, output drive type */
    PADREG->PADIO0.WORD = PAD_OUTPUT_PN_L1_OD;       /* UART1 TXD */
    PADREG->PADIO1.WORD = PAD_INPUT_PD_L1_PP;        /* UART1 RXD */
    PADREG->PADIO2.WORD = PAD_INPUT_PD_L1_PP;        /* UART1 CTS */
    PADREG->PADIO3.WORD = PAD_OUTPUT_PN_L1_OD;       /* UART1 RTS */
    PADREG->PADIO4.WORD = PAD_UNUSED_PD_L1_PP;
    PADREG->PADIO5.WORD = PAD_UNUSED_PD_L1_PP;
    PADREG->PADIO6.WORD = PAD_UNUSED_PD_L1_PP;
    PADREG->PADIO7.WORD = PAD_UNUSED_PD_L1_PP;
    PADREG->PADIO8.WORD = PAD_OUTPUT_PN_L1_OD;       /* UART2 TXD */
    PADREG->PADIO9.WORD = PAD_INPUT_PD_L1_PP;        /* UART2 RXD */
    PADREG->PADIO10.WORD = PAD_UNUSED_PD_L1_PP;
    PADREG->PADIO11.WORD = PAD_INPUT_PD_L1_PP;       /* SWO */
    PADREG->PADIO12.WORD = PAD_INPUT_PD_L1_PP;       /* SWCLK */
    PADREG->PADIO13.WORD = PAD_INPUT_PD_L1_PP;       /* SWDIO */
    PADREG->PADIO14.WORD = PAD_INPUT_PD_L1_PP;
    PADREG->PADIO15.WORD = PAD_UNUSED_PD_L1_PP;
    PADREG->PADIO16.WORD = PAD_UNUSED_PD_L1_PP;
    PADREG->PADIO17.WORD = PAD_UNUSED_PD_L1_PP;

    /** - Disable the clock for PAD peripheral device */
    CLOCK_DISABLE(CLOCK_PAD);

}

/** Find description at pad.h */
boolean fPadIOCtrl(uint8_t PadNum, uint8_t OutputDriveStrength, uint8_t OutputDriveType, uint8_t PullType)
{
    PadReg_t *PadRegOffset;
    /** \verbatim
        Table: O/p drive strength

            Drive strength    3.3V (min/typ/max)    1V (min/typ/max)
            000               1/1.4/2.1 mA        0.043/0.07/0.11 mA
            001               2/2.7/4.1 mA        0.086/0.15/0.215 mA
            010               4.1/5.3/7.8 mA        0.188/0.3/0.4 mA
            011               8.1/10.4/15 8 mA    0.4/0.6/0.81 mA
            100               20.8/26/37 mA*        1/1.6/2.2 mA
            101               40.5/50/70 mA*        2/3/4.3 mA
            11x               57/73/102 mA*       3/4.6/6.2 mA

        *Values are only accessible when CDBGPWRUPREQ is high.  This limits the maximum output current in functional mode. \endverbatim */


    if((PadNum  <= PAD_NUM_OF_IO) &&
            (OutputDriveStrength <= PAD_OP_DRIVE_STRGTH_MAX) &&
            (OutputDriveType <= PAD_OP_DRIVE_TYPE_MAX) && (PullType <= PAD_OP_PULL_TYPE_MAX)) {
        /** - Get PAD IO register address for the PAD number */
        PadRegOffset = (PadReg_t*)(PADREG_BASE + (PadNum * PAD_REG_ADRS_BYTE_SIZE));

        /** - Enable the clock for PAD peripheral device */
        CLOCK_ENABLE(CLOCK_PAD);

        /** - Set drive type, pulltype & drive strength */
        PadRegOffset->PADIO0.WORD = (uint32_t)((PullType << PAD_OP_PULL_TYPE_BIT_POS) |
                                               (OutputDriveStrength << PAD_OP_DRIVE_STRGTH_BIT_POS) |
                                               (OutputDriveType << PAD_OP_DRIVE_TYPE_BIT_POS));

        /** - Disable the clock for PAD peripheral device */
        CLOCK_DISABLE(CLOCK_PAD);
        return True;
    }
    /* Invalid parameter/s */
    return False;
}