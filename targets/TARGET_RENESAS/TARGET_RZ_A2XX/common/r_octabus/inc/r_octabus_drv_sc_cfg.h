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
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/* Copyright (c) 2020 Renesas Electronics Corporation.
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
/**********************************************************************************************************************
 * File Name    : r_octabus_drv_sc_cfg.h
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_octabus_lld_rza2m_api.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

#ifndef SC_DRIVERS_R_OCTABUS_INC_R_OCTABUS_DRV_SC_CFG_H_
#define SC_DRIVERS_R_OCTABUS_INC_R_OCTABUS_DRV_SC_CFG_H_

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

static const st_octabus_cfg_t OCTABUS_SC_TABLE[] = {
    /* This code is auto-generated. Do not edit manually */
    {
        OCTABUS_NO_INIT,
        OCTABUS_INIT_AT_APP,
        0x01000000,
        OCTABUS_PRECYCLE_DISABLE,
        OCTABUS_TTYPE_DOPI,
        5,
        5,
        0,
        OCTABUS_DQSENA_7_CYCLE,
        OCTABUS_DVRDLO_DOPI_1P5_CYCLE,
        OCTABUS_DVRDHI_DOPI_6P5_CYCLE,
        OCTABUS_DVRDCMD_7_CYCLE,
        OCTABUS_DVWLO_DOPI_1P5_CYCLE,
        OCTABUS_DVWHI_DOPI_1P5_CYCLE,
        OCTABUS_DVWCMD_7_CYCLE,
        0x00,
        0x00,
        0x80,
        0x00,
        OCTABUS_BYTE_ORDER_B1B0B3B2,
        0x02,
        0x04,
        OCTABUS_BYTE_ORDER_B1B0B3B2,
        0x02,
        0x04,
        132000000,
        0x00FFFFF0,
    },
    /* End of modification */
};

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

#endif /* SC_DRIVERS_R_OCTABUS_INC_R_OCTABUS_DRV_SC_CFG_H_ */
