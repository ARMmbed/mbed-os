/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2018-2020 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
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
/***********************************************************************************************************************
 * File Name    : phy.h
 * Version      : 1.00
 * Description  : Ethernet PHY device driver
 ***********************************************************************************************************************/

/* Guards against multiple inclusion */
#ifndef PHY_H
#define PHY_H

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/
/* PHY return definitions */
#define R_PHY_OK        (0)
#define R_PHY_ERROR     (-1)

/***********************************************************************************************************************
 Typedef definitions
 ***********************************************************************************************************************/
typedef enum LinkStatE {
    PHY_NO_LINK = 0, PHY_LINK_10H, PHY_LINK_10F, PHY_LINK_100H, PHY_LINK_100F
} linkstat_t;

/***********************************************************************************************************************
 Exported global variables
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 ***********************************************************************************************************************/
extern int16_t phy_init(uint32_t channel);
extern void phy_start_autonegotiate(uint32_t channel, uint8_t pause);
extern int16_t phy_set_autonegotiate(uint32_t channel, uint16_t *pline_speed_duplex, uint16_t *plocal_pause,
                                     uint16_t *ppartner_pause);
extern int16_t phy_get_link_status(uint32_t channel);

#ifdef __cplusplus
}
#endif

#endif /* PHY_H */

