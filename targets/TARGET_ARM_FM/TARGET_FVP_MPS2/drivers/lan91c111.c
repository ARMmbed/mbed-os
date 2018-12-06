/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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


#include "lan91c111.h"
#include <stddef.h>

static lan91_handle_t lan91c111_handle;

void LAN91_init(void)
{

    uint32_t tcr, stat;

    LAN91_ClearInterruptMasks();
    LAN91_Reset();

    /* Clear control registers. */
    LAN91_SelectBank(0);
    LREG(uint16_t, B0_RCR) = 0;
    LREG(uint16_t, B0_TCR) = 0;

    /* Write Configuration Registers */
    LAN91_SelectBank(1);
    LREG(uint16_t, B1_CR) = CR_EPH_POW_EN | CR_DEFAULT;

    /* Reset the PHY*/
    write_PHY(0, 0x8000);

    /* clear phy 18 status */
    read_PHY(18);

    /* Use 100MBit link speed by default. */
    LAN91_SelectBank(0);
    LREG(uint16_t, B0_RPCR) = RPCR_SPEED | LEDA_10M_100M | LEDB_TX_RX;

    /* Read Status Register */
    stat = read_PHY(18);

    /* Set the Control Register */
    LAN91_SelectBank(1);
    LREG(uint16_t, B1_CTR) = CTR_LE_ENABLE | CTR_CR_ENABLE | CTR_TE_ENABLE | CTR_AUTO_REL  | CTR_DEFAULT;

    /* Setup Transmit Control Register. */
    tcr = TCR_TXENA | TCR_PAD_EN;
    if (stat & 0x0040) {
        tcr |= TCR_FDUPLX;
    }
    LREG(uint16_t, B0_TCR) = (uint16_t)tcr;

    /* Reset MMU */
    LAN91_SelectBank(2);
    LREG(uint16_t, B2_MMUCR) = MMU_RESET;
    while (LREG(uint16_t, B2_MMUCR) & MMUCR_BUSY);

    /* Configure the Interrupts, allow  RX_OVRN and RCV intr. */
    LAN91_SetInterruptMasks(MSK_RCV);

    /* Set all buffers or data in handler for data transmit/receive process. */
    LAN91_SetHandler();
}

void read_MACaddr(uint8_t *addr)
{
    int i;

    LAN91_SelectBank(1);

    i = LREG(uint16_t, B1_IAR0);
    addr[0] = (uint8_t)i;
    addr[1] = (uint8_t)(i >> 8);

    i = LREG(uint16_t, B1_IAR2);
    addr[2] = (uint8_t)i;
    addr[3] = (uint8_t)(i >> 8);

    i = LREG(uint16_t, B1_IAR4);
    addr[4] = (uint8_t)i;
    addr[5] = (uint8_t)(i >> 8);
}

void LAN91_SetCallback(lan91_callback_t callback, void *userData)
{
    /* Set callback and userData. */
    lan91c111_handle.callback = callback;
    lan91c111_handle.userData = userData;
}


bool LAN91_send_frame(uint32_t *buff, uint32_t *size)
{
    /* Send frame to Lan91C111 ethernet controller */
    uint16_t *dp;
    uint8_t packnr;
    int i;

    LAN91_SelectBank(2);

    /* MMU allocate memory for transmitting*/
    LREG(uint16_t, B2_MMUCR) = MMU_ALLOC_TX;

    /* Check if Interrupt Status Register been set for MMU Allocate Ready */
    if (!(LREG(uint16_t, B2_IST) & IST_ALLOC_INT)) {
        /* Failed, Reset MMU */
        LREG(uint16_t, B2_MMUCR) = MMU_RESET;
        while (LREG(uint16_t, B2_MMUCR) & MMUCR_BUSY);
        return false;
    }

    /* set TX package number from allocated package number
       and also set pointer register as Auto Increase, write and transmit set */
    packnr = LREG(uint8_t, B2_ARR);
    LREG(uint8_t, B2_PNR) = packnr;
    LREG(uint16_t, B2_PTR) = PTR_AUTO_INCR;

    /* Reserve space for Status Word */
    LREG(uint16_t, B2_DATA0) = 0x0000;
    /* Total = Raw Data Size + STATUS WORD + BYTE COUNT + CONTROL BYTE + LAST BYTE */
    LREG(uint16_t, B2_DATA0) = (uint16_t) * size + 6;

    /* Copy frame data to Ethernet controller */
    dp = (uint16_t *)buff;
    for (i = *size; i > 1; i -= 2) {
        LREG(uint16_t, B2_DATA0) = *dp++;
    }

    /* If data is odd , Add a control word and odd byte. */
    if (i) {
        LREG(uint16_t, B2_DATA0) = (RFC_CRC | RFC_ODD) | (*dp & 0xFF);
    } else {
        /* Add control word. */
        LREG(uint16_t, B2_DATA0) = RFC_CRC;
    }

    LAN91_SelectBank(0);
    /* Enable transmitter. */
    LREG(uint16_t, B0_TCR)  = TCR_TXENA | TCR_PAD_EN;

    /* Enqueue the packet. */
    LAN91_SelectBank(2);
    LREG(uint16_t, B2_MMUCR) = MMU_ENQ_TX;

    return true;
}



bool LAN91_receive_frame(uint32_t *buff, uint32_t *size)
{
    uint32_t State, RxLen;
    uint32_t val, *dp;

    /* No receive packets queued in Rx FIFO queue */
    LAN91_SelectBank(2);
    State = LREG(uint16_t, B2_FIFO);
    if (State & FIFO_REMPTY) {
        return false;
    }


    /* Pointer Register set to  RCV + Auto Increase + Read access
    So that Data Register is use RX FIFO*/
    LREG(uint16_t, B2_PTR) = PTR_RCV | PTR_AUTO_INCR | PTR_READ;

    /* Read status word and packet length */
    val = LREG(uint32_t, B2_DATA);
    State = val & 0xFFFF;
    /* Raw Data Size = Total - STATUS WORD - BYTE COUNT - CONTROL BYTE - LAST BYTE */
    RxLen = (val >> 16) - 6;

    /* Check State word if Odd number of bytes in a frame. */
    if (State & RFS_ODDFRM) {
        RxLen++;
    }

    /* Packet too big, ignore it and free MMU and continue */
    if (RxLen > LAN91_ETH_MTU_SIZE) {
        LREG(uint16_t, B2_MMUCR) = MMU_REMV_REL_RX;
        return false;
    }

    if (buff != NULL) {
        /* Make sure that block is dword aligned */
        RxLen = (RxLen + 3) >> 2;
        *size = RxLen << 2;
        dp = buff;
        for (; RxLen; RxLen--) {
            *dp++ = LREG(uint32_t, B2_DATA);
        }
    }

    /* MMU free packet. Remove and Relase from TOP of RX */
    LREG(uint16_t, B2_MMUCR) = MMU_REMV_REL_RX;

    return true;
}



void ETHERNET_Handler(void)
{
    LAN91_SelectBank(2);
    if ((LREG(uint8_t, B2_IST) & IST_RCV) != 0) {
        LREG(uint8_t,  B2_MSK) = 0;
        /* Callback function. */
        if (lan91c111_handle.callback) {
            lan91c111_handle.callback(LAN91_RxEvent, lan91c111_handle.userData);
        }
    }
}


lan91_phy_status_t LAN91_GetLinkStatus(void)
{
    if (read_PHY(0x2u) & 0x4u) {
        return STATE_LINK_UP;
    } else {
        return STATE_LINK_DOWN;
    }
}

static void output_MDO(int bit_value)
{
    uint32_t val = MGMT_MDOE;

    if (bit_value) {
        val |= MGMT_MDO;
    }
    LREG(uint16_t, B3_MGMT) = (uint16_t)val;
    LREG(uint16_t, B3_MGMT) = (uint16_t)(val | MGMT_MCLK);
    LREG(uint16_t, B3_MGMT) = (uint16_t)val;
}



static int input_MDI(void)
{
    int val = 0;

    LREG(uint16_t, B3_MGMT) = 0;
    LREG(uint16_t, B3_MGMT) = MGMT_MCLK;
    if (LREG(uint16_t, B3_MGMT) & MGMT_MDI) {
        val = 1;
    }
    LREG(uint16_t, B3_MGMT) = 0;
    return (val);
}

static void write_PHY(uint32_t PhyReg, int Value)
{
    int i;

    LAN91_SelectBank(3);
    LREG(uint16_t, B3_MGMT) = MGMT_MDOE | MGMT_MDO;

    /* 32 consecutive ones on MDO to establish sync */
    for (i = 0; i < 32; i++) {
        LREG(uint16_t, B3_MGMT) = MGMT_MDOE | MGMT_MDO;
        LREG(uint16_t, B3_MGMT) = MGMT_MDOE | MGMT_MDO | MGMT_MCLK;
    }
    LREG(uint16_t, B3_MGMT) = MGMT_MDOE;

    /* start code (01) */
    output_MDO(0);
    output_MDO(1);

    /* write command (01) */
    output_MDO(0);
    output_MDO(1);

    /* write PHY address - which is five 0s for 91C111 */
    for (i = 0; i < 5; i++) {
        output_MDO(0);
    }

    /* write the PHY register to write (highest bit first) */
    for (i = 0; i < 5; i++) {
        output_MDO((PhyReg >> 4) & 0x01);
        PhyReg <<= 1;
    }

    /* turnaround MDO */
    output_MDO(1);
    output_MDO(0);

    /* write the data value (highest bit first) */
    for (i = 0; i < 16; i++) {
        output_MDO((Value >> 15) & 0x01);
        Value <<= 1;
    }

    /* turnaround MDO is tristated */
    LREG(uint16_t, B3_MGMT) = 0;
    LREG(uint16_t, B3_MGMT) = MGMT_MCLK;
    LREG(uint16_t, B3_MGMT) = 0;
}


static uint16_t read_PHY(uint32_t PhyReg)
{
    int i, val;

    LAN91_SelectBank(3);
    LREG(uint16_t, B3_MGMT) = MGMT_MDOE | MGMT_MDO;

    /* 32 consecutive ones on MDO to establish sync */
    for (i = 0; i < 32; i++) {
        LREG(uint16_t, B3_MGMT) = MGMT_MDOE | MGMT_MDO;
        LREG(uint16_t, B3_MGMT) = MGMT_MDOE | MGMT_MDO | MGMT_MCLK;
    }
    LREG(uint16_t, B3_MGMT) = MGMT_MDOE;

    /* start code (01) */
    output_MDO(0);
    output_MDO(1);

    /* read command (10) */
    output_MDO(1);
    output_MDO(0);

    /* write PHY address - which is five 0s for 91C111 */
    for (i = 0; i < 5; i++) {
        output_MDO(0);
    }

    /* write the PHY register to read (highest bit first) */
    for (i = 0; i < 5; i++) {
        output_MDO((PhyReg >> 4) & 0x01);
        PhyReg <<= 1;
    }

    /* turnaround MDO is tristated */
    LREG(uint16_t, B3_MGMT) = 0;
    LREG(uint16_t, B3_MGMT) = MGMT_MCLK;
    LREG(uint16_t, B3_MGMT) = 0;

    /* read the data value */
    val = 0;
    for (i = 0; i < 16; i++) {
        val <<= 1;
        val |= input_MDI();
    }

    /* turnaround MDO is tristated */
    LREG(uint16_t, B3_MGMT) = 0;
    LREG(uint16_t, B3_MGMT) = MGMT_MCLK;
    LREG(uint16_t, B3_MGMT) = 0;

    return (val);
}
