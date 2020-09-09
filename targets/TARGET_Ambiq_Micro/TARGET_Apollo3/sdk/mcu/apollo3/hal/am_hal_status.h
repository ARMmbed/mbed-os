//*****************************************************************************
//
//  am_hal_status.h
//! @file
//!
//! @brief Global status return codes for HAL interface.
//!
//! @addtogroup status3 Global Status Return Codes.
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
#ifndef AM_HAL_STATUS_H
#define AM_HAL_STATUS_H

#ifdef __cplusplus
extern "C"
{
#endif

  //
  // Global Status Returns
  //
  typedef enum
  {
    AM_HAL_STATUS_SUCCESS,
    AM_HAL_STATUS_FAIL,
    AM_HAL_STATUS_INVALID_HANDLE,
    AM_HAL_STATUS_IN_USE,
    AM_HAL_STATUS_TIMEOUT,
    AM_HAL_STATUS_OUT_OF_RANGE,
    AM_HAL_STATUS_INVALID_ARG,
    AM_HAL_STATUS_INVALID_OPERATION,
    AM_HAL_STATUS_MEM_ERR,
    AM_HAL_STATUS_HW_ERR,
    AM_HAL_STATUS_MODULE_SPECIFIC_START = 0x08000000,
  } am_hal_status_e;

#ifdef __cplusplus
}
#endif

#endif // AM_HAL_STATUS_H

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
