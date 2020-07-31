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
 * File Name    : r_ether_rza2_config.h
 * Version      : 1.00
 * Description  : Ethernet module device driver
 ***********************************************************************************************************************/

/* Guards against multiple inclusion */
#ifndef R_ETHER_RZA2_CONFIG_H
#define R_ETHER_RZA2_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif
/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/
/* Ethernet channel select.
 0 = disable
 1 = enable
 If only one of them is enabled, the API argument "channel" value is not referenced.
 */
#define ETHER_CH0_EN                                (0)
#define ETHER_CH1_EN                                (1)

/* Ethernet interface select.
 0 = MII  (Media Independent Interface)
 1 = RMII (Reduced Media Independent Interface)
 */
#define ETHER_CFG_MODE_SEL                          (0)

/* PHY-LSI address setting for ETHER0/1.
 */
#define ETHER_CFG_CH0_PHY_ADDRESS                   (0)     /* Please define the PHY-LSI address in the range of 0-31. */
#define ETHER_CFG_CH1_PHY_ADDRESS                   (0)     /* Please define the PHY-LSI address in the range of 0-31. */

/* The number of Rx descriptors. */
#define ETHER_CFG_EMAC_RX_DESCRIPTORS               (8)

/* The number of Tx descriptors. */
#define ETHER_CFG_EMAC_TX_DESCRIPTORS               (8)

/* Please define the size of the sending and receiving buffer in the value where one frame can surely be stored
 because the driver is single-frame/single-buffer processing.  */
#define ETHER_CFG_BUFSIZE                           (1536)  /* Must be 32-byte aligned */

/* Define the access timing of MII/RMII register */
#define ETHER_CFG_PHY_MII_WAIT                      (8)     /* Plese define the value of 1 or more */

/* Define the waiting time for reset completion of PHY-LSI */
#define ETHER_CFG_PHY_DELAY_RESET                   (0x00020000L)

/**
 * Link status read from LMON bit of ETHERC PSR register.  The state is hardware dependent.
 */
#define ETHER_CFG_LINK_PRESENT                      (0)

/*  Use LINKSTA signal for detect link status changes
 0 = unused  (use PHY-LSI status register)
 1 = use     (use LINKSTA signal)
 */
#define ETHER_CFG_USE_LINKSTA                       (0)     /* This setting is reflected in all channels */

/* Definition of whether or not to use KSZ8041NL of the Micrel Inc.
 0 = unused
 1 = use
 */
#define ETHER_CFG_USE_PHY_KSZ8041NL                 (0)

/***********************************************************************************************************************
 Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global variables
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 ***********************************************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* R_ETHER_RZA2_CONFIG_H */
