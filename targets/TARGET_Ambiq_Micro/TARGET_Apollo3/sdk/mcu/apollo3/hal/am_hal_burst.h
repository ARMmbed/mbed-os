//*****************************************************************************
//
//  am_hal_burst.h
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
#ifndef AM_HAL_BURST_H
#define AM_HAL_BURST_H

#ifdef __cplusplus
extern "C"
{
#endif


//*****************************************************************************
//
// Burst Mode Status enums
//
//*****************************************************************************
//
// Avail - the result of a feature availability interrogation.
//
typedef enum
{
    AM_HAL_BURST_AVAIL,
    AM_HAL_BURST_NOTAVAIL
} am_hal_burst_avail_e;

//
// Mode - the result of a change request.
//
typedef enum
{
    AM_HAL_BURST_MODE,
    AM_HAL_NORMAL_MODE,
} am_hal_burst_mode_e;

//*****************************************************************************
//
//! @brief Burst mode initialization function
//!
//! @param peBurstAvail - Availibility of feature
//!
//! This function initializes the Apollo3 MCU for Burst Mode operation. It does
//! not set the MCU into Burst Mode.  This should be called once at system
//! initialization if Burst Mode is going to be used in the system.
//!
//! @return status of API call.
//
//*****************************************************************************
extern uint32_t am_hal_burst_mode_initialize(am_hal_burst_avail_e *peBurstAvail);

//*****************************************************************************
//
//! @brief Burst mode enable function
//!
//! @param peBurstStatus - resulting mode after call.
//!
//! This function enables the Apollo3 MCU into Burst Mode operation.
//!
//! @return status of API call.
//
//*****************************************************************************
extern uint32_t am_hal_burst_mode_enable(am_hal_burst_mode_e *peBurstStatus);

//*****************************************************************************
//
//! @brief Burst mode disable function
//!
//! @param peBurstStatus - resulting mode after call.
//!
//! This function disables the Apollo3 MCU from Burst Mode operation. It returns
//! the MCU to Normal Mode.
//!
//! @return status of API call.
//
//*****************************************************************************
extern uint32_t am_hal_burst_mode_disable(am_hal_burst_mode_e *peBurstStatus);

//*****************************************************************************
//
//! @brief Return current burst mode state
//!
//! Implemented as a macro, this function returns the current burst mode state.
//!     AM_HAL_BURST_MODE
//!     AM_HAL_NORMAL_MODE
//
//*****************************************************************************
#define am_hal_burst_mode_status()                                          \
    (CLKGEN->FREQCTRL_b.BURSTSTATUS ? AM_HAL_BURST_MODE : AM_HAL_NORMAL_MODE)

#ifdef __cplusplus
}
#endif

#endif // AM_HAL_BURST_H

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
