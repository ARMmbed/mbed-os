//*****************************************************************************
//
//  am_hal_stimer.c
//! @file
//!
//! @brief Functions for interfacing with the system timer (STIMER).
//!
//! @addtogroup stimer3 System Timer (STIMER)
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
//! @brief Set up the stimer.
//!
//! @param ui32STimerConfig is the value to load into the configuration reg.
//!
//! This function should be used to perform the initial set-up of the
//! stimer.
//!
//! @return The 32-bit current config of the STimer Config register
//
//*****************************************************************************
uint32_t
am_hal_stimer_config(uint32_t ui32STimerConfig)
{
    uint32_t ui32CurrVal;

    //
    // Read the current config
    //
    ui32CurrVal = CTIMER->STCFG;

    //
    // Write our configuration value.
    //
    CTIMER->STCFG = ui32STimerConfig;

#if AM_PART_APOLLO2
    //
    // If all of the clock sources are not HFRC, disable LDO when sleeping if timers are enabled.
    //
    if ( (CTIMER->STCFG_b.CLKSELCTIMER->STCFG_b.CLKSEL == AM_REG_CTIMER_STCFG_CLKSEL_HFRC_DIV16)    ||
         (CTIMER->STCFG_b.CLKSELCTIMER->STCFG_b.CLKSEL == AM_REG_CTIMER_STCFG_CLKSEL_HFRC_DIV256) )
    {
        PWRCTRL->MISC_b.FORCEMEMVRLPTIMERS = 0;
    }
    else
    {
        PWRCTRL->MISC_b.FORCEMEMVRLPTIMERS = 1;
    }
#endif

    return ui32CurrVal;
}

//*****************************************************************************
//
//! @brief Get the current stimer value.
//!
//! This function can be used to read, uninvasively, the value in the stimer.
//!
//! @return The 32-bit value from the STimer counter register.
//
//*****************************************************************************
uint32_t
am_hal_stimer_counter_get(void)
{
    return CTIMER->STTMR;
}

//*****************************************************************************
//
//! @brief Clear the stimer counter.
//!
//! This function clears the STimer Counter and leaves the stimer running.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_stimer_counter_clear(void)
{
    //
    // Set the clear bit
    //
    CTIMER->STCFG |= CTIMER_STCFG_CLEAR_Msk;

    //
    // Reset the clear bit
    //
    CTIMER->STCFG &= ~CTIMER_STCFG_CLEAR_Msk;
}

//*****************************************************************************
//
//! @brief Set the compare value.
//!
//! @param ui32CmprInstance is the compare register instance number (0-7).
//! @param ui32Delta is the value to add to the STimer counter and load into
//!        the comparator register.
//!
//! NOTE: There is no way to set an absolute value into a comparator register.
//!       Only deltas added to the STimer counter can be written to the compare
//!       registers.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_stimer_compare_delta_set(uint32_t ui32CmprInstance, uint32_t ui32Delta)
{
    uint32_t cfgVal;
    uint32_t numTries = 0;

    if ( ui32CmprInstance > 7 )
    {
        return;
    }

    // We need to disable the compare temporarily while setting the delta value
    // That leaves a corner case where we could miss the trigger if setting a very
    // small delta. To avoid this, we take critical section, and we should ensure
    // that delta value is at least > 1

    //
    // Start a critical section.
    //
    AM_CRITICAL_BEGIN

    //
    // Get current CFG value
    //
    cfgVal = CTIMER->STCFG;

    //
    // Disable the compare if already enabled, when setting the new value
    //
    CTIMER->STCFG &= ~((AM_HAL_STIMER_CFG_COMPARE_A_ENABLE << ui32CmprInstance));

    // In rare case the delta might not be effective
    // We retry if that is the case.
    // Allow for some variability in the value owing to execution latency
    while (numTries++ < 4)
    {
        uint32_t expVal;
        uint32_t expMax;
        uint32_t cmpVal;

        // Expected value
        expVal = CTIMER->STTMR + ui32Delta;

        // Max allowed - taking care of latency
        expMax = expVal + 10;

        //
        // Set the delta
        //
        AM_REGVAL(AM_REG_STIMER_COMPARE(0, ui32CmprInstance)) = ui32Delta;

        // Read back the compare value
        cmpVal = AM_REGVAL(AM_REG_STIMER_COMPARE(0, ui32CmprInstance));

        // Make sure the value is in expected range
        if (!AM_HAL_U32_SMALLER(cmpVal, expVal) && !AM_HAL_U32_GREATER(cmpVal, expMax))
        {
            break;
        }
    }


    //
    // Restore Compare Enable bit
    //
    CTIMER->STCFG |= cfgVal & (AM_HAL_STIMER_CFG_COMPARE_A_ENABLE << ui32CmprInstance);

    //
    // End the critical section.
    //
    AM_CRITICAL_END
}

//*****************************************************************************
//
//! @brief Get the current stimer compare register value.
//!
//! @param ui32CmprInstance is the compare register instance number (0-7).
//!
//! This function can be used to read the value in an stimer compare register.
//!
//!
//! @return None.
//
//*****************************************************************************
uint32_t
am_hal_stimer_compare_get(uint32_t ui32CmprInstance)
{
    if ( ui32CmprInstance > 7 )
    {
        return 0;
    }

    return AM_REGVAL(AM_REG_STIMER_COMPARE(0, ui32CmprInstance));
}

//*****************************************************************************
//
//! @brief Start capturing data with the specified capture register.
//!
//! @param ui32CaptureNum is the Capture Register Number to read (0-3).
//! @param ui32GPIONumber is the pin number.
//! @param bPolarity: false (0) = Capture on low to high transition.
//!                   true  (1) = Capture on high to low transition.
//!
//! Use this function to start capturing.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_stimer_capture_start(uint32_t ui32CaptureNum,
                            uint32_t ui32GPIONumber,
                            bool bPolarity)
{
    uint32_t ui32CapCtrl;

    if ( ui32GPIONumber > (AM_HAL_GPIO_MAX_PADS-1) )
    {
        return;
    }

    //
    // Set the polarity and pin selection in the GPIO block.
    //
    switch (ui32CaptureNum)
    {
         case 0:
            GPIO->STMRCAP_b.STPOL0 = bPolarity;
            GPIO->STMRCAP_b.STSEL0 = ui32GPIONumber;
            ui32CapCtrl = CTIMER_CAPTURECONTROL_CAPTURE0_Msk;
            break;
         case 1:
            GPIO->STMRCAP_b.STPOL1 = bPolarity;
            GPIO->STMRCAP_b.STSEL1 = ui32GPIONumber;
            ui32CapCtrl = CTIMER_CAPTURECONTROL_CAPTURE1_Msk;
            break;
         case 2:
            GPIO->STMRCAP_b.STPOL2 = bPolarity;
            GPIO->STMRCAP_b.STSEL2 = ui32GPIONumber;
            ui32CapCtrl = CTIMER_CAPTURECONTROL_CAPTURE2_Msk;
            break;
         case 3:
            GPIO->STMRCAP_b.STPOL3 = bPolarity;
            GPIO->STMRCAP_b.STSEL3 = ui32GPIONumber;
            ui32CapCtrl = CTIMER_CAPTURECONTROL_CAPTURE3_Msk;
            break;
         default:
            return;     // error concealment.
    }

    //
    // Enable it in the CTIMER Block
    //
    CTIMER->CAPTURECONTROL |= ui32CapCtrl;
}

//*****************************************************************************
//
//! @brief Start capturing data with the specified capture register.
//!
//! @param ui32CaptureNum is the Capture Register Number to read.
//!
//! Use this function to start capturing.
//!
//! @return None.
//
//*****************************************************************************
void am_hal_stimer_capture_stop(uint32_t ui32CaptureNum)
{
    //
    // Disable it in the CTIMER block.
    //
    CTIMER->CAPTURECONTROL &=
        ~(CTIMER_CAPTURECONTROL_CAPTURE0_Msk <<
          ((CTIMER_CAPTURECONTROL_CAPTURE1_Pos -
            CTIMER_CAPTURECONTROL_CAPTURE0_Pos) * ui32CaptureNum));
}

//*****************************************************************************
//
//! @brief Get the current stimer nvram register value.
//!
//! @param ui32NvramNum is the NVRAM Register Number to read.
//! @param ui32NvramVal is the value to write to NVRAM.
//!
//! This function can be used to read the value in an stimer NVRAM register.
//!
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_stimer_nvram_set(uint32_t ui32NvramNum, uint32_t ui32NvramVal)
{
    if ( ui32NvramNum > 3 )
    {
        return;
    }

    //AM_REGn(CTIMER, 0, SNVR)
    //AM_REG_STIMER_NVRAM(0, ui32NvramNum) = ui32NvramVal;
}

//*****************************************************************************
//
//! @brief Get the current stimer nvram register value.
//!
//! @param ui32NvramNum is the NVRAM Register Number to read.
//!
//! This function can be used to read the value in an stimer NVRAM register.
//!
//!
//! @return None.
//
//*****************************************************************************
uint32_t am_hal_stimer_nvram_get(uint32_t ui32NvramNum)
{
    if ( ui32NvramNum > 3 )
    {
        return 0;
    }

    return AM_REGVAL(AM_REG_STIMER_NVRAM(0, ui32NvramNum));
}

//*****************************************************************************
//
//! @brief Get the current stimer capture register value.
//!
//! @param ui32CaptureNum is the Capture Register Number to read.
//!
//! This function can be used to read the value in an stimer capture register.
//!
//!
//! @return None.
//
//*****************************************************************************
uint32_t am_hal_stimer_capture_get(uint32_t ui32CaptureNum)
{
    if ( ui32CaptureNum > 3 )
    {
        return 0;
    }

    return AM_REGVAL(AM_REG_STIMER_CAPTURE(0, ui32CaptureNum));
}

//*****************************************************************************
//
//! @brief Enables the selected system timer interrupt.
//!
//! @param ui32Interrupt is the interrupt to be used.
//!
//! This function will enable the selected interrupts in the STIMER interrupt
//! enable register. In order to receive an interrupt from an stimer component,
//! you will need to enable the interrupt for that component in this main
//! register, as well as in the stimer configuration register (accessible though
//! am_hal_stimer_config()), and in the NVIC.
//!
//! ui32Interrupt should be the logical OR of one or more of the following
//! values:
//!
//!     AM_HAL_STIMER_INT_COMPAREA
//!     AM_HAL_STIMER_INT_COMPAREB
//!     AM_HAL_STIMER_INT_COMPAREC
//!     AM_HAL_STIMER_INT_COMPARED
//!     AM_HAL_STIMER_INT_COMPAREE
//!     AM_HAL_STIMER_INT_COMPAREF
//!     AM_HAL_STIMER_INT_COMPAREG
//!     AM_HAL_STIMER_INT_COMPAREH
//!
//!     AM_HAL_STIMER_INT_OVERFLOW
//!
//!     AM_HAL_STIMER_INT_CAPTUREA
//!     AM_HAL_STIMER_INT_CAPTUREB
//!     AM_HAL_STIMER_INT_CAPTUREC
//!     AM_HAL_STIMER_INT_CAPTURED
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_stimer_int_enable(uint32_t ui32Interrupt)
{
    //
    // Enable the interrupt at the module level.
    //
    CTIMERn(0)->STMINTEN |= ui32Interrupt;
}

//*****************************************************************************
//
//! @brief Return the enabled stimer interrupts.
//!
//! This function will return all enabled interrupts in the STIMER
//! interrupt enable register.
//!
//! @return return enabled interrupts. This will be a logical or of:
//!
//!     AM_HAL_STIMER_INT_COMPAREA
//!     AM_HAL_STIMER_INT_COMPAREB
//!     AM_HAL_STIMER_INT_COMPAREC
//!     AM_HAL_STIMER_INT_COMPARED
//!     AM_HAL_STIMER_INT_COMPAREE
//!     AM_HAL_STIMER_INT_COMPAREF
//!     AM_HAL_STIMER_INT_COMPAREG
//!     AM_HAL_STIMER_INT_COMPAREH
//!
//!     AM_HAL_STIMER_INT_OVERFLOW
//!
//!     AM_HAL_STIMER_INT_CAPTUREA
//!     AM_HAL_STIMER_INT_CAPTUREB
//!     AM_HAL_STIMER_INT_CAPTUREC
//!     AM_HAL_STIMER_INT_CAPTURED
//!
//! @return Return the enabled timer interrupts.
//
//*****************************************************************************
uint32_t
am_hal_stimer_int_enable_get(void)
{
    //
    // Return enabled interrupts.
    //
    return CTIMERn(0)->STMINTEN;
}

//*****************************************************************************
//
//! @brief Disables the selected stimer interrupt.
//!
//! @param ui32Interrupt is the interrupt to be used.
//!
//! This function will disable the selected interrupts in the STIMER
//! interrupt register.
//!
//! ui32Interrupt should be the logical OR of one or more of the following
//! values:
//!
//!     AM_HAL_STIMER_INT_COMPAREA
//!     AM_HAL_STIMER_INT_COMPAREB
//!     AM_HAL_STIMER_INT_COMPAREC
//!     AM_HAL_STIMER_INT_COMPARED
//!     AM_HAL_STIMER_INT_COMPAREE
//!     AM_HAL_STIMER_INT_COMPAREF
//!     AM_HAL_STIMER_INT_COMPAREG
//!     AM_HAL_STIMER_INT_COMPAREH
//!
//!     AM_HAL_STIMER_INT_OVERFLOW
//!
//!     AM_HAL_STIMER_INT_CAPTUREA
//!     AM_HAL_STIMER_INT_CAPTUREB
//!     AM_HAL_STIMER_INT_CAPTUREC
//!     AM_HAL_STIMER_INT_CAPTURED
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_stimer_int_disable(uint32_t ui32Interrupt)
{
    //
    // Disable the interrupt at the module level.
    //
    CTIMERn(0)->STMINTEN &= ~ui32Interrupt;
}

//*****************************************************************************
//
//! @brief Sets the selected stimer interrupt.
//!
//! @param ui32Interrupt is the interrupt to be used.
//!
//! This function will set the selected interrupts in the STIMER
//! interrupt register.
//!
//! ui32Interrupt should be the logical OR of one or more of the following
//! values:
//!
//!     AM_HAL_STIMER_INT_COMPAREA
//!     AM_HAL_STIMER_INT_COMPAREB
//!     AM_HAL_STIMER_INT_COMPAREC
//!     AM_HAL_STIMER_INT_COMPARED
//!     AM_HAL_STIMER_INT_COMPAREE
//!     AM_HAL_STIMER_INT_COMPAREF
//!     AM_HAL_STIMER_INT_COMPAREG
//!     AM_HAL_STIMER_INT_COMPAREH
//!
//!     AM_HAL_STIMER_INT_OVERFLOW
//!
//!     AM_HAL_STIMER_INT_CAPTUREA
//!     AM_HAL_STIMER_INT_CAPTUREB
//!     AM_HAL_STIMER_INT_CAPTUREC
//!     AM_HAL_STIMER_INT_CAPTURED
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_stimer_int_set(uint32_t ui32Interrupt)
{
    //
    // Set the interrupts.
    //
    CTIMERn(0)->STMINTSET = ui32Interrupt;
}

//*****************************************************************************
//
//! @brief Clears the selected stimer interrupt.
//!
//! @param ui32Interrupt is the interrupt to be used.
//!
//! This function will clear the selected interrupts in the STIMER
//! interrupt register.
//!
//! ui32Interrupt should be the logical OR of one or more of the following
//! values:
//!
//!     AM_HAL_STIMER_INT_COMPAREA
//!     AM_HAL_STIMER_INT_COMPAREB
//!     AM_HAL_STIMER_INT_COMPAREC
//!     AM_HAL_STIMER_INT_COMPARED
//!     AM_HAL_STIMER_INT_COMPAREE
//!     AM_HAL_STIMER_INT_COMPAREF
//!     AM_HAL_STIMER_INT_COMPAREG
//!     AM_HAL_STIMER_INT_COMPAREH
//!
//!     AM_HAL_STIMER_INT_OVERFLOW
//!
//!     AM_HAL_STIMER_INT_CAPTUREA
//!     AM_HAL_STIMER_INT_CAPTUREB
//!     AM_HAL_STIMER_INT_CAPTUREC
//!     AM_HAL_STIMER_INT_CAPTURED
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_stimer_int_clear(uint32_t ui32Interrupt)
{
    //
    // Disable the interrupt at the module level.
    //
    CTIMERn(0)->STMINTCLR = ui32Interrupt;
}


//*****************************************************************************
//
//! @brief Returns either the enabled or raw stimer interrupt status.
//!
//! This function will return the stimer interrupt status.
//!
//! @param bEnabledOnly if true returns the status of the enabled interrupts
//! only.
//!
//! The return value will be the logical OR of one or more of the following
//! values:
//!
//!
//! @return Returns the stimer interrupt status.
//
//*****************************************************************************
uint32_t
am_hal_stimer_int_status_get(bool bEnabledOnly)
{
    //
    // Return the desired status.
    //
    uint32_t ui32RetVal = CTIMERn(0)->STMINTSTAT;

    if ( bEnabledOnly )
    {
        ui32RetVal &= CTIMERn(0)->STMINTEN;
    }

    return ui32RetVal;
}

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
