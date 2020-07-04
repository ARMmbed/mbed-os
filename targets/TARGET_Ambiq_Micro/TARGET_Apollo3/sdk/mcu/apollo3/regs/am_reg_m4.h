//*****************************************************************************
//
//  am_reg_m4.h
//! @file
//!
//! @brief A collection of a few CMSIS-style macros that are not automatically
//!        generated in their respective core files.
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

#ifndef AM_REG_CM4_H
#define AM_REG_CM4_H

//*****************************************************************************
//
// am_reg_itm.h
// CMSIS-style defines.
//
//*****************************************************************************
#define ITM_LAR_KEYVAL      0xC5ACCE55

//*****************************************************************************
//
// am_reg_sysctrl.h
// CMSIS-style defines.
//
//*****************************************************************************
#define SCB_CPACR_CP11_Pos                  22
#define SCB_CPACR_CP11_Msk                  0x00C00000
#define SCB_CPACR_CP10_Pos                  20
#define SCB_CPACR_CP10_Msk                  0x00300000

//*****************************************************************************
//
// am_reg_tpiu.h
// CMSIS-style defines.
//
//*****************************************************************************
#define TPI_CSPSR_CWIDTH_1BIT      1
#define TPI_SPPR_TXMODE_UART       2
#define TPI_ITCTRL_Mode_NORMAL     0

#ifndef TPI_ACPR_SWOSCALER_Pos
//
// In the CMSIS 5.6.0 version of core_cm4.h, the SWOSCALER field was no longer
// defined, while the PRESCALER field was left intact even though previous CMSIS
// versions PRESCALER as deprecated.  On the off chance that future versions
// make a correction and remove PRESCALER, define SWOSCALER here (per 5.3.0).
//
#define TPI_ACPR_SWOSCALER_Pos              0U                                         /*!< TPI ACPR: SWOSCALER Position */
#define TPI_ACPR_SWOSCALER_Msk             (0xFFFFUL /*<< TPI_ACPR_SWOSCALER_Pos*/)    /*!< TPI ACPR: SWOSCALER Mask */
#endif

#endif // AM_REG_CM4_H
