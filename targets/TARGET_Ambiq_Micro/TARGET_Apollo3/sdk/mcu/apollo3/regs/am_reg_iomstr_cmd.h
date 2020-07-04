//*****************************************************************************
//
//  am_reg_iomstr_cmd.h
//! @file
//!
//! @brief Register macros for the IOMSTR module
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
#ifndef AM_REG_IOMSTR_CMD_H
#define AM_REG_IOMSTR_CMD_H

#if AM_PART_APOLLO2
//*****************************************************************************
//
// IOMSTR_CMD - Command Register
//
//*****************************************************************************
#define AM_REG_IOMSTR_CMD_CMD_POS_LENGTH             0x00000000
#define AM_REG_IOMSTR_CMD_CMD_POS_OFFSET             0x00000008
#define AM_REG_IOMSTR_CMD_CMD_POS_ADDRESS            0x00000010
#define AM_REG_IOMSTR_CMD_CMD_POS_CHNL               0x00000010
#define AM_REG_IOMSTR_CMD_CMD_POS_UPLNGTH            0x00000017
#define AM_REG_IOMSTR_CMD_CMD_POS_10BIT              0x0000001A
#define AM_REG_IOMSTR_CMD_CMD_POS_LSB                0x0000001B
#define AM_REG_IOMSTR_CMD_CMD_POS_CONT               0x0000001C
#define AM_REG_IOMSTR_CMD_CMD_POS_OPER               0x0000001D
#define AM_REG_IOMSTR_CMD_CMD_MSK_LENGTH             0x000000FF
#define AM_REG_IOMSTR_CMD_CMD_MSK_OFFSET             0x0000FF00
#define AM_REG_IOMSTR_CMD_CMD_MSK_ADDRESS            0x00FF0000
#define AM_REG_IOMSTR_CMD_CMD_MSK_CHNL               0x00070000
#define AM_REG_IOMSTR_CMD_CMD_MSK_UPLNGTH            0x07800000
#define AM_REG_IOMSTR_CMD_CMD_MSK_10BIT              0x04000000
#define AM_REG_IOMSTR_CMD_CMD_MSK_LSB                0x08000000
#define AM_REG_IOMSTR_CMD_CMD_MSK_CONT               0x10000000
#define AM_REG_IOMSTR_CMD_CMD_MSK_OPER               0xE0000000
#endif // AM_PART_APOLLO2

#endif // AM_REG_IOMSTR_CMD_H
