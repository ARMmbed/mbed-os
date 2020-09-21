//*****************************************************************************
//
//  am_hal_cachectrl.c
//! @file
//!
//! @brief Functions for interfacing with the CACHE controller.
//!
//! @addtogroup cachectrl3 Cache Control (CACHE)
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
//  Default settings for the cache.
//
//*****************************************************************************
const am_hal_cachectrl_config_t am_hal_cachectrl_defaults =
{
    .bLRU                       = 0,
    .eDescript                  = AM_HAL_CACHECTRL_DESCR_1WAY_128B_1024E,
    .eMode                      = AM_HAL_CACHECTRL_CONFIG_MODE_INSTR_DATA,
};

//*****************************************************************************
//
//  Configure the cache with given and recommended settings, but do not enable.
//
//*****************************************************************************
uint32_t
am_hal_cachectrl_config(const am_hal_cachectrl_config_t *psConfig)
{
    //
    // In the case where cache is currently enabled, we need to gracefully
    // bow out of that configuration before reconfiguring.  The best way to
    // accomplish that is to shut down the ID bits, leaving the cache enabled.
    // Once the instr and data caches have been disabled, we can safely set
    // any new configuration, including disabling the controller.
    //
    AM_CRITICAL_BEGIN
    CACHECTRL->CACHECFG &=
        ~(CACHECTRL_CACHECFG_DCACHE_ENABLE_Msk  |
          CACHECTRL_CACHECFG_ICACHE_ENABLE_Msk);
    AM_CRITICAL_END

    CACHECTRL->CACHECFG =
        _VAL2FLD(CACHECTRL_CACHECFG_ENABLE, 0)                              |
        _VAL2FLD(CACHECTRL_CACHECFG_CACHE_CLKGATE, 1)                       |
        _VAL2FLD(CACHECTRL_CACHECFG_CACHE_LS, 0)                            |
        _VAL2FLD(CACHECTRL_CACHECFG_DATA_CLKGATE, 1)                        |
        _VAL2FLD(CACHECTRL_CACHECFG_ENABLE_MONITOR, 0)                      |
        _VAL2FLD(CACHECTRL_CACHECFG_LRU, psConfig->bLRU)                    |
        _VAL2FLD(CACHECTRL_CACHECFG_CONFIG, psConfig->eDescript)            |
        ((psConfig->eMode << CACHECTRL_CACHECFG_ICACHE_ENABLE_Pos) &
            (CACHECTRL_CACHECFG_DCACHE_ENABLE_Msk   |
             CACHECTRL_CACHECFG_ICACHE_ENABLE_Msk));

    return AM_HAL_STATUS_SUCCESS;

} // am_hal_cachectrl_config()

//*****************************************************************************
//
//  Enable the cache.
//
//*****************************************************************************
uint32_t
am_hal_cachectrl_enable(void)
{
    //
    // Enable the cache
    //
    CACHECTRL->CACHECFG |= _VAL2FLD(CACHECTRL_CACHECFG_ENABLE, 1);

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_cachectrl_enable()

//*****************************************************************************
//
//  Disable the cache.
//
//*****************************************************************************
uint32_t
am_hal_cachectrl_disable(void)
{
    //
    // Shut down as gracefully as possible.
    // Disable the I/D cache enable bits first to allow a little time
    // for any in-flight transactions to hand off to the line buffer.
    // Then clear the enable.
    //
    AM_CRITICAL_BEGIN
    CACHECTRL->CACHECFG &= ~(_VAL2FLD(CACHECTRL_CACHECFG_ICACHE_ENABLE, 1) |
                             _VAL2FLD(CACHECTRL_CACHECFG_DCACHE_ENABLE, 1));
    CACHECTRL->CACHECFG &= ~_VAL2FLD(CACHECTRL_CACHECFG_ENABLE, 1);
    AM_CRITICAL_END

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_cachectrl_disable()

//*****************************************************************************
//
//  Control helper functions.
//
//*****************************************************************************
static bool
set_LPMMODE(uint32_t ui32value)
{
    uint32_t ui32Val;
    uint32_t *pui32RegAddr;

    if ( ui32value > (CACHECTRL_FLASHCFG_LPMMODE_Msk >> CACHECTRL_FLASHCFG_LPMMODE_Pos) )
    {
        return false;
    }

    //
    // Compute register address (assumes each reg is 1 word offset).
    //
    pui32RegAddr = (uint32_t*)&CACHECTRL->FLASHCFG;

    AM_CRITICAL_BEGIN
    ui32Val = am_hal_flash_load_ui32(pui32RegAddr);
    ui32Val &= ~(CACHECTRL_FLASHCFG_LPMMODE_Msk                 |
                 CACHECTRL_FLASHCFG_LPM_RD_WAIT_Msk);
    ui32Val |= _VAL2FLD(CACHECTRL_FLASHCFG_LPMMODE, ui32value)    |
               _VAL2FLD(CACHECTRL_FLASHCFG_LPM_RD_WAIT, 0x7);
    am_hal_flash_store_ui32(pui32RegAddr, ui32Val);
    AM_CRITICAL_END

    return true;
} // set_LPMMODE()

static bool
set_SEDELAY(uint32_t ui32value)
{
    uint32_t ui32Val;
    uint32_t *pui32RegAddr;

    if ( ui32value > (CACHECTRL_FLASHCFG_SEDELAY_Msk >> CACHECTRL_FLASHCFG_SEDELAY_Pos) )
    {
        return false;
    }

    //
    // Compute register address (assumes each reg is 1 word offset).
    //
    pui32RegAddr = (uint32_t*)&CACHECTRL->FLASHCFG;

    AM_CRITICAL_BEGIN
    ui32Val = am_hal_flash_load_ui32(pui32RegAddr);
    ui32Val &= ~(CACHECTRL_FLASHCFG_SEDELAY_Msk                 |
                 CACHECTRL_FLASHCFG_LPM_RD_WAIT_Msk);
    ui32Val |= _VAL2FLD(CACHECTRL_FLASHCFG_SEDELAY, ui32value)    |
               _VAL2FLD(CACHECTRL_FLASHCFG_LPM_RD_WAIT, 0x7);
    am_hal_flash_store_ui32(pui32RegAddr, ui32Val);
    AM_CRITICAL_END

    return true;
} // set_SEDELAY()

static bool
set_RDWAIT(uint32_t ui32value)
{
    uint32_t ui32Val;
    uint32_t *pui32RegAddr;

    if ( ui32value > (CACHECTRL_FLASHCFG_RD_WAIT_Msk >> CACHECTRL_FLASHCFG_RD_WAIT_Pos) )
    {
        return false;
    }

    //
    // Compute register address (assumes each reg is 1 word offset).
    //
    pui32RegAddr = (uint32_t*)&CACHECTRL->FLASHCFG;

    AM_CRITICAL_BEGIN
    ui32Val = am_hal_flash_load_ui32(pui32RegAddr);
    ui32Val &= ~(CACHECTRL_FLASHCFG_RD_WAIT_Msk                 |
                 CACHECTRL_FLASHCFG_LPM_RD_WAIT_Msk);
    ui32Val |= _VAL2FLD(CACHECTRL_FLASHCFG_RD_WAIT, ui32value)  |
               _VAL2FLD(CACHECTRL_FLASHCFG_LPM_RD_WAIT, 0x7);
    am_hal_flash_store_ui32(pui32RegAddr, ui32Val);
    AM_CRITICAL_END

    return true;
} // set_RDWAIT()

//*****************************************************************************
//
//  Select the cache configuration type.
//
//*****************************************************************************
uint32_t
am_hal_cachectrl_control(am_hal_cachectrl_control_e eControl, void *pArgs)
{
    uint32_t ui32Arg;
    uint32_t ui32SetMask = 0;

    switch ( eControl )
    {
        case AM_HAL_CACHECTRL_CONTROL_FLASH_CACHE_INVALIDATE:
            ui32SetMask = CACHECTRL_CTRL_INVALIDATE_Msk;
            break;
        case AM_HAL_CACHECTRL_CONTROL_STATISTICS_RESET:
            if ( !_FLD2VAL(CACHECTRL_CACHECFG_ENABLE_MONITOR, CACHECTRL->CACHECFG) )
            {
                //
                // The monitor must be enabled for the reset to have any affect.
                //
                return AM_HAL_STATUS_INVALID_OPERATION;
            }
            else
            {
                ui32SetMask = CACHECTRL_CTRL_RESET_STAT_Msk;
            }
            break;
        case AM_HAL_CACHECTRL_CONTROL_FLASH_ALL_SLEEP_ENABLE:
            ui32SetMask = CACHECTRL_CTRL_FLASH0_SLM_ENABLE_Msk      |
                          CACHECTRL_CTRL_FLASH1_SLM_ENABLE_Msk;
            break;
        case AM_HAL_CACHECTRL_CONTROL_FLASH_ALL_SLEEP_DISABLE:
            ui32SetMask = CACHECTRL_CTRL_FLASH0_SLM_DISABLE_Msk     |
                          CACHECTRL_CTRL_FLASH1_SLM_DISABLE_Msk;
            break;
        case AM_HAL_CACHECTRL_CONTROL_FLASH0_SLEEP_ENABLE:
            ui32SetMask = CACHECTRL_CTRL_FLASH0_SLM_ENABLE_Msk;
            break;
        case AM_HAL_CACHECTRL_CONTROL_FLASH0_SLEEP_DISABLE:
            ui32SetMask = CACHECTRL_CTRL_FLASH0_SLM_DISABLE_Msk;
            break;
        case AM_HAL_CACHECTRL_CONTROL_FLASH1_SLEEP_ENABLE:
            ui32SetMask = CACHECTRL_CTRL_FLASH1_SLM_ENABLE_Msk;
            break;
        case AM_HAL_CACHECTRL_CONTROL_FLASH1_SLEEP_DISABLE:
            ui32SetMask = CACHECTRL_CTRL_FLASH1_SLM_DISABLE_Msk;
            break;
        case AM_HAL_CACHECTRL_CONTROL_MONITOR_ENABLE:
            ui32SetMask = 0;
            AM_CRITICAL_BEGIN
            CACHECTRL->CACHECFG |= CACHECTRL_CACHECFG_ENABLE_MONITOR_Msk;
            AM_CRITICAL_END
            break;
        case AM_HAL_CACHECTRL_CONTROL_MONITOR_DISABLE:
            ui32SetMask = 0;
            AM_CRITICAL_BEGIN
            CACHECTRL->CACHECFG &= ~CACHECTRL_CACHECFG_ENABLE_MONITOR_Msk;
            AM_CRITICAL_END
            break;
        case AM_HAL_CACHECTRL_CONTROL_LPMMODE_RESET:
            //
            // Safely set the reset values for LPMMODE, SEDELAY, and RDWAIT.
            //
            if ( !set_LPMMODE(AM_HAL_CACHECTRL_FLASHCFG_LPMMODE_NEVER)      ||
                 !set_SEDELAY(0x7)                                          ||
                 !set_RDWAIT(0x3) )
            {
                return AM_HAL_STATUS_FAIL;
            }
            break;
        case AM_HAL_CACHECTRL_CONTROL_LPMMODE_RECOMMENDED:
            //
            // Safely set the as recommended values (from the datasheet)
            // for LPMMODE, SEDELAY, and RDWAIT.
            //
            if ( !set_LPMMODE(AM_HAL_CACHECTRL_FLASHCFG_LPMMODE_STANDBY)    ||
                 !set_SEDELAY(0x5)                                          ||
                 !set_RDWAIT(0x1) )
            {
                return AM_HAL_STATUS_FAIL;
            }
            break;
        case AM_HAL_CACHECTRL_CONTROL_LPMMODE_AGGRESSIVE:
            //
            // Safely set aggressive values for LPMMODE, SEDELAY, and RDWAIT.
            // (For now select recommended values.)
            //
            if ( !set_LPMMODE(AM_HAL_CACHECTRL_FLASHCFG_LPMMODE_STANDBY)    ||
                 !set_SEDELAY(0x6)                                          ||
                 !set_RDWAIT(0x1) )
            {
                return AM_HAL_STATUS_FAIL;
            }
            break;
        case AM_HAL_CACHECTRL_CONTROL_LPMMODE_SET:
            //
            // Safely set LPMMODE, SEDELAY, or RDWAIT.
            // The new value is passed by reference via pArgs.  That is, pArgs is
            //  assumed to be a pointer to a uint32_t of the new value.
            //
            if ( !pArgs )
            {
                return AM_HAL_STATUS_INVALID_ARG;
            }
            ui32Arg = *(uint32_t*)pArgs;
            if ( !set_LPMMODE(ui32Arg) )
            {
                return AM_HAL_STATUS_FAIL;
            }
            break;
        case AM_HAL_CACHECTRL_CONTROL_SEDELAY_SET:
            if ( !pArgs )
            {
                return AM_HAL_STATUS_INVALID_ARG;
            }
            ui32Arg = *(uint32_t*)pArgs;
            if ( !set_SEDELAY(ui32Arg) )
            {
                return AM_HAL_STATUS_FAIL;
            }
            break;
        case AM_HAL_CACHECTRL_CONTROL_RDWAIT_SET:
            if ( !pArgs )
            {
                return AM_HAL_STATUS_INVALID_ARG;
            }
            ui32Arg = *(uint32_t*)pArgs;
            if ( !set_RDWAIT(ui32Arg) )
            {
                return AM_HAL_STATUS_FAIL;
            }
            break;
        case AM_HAL_CACHECTRL_CONTROL_NC_CFG:
        {
            if ( pArgs == NULL )
            {
                return AM_HAL_STATUS_INVALID_ARG;
            }

            am_hal_cachectrl_nc_cfg_t *pNcCfg;
            pNcCfg = (am_hal_cachectrl_nc_cfg_t *)pArgs;
#ifndef AM_HAL_DISABLE_API_VALIDATION
            // Make sure the addresses are valid
            if ((pNcCfg->ui32StartAddr & ~CACHECTRL_NCR0START_ADDR_Msk) ||
                (pNcCfg->ui32EndAddr & ~CACHECTRL_NCR0START_ADDR_Msk))
            {
                return AM_HAL_STATUS_INVALID_ARG;
            }
#endif // AM_HAL_DISABLE_API_VALIDATION
            if (pNcCfg->eNCRegion == AM_HAL_CACHECTRL_NCR0)
            {
                CACHECTRL->NCR0START = pNcCfg->ui32StartAddr;
                CACHECTRL->NCR0END   = pNcCfg->ui32EndAddr;
                CACHECTRL->CACHECFG_b.ENABLE_NC0 = pNcCfg->bEnable;
            }
            else if (pNcCfg->eNCRegion == AM_HAL_CACHECTRL_NCR1)
            {
                CACHECTRL->NCR1START = pNcCfg->ui32StartAddr;
                CACHECTRL->NCR1END   = pNcCfg->ui32EndAddr;
                CACHECTRL->CACHECFG_b.ENABLE_NC1 = pNcCfg->bEnable;
            }
#ifndef AM_HAL_DISABLE_API_VALIDATION
            else
            {
                return AM_HAL_STATUS_INVALID_ARG;
            }
#endif // AM_HAL_DISABLE_API_VALIDATION
            return AM_HAL_STATUS_SUCCESS;
        }
        default:
            return AM_HAL_STATUS_INVALID_ARG;
    }

    //
    // All fields in the CACHECTRL register are write-only or read-only.
    // A write to CACHECTRL acts as a mask-set.  That is, only the bits
    // written as '1' have an effect, any bits written as '0' are unaffected.
    //
    // Important note - setting of an enable and disable simultanously has
    // unpredicable results.
    //
    if ( ui32SetMask )
    {
        CACHECTRL->CTRL = ui32SetMask;
    }

    return AM_HAL_STATUS_SUCCESS;

} // am_hal_cachectrl_control()

//*****************************************************************************
//
//  Cache controller status function
//
//*****************************************************************************
uint32_t
am_hal_cachectrl_status_get(am_hal_cachectrl_status_t *psStatus)
{
    uint32_t ui32Status;

    if ( psStatus == NULL )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }

    ui32Status = CACHECTRL->CTRL;

    psStatus->bFlash0SleepMode =
        _FLD2VAL(CACHECTRL_CTRL_FLASH0_SLM_STATUS, ui32Status);
    psStatus->bFlash1SleepMode =
        _FLD2VAL(CACHECTRL_CTRL_FLASH1_SLM_STATUS, ui32Status);
    psStatus->bCacheReady =
        _FLD2VAL(CACHECTRL_CTRL_CACHE_READY, ui32Status);

    return AM_HAL_STATUS_SUCCESS;

} // am_hal_cachectrl_status_get()


//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
