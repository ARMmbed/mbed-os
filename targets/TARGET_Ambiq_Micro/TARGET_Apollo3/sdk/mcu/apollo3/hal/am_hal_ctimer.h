//*****************************************************************************
//
//  am_hal_ctimer.h
//! @file
//!
//! @brief Functions for accessing and configuring the CTIMER.
//!
//! @addtogroup ctimer3 Counter/Timer (CTIMER)
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
#ifndef AM_HAL_CTIMER_H
#define AM_HAL_CTIMER_H

#ifdef __cplusplus
extern "C"
{
#endif

//
// Designate this peripheral.
//
#define AM_APOLLO3_CTIMER   1

//*****************************************************************************
//
// CTIMERADDRn()
//  This is a specialized version of AM_REGADDRn().  It is necessary because
//  the CTIMER does not work as a multi-module peripheral.  In typical
//  multi-module peripherals, the base address is defined as MODULE0_BASE.
//  For CTIMER it's CTIMER_BASE (there is no module 0 defined).
//
//  Usage:
//  CTIMER_ADDRn(CTIMER, n, reg).
//
// periph:  Must always be CTIMER.
// n:       The timer number specified as a macro, variable, etc.
// reg:     The register name always ending in '0'.  E.g. TMR0, CTRL0, CMPRB0,
//          etc (regardless of the timernum specified by 'n').
//
//*****************************************************************************
#define CTIMERADDRn(periph, n, reg) ( periph##_BASE                 +   \
                                      offsetof(periph##_Type, reg)  +   \
                                      (n * (offsetof(periph##_Type, TMR1) - offsetof(periph##_Type, TMR0))) )

//
// Enumerations for the eOutputType argument of am_hal_ctimer_output_config().
//
typedef enum
{
    AM_HAL_CTIMER_OUTPUT_NORMAL         = 0x0,
    AM_HAL_CTIMER_OUTPUT_SECONDARY      = 0x1,
    AM_HAL_CTIMER_OUTPUT_FORCE0         = 0x2,
    AM_HAL_CTIMER_OUTPUT_FORCE1         = 0x3
} am_hal_ctimer_outputtype_e;


//*****************************************************************************
//
//! CMSIS-Style macro for handling a variable CTIMER module number.
//
//*****************************************************************************
#define CTIMERn(n) ((CTIMER_Type*)(CTIMER_BASE + (n * ((uint32_t)&CTIMER->TMR1 - (uint32_t)&CTIMER->TMR0))))

//*****************************************************************************
//
//! Number of timers
//
//*****************************************************************************
#define AM_HAL_CTIMER_TIMERS_NUM    8

//*****************************************************************************
//
//! Timer offset value
//
//*****************************************************************************
#define AM_HAL_CTIMER_TIMER_OFFSET  ((uint32_t)&CTIMER->TMR1 - (uint32_t)&CTIMER->TMR0)

//*****************************************************************************
//
//! @name Interrupt Status Bits
//! @brief Interrupt Status Bits for enable/disble use
//!
//! These macros may be used to set and clear interrupt bits
//! @{
//
//*****************************************************************************
#define AM_HAL_CTIMER_INT_TIMERA0C0         CTIMER_INTEN_CTMRA0C0INT_Msk
#define AM_HAL_CTIMER_INT_TIMERA0C1         CTIMER_INTEN_CTMRA0C1INT_Msk
#define AM_HAL_CTIMER_INT_TIMERA1C0         CTIMER_INTEN_CTMRA1C0INT_Msk
#define AM_HAL_CTIMER_INT_TIMERA1C1         CTIMER_INTEN_CTMRA1C1INT_Msk
#define AM_HAL_CTIMER_INT_TIMERA2C0         CTIMER_INTEN_CTMRA2C0INT_Msk
#define AM_HAL_CTIMER_INT_TIMERA2C1         CTIMER_INTEN_CTMRA2C1INT_Msk
#define AM_HAL_CTIMER_INT_TIMERA3C0         CTIMER_INTEN_CTMRA3C0INT_Msk
#define AM_HAL_CTIMER_INT_TIMERA3C1         CTIMER_INTEN_CTMRA3C1INT_Msk
#define AM_HAL_CTIMER_INT_TIMERA4C0         CTIMER_INTEN_CTMRA4C0INT_Msk
#define AM_HAL_CTIMER_INT_TIMERA4C1         CTIMER_INTEN_CTMRA4C1INT_Msk
#define AM_HAL_CTIMER_INT_TIMERA5C0         CTIMER_INTEN_CTMRA5C0INT_Msk
#define AM_HAL_CTIMER_INT_TIMERA5C1         CTIMER_INTEN_CTMRA5C1INT_Msk
#define AM_HAL_CTIMER_INT_TIMERA6C0         CTIMER_INTEN_CTMRA6C0INT_Msk
#define AM_HAL_CTIMER_INT_TIMERA6C1         CTIMER_INTEN_CTMRA6C1INT_Msk
#define AM_HAL_CTIMER_INT_TIMERA7C0         CTIMER_INTEN_CTMRA7C0INT_Msk
#define AM_HAL_CTIMER_INT_TIMERA7C1         CTIMER_INTEN_CTMRA7C1INT_Msk

#define AM_HAL_CTIMER_INT_TIMERB0C0         CTIMER_INTEN_CTMRB0C0INT_Msk
#define AM_HAL_CTIMER_INT_TIMERB0C1         CTIMER_INTEN_CTMRB0C1INT_Msk
#define AM_HAL_CTIMER_INT_TIMERB1C0         CTIMER_INTEN_CTMRB1C0INT_Msk
#define AM_HAL_CTIMER_INT_TIMERB1C1         CTIMER_INTEN_CTMRB1C1INT_Msk
#define AM_HAL_CTIMER_INT_TIMERB2C0         CTIMER_INTEN_CTMRB2C0INT_Msk
#define AM_HAL_CTIMER_INT_TIMERB2C1         CTIMER_INTEN_CTMRB2C1INT_Msk
#define AM_HAL_CTIMER_INT_TIMERB3C0         CTIMER_INTEN_CTMRB3C0INT_Msk
#define AM_HAL_CTIMER_INT_TIMERB3C1         CTIMER_INTEN_CTMRB3C1INT_Msk
#define AM_HAL_CTIMER_INT_TIMERB4C0         CTIMER_INTEN_CTMRB4C0INT_Msk
#define AM_HAL_CTIMER_INT_TIMERB4C1         CTIMER_INTEN_CTMRB4C1INT_Msk
#define AM_HAL_CTIMER_INT_TIMERB5C0         CTIMER_INTEN_CTMRB5C0INT_Msk
#define AM_HAL_CTIMER_INT_TIMERB5C1         CTIMER_INTEN_CTMRB5C1INT_Msk
#define AM_HAL_CTIMER_INT_TIMERB6C0         CTIMER_INTEN_CTMRB6C0INT_Msk
#define AM_HAL_CTIMER_INT_TIMERB6C1         CTIMER_INTEN_CTMRB6C1INT_Msk
#define AM_HAL_CTIMER_INT_TIMERB7C0         CTIMER_INTEN_CTMRB7C0INT_Msk
#define AM_HAL_CTIMER_INT_TIMERB7C1         CTIMER_INTEN_CTMRB7C1INT_Msk
//! @}

//*****************************************************************************
//
//  DEPRECATED Interrupt Status Bits
//
//*****************************************************************************
#define AM_HAL_CTIMER_INT_TIMERA0           AM_HAL_CTIMER_INT_TIMERA0C0
#define AM_HAL_CTIMER_INT_TIMERB0           AM_HAL_CTIMER_INT_TIMERB0C0
#define AM_HAL_CTIMER_INT_TIMERA1           AM_HAL_CTIMER_INT_TIMERA1C0
#define AM_HAL_CTIMER_INT_TIMERB1           AM_HAL_CTIMER_INT_TIMERB1C0
#define AM_HAL_CTIMER_INT_TIMERA2           AM_HAL_CTIMER_INT_TIMERA2C0
#define AM_HAL_CTIMER_INT_TIMERB2           AM_HAL_CTIMER_INT_TIMERB2C0
#define AM_HAL_CTIMER_INT_TIMERA3           AM_HAL_CTIMER_INT_TIMERA3C0
#define AM_HAL_CTIMER_INT_TIMERB3           AM_HAL_CTIMER_INT_TIMERB3C0

//*****************************************************************************
//
//! @name Configuration options
//! @brief Configuration options for \e am_hal_ctimer_config_t
//!
//! These options are to be used with the \e am_hal_ctimer_config_t structure
//! used by \e am_hal_ctimer_config
//! @{
//
//*****************************************************************************
#define AM_HAL_CTIMER_CLK_PIN               _VAL2FLD(CTIMER_CTRL0_TMRA0CLK, 0x00)
#define AM_HAL_CTIMER_HFRC_12MHZ            _VAL2FLD(CTIMER_CTRL0_TMRA0CLK, 0x01)
#define AM_HAL_CTIMER_HFRC_3MHZ             _VAL2FLD(CTIMER_CTRL0_TMRA0CLK, 0x02)
#define AM_HAL_CTIMER_HFRC_187_5KHZ         _VAL2FLD(CTIMER_CTRL0_TMRA0CLK, 0x03)
#define AM_HAL_CTIMER_HFRC_47KHZ            _VAL2FLD(CTIMER_CTRL0_TMRA0CLK, 0x04)
#define AM_HAL_CTIMER_HFRC_12KHZ            _VAL2FLD(CTIMER_CTRL0_TMRA0CLK, 0x05)
#define AM_HAL_CTIMER_XT_32_768KHZ          _VAL2FLD(CTIMER_CTRL0_TMRA0CLK, 0x06)
#define AM_HAL_CTIMER_XT_16_384KHZ          _VAL2FLD(CTIMER_CTRL0_TMRA0CLK, 0x07)
#define AM_HAL_CTIMER_XT_2_048KHZ           _VAL2FLD(CTIMER_CTRL0_TMRA0CLK, 0x08)
#define AM_HAL_CTIMER_XT_256HZ              _VAL2FLD(CTIMER_CTRL0_TMRA0CLK, 0x09)
#define AM_HAL_CTIMER_LFRC_512HZ            _VAL2FLD(CTIMER_CTRL0_TMRA0CLK, 0x0A)
#define AM_HAL_CTIMER_LFRC_32HZ             _VAL2FLD(CTIMER_CTRL0_TMRA0CLK, 0x0B)
#define AM_HAL_CTIMER_LFRC_1HZ              _VAL2FLD(CTIMER_CTRL0_TMRA0CLK, 0x0C)
#define AM_HAL_CTIMER_LFRC_1_16HZ           _VAL2FLD(CTIMER_CTRL0_TMRA0CLK, 0x0D)
#define AM_HAL_CTIMER_RTC_100HZ             _VAL2FLD(CTIMER_CTRL0_TMRA0CLK, 0x0E)
#define AM_HAL_CTIMER_HCLK_DIV4             _VAL2FLD(CTIMER_CTRL0_TMRA0CLK, 0x0F)
#define AM_HAL_CTIMER_XT_DIV4               _VAL2FLD(CTIMER_CTRL0_TMRA0CLK, 0x10)
#define AM_HAL_CTIMER_XT_DIV8               _VAL2FLD(CTIMER_CTRL0_TMRA0CLK, 0x11)
#define AM_HAL_CTIMER_XT_DIV32              _VAL2FLD(CTIMER_CTRL0_TMRA0CLK, 0x12)
#define AM_HAL_CTIMER_RSVD                  _VAL2FLD(CTIMER_CTRL0_TMRA0CLK, 0x13)
//! @}

//*****************************************************************************
//
//! Timer function macros.
//!
//! @{
//
//*****************************************************************************
//! Single Count: Counts one time to the compare value, then the output
//! changes polarity and stays at that level, with an optional interrupt.
#define AM_HAL_CTIMER_FN_ONCE               _VAL2FLD(CTIMER_CTRL0_TMRA0FN, 0)
//! Repeated Count: Periodic 1-clock-cycle wide pulses with optional interrupts.
#define AM_HAL_CTIMER_FN_REPEAT             _VAL2FLD(CTIMER_CTRL0_TMRA0FN, 1)
//! Single Pulse (One Shot): A single pulse of programmed width, with an optional interrupt.
#define AM_HAL_CTIMER_FN_PWM_ONCE           _VAL2FLD(CTIMER_CTRL0_TMRA0FN, 2)
//! Repeated Pulse: A rectangular (or square) waveform with programmed high and
//! low widths, and optional interrupts on each cycle.
#define AM_HAL_CTIMER_FN_PWM_REPEAT         _VAL2FLD(CTIMER_CTRL0_TMRA0FN, 3)
//! Single Pattern: one burst of bits specified by the CMPR0/1/2/3 registers.
#define AM_HAL_CTIMER_FN_PTN_ONCE           _VAL2FLD(CTIMER_CTRL0_TMRA0FN, 4)
//! Repeated Pattern: repeated burst of bits specified by the CMPR0/1/2/3 registers.
#define AM_HAL_CTIMER_FN_PTN_REPEAT         _VAL2FLD(CTIMER_CTRL0_TMRA0FN, 5)
//! Continuous: Free running timer with a single level change on the output and
//! a single optional interrupt.
#define AM_HAL_CTIMER_FN_CONTINUOUS         _VAL2FLD(CTIMER_CTRL0_TMRA0FN, 6)
//! Alternate Pulse: like Repeated Pulse but alternating between two different
//! pulse width/spacing settings.
#define AM_HAL_CTIMER_FN_PWM_ALTERNATE      _VAL2FLD(CTIMER_CTRL0_TMRA0FN, 7)
//! @}

//*****************************************************************************
//
//! Half-timer options.
//!
//! @{
//
//*****************************************************************************
#define AM_HAL_CTIMER_INT_ENABLE            CTIMER_CTRL0_TMRA0IE0_Msk
//#define AM_HAL_CTIMER_PIN_ENABLE            CTIMER_CTRL0_TMRA0PE_Msk
#define AM_HAL_CTIMER_PIN_INVERT            CTIMER_CTRL0_TMRA0POL_Msk
#define AM_HAL_CTIMER_CLEAR                 CTIMER_CTRL0_TMRA0CLR_Msk
//! @}

//*****************************************************************************
//
//! Additional timer options.
//!
//! @{
//
//*****************************************************************************
#define AM_HAL_CTIMER_LINK                  CTIMER_CTRL0_CTLINK0_Msk
#define AM_HAL_CTIMER_ADC_TRIG              CTIMER_CTRL3_ADCEN_Msk
//! @}

//*****************************************************************************
//
//! Timer selection macros.
//!
//! @{
//
//*****************************************************************************
#define AM_HAL_CTIMER_TIMERA                0x0000FFFF
#define AM_HAL_CTIMER_TIMERB                0xFFFF0000
#define AM_HAL_CTIMER_BOTH                  0xFFFFFFFF
//! @}

//*****************************************************************************
//
//! Timer trigger options for Apollo3 Blue (rev B0 and later) including
//! Apollo3 Blue Plus.
//!
//! Valid only for CTIMER4 and CTIMER5 when CTLINK==1 and TMRA4TRIG==1
//!
//! @{
//
//*****************************************************************************
#define AM_HAL_CTIMER_AUX4_TMRB4TRIG_STIMERCAP0     CTIMER_AUX4_TMRB4TRIG_A7OUT
#define AM_HAL_CTIMER_AUX4_TMRB4TRIG_STIMERCAP1     CTIMER_AUX4_TMRB4TRIG_B7OUT
#define AM_HAL_CTIMER_AUX4_TMRB4TRIG_STIMERCAP2     CTIMER_AUX4_TMRB4TRIG_A1OUT
#define AM_HAL_CTIMER_AUX4_TMRB4TRIG_STIMERCAP3     CTIMER_AUX4_TMRB4TRIG_B1OUT
#define AM_HAL_CTIMER_AUX4_TMRB4TRIG_STIMERCMP0     CTIMER_AUX4_TMRB4TRIG_B3OUT2
#define AM_HAL_CTIMER_AUX4_TMRB4TRIG_STIMERCMP1     CTIMER_AUX4_TMRB4TRIG_A3OUT2
#define AM_HAL_CTIMER_AUX4_TMRB4TRIG_STIMERCMP2     CTIMER_AUX4_TMRB4TRIG_A1OUT2
#define AM_HAL_CTIMER_AUX4_TMRB4TRIG_STIMERCMP3     CTIMER_AUX4_TMRB4TRIG_B1OUT2
#define AM_HAL_CTIMER_AUX4_TMRB4TRIG_STIMERCMP4     CTIMER_AUX4_TMRB4TRIG_A6OUT2DUAL
#define AM_HAL_CTIMER_AUX4_TMRB4TRIG_STIMERCMP5     CTIMER_AUX4_TMRB4TRIG_A7OUT2DUAL
#define AM_HAL_CTIMER_AUX4_TMRB4TRIG_STIMERCMP6     CTIMER_AUX4_TMRB4TRIG_B5OUT2DUAL
#define AM_HAL_CTIMER_AUX4_TMRB4TRIG_STIMERCMP7     CTIMER_AUX4_TMRB4TRIG_A5OUT2DUAL

#define AM_HAL_CTIMER_AUX5_TMRB5TRIG_STIMERCAP0     CTIMER_AUX5_TMRB5TRIG_A7OUT
#define AM_HAL_CTIMER_AUX5_TMRB5TRIG_STIMERCAP1     CTIMER_AUX5_TMRB5TRIG_B7OUT
#define AM_HAL_CTIMER_AUX5_TMRB5TRIG_STIMERCAP2     CTIMER_AUX5_TMRB5TRIG_A1OUT
#define AM_HAL_CTIMER_AUX5_TMRB5TRIG_STIMERCAP3     CTIMER_AUX5_TMRB5TRIG_B1OUT
#define AM_HAL_CTIMER_AUX5_TMRB5TRIG_STIMERCMP0     CTIMER_AUX5_TMRB5TRIG_B3OUT2
#define AM_HAL_CTIMER_AUX5_TMRB5TRIG_STIMERCMP1     CTIMER_AUX5_TMRB5TRIG_A3OUT2
#define AM_HAL_CTIMER_AUX5_TMRB5TRIG_STIMERCMP2     CTIMER_AUX5_TMRB5TRIG_A1OUT2
#define AM_HAL_CTIMER_AUX5_TMRB5TRIG_STIMERCMP3     CTIMER_AUX5_TMRB5TRIG_B1OUT2
#define AM_HAL_CTIMER_AUX5_TMRB5TRIG_STIMERCMP4     CTIMER_AUX5_TMRB5TRIG_A6OUT2DUAL
#define AM_HAL_CTIMER_AUX5_TMRB5TRIG_STIMERCMP5     CTIMER_AUX5_TMRB5TRIG_A7OUT2DUAL
#define AM_HAL_CTIMER_AUX5_TMRB5TRIG_STIMERCMP6     CTIMER_AUX5_TMRB5TRIG_B5OUT2DUAL
#define AM_HAL_CTIMER_AUX5_TMRB5TRIG_STIMERCMP7     CTIMER_AUX5_TMRB5TRIG_A5OUT2DUAL
//! @}

//*****************************************************************************
//
//! @name All-In-One Configuration
//! @brief New API for multiple timer configuration.
//!
//! These options are to be used with the \e am_hal_ctimer_config_t structure
//! used by \e am_hal_ctimer_config
//! @{
//
//*****************************************************************************
//! CTimer AIO Compare Configuration.
typedef struct
{
    //
    //! Function Number.
    //
    uint32_t FN;
    //
    //! Timer Segment. Timer A, B, BOTH selector.
    //
    uint32_t AB;
    //
    //! Compare Register A0.
    //
    uint32_t A0;
    //
    //! Compare Register A1.
    //
    uint32_t A1;
    //
    //! Compare Register A2.
    //
    uint32_t A2;
    //
    //! Compare Register A3.
    //
    uint32_t A3;
    //
    //! Compare Register B0.
    //
    uint32_t B0;
    //
    //! Compare Register B1.
    //
    uint32_t B1;
    //
    //! Compare Register B2.
    //
    uint32_t B2;
    //
    //! Compare Register B3.
    //
    uint32_t B3;
    //
    //! LMT field values.
    //
    uint32_t LMT;
    //
    //! A "T" indicates that a 1 is loaded if the OUT2 output is used, otherwise a 0 is loaded.
    //
    uint32_t EN23;
    //
    //! TRIG: a single pattern will be triggered; TERM: a repeated pattern will be terminated.
    //
    uint32_t TRIG;
    //
    //! Select clock source: internal, external, a buck pulse, or output of another CTIMER.
    //
    uint32_t CLK;
    //
    //!  Enable the primary interrupt INT.
    //
    uint32_t IE0;
    //
    //!  Enable the secondary interrupt INT2.
    //
    uint32_t IE1;
    //
    //!  Select the polarity of the OUT output.
    //
    uint32_t POL;
    //
    //!  Select the polarity of the OUT2 output.
    //
    uint32_t POL23;
    //
    //! Select polarity of both OUT and OUT2 as a function of the trigger input.
    //
    uint32_t TINV;
    //
    //!  Disable clock synchronization on read.
    //
    uint32_t NOSYNC;
    //
    //! Enable the timer.
    // This is ANDed with the global enable in GLOBEN, and allows the counter to begin counting.
    //
    uint32_t EN;
    //
    //  Clear the timer. This will hold the timer at zero even if EN is asserted.
    //  It is typically cleared at the end of a configuration and
    //  is probably not included in the function structure.
    //
    //uint32_t CLR;

}
am_hal_ctimer_aio_config_t;

//! CTimer AIO Output Selection and Interconnect.
typedef struct
{
    //! Pad 0-9
    uint32_t OUTCFG0;
    //! Pad 10-19
    uint32_t OUTCFG1;
    //! Pad 20-29
    uint32_t OUTCFG2;
    //! Pad 30-31
    uint32_t OUTCFG3;
}
am_hal_ctimer_aio_connect_t;
//! @}

//*****************************************************************************
//
//! Timer configuration structure
//
//*****************************************************************************
typedef struct
{
    //
    //! Set to 1 to operate this timer as a 32-bit timer instead of two 16-bit
    //! timers.
    //
    uint32_t ui32Link;

    //
    //! Configuration options for TIMERA
    //
    uint32_t ui32TimerAConfig;

    //
    //! Configuration options for TIMERB
    //
    uint32_t ui32TimerBConfig;

}
am_hal_ctimer_config_t;

//*****************************************************************************
//
//! Function pointer type for CTimer interrupt handlers.
//
//*****************************************************************************
typedef void (*am_hal_ctimer_handler_t)(void);

//*****************************************************************************
//
// External function definitions
//
//*****************************************************************************
extern void am_hal_ctimer_globen(uint32_t ui32ConfigVal);

extern void am_hal_ctimer_config(uint32_t ui32TimerNumber,
                                 am_hal_ctimer_config_t *psConfig);

extern void am_hal_ctimer_config_single(uint32_t ui32TimerNumber,
                                        uint32_t ui32TimerSegment,
                                        uint32_t ui32ConfigVal);

extern void am_hal_ctimer_config_trigger(uint32_t ui32TimerNumber,
                                         uint32_t ui32TimerSegment,
                                         uint32_t ui32ConfigVal);

extern void am_hal_ctimer_start(uint32_t ui32TimerNumber,
                                uint32_t ui32TimerSegment);

extern void am_hal_ctimer_stop(uint32_t ui32TimerNumber,
                               uint32_t ui32TimerSegment);

extern void am_hal_ctimer_clear(uint32_t ui32TimerNumber,
                                uint32_t ui32TimerSegment);

extern uint32_t am_hal_ctimer_read(uint32_t ui32TimerNumber,
                                   uint32_t ui32TimerSegment);

extern uint32_t am_hal_ctimer_output_config(uint32_t ui32TimerNumber,
                                            uint32_t ui32TimerSegment,
                                            uint32_t ui32PadNum,
                                            uint32_t eOutputType,
                                            uint32_t eDriveStrength);

extern void am_hal_ctimer_input_config(uint32_t ui32TimerNumber,
                                       uint32_t ui32TimerSegment,
                                       uint32_t ui32TimerOutputConfig);

extern void am_hal_ctimer_compare_set(uint32_t ui32TimerNumber,
                                      uint32_t ui32TimerSegment,
                                      uint32_t ui32CompareReg,
                                      uint32_t ui32Value);

extern void am_hal_ctimer_aux_compare_set(uint32_t ui32TimerNumber,
                                      uint32_t ui32TimerSegment,
                                      uint32_t ui32CompareReg,
                                      uint32_t ui32Value);

extern void am_hal_ctimer_period_set(uint32_t ui32TimerNumber,
                                     uint32_t ui32TimerSegment,
                                     uint32_t ui32Period,
                                     uint32_t ui32OnTime);

extern void am_hal_ctimer_aux_period_set(uint32_t ui32TimerNumber,
                                     uint32_t ui32TimerSegment,
                                     uint32_t ui32Period,
                                     uint32_t ui32OnTime);

extern void am_hal_ctimer_adc_trigger_enable(void);
extern void am_hal_ctimer_adc_trigger_disable(void);
extern void am_hal_ctimer_int_enable(uint32_t ui32Interrupt);
extern uint32_t am_hal_ctimer_int_enable_get(void);
extern void am_hal_ctimer_int_disable(uint32_t ui32Interrupt);
extern void am_hal_ctimer_int_set(uint32_t ui32Interrupt);
extern void am_hal_ctimer_int_clear(uint32_t ui32Interrupt);
extern uint32_t am_hal_ctimer_int_status_get(bool bEnabledOnly);
extern void am_hal_ctimer_int_register(uint32_t ui32Interrupt,
                         am_hal_ctimer_handler_t pfnHandler);
extern void am_hal_ctimer_int_service(uint32_t ui32Status);

//
// General function to do triple back-to-back reads.
//
extern void am_hal_triple_read(uint32_t u32TimerAddr, uint32_t ui32Data[]);

#ifdef __cplusplus
}
#endif

#endif // AM_HAL_CTIMER_H

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
