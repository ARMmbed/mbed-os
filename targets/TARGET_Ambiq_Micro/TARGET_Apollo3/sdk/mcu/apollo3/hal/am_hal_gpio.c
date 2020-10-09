//*****************************************************************************
//
//  am_hal_gpio.c
//! @file
//!
//! @brief Functions for interfacing with the GPIO module
//!
//! @addtogroup gpio3 GPIO
//! @ingroup apollo3hal
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2020, Ambiq Micro
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// Third party software included in this distribution is subject to the
// additional license terms as defined in the /docs/licenses directory.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision 2.4.2 of the AmbiqSuite Development Package.
//
//*****************************************************************************
// SPDX-License-Identifier: BSD-3-Clause

#include <stdint.h>
#include <stdbool.h>
#include "am_mcu_apollo.h"

//*****************************************************************************
//  Local defines.
//*****************************************************************************
//
// Generally define GPIO PADREG and GPIOCFG bitfields
//
#define PADREG_FLD_76_S         6
#define PADREG_FLD_FNSEL_S      3
#define PADREG_FLD_DRVSTR_S     2
#define PADREG_FLD_INPEN_S      1
#define PADREG_FLD_PULLUP_S     0

#define GPIOCFG_FLD_INTD_S      3
#define GPIOCFG_FLD_OUTCFG_S    1
#define GPIOCFG_FLD_INCFG_S     0

//*****************************************************************************
//
// Globals
//
//*****************************************************************************
//*****************************************************************************
//  Define some common GPIO configurations.
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_HAL_GPIO_DISABLE =
{
    .uFuncSel       = 3,
    .eDriveStrength = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA,
    .eGPOutcfg      = AM_HAL_GPIO_PIN_OUTCFG_DISABLE
};

const am_hal_gpio_pincfg_t g_AM_HAL_GPIO_TRISTATE =
{
    .uFuncSel       = 3,
    .eDriveStrength = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA,
    .eGPOutcfg      = AM_HAL_GPIO_PIN_OUTCFG_TRISTATE
};

const am_hal_gpio_pincfg_t g_AM_HAL_GPIO_INPUT =
{
    .uFuncSel       = 3,
    .eGPOutcfg      = AM_HAL_GPIO_PIN_OUTCFG_DISABLE,
    .eGPInput       = AM_HAL_GPIO_PIN_INPUT_ENABLE,
    .eGPRdZero      = AM_HAL_GPIO_PIN_RDZERO_READPIN
};

//
// Input with various pullups (weak, 1.5K, 6K, 12K, 24K)
// The 1.5K - 24K pullup values are valid for select I2C enabled pads.
// For Apollo3 these pins are 0-1,5-6,8-9,25,27,39-40,42-43,48-49.
// The "weak" value is used for almost every other pad except pin 20.
//
const am_hal_gpio_pincfg_t g_AM_HAL_GPIO_INPUT_PULLUP =
{
    .uFuncSel       = 3,
    .eGPOutcfg      = AM_HAL_GPIO_PIN_OUTCFG_DISABLE,
    .eGPInput       = AM_HAL_GPIO_PIN_INPUT_ENABLE,
    .eGPRdZero      = AM_HAL_GPIO_PIN_RDZERO_READPIN,
    .ePullup        = AM_HAL_GPIO_PIN_PULLUP_WEAK
};

const am_hal_gpio_pincfg_t g_AM_HAL_GPIO_INPUT_PULLUP_1_5 =
{
    .uFuncSel       = 3,
    .eGPOutcfg      = AM_HAL_GPIO_PIN_OUTCFG_DISABLE,
    .eGPInput       = AM_HAL_GPIO_PIN_INPUT_ENABLE,
    .eGPRdZero      = AM_HAL_GPIO_PIN_RDZERO_READPIN,
    .ePullup        = AM_HAL_GPIO_PIN_PULLUP_1_5K
};

const am_hal_gpio_pincfg_t g_AM_HAL_GPIO_INPUT_PULLUP_6 =
{
    .uFuncSel       = 3,
    .eGPOutcfg      = AM_HAL_GPIO_PIN_OUTCFG_DISABLE,
    .eGPInput       = AM_HAL_GPIO_PIN_INPUT_ENABLE,
    .eGPRdZero      = AM_HAL_GPIO_PIN_RDZERO_READPIN,
    .ePullup        = AM_HAL_GPIO_PIN_PULLUP_6K
};

const am_hal_gpio_pincfg_t g_AM_HAL_GPIO_INPUT_PULLUP_12 =
{
    .uFuncSel       = 3,
    .eGPOutcfg      = AM_HAL_GPIO_PIN_OUTCFG_DISABLE,
    .eGPInput       = AM_HAL_GPIO_PIN_INPUT_ENABLE,
    .eGPRdZero      = AM_HAL_GPIO_PIN_RDZERO_READPIN,
    .ePullup        = AM_HAL_GPIO_PIN_PULLUP_12K
};

const am_hal_gpio_pincfg_t g_AM_HAL_GPIO_INPUT_PULLUP_24 =
{
    .uFuncSel       = 3,
    .eGPOutcfg      = AM_HAL_GPIO_PIN_OUTCFG_DISABLE,
    .eGPInput       = AM_HAL_GPIO_PIN_INPUT_ENABLE,
    .eGPRdZero      = AM_HAL_GPIO_PIN_RDZERO_READPIN,
    .ePullup        = AM_HAL_GPIO_PIN_PULLUP_24K
};

//
// Variations of output (drive strengths, read, etc)
//
const am_hal_gpio_pincfg_t g_AM_HAL_GPIO_OUTPUT =
{
    .uFuncSel       = 3,
    .eDriveStrength = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA,
    .eGPOutcfg      = AM_HAL_GPIO_PIN_OUTCFG_PUSHPULL
};

const am_hal_gpio_pincfg_t g_AM_HAL_GPIO_OUTPUT_4 =
{
    .uFuncSel       = 3,
    .eDriveStrength = AM_HAL_GPIO_PIN_DRIVESTRENGTH_4MA,
    .eGPOutcfg      = AM_HAL_GPIO_PIN_OUTCFG_PUSHPULL
};

const am_hal_gpio_pincfg_t g_AM_HAL_GPIO_OUTPUT_8 =
{
    .uFuncSel       = 3,
    .eDriveStrength = AM_HAL_GPIO_PIN_DRIVESTRENGTH_8MA,
    .eGPOutcfg      = AM_HAL_GPIO_PIN_OUTCFG_PUSHPULL
};

const am_hal_gpio_pincfg_t g_AM_HAL_GPIO_OUTPUT_12 =
{
    .uFuncSel       = 3,
    .eDriveStrength = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eGPOutcfg      = AM_HAL_GPIO_PIN_OUTCFG_PUSHPULL
};

const am_hal_gpio_pincfg_t g_AM_HAL_GPIO_OUTPUT_WITH_READ =
{
    .uFuncSel       = 3,
    .eDriveStrength = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA,
    .eGPOutcfg      = AM_HAL_GPIO_PIN_OUTCFG_PUSHPULL,
    .eGPInput       = AM_HAL_GPIO_PIN_INPUT_ENABLE,
    .eGPRdZero      = AM_HAL_GPIO_PIN_RDZERO_READPIN
};

//*****************************************************************************
//
//  g_ui8Inpen[]
//  This lookup table determines whether the INPEN bit is required based on
//  the pin number and FNSEL.
//
//*****************************************************************************
static const uint8_t
g_ui8Inpen[AM_HAL_GPIO_MAX_PADS] =
{
    //0     1     2     3     4     5     6     7     8     9
    0x23, 0x23, 0x27, 0x62, 0xA1, 0x03, 0x87, 0x10, 0x03, 0x53, // Pins 0-9
    0x00, 0xE1, 0x51, 0x81, 0x41, 0x55, 0x05, 0xC4, 0x80, 0x40, // Pins 10-19
    0x01, 0xB1, 0x40, 0x41, 0x14, 0x31, 0xA0, 0x31, 0x00, 0xF1, // Pins 20-29
    0x80, 0x11, 0x91, 0x21, 0xC1, 0x11, 0xE5, 0x11, 0x45, 0x30, // Pins 30-39
    0x37, 0x00, 0x30, 0x31, 0x00, 0x71, 0x00, 0x40, 0x30, 0x31  // Pins 40-49
};

//*****************************************************************************
//
//  g_ui8Bit76Capabilities[]
//  This lookup table specifies capabilities of each pad for PADREG bits 7:6.
//
//*****************************************************************************
#define CAP_PUP     0x01    // PULLUP
#define CAP_PDN     0x08    // PULLDOWN (pin 20 only)
#define CAP_VDD     0x02    // VDD PWR (power source)
#define CAP_VSS     0x04    // VSS PWR (ground sink)
#define CAP_RSV     0x80    // bits 7:6 are reserved for this pin
static const uint8_t
g_ui8Bit76Capabilities[AM_HAL_GPIO_MAX_PADS] =
{
    //0        1        2        3        4        5        6        7        8        9
    CAP_PUP, CAP_PUP, CAP_RSV, CAP_VDD, CAP_RSV, CAP_PUP, CAP_PUP, CAP_RSV, CAP_PUP, CAP_PUP,   // Pins 0-9
    CAP_RSV, CAP_RSV, CAP_RSV, CAP_RSV, CAP_RSV, CAP_RSV, CAP_RSV, CAP_RSV, CAP_RSV, CAP_RSV,   // Pins 10-19
    CAP_PDN, CAP_RSV, CAP_RSV, CAP_RSV, CAP_RSV, CAP_PUP, CAP_RSV, CAP_PUP, CAP_RSV, CAP_RSV,   // Pins 20-29
    CAP_RSV, CAP_RSV, CAP_RSV, CAP_RSV, CAP_RSV, CAP_RSV, CAP_VDD, CAP_VSS, CAP_RSV, CAP_PUP,   // Pins 30-39
    CAP_PUP, CAP_VSS, CAP_PUP, CAP_PUP, CAP_RSV, CAP_RSV, CAP_RSV, CAP_RSV, CAP_PUP, CAP_PUP    // Pins 40-49
};

//*****************************************************************************
//
// g_ui8nCEpins[]
// This lookup table lists the nCE funcsel value as a function of the pin.
//  Almost every pad has a nCE function (except for 4 pads).  Every one of those
//  nCE functions can select a polarity (active low or high) via the INTD field.
// All non-nCE functions use INCFG and INTD to select interrupt transition types.
// A lookup will return 0-7 if the pin supports nCE, and 8 if it does not.
//
// The truth table summarizes behavior.  For the purposes of this table, assume
//  "A" is the funcsel that selects nCE (and thus polarity is needed) for the
//  given pad.  Then "!A" is any other funcsel and selects interrupt transition.
//
//  funcsel     INCFG       INTD        Behavior
//    !A        0           0           Interrupt on L->H transition.
//    !A        0           1           Interrupt on H->L transition.
//    !A        1           0           No interrupts.
//    !A        1           1           Interrupt either direction.
//     A        x           0           nCE polarity active low.
//     A        x           1           nCE polarity active high.
//
//*****************************************************************************
static const uint8_t
g_ui8nCEpins[AM_HAL_GPIO_MAX_PADS] =
{
    // 0     1     2     3     4     5     6     7     8     9
    0x07, 0x07, 0x07, 0x02, 0x02, 0x08, 0x08, 0x00, 0x02, 0x02,     // Pads 0-9
    0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,     // Pads 10-19
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,     // Pads 20-29
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x08,     // Pads 30-39
    0x08, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01      // Pads 40-49
};

//*****************************************************************************
//
// g_ui8NCEtable[]
// This lookup table lists all available NCEs. It basically reproduces the
//  "NCE Encoding Table" from the datasheet.
// The format of this table is:
//  High nibble=IOM number; 0-5, MSPI=6 (IOMNUM_MSPI).
//  Low nibble=CE number (0-3).
//  Every 4 bytes (word) represent the next GPIO number/index.
//
//*****************************************************************************
static const uint8_t
g_ui8NCEtable[AM_HAL_GPIO_MAX_PADS][4] =
{
     // 0       1       2       3    = OUTCFG
    {0x32,   0x42,   0x52,   0x13},  // NCE0
    {0x02,   0x12,   0x22,   0x60},  // NCE1
    {0x33,   0x43,   0x53,   0x21},  // NCE2
    {0x30,   0x40,   0x50,   0x20},  // NCE3
    {0x31,   0x41,   0x51,   0x11},  // NCE4
    {0xFF,   0xFF,   0xFF,   0xFF},  // NCE5
    {0xFF,   0xFF,   0xFF,   0xFF},  // NCE6
    {0x31,   0x41,   0x51,   0x60},  // NCE7
    {0x30,   0x40,   0x50,   0x00},  // NCE8
    {0x33,   0x43,   0x53,   0x23},  // NCE9
    {0x32,   0x42,   0x52,   0x60},  // NCE10
    {0x00,   0x10,   0x20,   0x30},  // NCE11
    {0x30,   0x40,   0x50,   0x61},  // NCE12
    {0x31,   0x41,   0x51,   0x01},  // NCE13
    {0x02,   0x12,   0x22,   0x42},  // NCE14
    {0x03,   0x13,   0x23,   0x60},  // NCE15
    {0x00,   0x10,   0x20,   0x50},  // NCE16
    {0x01,   0x11,   0x21,   0x41},  // NCE17
    {0x02,   0x12,   0x22,   0x32},  // NCE18
    {0x03,   0x13,   0x33,   0x60},  // NCE19
    {0x31,   0x41,   0x51,   0x21},  // NCE20
    {0x32,   0x42,   0x52,   0x22},  // NCE21
    {0x33,   0x43,   0x53,   0x03},  // NCE22
    {0x00,   0x10,   0x20,   0x40},  // NCE23
    {0x01,   0x11,   0x21,   0x51},  // NCE24
    {0x32,   0x42,   0x52,   0x02},  // NCE25
    {0x33,   0x43,   0x53,   0x13},  // NCE26
    {0x30,   0x40,   0x50,   0x10},  // NCE27
    {0x31,   0x41,   0x51,   0x60},  // NCE28
    {0x32,   0x42,   0x52,   0x12},  // NCE29
    {0x33,   0x43,   0x53,   0x03},  // NCE30
    {0x00,   0x10,   0x20,   0x40},  // NCE31
    {0x01,   0x11,   0x21,   0x61},  // NCE32
    {0x02,   0x12,   0x22,   0x52},  // NCE33
    {0x03,   0x13,   0x23,   0x33},  // NCE34
    {0x00,   0x10,   0x20,   0x30},  // NCE35
    {0x31,   0x41,   0x51,   0x61},  // NCE36
    {0x32,   0x42,   0x52,   0x02},  // NCE37
    {0x03,   0x13,   0x33,   0x53},  // NCE38
    {0xFF,   0xFF,   0xFF,   0xFF},  // NCE39
    {0xFF,   0xFF,   0xFF,   0xFF},  // NCE40
    {0x01,   0x11,   0x21,   0x61},  // NCE41
    {0x00,   0x10,   0x20,   0x50},  // NCE42
    {0x01,   0x11,   0x21,   0x61},  // NCE43
    {0x02,   0x12,   0x22,   0x52},  // NCE44
    {0x33,   0x43,   0x53,   0x13},  // NCE45
    {0x30,   0x40,   0x50,   0x61},  // NCE46
    {0x01,   0x11,   0x21,   0x31},  // NCE47
    {0x02,   0x12,   0x22,   0x32},  // NCE48
    {0x03,   0x13,   0x23,   0x43}   // NCE49
};

// declare ap3_gpio_get_pinconfig_bitmasks
void ap3_gpio_get_pinconfig_bitmasks(am_hal_gpio_pincfg_allow_t sAllowableChanges, uint8_t *padRegMask, uint8_t *GPCfgMask, uint8_t *altPadCfgMask);

//*****************************************************************************
//
// Array of function pointers for handling GPIO interrupts.
//
//*****************************************************************************
static am_hal_gpio_handler_t gpio_ppfnHandlers[AM_HAL_GPIO_MAX_PADS];
static void                  *gpio_pHandlerCtxt[AM_HAL_GPIO_MAX_PADS];

//*****************************************************************************
//
// Helper functions
//  popcount()   - Determine how many bits are set in the given bitmasks.
//  pincfg_equ() - compare 2 am_hal_gpio_pincfg_t structures for equality.
//
//*****************************************************************************
static bool
pincfg_equ(void *cfg1, void *cfg2)
{
    uint32_t ui32A, ui32B;

    //
    // We're assuming that am_hal_gpio_pincfg_t boils down to a uint32_t,
    // which is its intent.
    //
    ui32A = *((uint32_t*)cfg1);
    ui32B = *((uint32_t*)cfg2);

    return ui32A == ui32B ? true : false;

} // pincfg_equ()

static uint32_t
popcount(uint64_t ui64bitmask)
{
    uint32_t uCnt = 0;
    while ( ui64bitmask )
    {
        uCnt += ui64bitmask & 1;
        ui64bitmask >>= 1;
    }
    return uCnt;
} // popcount()

//*****************************************************************************
//
//! @brief Configure an Apollo3 pin.
//!
//! @param ui32Pin    - pin number to be configured.
//! @param ui32Config - Contains multiple descriptor fields.
//!
//! This function configures a pin according to the parameters in ui32Config.
//! All parameters are validated, and the given pin is configured according
//! to the designated parameters.
//!
//! @return Status.
//
//*****************************************************************************
uint32_t
am_hal_gpio_pinconfig(uint32_t ui32Pin, am_hal_gpio_pincfg_t bfGpioCfg)

{
    uint32_t ui32Padreg, ui32AltPadCfg, ui32GPCfg;
    uint32_t ui32Funcsel, ui32PowerSw;
    bool bClearEnable = false;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( ui32Pin >= AM_HAL_GPIO_MAX_PADS )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    //
    // Initialize the PADREG accumulator variables.
    //
    ui32GPCfg = ui32Padreg = ui32AltPadCfg = 0;

    //
    // Get the requested function and/or power switch.
    //
    ui32Funcsel = bfGpioCfg.uFuncSel;
    ui32PowerSw = bfGpioCfg.ePowerSw;

    ui32Padreg |= ui32Funcsel << PADREG_FLD_FNSEL_S;

    //
    // Check for invalid configuration requests.
    //
    if ( bfGpioCfg.ePullup != AM_HAL_GPIO_PIN_PULLUP_NONE )
    {
        //
        // This setting is needed for all pullup settings including
        // AM_HAL_GPIO_PIN_PULLUP_WEAK and AM_HAL_GPIO_PIN_PULLDOWN.
        //
        ui32Padreg |= (0x1 << PADREG_FLD_PULLUP_S);

        //
        // Check for specific pullup or pulldown settings.
        //
        if ( (bfGpioCfg.ePullup >= AM_HAL_GPIO_PIN_PULLUP_1_5K) &&
             (bfGpioCfg.ePullup <= AM_HAL_GPIO_PIN_PULLUP_24K) )
        {
            ui32Padreg |= ((bfGpioCfg.ePullup - AM_HAL_GPIO_PIN_PULLUP_1_5K) <<
                           PADREG_FLD_76_S);
#ifndef AM_HAL_DISABLE_API_VALIDATION
            if ( !(g_ui8Bit76Capabilities[ui32Pin] & CAP_PUP) )
            {
                return AM_HAL_GPIO_ERR_PULLUP;
            }
        }
        else if ( bfGpioCfg.ePullup == AM_HAL_GPIO_PIN_PULLDOWN )
        {
            if ( ui32Pin != 20 )
            {
                return AM_HAL_GPIO_ERR_PULLDOWN;
            }
        }
        else if ( bfGpioCfg.ePullup == AM_HAL_GPIO_PIN_PULLUP_WEAK )
        {
            //
            // All pads except 20 support a weak pullup, for which we only need
            // to set PADnPULL and clear 7:6 (already done at this point).
            //
            if ( ui32Pin == 20 )
            {
                return AM_HAL_GPIO_ERR_PULLUP;
            }
#endif // AM_HAL_DISABLE_API_VALIDATION
        }
    }

    //
    // Check if requesting a power switch pin
    //
    if ( ui32PowerSw != AM_HAL_GPIO_PIN_POWERSW_NONE )
    {
        if ( (ui32PowerSw == AM_HAL_GPIO_PIN_POWERSW_VDD)  &&
             (g_ui8Bit76Capabilities[ui32Pin] & CAP_VDD) )
        {
            ui32Padreg |= 0x1 << PADREG_FLD_76_S;
        }
        else if ( (ui32PowerSw == AM_HAL_GPIO_PIN_POWERSW_VSS)  &&
                  (g_ui8Bit76Capabilities[ui32Pin] & CAP_VSS) )
        {
            ui32Padreg |= 0x2 << PADREG_FLD_76_S;
        }
        else
        {
            return AM_HAL_GPIO_ERR_PWRSW;
        }
    }

    //
    // Depending on the selected pin and FNSEL, determine if INPEN needs to be set.
    //
    ui32Padreg |= (g_ui8Inpen[ui32Pin] & (1 << ui32Funcsel)) ? (1 << PADREG_FLD_INPEN_S) : 0;

    //
    // Configure ui32GpCfg based on whether nCE requested.
    //
    if ( g_ui8nCEpins[ui32Pin] == ui32Funcsel )
    {
        uint32_t ui32Outcfg;
        uint8_t ui8CEtbl;

#ifndef AM_HAL_DISABLE_API_VALIDATION
        //
        // User is configuring a nCE. Verify the requested settings and set the
        // polarity and OUTCFG values (INCFG is not used here and should be 0).
        // Valid uNCE values are 0-3 (uNCE is a 2-bit field).
        // Valid uIOMnum are 0-6 (0-5 for IOMs, 6 for MSPI, 7 is invalid).
        //
        if ( bfGpioCfg.uIOMnum > IOMNUM_MAX )
        {
            return AM_HAL_GPIO_ERR_INVCE;   // Invalid CE specified
        }
#endif // AM_HAL_DISABLE_API_VALIDATION

        //
        // Construct the entry we expect to find in the table. We can determine
        // the OUTCFG value by looking for that value in the pin row.
        //
        ui8CEtbl = (bfGpioCfg.uIOMnum << 4) | bfGpioCfg.uNCE;
        for ( ui32Outcfg = 0; ui32Outcfg < 4; ui32Outcfg++ )
        {
            if ( g_ui8NCEtable[ui32Pin][ui32Outcfg] == ui8CEtbl )
            {
                break;
            }
        }

#ifndef AM_HAL_DISABLE_API_VALIDATION
        if ( ui32Outcfg >= 4 )
        {
            return AM_HAL_GPIO_ERR_INVCEPIN;
        }
#endif // AM_HAL_DISABLE_API_VALIDATION

        ui32GPCfg |= (ui32Outcfg       << GPIOCFG_FLD_OUTCFG_S) |
                     (bfGpioCfg.eCEpol << GPIOCFG_FLD_INTD_S)   |
                     (0                << GPIOCFG_FLD_INCFG_S);
    }
    else
    {
        //
        // It's not nCE, it's one of the other funcsels.
        // Start by setting the value of the requested GPIO input.
        //
        ui32Padreg |= (bfGpioCfg.eGPInput << PADREG_FLD_INPEN_S);

        //
        // Map the requested interrupt direction settings into the Apollo3
        //  GPIOCFG register field, which is a 4-bit field:
        //  [INTD(1):OUTCFG(2):INCFG(1)].
        // Bit0 of eIntDir maps to GPIOCFG.INTD  (b3).
        // Bit1 of eIntDir maps to GPIOCFG.INCFG (b0).
        //
        ui32GPCfg |= (bfGpioCfg.eGPOutcfg << GPIOCFG_FLD_OUTCFG_S)              |
                     (((bfGpioCfg.eIntDir >> 0) & 0x1) << GPIOCFG_FLD_INTD_S)   |
                     (((bfGpioCfg.eIntDir >> 1) & 0x1) << GPIOCFG_FLD_INCFG_S);

        if ( (bfGpioCfg.eGPOutcfg == AM_HAL_GPIO_PIN_OUTCFG_PUSHPULL) ||
             pincfg_equ(&bfGpioCfg, (void*)&g_AM_HAL_GPIO_DISABLE) )
        {
            //
            // For pushpull configurations, we must be sure to clear the ENABLE
            // bit.  In pushpull, these bits turn on FAST GPIO.  For regular
            // GPIO, they must be clear.
            //
            bClearEnable = true;
        }

        //
        // There is some overlap between eGPRdZero and eIntDir as both settings
        //  utilize the overloaded INCFG bit.
        // Therefore the two fields should be used in a mutually exclusive
        //  manner. For flexibility however they are not disallowed because
        //  their functionality is dependent on FUNCSEL and whether interrupts
        //  are used.
        //
        // In the vein of mutual exclusion, eGPRdZero is primarily intended for
        //  use when GPIO interrupts are not in use and can be used when no
        //  eIntDir setting is provided.
        // If eIntDir is provided, eGPRdZero is ignored and can only be
        //  achieved via the AM_HAL_GPIO_PIN_INTDIR_NONE setting.
        //
        if ( bfGpioCfg.eIntDir == 0 )
        {
            ui32GPCfg &= ~(1 << GPIOCFG_FLD_INCFG_S);
            ui32GPCfg |= (bfGpioCfg.eGPRdZero << GPIOCFG_FLD_INCFG_S);
        }
    }

    switch ( bfGpioCfg.eDriveStrength )
    {
        // DRIVESTRENGTH is a 2-bit field.
        //  bit0 maps to bit2 of a PADREG field.
        //  bit1 maps to bit0 of an ALTPADCFG field.
        case AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA:
            ui32Padreg    |= (0 << PADREG_FLD_DRVSTR_S);
            ui32AltPadCfg |= (0 << 0);
            break;
        case AM_HAL_GPIO_PIN_DRIVESTRENGTH_4MA:
            ui32Padreg    |= (1 << PADREG_FLD_DRVSTR_S);
            ui32AltPadCfg |= (0 << 0);
            break;
        case AM_HAL_GPIO_PIN_DRIVESTRENGTH_8MA:
            ui32Padreg    |= (0 << PADREG_FLD_DRVSTR_S);
            ui32AltPadCfg |= (1 << 0);
            break;
        case AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA:
            ui32Padreg    |= (1 << PADREG_FLD_DRVSTR_S);
            ui32AltPadCfg |= (1 << 0);
            break;
    }

    //
    // At this point, the 3 configuration variables, ui32GPCfg, ui32Padreg,
    //  and ui32AltPadCfg values are set (at bit position 0) and ready to write
    //  to their respective register bitfields.
    //
    uint32_t ui32GPCfgAddr, ui32PadregAddr, ui32AltpadAddr;
    uint32_t ui32GPCfgClearMask, ui32PadClearMask;
    uint32_t ui32GPCfgShft, ui32PadShft;

    ui32GPCfgAddr       = AM_REGADDR(GPIO, CFGA)       + ((ui32Pin >> 1) & ~0x3);
    ui32PadregAddr      = AM_REGADDR(GPIO, PADREGA)    + (ui32Pin & ~0x3);
    ui32AltpadAddr      = AM_REGADDR(GPIO, ALTPADCFGA) + (ui32Pin & ~0x3);

    ui32GPCfgShft       = ((ui32Pin & 0x7) << 2);
    ui32PadShft         = ((ui32Pin & 0x3) << 3);
    ui32GPCfgClearMask  = ~((uint32_t)0xF  << ui32GPCfgShft);
    ui32PadClearMask    = ~((uint32_t)0xFF << ui32PadShft);

    //
    // Get the new values into their rightful bit positions.
    //
    ui32Padreg    <<= ui32PadShft;
    ui32AltPadCfg <<= ui32PadShft;
    ui32GPCfg     <<= ui32GPCfgShft;

    AM_CRITICAL_BEGIN

    if ( bClearEnable )
    {
        //
        // We're configuring a mode that requires clearing the Enable bit.
        //
        am_hal_gpio_output_tristate_disable(ui32Pin);
    }

    GPIO->PADKEY = GPIO_PADKEY_PADKEY_Key;

    AM_REGVAL(ui32PadregAddr)  = (AM_REGVAL(ui32PadregAddr) & ui32PadClearMask)   | ui32Padreg;
    AM_REGVAL(ui32GPCfgAddr)   = (AM_REGVAL(ui32GPCfgAddr)  & ui32GPCfgClearMask) | ui32GPCfg;
    AM_REGVAL(ui32AltpadAddr)  = (AM_REGVAL(ui32AltpadAddr) & ui32PadClearMask)   | ui32AltPadCfg;

    GPIO->PADKEY = 0;

    AM_CRITICAL_END

    return AM_HAL_STATUS_SUCCESS;

} // am_hal_gpio_pinconfig()

//*****************************************************************************
//
// brief Configure specified pins for FAST GPIO operation.
//
// ui64PinMask - a mask specifying up to 8 pins to be configured and
//               used for FAST GPIO (only bits 0-49 are valid).
// bfGpioCfg   - The GPIO configuration (same as am_hal_gpio_pinconfig()).
//               All of the pins specified by ui64PinMask will be set to this
//               configuration.
// ui32Masks   - If provided, an array to receive 2 32-bit values of the
//               SET and CLEAR masks that are used for the BBSETCLEAR reg.
//               Two 32-bit wds are placed for each pin indicated by the mask.
//               The 2 32-bit values will be placed at incremental indexes.
//               For example, say pin numbers 5 and 19 are indicated in the
//               mask, and an array pointer is provided in ui32Masks.  This
//               array must be allocated by the caller to be at least 4 words.
//               ui32Masks[0] = the set   mask used for pin 5.
//               ui32Masks[1] = the clear mask used for pin 5.
//               ui32Masks[2] = the set   mask used for pin 19.
//               ui32Masks[3] = the clear mask used for pin 19.
//               It is recommended that this array be allocated to 16 uint32_t.
//
//*****************************************************************************
uint32_t
am_hal_gpio_fast_pinconfig(uint64_t ui64PinMask,
                           am_hal_gpio_pincfg_t bfGpioCfg,
                           uint32_t ui32Masks[])
{
    uint32_t ux, ui32pinnum, ui32retval, ui32Mask;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( (ui64PinMask & ~(((uint64_t)1 << AM_HAL_GPIO_MAX_PADS) - 1))   ||
         (popcount(ui64PinMask) > 8)                                  ||
         (bfGpioCfg.eGPOutcfg == AM_HAL_GPIO_PIN_OUTCFG_TRISTATE) )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    //
    // Roll through the pin mask and configure any designated pins per the
    // bfGpioCfg parameter, and enable for Fast GPIO.
    //
    ui32Mask = 0;
    ui32pinnum = 0;
    ux = 0;
    while ( ui64PinMask )
    {
        if ( ui64PinMask & 0x1 )
        {
            //
            // It is assumed that the caller will have disabled Fast GPIO and
            // initialized the pin value before calling this function. Therefore
            // no value initialization is done before the pin configuration, nor
            // is the am_hal_gpio_fastgpio_disable() called here.
            //
            // Configure the pin.
            //
            ui32retval = am_hal_gpio_pinconfig(ui32pinnum, bfGpioCfg);
            if ( ui32retval )
            {
                return ui32retval;
            }

            ui32Mask |= 1 << (ui32pinnum & 0x7);

            //
            // Enable the FAST GPIO for this pin
            //
            am_hal_gpio_fastgpio_enable(ui32pinnum);

            if ( ui32Masks )
            {
                ui32Masks[ux + 0] = _VAL2FLD(APBDMA_BBSETCLEAR_SET,   ui32Mask);
                ui32Masks[ux + 1] = _VAL2FLD(APBDMA_BBSETCLEAR_CLEAR, ui32Mask);
            }
            ux += 2;    // Get next indexes
        }
        ui32pinnum++;
        ui64PinMask >>= 1;
    }

    return AM_HAL_STATUS_SUCCESS;

} // am_hal_gpio_fast_pinconfig()

//*****************************************************************************
//
//! @brief Read GPIO.
//!
//! @param ui32Pin    - pin number to be read.
//! @param eReadType  - State type to read.  One of:
//!     AM_HAL_GPIO_INPUT_READ
//!     AM_HAL_GPIO_OUTPUT_READ
//!     AM_HAL_GPIO_ENABLE_READ
//! @param pui32ReadState - Pointer to the value to contain the read state.
//!        When reading the value of a bit, will be either 0 or 1.
//!
//! This function reads a pin state as given by ui32Type.
//!
//! @return Status.
//
//*****************************************************************************
uint32_t
am_hal_gpio_state_read(uint32_t ui32Pin,
                       am_hal_gpio_read_type_e eReadType,
                       uint32_t *pui32ReadState)
{
    uint32_t ui32ReadValue = 0xFFFFFFFF;
    uint32_t ui32BaseAddr, ui32Shift;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( pui32ReadState == NULL )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }

    if ( ui32Pin >= AM_HAL_GPIO_MAX_PADS )
    {
        *pui32ReadState = ui32ReadValue;
        return AM_HAL_STATUS_OUT_OF_RANGE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    //
    // Compute base address + offset of 0 or 4.
    //
    ui32BaseAddr = ((ui32Pin & 0x20) >> 3);   // 0 or 4
    ui32Shift    = ui32Pin & 0x1F;

    switch ( eReadType )
    {
        case AM_HAL_GPIO_INPUT_READ:
            //
            // Assumes eIntDir != AM_HAL_GPIO_PIN_INTDIR_NONE   &&
            //         eIntDir != AM_HAL_GPIO_PIN_INTDIR_BOTH
            // If either of those configs are set, returns 0.
            //
            ui32ReadValue = AM_REGVAL(AM_REGADDR(GPIO, RDA) + ui32BaseAddr);
            ui32ReadValue = (ui32ReadValue >> ui32Shift) & 0x01;
            break;
        case AM_HAL_GPIO_OUTPUT_READ:
            ui32ReadValue = AM_REGVAL(AM_REGADDR(GPIO, WTA) + ui32BaseAddr);
            ui32ReadValue = (ui32ReadValue >> ui32Shift) & 0x01;
            break;
        case AM_HAL_GPIO_ENABLE_READ:
            ui32ReadValue = AM_REGVAL(AM_REGADDR(GPIO, ENA) + ui32BaseAddr);
            ui32ReadValue = (ui32ReadValue >> ui32Shift) & 0x01;
            break;
        default:
            return AM_HAL_STATUS_INVALID_ARG;
    }

    *pui32ReadState = ui32ReadValue;

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_gpio_state_read()

//*****************************************************************************
//
//! @brief Write GPIO.
//!
//! @param ui32Pin    - pin number to be read.
//!
//! @param ui32Type   - State type to write.  One of:
//!     AM_HAL_GPIO_OUTPUT_SET              - Write a one to a GPIO.
//!     AM_HAL_GPIO_OUTPUT_CLEAR            - Write a zero to a GPIO.
//!     AM_HAL_GPIO_OUTPUT_TOGGLE           - Toggle the GPIO value.
//!     The following two apply when output is set for TriState (OUTCFG==3).
//!     AM_HAL_GPIO_OUTPUT_TRISTATE_ENABLE  - Enable  a tri-state GPIO.
//!     AM_HAL_GPIO_OUTPUT_TRISTATE_DISABLE - Disable a tri-state GPIO.
//!
//! This function writes a GPIO value.
//!
//! @return Status.
//!         Fails if the pad is not configured for GPIO (PADFNCSEL != 3).
//
//*****************************************************************************
uint32_t
am_hal_gpio_state_write(uint32_t ui32Pin, am_hal_gpio_write_type_e eWriteType)
{
    uint32_t ui32Mask, ui32Off;
    uint32_t ui32Return = AM_HAL_STATUS_SUCCESS;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( ui32Pin >= AM_HAL_GPIO_MAX_PADS )
    {
        return AM_HAL_STATUS_OUT_OF_RANGE;
    }

    if ( eWriteType > AM_HAL_GPIO_OUTPUT_TRISTATE_TOGGLE )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    ui32Mask = (uint32_t)0x1 << (ui32Pin % 32);
    ui32Off  = (ui32Pin & 0x20) >> 3;   // 0 or 4

    AM_CRITICAL_BEGIN;
    switch ( eWriteType )
    {
        case AM_HAL_GPIO_OUTPUT_SET:                // Write a one to a GPIO.
            AM_REGVAL(AM_REGADDR(GPIO, WTSA) + ui32Off) = ui32Mask;
            break;
        case AM_HAL_GPIO_OUTPUT_CLEAR:              // Write a zero to a GPIO.
            AM_REGVAL(AM_REGADDR(GPIO, WTCA) + ui32Off) = ui32Mask;
            break;
        case AM_HAL_GPIO_OUTPUT_TOGGLE:             // Toggle the GPIO value.
            AM_REGVAL(AM_REGADDR(GPIO, WTA) + ui32Off) ^= ui32Mask;
            break;
        case AM_HAL_GPIO_OUTPUT_TRISTATE_ENABLE:    // Enable  a tri-state GPIO.
            AM_REGVAL(AM_REGADDR(GPIO, ENSA) + ui32Off) = ui32Mask;
            break;
        case AM_HAL_GPIO_OUTPUT_TRISTATE_DISABLE:   // Disable a tri-state GPIO.
            AM_REGVAL(AM_REGADDR(GPIO, ENCA) + ui32Off) = ui32Mask;
            break;
        case AM_HAL_GPIO_OUTPUT_TRISTATE_TOGGLE:   // Toggle a tri-state GPIO.
            AM_REGVAL(AM_REGADDR(GPIO, ENCA) + ui32Off) ^= ui32Mask;
            break;
        default:
            // Type values were validated on entry.
            // We can't return from here because we're in a critical section.
            ui32Return = AM_HAL_STATUS_INVALID_ARG;
            break;
    }

    AM_CRITICAL_END;

    return ui32Return;
} // am_hal_gpio_state_write()

//*****************************************************************************
//
// Enable GPIO interrupts.
//
//*****************************************************************************
uint32_t
am_hal_gpio_interrupt_enable(uint64_t ui64InterruptMask)
{
#ifndef AM_HAL_DISABLE_API_VALIDATION
    //
    // Check parameters
    //
    if ( ui64InterruptMask &  ~(((uint64_t)1 << AM_HAL_GPIO_MAX_PADS) - 1) )
    {
        return AM_HAL_STATUS_OUT_OF_RANGE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    //
    // Enable the interrupts.
    //
    AM_CRITICAL_BEGIN

    GPIO->INT0EN |= (uint32_t)(ui64InterruptMask & 0xFFFFFFFF);
    GPIO->INT1EN |= (uint32_t)(ui64InterruptMask >> 32);

    AM_CRITICAL_END

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;

} // am_hal_gpio_interrupt_enable()

//*****************************************************************************
//
// Disable GPIO interrupts.
//
//*****************************************************************************
uint32_t
am_hal_gpio_interrupt_disable(uint64_t ui64InterruptMask)
{
#ifndef AM_HAL_DISABLE_API_VALIDATION
    //
    // Check parameters
    //
    if ( ui64InterruptMask &  ~(((uint64_t)1 << AM_HAL_GPIO_MAX_PADS) - 1) )
    {
        return AM_HAL_STATUS_OUT_OF_RANGE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    //
    // Disable the interrupts.
    //
    AM_CRITICAL_BEGIN

    GPIO->INT0EN &= ~((uint32_t)(ui64InterruptMask & 0xFFFFFFFF));
    GPIO->INT1EN &= ~((uint32_t)(ui64InterruptMask >> 32));

    AM_CRITICAL_END

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;

} // am_hal_gpio_interrupt_disable()

//*****************************************************************************
//
// Clear GPIO interrupts.
//
//*****************************************************************************
uint32_t
am_hal_gpio_interrupt_clear(uint64_t ui64InterruptMask)
{
#ifndef AM_HAL_DISABLE_API_VALIDATION
    //
    // Check parameters
    //
    if ( ui64InterruptMask &  ~(((uint64_t)1 << AM_HAL_GPIO_MAX_PADS) - 1) )
    {
        return AM_HAL_STATUS_OUT_OF_RANGE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    //
    // Clear the interrupts.
    //
    AM_CRITICAL_BEGIN

    GPIO->INT0CLR = (uint32_t)(ui64InterruptMask & 0xFFFFFFFF);
    GPIO->INT1CLR = (uint32_t)(ui64InterruptMask >> 32);

    AM_CRITICAL_END

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;

} // am_hal_gpio_interrupt_clear()

//*****************************************************************************
//
// Get GPIO interrupt status.
//
//*****************************************************************************
uint32_t
am_hal_gpio_interrupt_status_get(bool bEnabledOnly, uint64_t *pui64IntStatus)
{

    uint64_t ui64RetVal, ui64Mask;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( pui64IntStatus == NULL )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    //
    // Initialize variable outside critical section
    //
    ui64Mask   = 0xFFFFFFFFFFFFFFFF;

    //
    // Combine upper or lower GPIO words into one 64 bit return value.
    //
    AM_CRITICAL_BEGIN

    ui64RetVal  = ((uint64_t)GPIO->INT1STAT) << 32;
    ui64RetVal |= ((uint64_t)GPIO->INT0STAT) << 0;

    if ( bEnabledOnly )
    {
        ui64Mask    = ((uint64_t)GPIO->INT1EN) << 32;
        ui64Mask   |= ((uint64_t)GPIO->INT0EN) << 0;
    }

    ui64RetVal &= ui64Mask;

    *pui64IntStatus = ui64RetVal;

    AM_CRITICAL_END

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;

} // am_hal_gpio_interrupt_status_get()

//*****************************************************************************
//
// GPIO interrupt service routine registration.
//
//*****************************************************************************
uint32_t
am_hal_gpio_interrupt_register(uint32_t ui32GPIONumber,
                               am_hal_gpio_handler_t pfnHandler)
{
#ifndef AM_HAL_DISABLE_API_VALIDATION
    //
    // Check parameters
    //
    if ( ui32GPIONumber >= AM_HAL_GPIO_MAX_PADS )
    {
        return AM_HAL_STATUS_OUT_OF_RANGE;
    }

    if ( pfnHandler == NULL )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    //
    // Store the handler function pointer.
    //
    gpio_ppfnHandlers[ui32GPIONumber] = pfnHandler;

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;

} // am_hal_gpio_interrupt_register()

//*****************************************************************************
//
//! @brief Advanced GPIO interrupt service routine registration.
//!
//! @param ui32GPIONumber - GPIO number (0-49) to be registered.
//!
//! @param pfnHandler - Function pointer to the callback.
//!
//! @param pCtxt      - context for the callback.
//!
//! @return Status.
//!         Fails if pfnHandler is NULL or ui32GPIONumber > 49.
//
//*****************************************************************************
uint32_t
am_hal_gpio_interrupt_register_adv(uint32_t ui32GPIONumber,
                               am_hal_gpio_handler_adv_t pfnHandler, void *pCtxt)
{
#ifndef AM_HAL_DISABLE_API_VALIDATION
    //
    // Check parameters
    //
    if ( ui32GPIONumber >= AM_HAL_GPIO_MAX_PADS )
    {
        return AM_HAL_STATUS_OUT_OF_RANGE;
    }

    if ( pfnHandler == NULL )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    //
    // Store the handler function pointer.
    //
    gpio_ppfnHandlers[ui32GPIONumber] = (am_hal_gpio_handler_t)((uint32_t)pfnHandler & ~0x1);
    gpio_pHandlerCtxt[ui32GPIONumber] = pCtxt;

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;

} // am_hal_gpio_interrupt_register_adv()

//*****************************************************************************
//
// GPIO interrupt service routine.
//
//*****************************************************************************
uint32_t
am_hal_gpio_interrupt_service(uint64_t ui64Status)
{
    uint32_t ui32RetStatus = AM_HAL_STATUS_SUCCESS;
    uint32_t ui32Status, ui32Clz, ui32FFS, ui32Cnt;

    am_hal_gpio_handler_t pfnHandler;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    //
    // Check parameters
    //
    if ( ui64Status  &  ~(((uint64_t)1 << AM_HAL_GPIO_MAX_PADS) - 1) )
    {
        return AM_HAL_STATUS_OUT_OF_RANGE;
    }

    if ( ui64Status == 0 )
    {
        return AM_HAL_STATUS_FAIL;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    //
    // Handle interrupts.
    // The 1st iteration handles any active interrupts in the lower 32 bits.
    // The 2nd iteration handles any active interrupts in the upper 32 bits.
    // (The order of handling upper or lower bits is somewhat arbitrary.)
    //
    ui32Cnt = 0;
    while ( ui32Cnt < 33 )
    {
        //
        // Get upper or lower status word.
        //
        ui32Status = (uint32_t)(ui64Status >> ui32Cnt);

        while ( ui32Status )
        {
            //
            // We need to FFS (Find First Set).  We can easily zero-base FFS
            // since we know that at least 1 bit is set in ui32Status.
            // FFS(x) = 31 - clz(x & -x).       // Zero-based version of FFS.
            //
            ui32FFS = ui32Status & (uint32_t)(-(int32_t)ui32Status);
#ifdef __IAR_SYSTEMS_ICC__
            ui32Clz = __CLZ(ui32FFS);
#else
            ui32Clz = __builtin_clz(ui32FFS);
#endif
            ui32FFS = 31 - ui32Clz;

            //
            // Turn off the bit we picked in the working copy
            //
            ui32Status &= ~(0x00000001 << ui32FFS);

            //
            // Check the bit handler table to see if there is an interrupt handler
            // registered for this particular bit.
            //
            pfnHandler = gpio_ppfnHandlers[ui32Cnt + ui32FFS];
            if ( pfnHandler )
            {
                //
                // If we found an interrupt handler routine, call it now.
                //
                if ((uint32_t)pfnHandler & 0x1)
                {
                    pfnHandler();
                }
                else
                {
                    am_hal_gpio_handler_adv_t padvHandler = (am_hal_gpio_handler_adv_t)((uint32_t)pfnHandler | 0x1);
                    padvHandler(gpio_pHandlerCtxt[ui32Cnt + ui32FFS]);
                }
            }
            else
            {
                //
                // No handler was registered for the GPIO that interrupted.
                // Return an error.
                //
                ui32RetStatus = AM_HAL_STATUS_INVALID_OPERATION;
            }
        }
        ui32Cnt += 32;
    }

    //
    // Return the status.
    //
    return ui32RetStatus;

} // am_hal_gpio_interrupt_service()


//*****************************************************************************	
//	
//! @brief Configure an Apollo3 pin.	
//!	
//! @param ui32Pin    - pin number to be configured.	
//! @param ui32Config - Contains multiple descriptor fields.	
//!	
//! This function configures a pin according to the parameters in ui32Config.	
//! All parameters are validated, and the given pin is configured according	
//! to the designated parameters.	
//!	
//! @return Status.	
//	
//*****************************************************************************	
uint32_t ap3_hal_gpio_pinconfig_partial(uint32_t ui32Pin, am_hal_gpio_pincfg_t bfGpioCfg, am_hal_gpio_pincfg_allow_t sAllowableChanges) //am_hal_gpio_pincfg_t bfGpioCfgMsk)	
{	
    uint32_t ui32Padreg, ui32AltPadCfg, ui32GPCfg;	
    uint32_t ui32Funcsel, ui32PowerSw;	
    uint8_t padRegMask = 0, GPCfgMask = 0, altPadCfgMask = 0;	
    bool bClearEnable = false;	

#ifndef AM_HAL_DISABLE_API_VALIDATION	
    if (ui32Pin >= AM_HAL_GPIO_MAX_PADS)	
    {	
        return AM_HAL_STATUS_INVALID_ARG;	
    }	
#endif // AM_HAL_DISABLE_API_VALIDATION	
    ap3_gpio_get_pinconfig_bitmasks(sAllowableChanges, &padRegMask, &GPCfgMask, &altPadCfgMask);	
    //	
    // Initialize the PADREG accumulator variables.	
    //	
    ui32GPCfg = ui32Padreg = ui32AltPadCfg = 0;	

    //	
    // Get the requested function and/or power switch.	
    //	
    ui32Funcsel = bfGpioCfg.uFuncSel;	
    ui32PowerSw = bfGpioCfg.ePowerSw;	

    ui32Padreg |= ui32Funcsel << PADREG_FLD_FNSEL_S;	

    //	
    // Check for invalid configuration requests.	
    //	
    if (bfGpioCfg.ePullup != AM_HAL_GPIO_PIN_PULLUP_NONE)	
    {	
        //	
        // This setting is needed for all pullup settings including	
        // AM_HAL_GPIO_PIN_PULLUP_WEAK and AM_HAL_GPIO_PIN_PULLDOWN.	
        //	
        ui32Padreg |= (0x1 << PADREG_FLD_PULLUP_S);	

        //	
        // Check for specific pullup or pulldown settings.	
        //	
        if ((bfGpioCfg.ePullup >= AM_HAL_GPIO_PIN_PULLUP_1_5K) &&	
            (bfGpioCfg.ePullup <= AM_HAL_GPIO_PIN_PULLUP_24K))	
        {	
            ui32Padreg |= ((bfGpioCfg.ePullup - AM_HAL_GPIO_PIN_PULLUP_1_5K) << PADREG_FLD_76_S);	
#ifndef AM_HAL_DISABLE_API_VALIDATION	
            if (!(g_ui8Bit76Capabilities[ui32Pin] & CAP_PUP))	
            {	
                return AM_HAL_GPIO_ERR_PULLUP;	
            }	
        }	
        else if (bfGpioCfg.ePullup == AM_HAL_GPIO_PIN_PULLDOWN)	
        {	
            if (ui32Pin != 20)	
            {	
                return AM_HAL_GPIO_ERR_PULLDOWN;	
            }	
        }	
        else if (bfGpioCfg.ePullup == AM_HAL_GPIO_PIN_PULLUP_WEAK)	
        {	
            //	
            // All pads except 20 support a weak pullup, for which we only need	
            // to set PADnPULL and clear 7:6 (already done at this point).	
            //	
            if (ui32Pin == 20)	
            {	
                return AM_HAL_GPIO_ERR_PULLUP;	
            }	
#endif // AM_HAL_DISABLE_API_VALIDATION	
        }	
    }	

    //	
    // Check if requesting a power switch pin	
    //	
    if (ui32PowerSw != AM_HAL_GPIO_PIN_POWERSW_NONE)	
    {	
        if ((ui32PowerSw == AM_HAL_GPIO_PIN_POWERSW_VDD) &&	
            (g_ui8Bit76Capabilities[ui32Pin] & CAP_VDD))	
        {	
            ui32Padreg |= 0x1 << PADREG_FLD_76_S;	
        }	
        else if ((ui32PowerSw == AM_HAL_GPIO_PIN_POWERSW_VSS) &&	
                 (g_ui8Bit76Capabilities[ui32Pin] & CAP_VSS))	
        {	
            ui32Padreg |= 0x2 << PADREG_FLD_76_S;	
        }	
        else	
        {	
            return AM_HAL_GPIO_ERR_PWRSW;	
        }	
    }	

    //	
    // Depending on the selected pin and FNSEL, determine if INPEN needs to be set.	
    //	
    ui32Padreg |= (g_ui8Inpen[ui32Pin] & (1 << ui32Funcsel)) ? (1 << PADREG_FLD_INPEN_S) : 0;	

    //	
    // Configure ui32GpCfg based on whether nCE requested.	
    //	
    if (g_ui8nCEpins[ui32Pin] == ui32Funcsel)	
    {	
        uint32_t ui32Outcfg;	
        uint8_t ui8CEtbl;	

#ifndef AM_HAL_DISABLE_API_VALIDATION	
        //	
        // User is configuring a nCE. Verify the requested settings and set the	
        // polarity and OUTCFG values (INCFG is not used here and should be 0).	
        // Valid uNCE values are 0-3 (uNCE is a 2-bit field).	
        // Valid uIOMnum are 0-6 (0-5 for IOMs, 6 for MSPI, 7 is invalid).	
        //	
        if (bfGpioCfg.uIOMnum > IOMNUM_MAX)	
        {	
            return AM_HAL_GPIO_ERR_INVCE; // Invalid CE specified	
        }	
#endif // AM_HAL_DISABLE_API_VALIDATION	

        //	
        // Construct the entry we expect to find in the table. We can determine	
        // the OUTCFG value by looking for that value in the pin row.	
        //	
        ui8CEtbl = (bfGpioCfg.uIOMnum << 4) | bfGpioCfg.uNCE;	
        for (ui32Outcfg = 0; ui32Outcfg < 4; ui32Outcfg++)	
        {	
            if (g_ui8NCEtable[ui32Pin][ui32Outcfg] == ui8CEtbl)	
            {	
                break;	
            }	
        }	

#ifndef AM_HAL_DISABLE_API_VALIDATION	
        if (ui32Outcfg >= 4)	
        {	
            return AM_HAL_GPIO_ERR_INVCEPIN;	
        }	
#endif // AM_HAL_DISABLE_API_VALIDATION	

        ui32GPCfg |= (ui32Outcfg << GPIOCFG_FLD_OUTCFG_S) |	
                     (bfGpioCfg.eCEpol << GPIOCFG_FLD_INTD_S) |	
                     (0 << GPIOCFG_FLD_INCFG_S);	
    }	
    else	
    {	
        //	
        // It's not nCE, it's one of the other funcsels.	
        // Start by setting the value of the requested GPIO input.	
        //	
        ui32Padreg |= (bfGpioCfg.eGPInput << PADREG_FLD_INPEN_S);	

        //	
        // Map the requested interrupt direction settings into the Apollo3	
        //  GPIOCFG register field, which is a 4-bit field:	
        //  [INTD(1):OUTCFG(2):INCFG(1)].	
        // Bit0 of eIntDir maps to GPIOCFG.INTD  (b3).	
        // Bit1 of eIntDir maps to GPIOCFG.INCFG (b0).	
        //	
        ui32GPCfg |= (bfGpioCfg.eGPOutcfg << GPIOCFG_FLD_OUTCFG_S) |	
                     (((bfGpioCfg.eIntDir >> 0) & 0x1) << GPIOCFG_FLD_INTD_S) |	
                     (((bfGpioCfg.eIntDir >> 1) & 0x1) << GPIOCFG_FLD_INCFG_S);	

        if ((bfGpioCfg.eGPOutcfg == AM_HAL_GPIO_PIN_OUTCFG_PUSHPULL) ||	
            pincfg_equ(&bfGpioCfg, (void *)&g_AM_HAL_GPIO_DISABLE))	
        {	
            //	
            // For pushpull configurations, we must be sure to clear the ENABLE	
            // bit.  In pushpull, these bits turn on FAST GPIO.  For regular	
            // GPIO, they must be clear.	
            //	
            bClearEnable = true;	
        }	

        //	
        // There is some overlap between eGPRdZero and eIntDir as both settings	
        //  utilize the overloaded INCFG bit.	
        // Therefore the two fields should be used in a mutually exclusive	
        //  manner. For flexibility however they are not disallowed because	
        //  their functionality is dependent on FUNCSEL and whether interrupts	
        //  are used.	
        //	
        // In the vein of mutual exclusion, eGPRdZero is primarily intended for	
        //  use when GPIO interrupts are not in use and can be used when no	
        //  eIntDir setting is provided.	
        // If eIntDir is provided, eGPRdZero is ignored and can only be	
        //  achieved via the AM_HAL_GPIO_PIN_INTDIR_NONE setting.	
        //	
        if (bfGpioCfg.eIntDir == 0)	
        {	
            ui32GPCfg &= ~(1 << GPIOCFG_FLD_INCFG_S);	
            ui32GPCfg |= (bfGpioCfg.eGPRdZero << GPIOCFG_FLD_INCFG_S);	
        }	
    }	

    switch (bfGpioCfg.eDriveStrength)	
    {	
    // DRIVESTRENGTH is a 2-bit field.	
    //  bit0 maps to bit2 of a PADREG field.	
    //  bit1 maps to bit0 of an ALTPADCFG field.	
    case AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA:	
        ui32Padreg |= (0 << PADREG_FLD_DRVSTR_S);	
        ui32AltPadCfg |= (0 << 0);	
        break;	
    case AM_HAL_GPIO_PIN_DRIVESTRENGTH_4MA:	
        ui32Padreg |= (1 << PADREG_FLD_DRVSTR_S);	
        ui32AltPadCfg |= (0 << 0);	
        break;	
    case AM_HAL_GPIO_PIN_DRIVESTRENGTH_8MA:	
        ui32Padreg |= (0 << PADREG_FLD_DRVSTR_S);	
        ui32AltPadCfg |= (1 << 0);	
        break;	
    case AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA:	
        ui32Padreg |= (1 << PADREG_FLD_DRVSTR_S);	
        ui32AltPadCfg |= (1 << 0);	
        break;	
    }	

    //	
    // At this point, the 3 configuration variables, ui32GPCfg, ui32Padreg,	
    //  and ui32AltPadCfg values are set (at bit position 0) and ready to write	
    //  to their respective register bitfields.	
    //	
    uint32_t ui32GPCfgAddr, ui32PadregAddr, ui32AltpadAddr;	
    uint32_t ui32GPCfgClearMask, ui32PadClearMask, ui32AltPadClearMask;	
    uint32_t ui32GPCfgShft, ui32PadShft;	

    ui32GPCfgAddr = AM_REGADDR(GPIO, CFGA) + ((ui32Pin >> 1) & ~0x3);	
    ui32PadregAddr = AM_REGADDR(GPIO, PADREGA) + (ui32Pin & ~0x3);	
    ui32AltpadAddr = AM_REGADDR(GPIO, ALTPADCFGA) + (ui32Pin & ~0x3);	

    ui32GPCfgShft = ((ui32Pin & 0x7) << 2);	
    ui32PadShft = ((ui32Pin & 0x3) << 3);	
    ui32GPCfgClearMask = ~((uint32_t)GPCfgMask << ui32GPCfgShft);	
    ui32PadClearMask = ~((uint32_t)padRegMask << ui32PadShft);	
    ui32AltPadClearMask = ~((uint32_t)altPadCfgMask << ui32PadShft);	

    //	
    // Get the new values into their rightful bit positions.	
    //	
    ui32Padreg = (ui32Padreg & (uint32_t)padRegMask) << ui32PadShft;	
    ui32AltPadCfg = (ui32AltPadCfg & (uint32_t)altPadCfgMask) << ui32PadShft;	
    ui32GPCfg = (ui32GPCfg & (uint32_t)GPCfgMask) << ui32GPCfgShft;	

    AM_CRITICAL_BEGIN	

    if (bClearEnable)	
    {	
        //	
        // We're configuring a mode that requires clearing the Enable bit.	
        //	
        am_hal_gpio_output_tristate_disable(ui32Pin);	
    }	

    GPIO->PADKEY = GPIO_PADKEY_PADKEY_Key;	

    AM_REGVAL(ui32PadregAddr) = (AM_REGVAL(ui32PadregAddr) & ui32PadClearMask) | ui32Padreg;	
    AM_REGVAL(ui32GPCfgAddr) = (AM_REGVAL(ui32GPCfgAddr) & ui32GPCfgClearMask) | ui32GPCfg;	
    AM_REGVAL(ui32AltpadAddr) = (AM_REGVAL(ui32AltpadAddr) & ui32AltPadClearMask) | ui32AltPadCfg;	

    GPIO->PADKEY = 0;	

    AM_CRITICAL_END	

    return AM_HAL_STATUS_SUCCESS;	

} //ap3_hal_gpio_pinconfig_partial	

void ap3_gpio_get_pinconfig_bitmasks(am_hal_gpio_pincfg_allow_t sAllowableChanges, uint8_t *padRegMask, uint8_t *GPCfgMask, uint8_t *altPadCfgMask)	
{	
    *padRegMask = 0;	
    *GPCfgMask = 0;	
    *altPadCfgMask = 0;	

    if (sAllowableChanges.uFuncSel)	
    {	
        *padRegMask |= 0x38; //bits 3-5              PadReg	
    }	
    if (sAllowableChanges.ePowerSw)	
    {	
        *padRegMask |= 0xC0; //bits 6 and 7          PadReg	
    }	
    if (sAllowableChanges.ePullup)	
    {	
        *padRegMask |= 0xC1; //bits 6 and 7 and 0    PadReg	
    }	
    if (sAllowableChanges.eDriveStrength)	
    {	
        *padRegMask |= 0x04;    //bit 2              PadReg	
        *altPadCfgMask |= 0x10; //bit 4              AltPadReg	
    }	
    if (sAllowableChanges.eGPOutcfg)	
    {	
        *GPCfgMask |= 0x06; //bits 1 and 2          CFGReg	
    }	
    if (sAllowableChanges.eGPInput)	
    {	
        *padRegMask |= 0x02; //bit 1                PadReg	
    }	
    if (sAllowableChanges.eIntDir)	
    {	
        *GPCfgMask |= 0x09; //bit 0 and 3           CFGReg	
    }	
    if (sAllowableChanges.eGPRdZero)	
    {	
        *GPCfgMask |= 0x01; //bit 0                 CFGReg	
    }	
}

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
