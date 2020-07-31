/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2018-2020 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : rza_io_regrw.h
 * Description  : Low level register read/write header
 *********************************************************************************************************************/
/* Copyright (c) 2018-2020 Renesas Electronics Corporation.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#ifndef RZA_IO_REGRW_H
#define RZA_IO_REGRW_H

#define IOREG_NONMASK_ACCESS    (0xFFFFFFFFuL)
#define IOREG_NONSHIFT_ACCESS   (0)

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/******************************************************************************
 * @fn            RZA_IO_RegWrite_8
 * @brief         IO register 8-bit write
 *                To use macros defined in iodefine files is convenient.
 * @param [in]    ioreg: address for target register
 * @param [in]    write_value: value to be write
 * @param [in]    shift: target bit offset from LSB
 * @param [in]    mask: target data mask (bits position of '0' are leave alone)
 *                       If no one need to keep previous value or shift, use
 *                       IOREG_NONMASK_ACCESS instead.
 *****************************************************************************/
extern void     RZA_IO_RegWrite_8(volatile uint8_t   *ioreg, uint8_t  write_value, uint8_t  shift, uint32_t mask);

/******************************************************************************
 * @fn            RZA_IO_RegWrite_16
 * @brief         IO register 16-bit write
 *                To use macros defined in iodefine files is convenient.
 * @param [in]    ioreg: address for target register
 * @param [in]    write_value: value to be write
 * @param [in]    shift: target bit offset from LSB
 * @param [in]    mask: target data mask (bits position of '0' are leave alone)
 *                       If no one need to keep previous value or shift, use
 *                       IOREG_NONMASK_ACCESS instead.
 *****************************************************************************/
extern void     RZA_IO_RegWrite_16(volatile uint16_t *ioreg, uint16_t write_value, uint16_t shift, uint32_t mask);

/******************************************************************************
 * @fn            RZA_IO_RegWrite_32
 * @brief         IO register 32-bit write
 *                To use macros defined in iodefine files is convenient.
 * @param [in]    ioreg: address for target register
 * @param [in]    write_value: value to be write
 * @param [in]    shift: target bit offset from LSB
 * @param [in]    mask: target data mask (bits position of '0' are leave alone)
 *                       If no one need to keep previous value or shift, use
 *                       IOREG_NONMASK_ACCESS instead.
 *****************************************************************************/
extern void     RZA_IO_RegWrite_32(volatile unsigned long *ioreg, uint32_t write_value, uint32_t shift, uint32_t mask);

/******************************************************************************
 * @fn            RZA_IO_RegRead_8
 * @brief         IO register 8-bit read
 *                To use macros defined in iodefine files is convenient.
 * @param [in]    ioreg: address for target register
 * @param [in]    shift: target bit offset from LSB
 * @param [in]    mask: target data mask (masked before shift)
 *                       If no one need to mask and shift, use
 *                       IOREG_NONMASK_ACCESS instead.
 * @retval        read value from register
 *****************************************************************************/
extern uint8_t  RZA_IO_RegRead_8(volatile uint8_t   *ioreg, uint8_t  shift, uint32_t mask);

/******************************************************************************
 * @fn            RZA_IO_RegRead_16
 * @brief         IO register 16-bit read
 *                To use macros defined in iodefine files is convenient.
 * @param [in]    ioreg: address for target register
 * @param [in]    shift: target bit offset from LSB
 * @param [in]    mask: target data mask (masked before shift)
 *                       If no one need to mask and shift, use
 *                       IOREG_NONMASK_ACCESS instead.
 * @retval        read value from register
 *****************************************************************************/
extern uint16_t RZA_IO_RegRead_16(volatile uint16_t *ioreg, uint16_t shift, uint32_t mask);

/******************************************************************************
 * @fn            RZA_IO_RegRead_32
 * @brief         IO register 32-bit read
 *                To use macros defined in iodefine files is convenient.
 * @param [in]    ioreg: address for target register
 * @param [in]    shift: target bit offset from LSB
 * @param [in]    mask: target data mask (masked before shift)
 *                       If no one need to mask and shift, use
 *                       IOREG_NONMASK_ACCESS instead.
 * @retval        read value from register
 *****************************************************************************/
extern uint32_t RZA_IO_RegRead_32(volatile unsigned long *ioreg, uint32_t shift, uint32_t mask);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* RZA_IO_REGRW_H */

/* End of File */
