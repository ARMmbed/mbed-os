/**
******************************************************************************
* @file rfAna.c
* @brief Implementation of rfAna hw module functions
* @internal
* @author ON Semiconductor
* $Rev: 3445 $
* $Date: 2015-06-22 13:51:24 +0530 (Mon, 22 Jun 2015) $
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
* @ingroup rfAna
*
* @details
*
* <h1> Reference document(s) </h1>
*/

/*************************************************************************************************
*                                                                                                *
*  Header files                                                                                  *
*                                                                                                *
*************************************************************************************************/

#include "memory_map.h"
#include "rfAna.h"
#include "clock.h"

/*************************************************************************************************
*                                                                                                *
*  Global variables                                                                              *
*                                                                                                *
*************************************************************************************************/

/** Rf channel and tx power lookup tables (constant)
 * @details
 *
 * The rf channel table is used to program internal hardware register for different 15.4 rf channels.
 * It has 16 entries corresponding to 16 15.4 channels.
 *    Entry 1 <-> Channel 11
 *    ...
 *    Entry 16 <-> Channel 26
 *
 * Each entry is compound of 4 items.
 *       Item 0: Rx Frequency integer divide portion
 *       Item 1: Rx Frequency fractional divide portion
 *       Item 2: Tx Frequency integer divide portion
 *       Item 3: Tx Frequency fractional divide portion
 *
 * The tx power table is used to program internal hardware register for different 15.4 tx power levels.
 * It has 43 entries corresponding to tx power levels from -32dBm to +10dBm.
 *    Entry 1 <-> -32dB
 *    Entry 2 <-> -31dB
 *    ...
 *    Entry 2 <-> 9dB
 *    Entry 43 <-> +10dB
 *
 * Each entry is compound of 1 byte.
 */

// RR: Making high side injection changes to RevD

/** This rf LUT is built for high side injection, using low side injection
 * would requiere to change this LUT. */
const uint32_t rfLut[16][4] =  {{0x50,0x00D4A7,0x4B,0x00A000},
    {0x50,0x017F52,0x4B,0x014001},
    {0x51,0xFE29FB,0x4B,0x01E001},
    {0x51,0xFED4A6,0x4C,0xFE7FFF},
    {0x51,0xFF7F51,0x4C,0xFF1FFF},
    {0x51,0x0029FC,0x4C,0xFFC000},
    {0x51,0x00D4A7,0x4C,0x006000},
    {0x51,0x017F52,0x4C,0x010001},
    {0x52,0xFE29FB,0x4C,0x01A001},
    {0x52,0xFED4A6,0x4D,0xFE3FFF},
    {0x52,0xFF7F51,0x4D,0xFEDFFF},
    {0x52,0x0029FC,0x4D,0xFF8000},
    {0x52,0x00D4A7,0x4D,0x002000},
    {0x52,0x017F52,0x4D,0x00C001},
    {0x53,0xFE29FB,0x4D,0x016001},
    {0x53,0xFED4A6,0x4E,0xFDFFFE}
};

const uint8_t txPowerLut[43]  =  {0,0,0, // -32dBm to -30dBm
                                  0,0,0,0,0,0,0,0,0,0, // -29dBm to -20dBm
                                  0,0,0,0,0,0,0,0,1,2, // -19dBm to -10dBm
                                  3,4,5,6,7,8,9,10,11,12, // -9dBm to 0dBm
                                  13,14,15,16,17,18,19,20,20,20
                                 }; // +1dBm to +10 dBm

/*************************************************************************************************
*                                                                                                *
*  Functions                                                                                     *
*                                                                                                *
*************************************************************************************************/

void fRfAnaInit()
{
    // Enable rfana clock
    CLOCK_ENABLE(CLOCK_RFANA);

    // Set PLL timing
    RFANAREG->PLL_TIMING.BITS.PLL_RESET_TIME = 0x1E; // 30us
    RFANAREG->PLL_TIMING.BITS.PLL_LOCK_TIME = 0x2F; // 47us

    // Set other parameters
    RFANAREG->RX_CONTROL.BITS.LNA_GAIN_MODE = 0x1; // High Gain mode
    RFANAREG->RX_CONTROL.BITS.ADC_DITHER_MODE = 0x0; // Dither mode disabled
}

boolean fRfAnaIoctl (uint32_t request, void *argument)
{
    uint8_t channel, txPower;

    // Enable rfana clock (in case fRfAnaIoctl is used before call of fRfAnaInit)
    CLOCK_ENABLE(CLOCK_RFANA);

    switch(request) {
        case SET_RF_CHANNEL:
            channel = *(uint8_t*)argument;

            // Set tx/rx integer/fractional divide portions
            RFANAREG->TX_LO_CONTROL.BITS.FRACT_WORD = rfLut[channel - 11][3];
            RFANAREG->TX_LO_CONTROL.BITS.INT_WORD = rfLut[channel - 11][2];
            RFANAREG->RX_LO_CONTROL.BITS.FRACT_WORD = rfLut[channel - 11][1];
            RFANAREG->RX_LO_CONTROL.BITS.INT_WORD = rfLut[channel - 11][0];

            // Set tx/rx vco trims
            /** REVD is requiering to adjust tx/rx vco trims each time a new 15.4 channel is used, in revB it is done
             * from trims stored in dedicated registers available in digital.*/
            if (channel < 19) {
                RFANATRIMREG->PLL_TRIM.BITS.TX_VCO_TRIM = (RFANATRIMREG->TX_VCO_TRIM_LUT1) >> ((channel - 11) * 4);
                RFANATRIMREG->PLL_TRIM.BITS.RX_VCO_TRIM = (RFANATRIMREG->RX_VCO_TRIM_LUT1) >> ((channel - 11) * 4);
            } else {
                RFANATRIMREG->PLL_TRIM.BITS.TX_VCO_TRIM = (RFANATRIMREG->TX_VCO_TRIM_LUT2) >> ((channel - 19) * 4);
                RFANATRIMREG->PLL_TRIM.BITS.RX_VCO_TRIM = (RFANATRIMREG->RX_VCO_TRIM_LUT2) >> ((channel - 19) * 4);
            }
            break;
        case SET_TX_POWER:
            txPower = *(uint8_t*)argument;

            // Set tx power register
            if ((txPower & 0x20) == 0) {
                RFANAREG->TX_POWER = (txPowerLut[txPower + 32] & 0xFF);
            } else {
                RFANAREG->TX_POWER = (txPowerLut[txPower - 32] & 0xFF);
            }

            break;
        default:
            return False;
    }
    return True;
}
