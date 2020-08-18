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
 * File Name    : phy.c
 * Version      : 1.00
 * Description  : Ethernet PHY device driver
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
#include "iodefine.h"
#include "iobitmask.h"
#include "cmsis.h"

/* Access to peripherals and board defines. */

#include "r_ether_rza2_config.h"
#include "src/r_ether_rza2_private.h"
#include "src/phy/phy.h"

/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/
/* Media Independent Interface */
#define PHY_MII_ST                      (1)
#define PHY_MII_READ                    (2)
#define PHY_MII_WRITE                   (1)

/* Standard PHY Registers */
#define PHY_REG_CONTROL                 (0)
#define PHY_REG_STATUS                  (1)
#define PHY_REG_IDENTIFIER1             (2)
#define PHY_REG_IDENTIFIER2             (3)
#define PHY_REG_AN_ADVERTISEMENT        (4)
#define PHY_REG_AN_LINK_PARTNER         (5)
#define PHY_REG_AN_EXPANSION            (6)

/* Vendor Specific PHY Registers */
#ifdef ETHER_CFG_USE_PHY_KSZ8041NL
#define PHY_REG_PHY_CONTROL_1           (0x1E)
#endif /* MICREL_KSZ8041NL */

/* Basic Mode Control Register Bit Definitions */
#define PHY_CONTROL_RESET               (1 << 15)
#define PHY_CONTROL_LOOPBACK            (1 << 14)
#define PHY_CONTROL_100_MBPS            (1 << 13)
#define PHY_CONTROL_AN_ENABLE           (1 << 12)
#define PHY_CONTROL_POWER_DOWN          (1 << 11)
#define PHY_CONTROL_ISOLATE             (1 << 10)
#define PHY_CONTROL_AN_RESTART          (1 << 9)
#define PHY_CONTROL_FULL_DUPLEX         (1 << 8)
#define PHY_CONTROL_COLLISION           (1 << 7)

/* Basic Mode Status Register Bit Definitions */
#define PHY_STATUS_100_T4               (1 << 15)
#define PHY_STATUS_100F                 (1 << 14)
#define PHY_STATUS_100H                 (1 << 13)
#define PHY_STATUS_10F                  (1 << 12)
#define PHY_STATUS_10H                  (1 << 11)
#define PHY_STATUS_AN_COMPLETE          (1 << 5)
#define PHY_STATUS_RM_FAULT             (1 << 4)
#define PHY_STATUS_AN_ABILITY           (1 << 3)
#define PHY_STATUS_LINK_UP              (1 << 2)
#define PHY_STATUS_JABBER               (1 << 1)
#define PHY_STATUS_EX_CAPABILITY        (1 << 0)

/* Auto Negotiation Advertisement Bit Definitions */
#define PHY_AN_ADVERTISEMENT_NEXT_PAGE  (1 << 15)
#define PHY_AN_ADVERTISEMENT_RM_FAULT   (1 << 13)
#define PHY_AN_ADVERTISEMENT_ASM_DIR    (1 << 11)
#define PHY_AN_ADVERTISEMENT_PAUSE      (1 << 10)
#define PHY_AN_ADVERTISEMENT_100_T4     (1 << 9)
#define PHY_AN_ADVERTISEMENT_100F       (1 << 8)
#define PHY_AN_ADVERTISEMENT_100H       (1 << 7)
#define PHY_AN_ADVERTISEMENT_10F        (1 << 6)
#define PHY_AN_ADVERTISEMENT_10H        (1 << 5)
#define PHY_AN_ADVERTISEMENT_SELECTOR   (1 << 0)

/* Auto Negotiate Link Partner Ability Bit Definitions */
#define PHY_AN_LINK_PARTNER_NEXT_PAGE   (1 << 15)
#define PHY_AN_LINK_PARTNER_ACK         (1 << 14)
#define PHY_AN_LINK_PARTNER_RM_FAULT    (1 << 13)
#define PHY_AN_LINK_PARTNER_ASM_DIR     (1 << 11)
#define PHY_AN_LINK_PARTNER_PAUSE       (1 << 10)
#define PHY_AN_LINK_PARTNER_100_T4      (1 << 9)
#define PHY_AN_LINK_PARTNER_100F        (1 << 8)
#define PHY_AN_LINK_PARTNER_100H        (1 << 7)
#define PHY_AN_LINK_PARTNER_10F         (1 << 6)
#define PHY_AN_LINK_PARTNER_10H         (1 << 5)
#define PHY_AN_LINK_PARTNER_SELECTOR    (1 << 0)

/***********************************************************************************************************************
 Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global variables (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Private global variables and functions
 ***********************************************************************************************************************/
static uint16_t phy_read(uint32_t ether_channel, uint16_t reg_addr);
static void phy_write(uint32_t ether_channel, uint16_t reg_addr, uint16_t data);
static void phy_preamble(uint32_t ether_channel);
static void phy_reg_set(uint32_t ether_channel, uint16_t reg_addr, int32_t option);
static void phy_reg_read(uint32_t ether_channel, uint16_t *pdata);
static void phy_reg_write(uint32_t ether_channel, uint16_t data);
static void phy_trans_zto0(uint32_t ether_channel);
static void phy_trans_1to0(uint32_t ether_channel);
static void phy_mii_write1(uint32_t ether_channel);
static void phy_mii_write0(uint32_t ether_channel);
static int16_t phy_get_pir_address(uint32_t ether_channel, volatile uint32_t **pppir_addr);
static uint32_t phy_get_ctrl_tbl_idx(uint32_t ether_channel);

static uint16_t local_advertise[ETHER_CHANNEL_MAX]; /* the capabilities of the local link as PHY data */

/**
 * Public functions
 */

/***********************************************************************************************************************
 * Function Name: phy_init
 * Description  : Resets Ethernet PHY device
 * Arguments    : ether_channel -
 *                    Ethernet channel number
 * Return Value : R_PHY_OK -
 *
 *                R_PHY_ERROR -
 *
 ***********************************************************************************************************************/
int16_t phy_init(uint32_t ether_channel)
{
    uint16_t reg;
    uint32_t count;

    /* Reset PHY */
    phy_write(ether_channel, PHY_REG_CONTROL, PHY_CONTROL_RESET);

    count = 0;

    /* Reset completion waiting */
    do {
        reg = phy_read(ether_channel, PHY_REG_CONTROL);
        count++;
    } while ((reg & PHY_CONTROL_RESET) && (count < ETHER_CFG_PHY_DELAY_RESET));

    if (count < ETHER_CFG_PHY_DELAY_RESET) {
        /*
         * When KSZ8041NL of the Micrel, Inc. is used,
         * the pin that outputs the state of LINK is used combinedly with ACTIVITY in default.
         * The setting of the pin is changed so that only the state of LINK is output.
         */
#if ETHER_CFG_USE_PHY_KSZ8041NL != 0
        reg = phy_read(ether_channel, PHY_REG_PHY_CONTROL_1);
        reg &= ~0x8000;
        reg |= 0x4000;
        phy_write(ether_channel, PHY_REG_PHY_CONTROL_1, reg);
#endif /* ETHER_CFG_USE_PHY_KSZ8041NL != 0 */

        return R_PHY_OK;
    }
    return R_PHY_ERROR;
} /* End of function phy_init() */

/***********************************************************************************************************************
 * Function Name: phy_start_autonegotiate
 * Description  : Starts auto-negotiate
 * Arguments    : ether_channel -
 *                    Ethernet channel number
 *                pause -
 *                    Using state of pause frames
 * Return Value : none
 ***********************************************************************************************************************/
void phy_start_autonegotiate(uint32_t ether_channel, uint8_t pause)
{
    volatile uint16_t reg = 0;
    uint32_t ether_channel_index = phy_get_ctrl_tbl_idx(ether_channel);

    /* Set local ability */
    /* When pause frame is not used */
    if (ETHER_FLAG_OFF == pause) {
        local_advertise[ether_channel_index] = ((((PHY_AN_ADVERTISEMENT_100F |
                                                   PHY_AN_ADVERTISEMENT_100H) |
                                                  PHY_AN_ADVERTISEMENT_10F) |
                                                 PHY_AN_ADVERTISEMENT_10H) |
                                                PHY_AN_ADVERTISEMENT_SELECTOR);

    }

    /* When pause frame is used */
    else {
        local_advertise[ether_channel_index] = ((((((PHY_AN_ADVERTISEMENT_ASM_DIR |
                                                     PHY_AN_ADVERTISEMENT_PAUSE) |
                                                    PHY_AN_ADVERTISEMENT_100F) |
                                                   PHY_AN_ADVERTISEMENT_100H) |
                                                  PHY_AN_ADVERTISEMENT_10F) |
                                                 PHY_AN_ADVERTISEMENT_10H) |
                                                PHY_AN_ADVERTISEMENT_SELECTOR);
    }

    /* Configure what the PHY and the Ethernet controller on this board supports */
    phy_write(ether_channel, PHY_REG_AN_ADVERTISEMENT, local_advertise[ether_channel_index]);
    phy_write(ether_channel, PHY_REG_CONTROL, (PHY_CONTROL_AN_ENABLE |
                                               PHY_CONTROL_AN_RESTART));

    reg = phy_read(ether_channel, PHY_REG_AN_ADVERTISEMENT);
    (void)reg;

} /* End of function phy_start_autonegotiate() */

/***********************************************************************************************************************
 * Function Name: phy_set_autonegotiate
 * Description  : reports the other side's physical capability
 * Arguments    : ether_channel -
 *                    Ethernet channel number
 *              : *pline_speed_duplex -
 *                    a pointer to the location of both the line speed and the duplex
 *                *plocal_pause -
 *                    a pointer to the location to store the local pause bits.
 *                *ppartner_pause -
 *                    a pointer to the location to store the partner pause bits.
 * Return Value : R_PHY_OK -
 *
 *                R_PHY_ERROR -
 *
 * Note         : The value returned to local_pause and patner_pause is used
 *                as it is as an argument of ether_pause_resolution function.
 ***********************************************************************************************************************/
int16_t phy_set_autonegotiate(uint32_t ether_channel, uint16_t *pline_speed_duplex, uint16_t *plocal_pause,
                              uint16_t *ppartner_pause)
{
    uint16_t reg;
    uint32_t ether_channel_index = phy_get_ctrl_tbl_idx(ether_channel);

    /* Because reading the first time shows the previous state, the Link status bit is read twice. */
    reg = phy_read(ether_channel, PHY_REG_STATUS);
    reg = phy_read(ether_channel, PHY_REG_STATUS);

    /* When the link isn't up, return error */
    if (PHY_STATUS_LINK_UP != (reg & PHY_STATUS_LINK_UP)) {
        return R_PHY_ERROR;
    }

    /* Establish local pause capability */
    if (PHY_AN_ADVERTISEMENT_PAUSE == (local_advertise[ether_channel_index] & PHY_AN_ADVERTISEMENT_PAUSE)) {
        (*plocal_pause) |= (1 << 1);
    }

    if (PHY_AN_ADVERTISEMENT_ASM_DIR == (local_advertise[ether_channel_index] & PHY_AN_ADVERTISEMENT_ASM_DIR)) {
        (*plocal_pause) |= 1;
    }

    /* When the auto-negotiation isn't completed, return error */
    if (PHY_STATUS_AN_COMPLETE != (reg & PHY_STATUS_AN_COMPLETE)) {
        return R_PHY_ERROR;
    }

    /* Get the link partner response */
    reg = phy_read(ether_channel, PHY_REG_AN_LINK_PARTNER);

    /* Establish partner pause capability */
    if (PHY_AN_LINK_PARTNER_PAUSE == (reg & PHY_AN_LINK_PARTNER_PAUSE)) {
        (*ppartner_pause) = (1 << 1);
    }

    if (PHY_AN_LINK_PARTNER_ASM_DIR == (reg & PHY_AN_LINK_PARTNER_ASM_DIR)) {
        (*ppartner_pause) |= 1;
    }

    /* Establish the line speed and the duplex */
    if (PHY_AN_LINK_PARTNER_10H == (reg & PHY_AN_LINK_PARTNER_10H)) {
        (*pline_speed_duplex) = PHY_LINK_10H;
    }

    if (PHY_AN_LINK_PARTNER_10F == (reg & PHY_AN_LINK_PARTNER_10F)) {
        (*pline_speed_duplex) = PHY_LINK_10F;
    }

    if (PHY_AN_LINK_PARTNER_100H == (reg & PHY_AN_LINK_PARTNER_100H)) {
        (*pline_speed_duplex) = PHY_LINK_100H;
    }

    if (PHY_AN_LINK_PARTNER_100F == (reg & PHY_AN_LINK_PARTNER_100F)) {
        (*pline_speed_duplex) = PHY_LINK_100F;
    }

    return R_PHY_OK;
} /* End of function phy_set_autonegotiate() */

/***********************************************************************************************************************
 * Function Name: phy_get_link_status
 * Description  : Returns the status of the physical link
 * Arguments    : ether_channel -
 *                    Ethernet channel number
 * Return Value : -1 if links is down, 0 otherwise
 ***********************************************************************************************************************/
int16_t phy_get_link_status(uint32_t ether_channel)
{
    uint16_t reg;

    /* Because reading the first time shows the previous state, the Link status bit is read twice. */
    reg = phy_read(ether_channel, PHY_REG_STATUS);
    reg = phy_read(ether_channel, PHY_REG_STATUS);


    /* When the link isn't up, return error */
    if (PHY_STATUS_LINK_UP != (reg & PHY_STATUS_LINK_UP)) {
        /* Link is down */
        return R_PHY_ERROR;
    }

    /* Link is up */
    return R_PHY_OK;
} /* End of function phy_get_link_status() */

/**
 * Private functions
 */

/***********************************************************************************************************************
 * Function Name: phy_read
 * Description  : Reads a PHY register
 * Arguments    : ether_channel -
 *                    Ethernet channel number
 *                reg_addr -
 *                    address of the PHY register
 * Return Value : read value
 ***********************************************************************************************************************/
static uint16_t phy_read(uint32_t ether_channel, uint16_t reg_addr)
{
    uint16_t data;

    /*
     * The value is read from the PHY register by the frame format of MII Management Interface provided
     * for by Table 22-12 of 22.2.4.5 of IEEE 802.3-2008_section2.
     */
    phy_preamble(ether_channel);
    phy_reg_set(ether_channel, reg_addr, PHY_MII_READ);
    phy_trans_zto0(ether_channel);
    phy_reg_read(ether_channel, &data);
    phy_trans_zto0(ether_channel);

    return (data);
} /* End of function phy_read() */

/***********************************************************************************************************************
 * Function Name: phy_write
 * Description  : Writes to a PHY register
 * Arguments    : ether_channel -
 *                    Ethernet channel number
 *                reg_addr -
 *                    address of the PHY register
 *                data -
 *                    value
 * Return Value : none
 ***********************************************************************************************************************/
static void phy_write(uint32_t ether_channel, uint16_t reg_addr, uint16_t data)
{
    /*
     * The value is read from the PHY register by the frame format of MII Management Interface provided
     * for by Table 22-12 of 22.2.4.5 of IEEE 802.3-2008_section2.
     */
    phy_preamble(ether_channel);
    phy_reg_set(ether_channel, reg_addr, PHY_MII_WRITE);
    phy_trans_1to0(ether_channel);
    phy_reg_write(ether_channel, data);
    phy_trans_zto0(ether_channel);
} /* End of function phy_write() */

/***********************************************************************************************************************
 * Function Name: phy_preamble
 * Description  : As preliminary preparation for access to the PHY module register,
 *                "1" is output via the MII management interface.
 * Arguments    : ether_channel -
 *                    Ethernet channel number
 * Return Value : none
 ***********************************************************************************************************************/
static void phy_preamble(uint32_t ether_channel)
{
    int16_t i;

    /*
     * The processing of PRE (preamble) about the frame format of MII Management Interface which is
     * provided by "Table 22-12" of "22.2.4.5" of "IEEE 802.3-2008_section2".
     */
    i = 32;
    while (i > 0) {
        phy_mii_write1(ether_channel);
        i--;
    }
} /* End of function phy_preamble() */

/***********************************************************************************************************************
 * Function Name: phy_reg_set
 * Description  : Sets a PHY device to read or write mode
 * Arguments    : ether_channel -
 *                    Ethernet channel number
 *                reg_addr -
 *                    address of the PHY register
 *                option -
 *                    mode
 * Return Value : none
 ***********************************************************************************************************************/
static void phy_reg_set(uint32_t ether_channel, uint16_t reg_addr, int32_t option)
{
    int32_t i;
    uint16_t data;
    uint32_t ether_channel_index = phy_get_ctrl_tbl_idx(ether_channel);

    /*
     * The processing of ST (start of frame),OP (operation code), PHYAD (PHY Address), and
     * REGAD (Register Address)  about the frame format of MII Management Interface which is
     * provided by "Table 22-12" of "22.2.4.5" of "IEEE 802.3-2008_section2".
     */
    data = 0;
    data = (PHY_MII_ST << 14); /* ST code    */

    if (PHY_MII_READ == option) {
        data |= (PHY_MII_READ << 12); /* OP code(RD)  */
    } else {
        data |= (PHY_MII_WRITE << 12); /* OP code(WT)  */
    }

    data |= (uint16_t)(g_eth_control_ch[ether_channel_index].phy_address << 7);  /* PHY Address  */

    data |= (reg_addr << 2); /* Reg Address  */

    i = 14;
    while (i > 0) {
        if (0 == (data & 0x8000)) {
            phy_mii_write0(ether_channel);
        } else {
            phy_mii_write1(ether_channel);
        }
        data <<= 1;
        i--;
    }
} /* End of function phy_reg_set() */

/***********************************************************************************************************************
 * Function Name: phy_reg_read
 * Description  : Reads PHY register through MII interface
 * Arguments    : ether_channel -
 *                    Ethernet channel number
 *                pdata -
 *                    pointer to store the data read
 * Return Value : none
 ***********************************************************************************************************************/
static void phy_reg_read(uint32_t ether_channel, uint16_t *pdata)
{
    int32_t i;
    int32_t j;
    uint16_t reg_data;
    int16_t ret;
    volatile uint32_t *petherc_pir;

    ret = phy_get_pir_address(ether_channel, &petherc_pir);
    if (R_PHY_ERROR == ret) {
        return;
    }

    /*
     * The processing of DATA (data) about reading of the frame format of MII Management Interface which is
     * provided by "Table 22-12" of "22.2.4.5" of "IEEE 802.3-2008_section2".
     */
    reg_data = 0;
    i = 16;
    while (i > 0) {
        for (j = ETHER_CFG_PHY_MII_WAIT; j > 0; j--) {
            (*petherc_pir) = 0x00000000;
        }

        for (j = ETHER_CFG_PHY_MII_WAIT; j > 0; j--) {
            (*petherc_pir) = 0x00000001;
        }
        reg_data <<= 1;

        reg_data |= (uint16_t)(((*petherc_pir) & 0x00000008) >> 3);  /* MDI read  */

        for (j = ETHER_CFG_PHY_MII_WAIT; j > 0; j--) {
            (*petherc_pir) = 0x00000001;
        }

        for (j = ETHER_CFG_PHY_MII_WAIT; j > 0; j--) {
            (*petherc_pir) = 0x00000000;
        }

        i--;
    }
    (*pdata) = reg_data;
} /* End of function phy_reg_read() */

/***********************************************************************************************************************
 * Function Name: phy_reg_write
 * Description  : Writes to PHY register through MII interface
 * Arguments    : ether_channel -
 *                    Ethernet channel number
 *                data -
 *                    value to write
 * Return Value : none
 ***********************************************************************************************************************/
static void phy_reg_write(uint32_t ether_channel, uint16_t data)
{
    int32_t i;

    /*
     * The processing of DATA (data) about writing of the frame format of MII Management Interface which is
     * provided by "Table 22-12" of "22.2.4.5" of "IEEE 802.3-2008_section2".
     */
    i = 16;
    while (i > 0) {
        if (0 == (data & 0x8000)) {
            phy_mii_write0(ether_channel);
        } else {
            phy_mii_write1(ether_channel);
        }
        i--;
        data <<= 1;
    }
} /* End of function phy_reg_write() */

/***********************************************************************************************************************
 * Function Name: phy_trans_zto0
 * Description  : Performs bus release so that PHY can drive data
 *              : for read operation
 * Arguments    : ether_channel -
 *                    Ethernet channel number
 * Return Value : none
 ***********************************************************************************************************************/
static void phy_trans_zto0(uint32_t ether_channel)
{
    int32_t j;
    int16_t ret;
    volatile uint32_t *petherc_pir;

    ret = phy_get_pir_address(ether_channel, &petherc_pir);
    if (R_PHY_ERROR == ret) {
        return;
    }

    /*
     * The processing of TA (turnaround) about reading of the frame format of MII Management Interface which is
     * provided by "Table 22-12" of "22.2.4.5" of "IEEE 802.3-2008_section2".
     */
    for (j = ETHER_CFG_PHY_MII_WAIT; j > 0; j--) {
        (*petherc_pir) = 0x00000000;
    }

    for (j = ETHER_CFG_PHY_MII_WAIT; j > 0; j--) {
        (*petherc_pir) = 0x00000001;
    }

    for (j = ETHER_CFG_PHY_MII_WAIT; j > 0; j--) {
        (*petherc_pir) = 0x00000001;
    }

    for (j = ETHER_CFG_PHY_MII_WAIT; j > 0; j--) {
        (*petherc_pir) = 0x00000000;
    }

} /* End of function phy_trans_zto0() */

/***********************************************************************************************************************
 * Function Name: phy_trans_1to0
 * Description  : Switches data bus so MII interface can drive data
 *              : for write operation
 * Arguments    : ether_channel -
 *                    Ethernet channel number
 * Return Value : none
 ***********************************************************************************************************************/
static void phy_trans_1to0(uint32_t ether_channel)
{
    /*
     * The processing of TA (turnaround) about writing of the frame format of MII Management Interface which is
     * provided by "Table 22-12" of "22.2.4.5" of "IEEE 802.3-2008_section2".
     */
    phy_mii_write1(ether_channel);
    phy_mii_write0(ether_channel);
} /* End of function phy_trans_1to0() */

/***********************************************************************************************************************
 * Function Name: phy_mii_write1
 * Description  : Outputs 1 to the MII interface
 * Arguments    : ether_channel -
 *                    Ethernet channel number
 * Return Value : none
 ***********************************************************************************************************************/
static void phy_mii_write1(uint32_t ether_channel)
{
    int32_t j;
    int16_t ret;
    volatile uint32_t *petherc_pir;

    ret = phy_get_pir_address(ether_channel, &petherc_pir);
    if (R_PHY_ERROR == ret) {
        return;
    }

    /*
     * The processing of one bit about frame format of MII Management Interface which is
     * provided by "Table 22-12" of "22.2.4.5" of "IEEE 802.3-2008_section2".
     * The data that 1 is output.
     */
    for (j = ETHER_CFG_PHY_MII_WAIT; j > 0; j--) {
        (*petherc_pir) = 0x00000006;
    }

    for (j = ETHER_CFG_PHY_MII_WAIT; j > 0; j--) {
        (*petherc_pir) = 0x00000007;
    }

    for (j = ETHER_CFG_PHY_MII_WAIT; j > 0; j--) {
        (*petherc_pir) = 0x00000007;
    }

    for (j = ETHER_CFG_PHY_MII_WAIT; j > 0; j--) {
        (*petherc_pir) = 0x00000006;
    }

} /* End of function phy_mii_write1() */

/***********************************************************************************************************************
 * Function Name: phy_mii_write0
 * Description  : Outputs 0 to the MII interface
 * Arguments    : ether_channel -
 *                    Ethernet channel number
 * Return Value : none
 ***********************************************************************************************************************/
static void phy_mii_write0(uint32_t ether_channel)
{
    int32_t j;
    int16_t ret;
    volatile uint32_t *petherc_pir;

    ret = phy_get_pir_address(ether_channel, &petherc_pir);
    if (R_PHY_ERROR == ret) {
        return;
    }

    /*
     * The processing of one bit about frame format of MII Management Interface which is
     * provided by "Table 22-12" of "22.2.4.5" of "IEEE 802.3-2008_section2".
     * The data that 0 is output.
     */
    for (j = ETHER_CFG_PHY_MII_WAIT; j > 0; j--) {
        (*petherc_pir) = 0x00000002;
    }

    for (j = ETHER_CFG_PHY_MII_WAIT; j > 0; j--) {
        (*petherc_pir) = 0x00000003;
    }

    for (j = ETHER_CFG_PHY_MII_WAIT; j > 0; j--) {
        (*petherc_pir) = 0x00000003;
    }

    for (j = ETHER_CFG_PHY_MII_WAIT; j > 0; j--) {
        (*petherc_pir) = 0x00000002;
    }

} /* End of function phy_mii_write0() */

/***********************************************************************************************************************
 * Function Name: phy_get_pir_address
 * Description  : Get the address of the PHY interface register.
 * Arguments    : ether_channel -
 *                    Ethernet channel number
 *                pppir_addr -
 *                    Pointer of the PHY interface register
 * Return Value : none
 ***********************************************************************************************************************/
static int16_t phy_get_pir_address(uint32_t ether_channel, volatile uint32_t **pppir_addr)
{
    volatile uint32_t *petherc_pir;
    uint32_t ether_channel_index = phy_get_ctrl_tbl_idx(ether_channel);

    petherc_pir = g_eth_control_ch[ether_channel_index].preg_pir;

    (*pppir_addr) = petherc_pir;

    return R_PHY_OK;
} /* End of function phy_get_pir_address() */

/***********************************************************************************************************************
 * Function Name: phy_get_ctrl_tbl_idx
 * Description  : get index of control table.
 * Arguments    : ether_channel -
 *                    Ethernet channel number
 * Return Value : Index of control table
 ***********************************************************************************************************************/
static uint32_t phy_get_ctrl_tbl_idx(uint32_t ether_channel)
{
#if (ETHER_CHANNEL_MAX == 1)
    return 0;
#else
    return ether_channel;
#endif
} /* End of function phy_get_ctrl_tbl_idx() */
