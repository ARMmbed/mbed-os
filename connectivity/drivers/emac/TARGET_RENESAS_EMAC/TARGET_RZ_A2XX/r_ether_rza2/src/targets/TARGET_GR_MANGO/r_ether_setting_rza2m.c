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
* File Name    : r_ether_setting_rza2m.c
* Version      : 1.00
* Device       : RZA2M
* Description  : Ethernet module device driver
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "iodefine.h"
#include "iobitmask.h"
#include "cmsis.h"
#include "pinmap.h"

#include "r_ether_rza2_if.h"
#include "src/r_ether_rza2_private.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: ether_set_phy_mode
* Description  :
* Arguments    : connect -
*                    Ethernet channel number
* Return Value : none
***********************************************************************************************************************/
void ether_set_phy_mode(uint8_t connect)
{
#if (ETHER_CH0_EN == 1)
#error "Not support in this board."
#endif

#if (ETHER_CH1_EN == 1)
    if (PORT_CONNECT_ET1 == (connect & PORT_CONNECT_ET1)) {
#if (ETHER_CFG_MODE_SEL == 0)
        /* CH1 MII */
        GPIO.PFENET.BIT.PHYMODE1 = 1;
        pin_function(P3_3, 1); // ET1_MDC
        pin_function(P3_4, 1); // ET1_MDIO
        pin_function(PC_0, 3); // ET1_TXCLK
        pin_function(PC_4, 3); // ET1_TXER
        pin_function(PK_0, 1); // ET1_TXEN
        pin_function(PK_1, 1); // ET1_TXD0
        pin_function(PK_2, 1); // ET1_TXD1
        pin_function(PC_1, 3); // ET1_TXD2
        pin_function(PC_2, 3); // ET1_TXD3
        pin_function(PK_3, 1); // ET1_RXCLK
        pin_function(P3_1, 1); // ET1_RXER
        pin_function(PC_5, 3); // ET1_RXDV
        pin_function(PK_4, 1); // ET1_RXD0
        pin_function(P3_5, 1); // ET1_RXD1
        pin_function(PC_6, 3); // ET1_RXD2
        pin_function(PC_7, 3); // ET1_RXD3
        pin_function(P3_2, 1); // ET1_CRS
        pin_function(PC_3, 3); // ET1_COL
#elif (ETHER_CFG_MODE_SEL == 1)
        /* CH1 RMII */
        GPIO.PFENET.BIT.PHYMODE1 = 0;
        GPIO.PMODEPFS.BIT.ET1_EXOUT_SEL = 0;
#error "Not support in this board."
#endif
    }
#endif

} /* End of function ether_set_phy_mode() */

/* End of File */
