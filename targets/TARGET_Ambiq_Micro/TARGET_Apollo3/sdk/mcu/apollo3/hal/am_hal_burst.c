//*****************************************************************************
//
//  am_hal_burst.c
//! @file
//!
//! @brief Functions for controlling Burst Mode operation.
//!
//! @addtogroup burstmode3
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

//
// Globals.
//
bool    g_am_hal_burst_mode_available = false;

// ****************************************************************************
//
//  am_hal_burst_mode_initialize()
//  Burst mode initialization function
//
// ****************************************************************************
uint32_t
am_hal_burst_mode_initialize(am_hal_burst_avail_e *peBurstAvail)
{
    uint32_t    ui32Status;
    //
    // Check if the Burst Mode feature is available based on the SKU.
    //
    if ( 0 == MCUCTRL->SKU_b.ALLOWBURST )
    {
        //
        // Burst mode is not available.
        //
        g_am_hal_burst_mode_available = false;
        *peBurstAvail = AM_HAL_BURST_NOTAVAIL;
        return AM_HAL_STATUS_INVALID_OPERATION;
    }

    //
    // Enable the Burst Feature Event (DEVPWREVENTEN).
    //
    PWRCTRL->DEVPWREVENTEN_b.BURSTEVEN = 1;

    //
    // BLE buck is shared by Burst as well
    // Enable the BLE buck trim values if in use
    //
    if (PWRCTRL->SUPPLYSRC_b.BLEBUCKEN)
    {
      am_hal_pwrctrl_blebuck_trim();
    }

    //
    // Enable the Burst Functionality (FEATUREENABLE).
    //
    MCUCTRL->FEATUREENABLE_b.BURSTREQ = 1;

    ui32Status = am_hal_flash_delay_status_check(10000,
                            (uint32_t)&MCUCTRL->FEATUREENABLE,
                            MCUCTRL_FEATUREENABLE_BURSTACK_Msk,
                            MCUCTRL_FEATUREENABLE_BURSTACK_Msk,
                            true);

    if ( ui32Status != AM_HAL_STATUS_SUCCESS )
    {
        g_am_hal_burst_mode_available = false;
        *peBurstAvail = AM_HAL_BURST_NOTAVAIL;
        return ui32Status;
    }

    if ( 0 == MCUCTRL->FEATUREENABLE_b.BURSTAVAIL )
    {
        //
        // Burst mode is not available.
        //
        g_am_hal_burst_mode_available = false;
        *peBurstAvail = AM_HAL_BURST_NOTAVAIL;
        return AM_HAL_STATUS_INVALID_OPERATION;
    }

    //
    // Check the ACK for the Burst Functionality.
    //
    if ( MCUCTRL->FEATUREENABLE_b.BURSTACK == 0 )
    {
        //
        // If NACK, return status.
        //
        g_am_hal_burst_mode_available = false;
        *peBurstAvail = AM_HAL_BURST_NOTAVAIL;
        return AM_HAL_STATUS_INVALID_OPERATION;
    }

    //
    // Return Availability
    //
    g_am_hal_burst_mode_available = true;
    *peBurstAvail = AM_HAL_BURST_AVAIL;
    return AM_HAL_STATUS_SUCCESS;
}

// ****************************************************************************
//
//  am_hal_burst_mode_enable()
//  Burst mode enable function
//
// ****************************************************************************
uint32_t
am_hal_burst_mode_enable(am_hal_burst_mode_e *peBurstStatus)
{
    uint32_t    ui32Status;

    //
    // Check if Burst Mode is allowed and return status if it is not.
    //
    if (!g_am_hal_burst_mode_available)
    {
        *peBurstStatus = AM_HAL_NORMAL_MODE;
        return AM_HAL_STATUS_INVALID_OPERATION;
    }

    //
    // Request Burst Mode Enable (FREQCTRL)
    //
    CLKGEN->FREQCTRL_b.BURSTREQ = CLKGEN_FREQCTRL_BURSTREQ_EN;

//    while (0 == AM_BFR(CLKGEN, FREQCTRL, BURSTACK));
    ui32Status = am_hal_flash_delay_status_check(10000,
                    (uint32_t)&CLKGEN->FREQCTRL,
                    CLKGEN_FREQCTRL_BURSTSTATUS_Msk,
                    CLKGEN_FREQCTRL_BURSTSTATUS_Msk,
                    true);

    if ( ui32Status != AM_HAL_STATUS_SUCCESS )
    {
        *peBurstStatus = AM_HAL_NORMAL_MODE;
        return ui32Status;
    }

    //
    // Check that the Burst Request was ACK'd.
    //
    if ( 0 == CLKGEN->FREQCTRL_b.BURSTACK )
    {
        *peBurstStatus = AM_HAL_NORMAL_MODE;
        return AM_HAL_STATUS_FAIL;
    }

    //
    // Check the Burst Mode Status (FREQCTRL)
    //
    if ( CLKGEN->FREQCTRL_b.BURSTSTATUS > 0)
    {
        *peBurstStatus =  AM_HAL_BURST_MODE;
    }
    else
    {
        *peBurstStatus =  AM_HAL_NORMAL_MODE;
    }

    return AM_HAL_STATUS_SUCCESS;
}

// ****************************************************************************
//
//  am_hal_burst_mode_disable()
//  Burst mode disable function
//
// ****************************************************************************
uint32_t
am_hal_burst_mode_disable(am_hal_burst_mode_e *peBurstStatus)
{
    uint32_t    ui32Status;

    //
    // Request Burst Mode Enable (FREQCTRL)
    //
    //
    // Safely disable burst mode.
    //
    AM_CRITICAL_BEGIN
    am_hal_flash_store_ui32((uint32_t*)&CLKGEN->FREQCTRL, CLKGEN_FREQCTRL_BURSTREQ_DIS);
    AM_CRITICAL_END

    //
    // Disable the Burst Feature Event (DEVPWREVENTEN).
    //
    PWRCTRL->DEVPWREVENTEN_b.BURSTEVEN = 0;

    ui32Status = am_hal_flash_delay_status_check(10000,
                            (uint32_t)&CLKGEN->FREQCTRL,
                            CLKGEN_FREQCTRL_BURSTSTATUS_Msk,
                            0,
                            true);

    if ( ui32Status != AM_HAL_STATUS_SUCCESS )
    {
        *peBurstStatus = AM_HAL_NORMAL_MODE;
        return ui32Status;
    }

    //
    // Check the Burst Mode Status (FREQCTRL)
    //
    //
    // Check the Burst Mode Status (FREQCTRL)
    //
    if ( CLKGEN->FREQCTRL_b.BURSTSTATUS > 0 )
    {
        *peBurstStatus = AM_HAL_BURST_MODE;
    }
    else
    {
        *peBurstStatus = AM_HAL_NORMAL_MODE;
    }


    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// am_hal_burst_mode_status() - Return current burst mode state.
//
// Implemented as a macro, this function returns the current burst mode state.
//     AM_HAL_BURST_MODE
//     AM_HAL_NORMAL_MODE
//
//*****************************************************************************

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
