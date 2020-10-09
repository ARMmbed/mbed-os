//*****************************************************************************
//
//  am_reg_macros.h
//! @file
//!
//! @brief Helper macros for using hardware registers.
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

#ifndef AM_REG_MACROS_H
#define AM_REG_MACROS_H

#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// For direct 32-bit access to a register or memory location, use AM_REGVAL:
//      AM_REGVAL(0x1234567) |= 0xDEADBEEF;
//
//*****************************************************************************
#define AM_REGVAL(x)               (*((volatile uint32_t *)(x)))
#define AM_REGVAL_FLOAT(x)         (*((volatile float *)(x)))

//*****************************************************************************
//
// AM_REGADDR()
// One thing CMSIS does not do well natively is to provide for static register
// address computation. The address-of operator (e.g. &periph->reg) is helpful,
// but does run into problems, such as when attempting to cast the resulting
// pointer to a uint32_t.  The standard C macro, offsetof() can help.
//
// Use AM_REGADDR() for single-module peripherals.
// Use AM_REGADDRn() for multi-module peripherals (e.g. IOM, UART).
//
//*****************************************************************************
#define AM_REGADDR(periph, reg) ( periph##_BASE + offsetof(periph##_Type, reg) )

#define AM_REGADDRn(periph, n, reg) ( periph##0_BASE                    +   \
                                      offsetof(periph##0_Type, reg)     +   \
                                      (n * (periph##1_BASE - periph##0_BASE)) )

//*****************************************************************************
//
// Critical section assembly macros
//
// These macros implement critical section protection using inline assembly
// for various compilers.  They are intended to be used in other register
// macros or directly in sections of code.
//
// Important usage note: These macros create a local scope and therefore MUST
// be used in pairs.
//
//*****************************************************************************
#define AM_CRITICAL_BEGIN                                                   \
    if ( 1 )                                                                \
    {                                                                       \
        volatile uint32_t ui32Primask_04172010;                             \
        ui32Primask_04172010 = am_hal_interrupt_master_disable();

#define AM_CRITICAL_END                                                     \
        am_hal_interrupt_master_set(ui32Primask_04172010);                  \
    }

#ifdef __cplusplus
}
#endif

#endif // AM_REG_MACROS_H

