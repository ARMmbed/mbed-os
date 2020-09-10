//*****************************************************************************
//
//  am_hal_mcuctrl.c
//! @file
//!
//! @brief Functions for interfacing with the MCUCTRL.
//!
//! @addtogroup mcuctrl3 MCU Control (MCUCTRL)
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
//
// Global Variables.
//
//*****************************************************************************
//
// Define the flash sizes from CHIPPN.
//
const uint32_t
g_am_hal_mcuctrl_flash_size[AM_HAL_MCUCTRL_CHIPPN_FLASH_SIZE_N] =
{
     16 * 1024,             /* 0x0 0x00004000   16 KB */
     32 * 1024,             /* 0x1 0x00008000   32 KB */
     64 * 1024,             /* 0x2 0x00010000   64 KB */
    128 * 1024,             /* 0x3 0x00020000  128 KB */
    256 * 1024,             /* 0x4 0x00040000  256 KB */
    512 * 1024,             /* 0x5 0x00080000  512 KB */
      1 * 1024 * 1024,      /* 0x6 0x00100000    1 MB */
      2 * 1024 * 1024,      /* 0x7 0x00200000    2 MB */
      3 * 1024 * 1024 / 2,  /* 0x8 0x00600000  1.5 MB */
    0, 0, 0, 0, 0, 0, 0
};

const uint32_t
g_am_hal_mcuctrl_sram_size[AM_HAL_MCUCTRL_CHIPPN_SRAM_SIZE_N] =
{
     16 * 1024,             /* 0x0 0x00004000   16 KB */
     32 * 1024,             /* 0x1 0x00008000   32 KB */
     64 * 1024,             /* 0x2 0x00010000   64 KB */
    128 * 1024,             /* 0x3 0x00020000  128 KB */
    256 * 1024,             /* 0x4 0x00040000  256 KB */
    512 * 1024,             /* 0x5 0x00080000  512 KB */
      1 * 1024 * 1024,      /* 0x6 0x00100000    1 MB */
    384 * 1024,             /* 0x7 0x00200000  384 KB */
    768 * 1024,             /* 0x8 0x000C0000  768 KB */
    0, 0, 0, 0, 0, 0, 0
};

// ****************************************************************************
//
//  device_info_get()
//  Gets all relevant device information.
//
// ****************************************************************************
static void
device_info_get(am_hal_mcuctrl_device_t *psDevice)
{
    //
    // Read the Part Number.
    //
    psDevice->ui32ChipPN = MCUCTRL->CHIPPN;

    //
    // Read the Chip ID0.
    //
    psDevice->ui32ChipID0 = MCUCTRL->CHIPID0;

    //
    // Read the Chip ID1.
    //
    psDevice->ui32ChipID1 = MCUCTRL->CHIPID1;

    //
    // Read the Chip Revision.
    //
    psDevice->ui32ChipRev = MCUCTRL->CHIPREV;

    //
    // Read the Chip VENDOR ID.
    //
    psDevice->ui32VendorID = MCUCTRL->VENDORID;

    //
    // Read the SKU (new for Apollo3).
    //
    psDevice->ui32SKU = MCUCTRL->SKU;

    //
    // Qualified from Part Number.
    //
    psDevice->ui32Qualified = (psDevice->ui32ChipPN >> MCUCTRL_CHIPPN_PARTNUM_QUAL_S) & 0x1;

    //
    // Flash size from Part Number.
    //
    psDevice->ui32FlashSize =
        g_am_hal_mcuctrl_flash_size[
            (psDevice->ui32ChipPN & MCUCTRL_CHIPPN_PARTNUM_FLASHSIZE_M) >>
            MCUCTRL_CHIPPN_PARTNUM_FLASHSIZE_S];

    //
    // SRAM size from Part Number.
    //
    psDevice->ui32SRAMSize =
        g_am_hal_mcuctrl_sram_size[
            (psDevice->ui32ChipPN & MCUCTRL_CHIPPN_PARTNUM_SRAMSIZE_M) >>
             MCUCTRL_CHIPPN_PARTNUM_SRAMSIZE_S];

    //
    // Now, let's look at the JEDEC info.
    // The full partnumber is 12 bits total, but is scattered across 2 registers.
    // Bits [11:8] are 0xE.
    // Bits [7:4] are 0xE for Apollo, 0xD for Apollo2, 0xC for Apollo3.
    // Bits [3:0] are defined differently for Apollo and Apollo2/Apollo3.
    //   For Apollo, the low nibble is 0x0.
    //   For Apollo2/Apollo3, the low nibble indicates flash and SRAM size.
    //
    psDevice->ui32JedecPN  = JEDEC->PID0_b.PNL8 << 0;
    psDevice->ui32JedecPN |= JEDEC->PID1_b.PNH4 << 8;

    //
    // JEPID is the JEP-106 Manufacturer ID Code, which is assigned to Ambiq as
    //  0x1B, with parity bit is 0x9B.  It is 8 bits located across 2 registers.
    //
    psDevice->ui32JedecJEPID  = JEDEC->PID1_b.JEPIDL << 0;
    psDevice->ui32JedecJEPID |= JEDEC->PID2_b.JEPIDH << 4;

    //
    // CHIPREV is 8 bits located across 2 registers.
    //
    psDevice->ui32JedecCHIPREV  = JEDEC->PID2_b.CHIPREVH4 << 4;
    psDevice->ui32JedecCHIPREV |= JEDEC->PID3_b.CHIPREVL4 << 0;

    //
    // Let's get the Coresight ID (32-bits across 4 registers)
    // For Apollo and Apollo2, it's expected to be 0xB105100D.
    //
    psDevice->ui32JedecCID  = JEDEC->CID3_b.CID << 24;
    psDevice->ui32JedecCID |= JEDEC->CID2_b.CID << 16;
    psDevice->ui32JedecCID |= JEDEC->CID1_b.CID <<  8;
    psDevice->ui32JedecCID |= JEDEC->CID0_b.CID <<  0;
} // device_info_get()

//*****************************************************************************
//
//  mcuctrl_fault_status()
//  Gets the fault status and capture registers.
//
//*****************************************************************************
static void
mcuctrl_fault_status(am_hal_mcuctrl_fault_t *psFault)
{
    uint32_t ui32FaultStat;

    //
    // Read the Fault Status Register.
    //
    ui32FaultStat = MCUCTRL->FAULTSTATUS;
    psFault->bICODE = (bool)(ui32FaultStat & MCUCTRL_FAULTSTATUS_ICODEFAULT_Msk);
    psFault->bDCODE = (bool)(ui32FaultStat & MCUCTRL_FAULTSTATUS_DCODEFAULT_Msk);
    psFault->bSYS   = (bool)(ui32FaultStat & MCUCTRL_FAULTSTATUS_SYSFAULT_Msk);

    //
    // Read the DCODE fault capture address register.
    //
    psFault->ui32DCODE = MCUCTRL->DCODEFAULTADDR;

    //
    // Read the ICODE fault capture address register.
    //
    psFault->ui32ICODE |= MCUCTRL->ICODEFAULTADDR;

    //
    // Read the ICODE fault capture address register.
    //
    psFault->ui32SYS |= MCUCTRL->SYSFAULTADDR;
} // mcuctrl_fault_status()

// ****************************************************************************
//
//  am_hal_mcuctrl_control()
//  Apply various specific commands/controls on the MCUCTRL module.
//
// ****************************************************************************
uint32_t
am_hal_mcuctrl_control(am_hal_mcuctrl_control_e eControl, void *pArgs)
{
    uint32_t ui32Tbl;

    switch ( eControl )
    {
        case AM_HAL_MCUCTRL_CONTROL_FAULT_CAPTURE_ENABLE:
            //
            // Enable the Fault Capture registers.
            //
            MCUCTRL->FAULTCAPTUREEN_b.FAULTCAPTUREEN = 1;
            break;

        case AM_HAL_MCUCTRL_CONTROL_FAULT_CAPTURE_DISABLE:
            //
            // Disable the Fault Capture registers.
            //
            MCUCTRL->FAULTCAPTUREEN_b.FAULTCAPTUREEN = 0;
            break;

        case AM_HAL_MCUCTRL_CONTROL_EXTCLK32K_ENABLE:
            //
            // Configure the bits in XTALCTRL that enable external 32KHz clock.
            //
            MCUCTRL->XTALCTRL &=
                ~(MCUCTRL_XTALCTRL_PDNBCMPRXTAL_Msk                 |
                  MCUCTRL_XTALCTRL_PDNBCOREXTAL_Msk                 |
                  MCUCTRL_XTALCTRL_BYPCMPRXTAL_Msk                  |
                  MCUCTRL_XTALCTRL_FDBKDSBLXTAL_Msk                 |
                  MCUCTRL_XTALCTRL_XTALSWE_Msk);

            MCUCTRL->XTALCTRL |=
                _VAL2FLD(MCUCTRL_XTALCTRL_PDNBCMPRXTAL, MCUCTRL_XTALCTRL_PDNBCMPRXTAL_PWRDNCOMP)    |
                _VAL2FLD(MCUCTRL_XTALCTRL_PDNBCOREXTAL, MCUCTRL_XTALCTRL_PDNBCOREXTAL_PWRDNCORE)    |
                _VAL2FLD(MCUCTRL_XTALCTRL_BYPCMPRXTAL,  MCUCTRL_XTALCTRL_BYPCMPRXTAL_BYPCOMP)       |
                _VAL2FLD(MCUCTRL_XTALCTRL_FDBKDSBLXTAL, MCUCTRL_XTALCTRL_FDBKDSBLXTAL_DIS)          |
                _VAL2FLD(MCUCTRL_XTALCTRL_XTALSWE,      MCUCTRL_XTALCTRL_XTALSWE_OVERRIDE_EN);
            break;

        case AM_HAL_MCUCTRL_CONTROL_EXTCLK32K_DISABLE:
            //
            // Configure the bits in XTALCTRL that disable external 32KHz
            // clock, thus re-configuring for the crystal.
            //
            MCUCTRL->XTALCTRL &=
                ~(MCUCTRL_XTALCTRL_PDNBCMPRXTAL_Msk                 |
                  MCUCTRL_XTALCTRL_PDNBCOREXTAL_Msk                 |
                  MCUCTRL_XTALCTRL_BYPCMPRXTAL_Msk                  |
                  MCUCTRL_XTALCTRL_FDBKDSBLXTAL_Msk                 |
                  MCUCTRL_XTALCTRL_XTALSWE_Msk);

            MCUCTRL->XTALCTRL |=
                _VAL2FLD(MCUCTRL_XTALCTRL_PDNBCMPRXTAL, MCUCTRL_XTALCTRL_PDNBCMPRXTAL_PWRUPCOMP)    |
                _VAL2FLD(MCUCTRL_XTALCTRL_PDNBCOREXTAL, MCUCTRL_XTALCTRL_PDNBCOREXTAL_PWRUPCORE)    |
                _VAL2FLD(MCUCTRL_XTALCTRL_BYPCMPRXTAL,  MCUCTRL_XTALCTRL_BYPCMPRXTAL_USECOMP)       |
                _VAL2FLD(MCUCTRL_XTALCTRL_FDBKDSBLXTAL, MCUCTRL_XTALCTRL_FDBKDSBLXTAL_EN)           |
                _VAL2FLD(MCUCTRL_XTALCTRL_XTALSWE,      MCUCTRL_XTALCTRL_XTALSWE_OVERRIDE_DIS);
            break;

        case AM_HAL_MCUCTRL_CONTROL_SRAM_PREFETCH:
            {
                uint32_t ui32SramPrefetch = *(uint32_t*)pArgs;
                uint32_t ui32SetMsk, ui32ClrMsk, ui32SRAMreg;

                //
                // Validate the input flags.
                //
                if  ( ui32SramPrefetch &
                        ~(AM_HAL_MCUCTRL_SRAM_PREFETCH_INSTR        |
                          AM_HAL_MCUCTRL_SRAM_PREFETCH_INSTRCACHE   |
                          AM_HAL_MCUCTRL_SRAM_PREFETCH_DATA         |
                          AM_HAL_MCUCTRL_SRAM_PREFETCH_DATACACHE    |
                          AM_HAL_MCUCTRL_SRAM_NOPREFETCH_INSTR      |
                          AM_HAL_MCUCTRL_SRAM_NOPREFETCH_INSTRCACHE |
                          AM_HAL_MCUCTRL_SRAM_NOPREFETCH_DATA       |
                          AM_HAL_MCUCTRL_SRAM_NOPREFETCH_DATACACHE) )
                {
                    return AM_HAL_STATUS_INVALID_ARG;
                }


                //
                // Given the rule that NOxxx overrides xxx, and keeping in mind
                // that the cache settings cannot be set unless the regular
                // prefetch is also being set or is already set, the following
                // truth table results.

                // Note - this same TT also applies to data settings.
                // nc=no change.
                //          I  IC  NI  NIC:  I   IC
                // 0x0:     0  0   0   0  :  nc  nc
                // 0x1:     0  0   0   1  :  nc  0
                // 0x2:     0  0   1   0  :  0   0
                // 0x3:     0  0   1   1  :  0   0
                // 0x4:     0  1   0   0  :  INVALID
                // 0x5:     0  1   0   1  :  nc  nc
                // 0x6:     0  1   1   0  :  INVALID
                // 0x7:     0  1   1   1  :  0   0
                // 0x8:     1  0   0   0  :  1   0
                // 0x9:     1  0   0   1  :  1   0
                // 0xA:     1  0   1   0  :  0   0
                // 0xB:     1  0   1   1  :  0   0
                // 0xC:     1  1   0   0  :  1   1
                // 0xD:     1  1   0   1  :  1   0
                // 0xE:     1  1   1   0  :  INVALID
                // 0xF:     1  1   1   1  :  0   0
                //

                ui32Tbl = 0;
                ui32Tbl |= (ui32SramPrefetch & AM_HAL_MCUCTRL_SRAM_PREFETCH_INSTR)        ? (1 << 3) : 0;
                ui32Tbl |= (ui32SramPrefetch & AM_HAL_MCUCTRL_SRAM_PREFETCH_INSTRCACHE)   ? (1 << 2) : 0;
                ui32Tbl |= (ui32SramPrefetch & AM_HAL_MCUCTRL_SRAM_NOPREFETCH_INSTR)      ? (1 << 1) : 0;
                ui32Tbl |= (ui32SramPrefetch & AM_HAL_MCUCTRL_SRAM_NOPREFETCH_INSTRCACHE) ? (1 << 0) : 0;

                //
                // Now augment the table entries with current register settings.
                //
                ui32SRAMreg = MCUCTRL->SRAMMODE;

                ui32Tbl |= ui32SRAMreg & MCUCTRL_SRAMMODE_IPREFETCH_Msk       ? (1 << 3) : 0;
                ui32Tbl |= ui32SRAMreg & MCUCTRL_SRAMMODE_IPREFETCH_CACHE_Msk ? (1 << 2) : 0;

                ui32SetMsk = ui32ClrMsk = 0;
                switch ( ui32Tbl )
                {
                    case 0x0:
                    case 0x5:
                        break;
                    case 0x1:
                        ui32ClrMsk = MCUCTRL_SRAMMODE_IPREFETCH_CACHE_Msk;
                        break;
                    case 0x2:
                    case 0x3:
                    case 0x7:
                    case 0xA:
                    case 0xB:
                    case 0xF:
                        ui32ClrMsk = MCUCTRL_SRAMMODE_IPREFETCH_Msk | MCUCTRL_SRAMMODE_IPREFETCH_CACHE_Msk;
                        break;
                    case 0x4:
                    case 0x6:
                    case 0xE:
                        return AM_HAL_STATUS_INVALID_OPERATION;
                    case 0x8:
                    case 0x9:
                    case 0xD:
                        ui32SetMsk = MCUCTRL_SRAMMODE_IPREFETCH_Msk;
                        ui32ClrMsk = MCUCTRL_SRAMMODE_IPREFETCH_CACHE_Msk;
                        break;
                    case 0xC:
                        ui32SetMsk = MCUCTRL_SRAMMODE_IPREFETCH_Msk | MCUCTRL_SRAMMODE_IPREFETCH_CACHE_Msk;
                        break;
                    default:
                        return AM_HAL_STATUS_INVALID_ARG;
                } // switch()

                //
                // Now, repeat with data settings.
                //
                ui32Tbl = 0;
                ui32Tbl |= (ui32SramPrefetch & AM_HAL_MCUCTRL_SRAM_PREFETCH_DATA)        ? (1 << 3) : 0;
                ui32Tbl |= (ui32SramPrefetch & AM_HAL_MCUCTRL_SRAM_PREFETCH_DATACACHE)   ? (1 << 2) : 0;
                ui32Tbl |= (ui32SramPrefetch & AM_HAL_MCUCTRL_SRAM_NOPREFETCH_DATA)      ? (1 << 1) : 0;
                ui32Tbl |= (ui32SramPrefetch & AM_HAL_MCUCTRL_SRAM_NOPREFETCH_DATACACHE) ? (1 << 0) : 0;

                //
                // Now augment the table entries with current register settings.
                //
                ui32Tbl |= ui32SRAMreg & MCUCTRL_SRAMMODE_DPREFETCH_Msk       ? (1 << 3) : 0;
                ui32Tbl |= ui32SRAMreg & MCUCTRL_SRAMMODE_DPREFETCH_CACHE_Msk ? (1 << 2) : 0;

                switch ( ui32Tbl )
                {
                    case 0x0:
                    case 0x5:
                        break;
                    case 0x1:
                        ui32ClrMsk = MCUCTRL_SRAMMODE_DPREFETCH_CACHE_Msk;
                        break;
                    case 0x2:
                    case 0x3:
                    case 0x7:
                    case 0xA:
                    case 0xB:
                    case 0xF:
                        ui32ClrMsk = MCUCTRL_SRAMMODE_DPREFETCH_Msk | MCUCTRL_SRAMMODE_DPREFETCH_CACHE_Msk;
                        break;
                    case 0x4:
                    case 0x6:
                    case 0xE:
                        return AM_HAL_STATUS_INVALID_OPERATION;
                    case 0x8:
                    case 0x9:
                    case 0xD:
                        ui32SetMsk = MCUCTRL_SRAMMODE_DPREFETCH_Msk;
                        ui32ClrMsk = MCUCTRL_SRAMMODE_DPREFETCH_CACHE_Msk;
                        break;
                    case 0xC:
                        ui32SetMsk = MCUCTRL_SRAMMODE_DPREFETCH_Msk | MCUCTRL_SRAMMODE_DPREFETCH_CACHE_Msk;
                        break;
                    default:
                        return AM_HAL_STATUS_INVALID_ARG;
                } // switch()


                //
                // Arrange the register update such that clrmsk will have precedence
                // over setmsk.
                //
                AM_CRITICAL_BEGIN
                ui32SRAMreg = MCUCTRL->SRAMMODE;
                ui32SRAMreg |= ui32SetMsk;
                ui32SRAMreg &= ~ui32ClrMsk;
                MCUCTRL->SRAMMODE = ui32SRAMreg;
                AM_CRITICAL_END
            } // case AM_HAL_MCUCTRL_CONTROL_SRAM_PREFETCH
            break;

        default:
            return AM_HAL_STATUS_INVALID_ARG;
    }

    //
    // Return success status.
    //
    return AM_HAL_STATUS_SUCCESS;

} // am_hal_mcuctrl_control()

// ****************************************************************************
//
//  am_hal_mcuctrl_status_get()
//! This function returns  current status of the MCUCTRL as obtained from
//! various registers of the MCUCTRL block.
//
// ****************************************************************************
uint32_t
am_hal_mcuctrl_status_get(am_hal_mcuctrl_status_t *psStatus)
{
    uint32_t ui32Status;

    if ( psStatus == NULL )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }

    ui32Status = MCUCTRL->FEATUREENABLE;
    psStatus->bBurstAck =
        _FLD2VAL(MCUCTRL_FEATUREENABLE_BURSTACK, ui32Status);
    psStatus->bBLEAck =
        _FLD2VAL(MCUCTRL_FEATUREENABLE_BLEACK, ui32Status);

    psStatus->bDebuggerLockout =
        _FLD2VAL(MCUCTRL_DEBUGGER_LOCKOUT, MCUCTRL->DEBUGGER);

    psStatus->bADCcalibrated =
        _FLD2VAL(MCUCTRL_ADCCAL_ADCCALIBRATED, MCUCTRL->ADCCAL);

    psStatus->bBattLoadEnabled =
        _FLD2VAL(MCUCTRL_ADCBATTLOAD_BATTLOAD, MCUCTRL->ADCBATTLOAD);

    ui32Status = MCUCTRL->BOOTLOADER;
    psStatus->bSecBootOnColdRst =
        _FLD2VAL(MCUCTRL_BOOTLOADER_SECBOOT, ui32Status);
    psStatus->bSecBootOnWarmRst =
        _FLD2VAL(MCUCTRL_BOOTLOADER_SECBOOTONRST, ui32Status);

    return AM_HAL_STATUS_SUCCESS;

} // am_hal_mcuctrl_status_get()

// ****************************************************************************
//
//  am_hal_mcuctrl_info_get()
//  Get information of the given MCUCTRL item.
//
// ****************************************************************************
uint32_t
am_hal_mcuctrl_info_get(am_hal_mcuctrl_infoget_e eInfoGet, void *pInfo)
{
    am_hal_mcuctrl_feature_t *psFeature;
    uint32_t ui32Feature;

    if ( pInfo == NULL )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }

    switch ( eInfoGet )
    {
        case AM_HAL_MCUCTRL_INFO_FEATURES_AVAIL:
            psFeature = (am_hal_mcuctrl_feature_t*)pInfo;
            ui32Feature = MCUCTRL->FEATUREENABLE;
            psFeature->bBurstAvail =
                _FLD2VAL(MCUCTRL_FEATUREENABLE_BURSTAVAIL, ui32Feature);
            psFeature->bBLEavail =
                _FLD2VAL(MCUCTRL_FEATUREENABLE_BLEAVAIL, ui32Feature);

            ui32Feature = MCUCTRL->BOOTLOADER;
            psFeature->ui8SecBootFeature =
                _FLD2VAL(MCUCTRL_BOOTLOADER_SECBOOTFEATURE, ui32Feature);

            ui32Feature = MCUCTRL->SKU;
            psFeature->bBLEFeature =
                _FLD2VAL(MCUCTRL_SKU_ALLOWBLE, ui32Feature);
            psFeature->bBurstFeature =
                _FLD2VAL(MCUCTRL_SKU_ALLOWBURST, ui32Feature);
            break;

        case AM_HAL_MCUCTRL_INFO_DEVICEID:
            device_info_get((am_hal_mcuctrl_device_t *)pInfo);
            break;

        case AM_HAL_MCUCTRL_INFO_FAULT_STATUS:
            mcuctrl_fault_status((am_hal_mcuctrl_fault_t*)pInfo);
            break;

        default:
            return AM_HAL_STATUS_INVALID_ARG;
    }
    //
    // Return success status.
    //
    return AM_HAL_STATUS_SUCCESS;

} // am_hal_mcuctrl_info_get()

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
