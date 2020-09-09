//*****************************************************************************
//
//  am_hal_clkgen.h
//! @file
//!
//! @brief Functions for accessing and configuring the CLKGEN.
//!
//! @addtogroup clkgen3 Clock Generator (CLKGEN)
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
#ifndef AM_HAL_CLKGEN_H
#define AM_HAL_CLKGEN_H

#ifdef __cplusplus
extern "C"
{
#endif


//
// Designate this peripheral.
//
#define AM_APOLLO3_CLKGEN   1

//*****************************************************************************
//
//! @name System Clock max frequency
//! @brief Defines the maximum clock frequency for this device.
//!
//! These macros provide a definition of the maximum clock frequency.
//!
//! @{
//
//*****************************************************************************
#define AM_HAL_CLKGEN_FREQ_MAX_HZ       48000000
#define AM_HAL_CLKGEN_FREQ_MAX_KHZ      (AM_HAL_CLKGEN_FREQ_MAX_HZ / 1000)
#define AM_HAL_CLKGEN_FREQ_MAX_MHZ      (AM_HAL_CLKGEN_FREQ_MAX_HZ / 1000000)
#define AM_HAL_CLKGEN_CORESEL_MAXDIV    1
//! @}

//
// Control operations.
//
typedef enum
{
    AM_HAL_CLKGEN_CONTROL_SYSCLK_MAX,
    AM_HAL_CLKGEN_CONTROL_XTAL_START,
    AM_HAL_CLKGEN_CONTROL_LFRC_START,
    AM_HAL_CLKGEN_CONTROL_XTAL_STOP,
    AM_HAL_CLKGEN_CONTROL_LFRC_STOP,
    AM_HAL_CLKGEN_CONTROL_SYSCLK_DIV2,
    AM_HAL_CLKGEN_CONTROL_RTC_SEL_XTAL,
    AM_HAL_CLKGEN_CONTROL_RTC_SEL_LFRC,
    AM_HAL_CLKGEN_CONTROL_HFADJ_ENABLE,
    AM_HAL_CLKGEN_CONTROL_HFADJ_DISABLE,
} am_hal_clkgen_control_e;

//
// Current RTC oscillator.
//
typedef enum
{
    AM_HAL_CLKGEN_STATUS_RTCOSC_XTAL,
    AM_HAL_CLKGEN_STATUS_RTCOSC_LFRC,
} am_hal_clkgen_status_rtcosc_e;

//
// CLKOUT
//
typedef enum
{
    AM_HAL_CLKGEN_CLKOUT_LFRC_1024  = 0x0,      // LFRC
    AM_HAL_CLKGEN_CLKOUT_XTAL_16384,            // XTAL / 2
    AM_HAL_CLKGEN_CLKOUT_XTAL_8192,             // XTAL / 4
    AM_HAL_CLKGEN_CLKOUT_XTAL_4096,             // XTAL / 8
    AM_HAL_CLKGEN_CLKOUT_XTAL_2048,             // XTAL / 16
    AM_HAL_CLKGEN_CLKOUT_XTAL_1024,             // XTAL / 32
    AM_HAL_CLKGEN_CLKOUT_RTC_1HZ    = 0x10,     // RTC
    AM_HAL_CLKGEN_CLKOUT_XTAL_0_015 = 0x16,     // XTAL / 2097152 = 0.015625 Hz
    AM_HAL_CLKGEN_CLKOUT_XTAL_32768,            // XTAL
    AM_HAL_CLKGEN_CLKOUT_CG_100,                // ClkGen 100Hz
    AM_HAL_CLKGEN_CLKOUT_LFRC_512 = 0x23,       // LFRC / 2     = 512 Hz
    AM_HAL_CLKGEN_CLKOUT_LFRC_32,               // LFRC / 32    =  32 Hz
    AM_HAL_CLKGEN_CLKOUT_LFRC_2,                // LFRC / 512   =   2 Hz
    AM_HAL_CLKGEN_CLKOUT_LFRC_0_03,             // LFRC / 32768 = 0.03125 Hz
    AM_HAL_CLKGEN_CLKOUT_XTAL_128,              // XTAL / 256   = 128 Hz
    AM_HAL_CLKGEN_CLKOUT_XTAL_4,                // XTAL / 8192  =  4 Hz
    AM_HAL_CLKGEN_CLKOUT_XTAL_0_5,              // XTAL / 65536 =  0.5 Hz
    // The next 5 are Uncalibrated LFRC
    AM_HAL_CLKGEN_CLKOUT_ULFRC_64,              // ULFRC / 16   = 64 Hz (uncal LFRC)
    AM_HAL_CLKGEN_CLKOUT_ULFRC_8,               // ULFRC / 128  =  8 Hz (uncal LFRC)
    AM_HAL_CLKGEN_CLKOUT_ULFRC_1,               // ULFRC / 1024 =  1 Hz (uncal LFRC)
    AM_HAL_CLKGEN_CLKOUT_ULFRC_0_25,            // ULFRC / 4096 = 0.25 Hz (uncal LFRC)
    AM_HAL_CLKGEN_CLKOUT_ULFRC_0_0009,          // ULFRC / 1M   = 0.000976 Hz (uncal LFRC)
    //
    AM_HAL_CLKGEN_CLKOUT_LFRC_0_0004 = 0x31,    // LFRC / 2M    = 0.00048828125 Hz
    // Following are Not Autoenabled ("NE")
    AM_HAL_CLKGEN_CLKOUT_XTALNE_32768 = 0x35,   // XTALNE / 1   = 32768 Hz
    AM_HAL_CLKGEN_CLKOUT_XTALNE_2048,           // XTALNE / 16  =  2048 Hz
    AM_HAL_CLKGEN_CLKOUT_LFRCNE_32,             // LFRCNE / 32  =    32 Hz
    AM_HAL_CLKGEN_CLKOUT_LFRCNE_1024 = 0x39     // LFRCNE / 1   =  1024 Hz
} am_hal_clkgen_clkout_e;

//
// ClkGen Interrupts
//
typedef enum
{
    AM_HAL_CLKGEN_INTERRUPT_OF          = CLKGEN_INTRPTEN_OF_Msk,
    AM_HAL_CLKGEN_INTERRUPT_ACC         = CLKGEN_INTRPTEN_ACC_Msk,
    AM_HAL_CLKGEN_INTERRUPT_ACF         = CLKGEN_INTRPTEN_ACF_Msk
} am_hal_clkgen_interrupt_e;

//
// Status structure.
//
typedef struct
{
    //
    // ui32SysclkFreq
    //  Returns the current system clock frequency, in hertz.
    //
    uint32_t    ui32SysclkFreq;

    //
    // ui32RTCoscillator
    //
    //  Returns the current RTC oscillator as one of:
    //  AM_HAL_CLKGEN_STATUS_RTCOSC_LFRC
    //  AM_HAL_CLKGEN_STATUS_RTCOSC_XTAL
    //
    uint32_t    eRTCOSC;

    //
    // bXtalFailure
    //  true = XTAL has failed (is enabled but not oscillating).  Also if the
    //         LFRC is selected as the oscillator in OCTRL.OSEL.
    //
    bool        bXtalFailure;
} am_hal_clkgen_status_t;


// ****************************************************************************
//
//! @brief Apply various specific commands/controls on the CLKGEN module.
//!
//! This function is used to apply various controls on CLKGEN.
//!
//! @note IMPORTANT! This function MUST be called very early in execution of
//!       an application with the parameter AM_HAL_CLKGEN_CONTROL_SYSCLK_MAX
//!       in order to set Apollo3 to its required operating frequency.
//!
//! @param eControl - One of the following:
//!     AM_HAL_CLKGEN_CONTROL_SYSCLK_MAX
//!     AM_HAL_CLKGEN_CONTROL_XTAL_START
//!     AM_HAL_CLKGEN_CONTROL_LFRC_START
//!     AM_HAL_CLKGEN_CONTROL_XTAL_STOP
//!     AM_HAL_CLKGEN_CONTROL_LFRC_STOP
//!     AM_HAL_CLKGEN_CONTROL_RTC_SEL_XTAL
//!     AM_HAL_CLKGEN_CONTROL_RTC_SEL_LFRC
//!     AM_HAL_CLKGEN_CONTROL_HFADJ_ENABLE
//!     AM_HAL_CLKGEN_CONTROL_HFADJ_DISABLE
//!
//! @return status      - generic or interface specific status.
//!
//! @note After starting the XTAL, a 2 second warm-up delay is required.
//
// ****************************************************************************
extern uint32_t am_hal_clkgen_control(am_hal_clkgen_control_e eControl,
                                      void *pArgs);

// ****************************************************************************
//
//! @brief Get CLKGEN status.
//!
//! This function returns the current value of various CLKGEN statuses.
//!
//! @param psStatus - ptr to a status structure to receive the current statuses.
//!
//! @return status      - generic or interface specific status.
//!
//! @note After selection of the RTC Oscillator, a 2 second delay is required
//! before the new oscillator takes effect.  Therefore the CLKGEN.STATUS.OMODE
//! bit will not reflect the new status until after the 2s wait period.
//
// ****************************************************************************
extern uint32_t am_hal_clkgen_status_get(am_hal_clkgen_status_t *psStatus);

// ****************************************************************************
//
//! @brief Enable CLKOUT.
//!
//! This function is used to enable and select a CLKOUT frequency.
//!
//! @param bEnable: true to enable, false to disable.
//! @param eClkSelect - One of the following:
//!     AM_HAL_CLKGEN_CLKOUT_LFRC_1024
//!     AM_HAL_CLKGEN_CLKOUT_XTAL_16384
//!     AM_HAL_CLKGEN_CLKOUT_XTAL_8192
//!     AM_HAL_CLKGEN_CLKOUT_XTAL_4096
//!     AM_HAL_CLKGEN_CLKOUT_XTAL_2048
//!     AM_HAL_CLKGEN_CLKOUT_XTAL_1024
//!     AM_HAL_CLKGEN_CLKOUT_RTC_1HZ
//!     AM_HAL_CLKGEN_CLKOUT_XTAL_0_015
//!     AM_HAL_CLKGEN_CLKOUT_XTAL_32768
//!     AM_HAL_CLKGEN_CLKOUT_CG_100
//!     AM_HAL_CLKGEN_CLKOUT_LFRC_512
//!     AM_HAL_CLKGEN_CLKOUT_LFRC_32
//!     AM_HAL_CLKGEN_CLKOUT_LFRC_2
//!     AM_HAL_CLKGEN_CLKOUT_LFRC_0_03
//!     AM_HAL_CLKGEN_CLKOUT_XTAL_128
//!     AM_HAL_CLKGEN_CLKOUT_XTAL_4
//!     AM_HAL_CLKGEN_CLKOUT_XTAL_0_5
//!
//!     The next 5 are Uncalibrated LFRC
//!     AM_HAL_CLKGEN_CLKOUT_ULFRC_64
//!     AM_HAL_CLKGEN_CLKOUT_ULFRC_8
//!     AM_HAL_CLKGEN_CLKOUT_ULFRC_1
//!     AM_HAL_CLKGEN_CLKOUT_ULFRC_0_25
//!     AM_HAL_CLKGEN_CLKOUT_ULFRC_0_0009
//!
//!     AM_HAL_CLKGEN_CLKOUT_LFRC_0_0004
//!
//!     Following are Not Autoenabled ("NE")
//!     AM_HAL_CLKGEN_CLKOUT_XTALNE_32768
//!     AM_HAL_CLKGEN_CLKOUT_XTALNE_2048
//!     AM_HAL_CLKGEN_CLKOUT_LFRCNE_32
//!     AM_HAL_CLKGEN_CLKOUT_LFRCNE_1024
//!
//! @return status      - generic or interface specific status.
//
// ****************************************************************************
extern uint32_t am_hal_clkgen_clkout_enable(bool bEnable,
                                            am_hal_clkgen_clkout_e eClkSelect);

// ****************************************************************************
//
//! @brief Enable selected CLKGEN Interrupts.
//!
//! Use this function to enable the interrupts.
//!
//! @param ui32IntMask - One or more of the following bitmasks.
//!     AM_HAL_CLKGEN_INTERRUPT_OF
//!     AM_HAL_CLKGEN_INTERRUPT_ACC
//!     AM_HAL_CLKGEN_INTERRUPT_ACF
//!
//! @return status      - generic or interface specific status.
//
// ****************************************************************************
extern uint32_t am_hal_clkgen_interrupt_enable(am_hal_clkgen_interrupt_e ui32IntMask);

// ****************************************************************************
//
//! @brief Disable selected CLKGEN Interrupts.
//!
//! Use this function to disable the CLKGEN interrupts.
//!
//! @param ui32IntMask - One or more of the following bitmasks.
//!     AM_HAL_CLKGEN_INTERRUPT_OF
//!     AM_HAL_CLKGEN_INTERRUPT_ACC
//!     AM_HAL_CLKGEN_INTERRUPT_ACF
//!
//! @return status      - generic or interface specific status.
//
// ****************************************************************************
extern uint32_t am_hal_clkgen_interrupt_disable(am_hal_clkgen_interrupt_e ui32IntMask);

//*****************************************************************************
//
//! @brief IOM interrupt clear
//!
//! @param ui32IntMask  - interface specific interrupt mask.
//!
//! This function clears the interrupts for the given peripheral.
//!
//! The following are valid clear bits, any of which can be ORed together.
//!     AM_HAL_CLKGEN_INTERRUPT_OF
//!     AM_HAL_CLKGEN_INTERRUPT_ACC
//!     AM_HAL_CLKGEN_INTERRUPT_ACF
//!
//! @return status      - generic or interface specific status.
//
//*****************************************************************************
extern uint32_t am_hal_clkgen_interrupt_clear(am_hal_clkgen_interrupt_e ui32IntMask);

// ****************************************************************************
//
//! @brief Return CLKGEN interrupts.
//!
//! Use this function to get all CLKGEN interrupts, or only the interrupts
//! that are enabled.
//!
//! @return All or only enabled CLKGEN interrupts.
//
// ****************************************************************************
extern uint32_t am_hal_clkgen_interrupt_status_get(bool bEnabledOnly,
                                                   uint32_t *pui32IntStatus);

// ****************************************************************************
//
//! @brief Sets the interrupt status.
//!
//! This function sets the CLKGEN interrupts.
//!
//! @param ui32IntMask - One or more of the following bitmasks.
//!     AM_HAL_CLKGEN_INTERRUPT_OF
//!     AM_HAL_CLKGEN_INTERRUPT_ACC
//!     AM_HAL_CLKGEN_INTERRUPT_ACF
//!
//! @return None.
//
// ****************************************************************************
extern uint32_t am_hal_clkgen_interrupt_set(am_hal_clkgen_interrupt_e ui32IntMask);

#ifdef __cplusplus
}
#endif

#endif // AM_HAL_CLKGEN_H

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
