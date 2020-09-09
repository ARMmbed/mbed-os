//*****************************************************************************
//
//  am_hal_interrupt.c
//! @file
//!
//! @brief Helper functions supporting interrupts and NVIC operation.
//!
//! These functions may be used for NVIC-level interrupt configuration.
//!
//! @addtogroup interrupt3 Interrupt (ARM NVIC support functions)
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
//! @brief Globally enable interrupt service routines
//!
//! This function allows interrupt signals from the NVIC to trigger ISR entry
//! in the CPU. This function must be called if interrupts are to be serviced
//! in software.
//!
//! @return 1 if interrupts were previously disabled, 0 otherwise.
//
//*****************************************************************************
#if (defined (__ARMCC_VERSION)) && (__ARMCC_VERSION < 6000000)
__asm uint32_t
am_hal_interrupt_master_enable(void)
{
    mrs     r0, PRIMASK
    cpsie   i
    bx      lr
}
#elif (defined (__ARMCC_VERSION)) && (__ARMCC_VERSION >= 6000000)
uint32_t __attribute__((naked))
am_hal_interrupt_master_enable(void)
{
    __asm("    mrs     r0, PRIMASK");
    __asm("    cpsie i");
    __asm("    bx lr");
}
#elif defined(__GNUC_STDC_INLINE__)
uint32_t __attribute__((naked))
am_hal_interrupt_master_enable(void)
{
    __asm("    mrs     r0, PRIMASK");
    __asm("    cpsie i");
    __asm("    bx lr");
}
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma diag_suppress = Pe940   // Suppress IAR compiler warning about missing
                                // return statement on a non-void function
__stackless uint32_t
am_hal_interrupt_master_enable(void)
{
    __asm("    mrs     r0, PRIMASK");
    __asm("    cpsie i");
    __asm("    bx lr");
}
#pragma diag_default = Pe940    // Restore IAR compiler warning
#else
#error Compiler is unknown, please contact Ambiq support team
#endif

//*****************************************************************************
//
//! @brief Globally disable interrupt service routines
//!
//! This function prevents interrupt signals from the NVIC from triggering ISR
//! entry in the CPU. This will effectively stop incoming interrupt sources
//! from triggering their corresponding ISRs.
//!
//! @note Any external interrupt signal that occurs while the master interrupt
//! disable is active will still reach the "pending" state in the NVIC, but it
//! will not be allowed to reach the "active" state or trigger the
//! corresponding ISR. Instead, these interrupts are essentially "queued" until
//! the next time the master interrupt enable instruction is executed. At that
//! time, the interrupt handlers will be executed in order of decreasing
//! priority.
//!
//! @return 1 if interrupts were previously disabled, 0 otherwise.
//
//*****************************************************************************
#if (defined (__ARMCC_VERSION)) && (__ARMCC_VERSION < 6000000)
__asm uint32_t
am_hal_interrupt_master_disable(void)
{
    mrs     r0, PRIMASK
    cpsid   i
    bx      lr
}
#elif (defined (__ARMCC_VERSION)) && (__ARMCC_VERSION >= 6000000)
uint32_t __attribute__((naked))
am_hal_interrupt_master_disable(void)
{
    __asm("    mrs     r0, PRIMASK");
    __asm("    cpsid i");
    __asm("    bx lr");
}
#elif defined(__GNUC_STDC_INLINE__)
uint32_t __attribute__((naked))
am_hal_interrupt_master_disable(void)
{
    __asm("    mrs     r0, PRIMASK");
    __asm("    cpsid i");
    __asm("    bx lr");
}
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma diag_suppress = Pe940   // Suppress IAR compiler warning about missing
                                // return statement on a non-void function
__stackless uint32_t
am_hal_interrupt_master_disable(void)
{
    __asm("    mrs     r0, PRIMASK");
    __asm("    cpsid i");
    __asm("    bx lr");
}
#pragma diag_default = Pe940    // Restore IAR compiler warning
#else
#error Compiler is unknown, please contact Ambiq support team
#endif

//*****************************************************************************
//
//! @brief Sets the master interrupt state based on the input.
//!
//! @param ui32InterruptState - Desired PRIMASK value.
//!
//! This function directly writes the PRIMASK register in the ARM core. A value
//! of 1 will disable interrupts, while a value of zero will enable them.
//!
//! This function may be used along with am_hal_interrupt_master_disable() to
//! implement a nesting critical section. To do this, call
//! am_hal_interrupt_master_disable() to start the critical section, and save
//! its return value. To complete the critical section, call
//! am_hal_interrupt_master_set() using the saved return value as \e
//! ui32InterruptState. This will safely restore PRIMASK to the value it
//! contained just before the start of the critical section.
//!
//! @return None.
//
//*****************************************************************************
#if (defined (__ARMCC_VERSION)) && (__ARMCC_VERSION <  6000000)
__asm void
am_hal_interrupt_master_set(uint32_t ui32InterruptState)
{
    msr     PRIMASK, r0
    bx      lr
}
#elif (defined (__ARMCC_VERSION)) && (__ARMCC_VERSION >= 6000000)
void __attribute__((naked))
am_hal_interrupt_master_set(uint32_t ui32InterruptState)
{
    __asm("    msr     PRIMASK, r0");
    __asm("    bx lr");
}
#elif defined(__GNUC_STDC_INLINE__)
void __attribute__((naked))
am_hal_interrupt_master_set(uint32_t ui32InterruptState)
{
    __asm("    msr     PRIMASK, r0");
    __asm("    bx lr");
}
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma diag_suppress = Pe940   // Suppress IAR compiler warning about missing
                                // return statement on a non-void function
__stackless void
am_hal_interrupt_master_set(uint32_t ui32InterruptState)
{
    __asm("    msr     PRIMASK, r0");
    __asm("    bx lr");
}
#pragma diag_default = Pe940    // Restore IAR compiler warning
#endif

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
