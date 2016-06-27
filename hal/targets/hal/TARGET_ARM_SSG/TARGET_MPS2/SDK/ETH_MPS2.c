/* MPS2 Peripheral Library
*
* Copyright (c) 2006-2015 ARM Limited
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, 
* this list of conditions and the following disclaimer.
* 
* 2. Redistributions in binary form must reproduce the above copyright notice, 
* this list of conditions and the following disclaimer in the documentation 
* and/or other materials provided with the distribution.
* 
* 3. Neither the name of the copyright holder nor the names of its contributors 
* may be used to endorse or promote products derived from this software without 
* specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
* POSSIBILITY OF SUCH DAMAGE. 
*/

/*
 * Code implementation file for the LAN Ethernet interface.
 */

#include <stdio.h>
#include "wait_api.h"
#include "ETH_MPS2.h"

// SMSC9220 low-level operations
unsigned int smsc9220_mac_regread(unsigned char regoffset, unsigned int *data)
{
    unsigned int val, maccmd;
    int timedout;
    int error;

    error = 0;
    val = SMSC9220->MAC_CSR_CMD;
    if(!(val & ((unsigned int)1 << 31))) {    // Make sure there's no pending operation
        maccmd = 0;
        maccmd |= regoffset;
        maccmd |= ((unsigned int)1 << 30);     // Indicates read
        maccmd |= ((unsigned int)1 << 31);     // Start bit
        SMSC9220->MAC_CSR_CMD = maccmd;        // Start operation

        timedout = 50;
        do {
            val = SMSC9220->BYTE_TEST;  // A no-op read.
            wait_ms(1);
            timedout--;
        } while(timedout && (SMSC9220->MAC_CSR_CMD & ((unsigned int)1 << 31)));

        if(!timedout) {
            error = 1;
        }
        else
            *data = SMSC9220->MAC_CSR_DATA;
    } else {
        *data = 0;
    }
    return error;
}

unsigned int smsc9220_mac_regwrite(unsigned char regoffset, unsigned int data)
{
    unsigned int read, maccmd;
    int timedout;
    int error;

    error = 0;
    read = SMSC9220->MAC_CSR_CMD;
    if(!(read & ((unsigned int)1 << 31))) { // Make sure there's no pending operation
        SMSC9220->MAC_CSR_DATA = data;      // Store data.
        maccmd = 0;
        maccmd |= regoffset;
        maccmd &= ~((unsigned int)1 << 30); // Clear indicates write
        maccmd |= ((unsigned int)1 << 31);  // Indicate start of operation
        SMSC9220->MAC_CSR_CMD = maccmd;

        timedout = 50;
        do {
            read = SMSC9220->BYTE_TEST;     // A no-op read.
            wait_ms(1);
            timedout--;
        } while(timedout && (SMSC9220->MAC_CSR_CMD & ((unsigned int)1 << 31)));

        if(!timedout) {
            error = 1;
        }
    } else {
       printf("Warning: SMSC9220 MAC CSR is busy. No data written.\n");
    }
    return error;
}

unsigned int smsc9220_phy_regread(unsigned char regoffset, unsigned short *data)
{
    unsigned int val, phycmd; int error;
    int timedout;

    error = 0;

    smsc9220_mac_regread(SMSC9220_MAC_MII_ACC, &val);

    if(!(val & 1)) {    // Not busy
        phycmd = 0;
        phycmd |= (1 << 11);                 // 1 to [15:11]
        phycmd |= ((regoffset & 0x1F) << 6); // Put regoffset to [10:6]
        phycmd &= ~(1 << 1);                 // Clear [1] indicates read.
        phycmd |= (1 << 0);                  // Set [0] indicates operation start

        smsc9220_mac_regwrite(SMSC9220_MAC_MII_ACC, phycmd);

        val = 0;
        timedout = 50;
        do {
            wait_ms(1);
            timedout--;
            smsc9220_mac_regread(SMSC9220_MAC_MII_ACC,&val);
        } while(timedout && (val & ((unsigned int)1 << 0)));

        if(!timedout) {
            error = 1;
        }
        else
            smsc9220_mac_regread(SMSC9220_MAC_MII_DATA, (unsigned int *)data);

    } else {
        *data = 0;
    }
    return error;
}

unsigned int smsc9220_phy_regwrite(unsigned char regoffset, unsigned short data)
{
    unsigned int val, phycmd; int error;
    int timedout;

    error = 0;

    smsc9220_mac_regread(SMSC9220_MAC_MII_ACC, &val);

    if(!(val & 1)) {    // Not busy
        smsc9220_mac_regwrite(SMSC9220_MAC_MII_DATA, (data & 0xFFFF)); // Load the data
        phycmd = 0;
        phycmd |= (1 << 11);                    // 1 to [15:11]
        phycmd |= ((regoffset & 0x1F) << 6);     // Put regoffset to [10:6]
        phycmd |= (1 << 1);                     // Set [1] indicates write.
        phycmd |= (1 << 0);                     // Set [0] indicates operation start
        smsc9220_mac_regwrite(SMSC9220_MAC_MII_ACC, phycmd);   // Start operation

        phycmd = 0;
        timedout = 50;

        do {

            wait_ms(1);
            timedout--;
            smsc9220_mac_regread(SMSC9220_MAC_MII_ACC, &phycmd);
        } while(timedout && (phycmd & (1 << 0)));

        if(!timedout) {
            error = 1;
        }

    } else {
        printf("Warning: SMSC9220 MAC MII is busy. No data written.\n");
    }
    return error;
}

// Returns smsc9220 id.
unsigned int smsc9220_read_id(void)
{
    return SMSC9220->ID_REV;
}

// Initiates a soft reset, returns failure or success.
unsigned int smsc9220_soft_reset(void)
{
    int timedout;

    timedout = 10;
    // Soft reset
    SMSC9220->HW_CFG |= 1;

    do {
        wait_ms(1);
        timedout--;
    } while(timedout && (SMSC9220->HW_CFG & 1));

    if(!timedout)
        return 1;

    return 0;
}

void smsc9220_set_txfifo(unsigned int val)
{
    // 2kb minimum, 14kb maximum
    if(val < 2 || val > 14)
        return;

    SMSC9220->HW_CFG = val << 16;
}


unsigned int smsc9220_wait_eeprom(void)
{
    int timedout;

    timedout = 50;

    do {
        wait_ms(1);
        timedout--;

    } while(timedout && (SMSC9220->E2P_CMD & ((unsigned int) 1 << 31)));

    if(!timedout)
        return 1;

    return 0;
}

/* initialise irqs */
void smsc9220_init_irqs(void)
{
    SMSC9220->INT_EN    = 0x0;
    SMSC9220->INT_STS   = 0xFFFFFFFF;   // clear all interrupts
    SMSC9220->IRQ_CFG   = 0x22000100;   // irq deassertion at 220 usecs and master IRQ enable.
}

unsigned int smsc9220_check_phy(void)
{
    unsigned short phyid1, phyid2;

    smsc9220_phy_regread(SMSC9220_PHY_ID1,&phyid1);
    smsc9220_phy_regread(SMSC9220_PHY_ID2,&phyid2);
    return ((phyid1 == 0xFFFF && phyid2 == 0xFFFF) ||
            (phyid1 == 0x0 && phyid2 == 0x0));
}

unsigned int smsc9220_reset_phy(void)
{
    unsigned short read;
    int error;

    error = 0;
    if(smsc9220_phy_regread(SMSC9220_PHY_BCONTROL, &read)) {
        error = 1;
        return error;
    }

    read |= (1 << 15);
    if(smsc9220_phy_regwrite(SMSC9220_PHY_BCONTROL, read)) {
        error = 1;
        return error;
    }
    return 0;
}

/* Advertise all speeds and pause capabilities */
void smsc9220_advertise_cap(void)
{
    unsigned short aneg_adv;
    aneg_adv = 0;


    smsc9220_phy_regread(SMSC9220_PHY_ANEG_ADV, &aneg_adv);
    aneg_adv |= 0xDE0;

    smsc9220_phy_regwrite(SMSC9220_PHY_ANEG_ADV, aneg_adv);
    smsc9220_phy_regread(SMSC9220_PHY_ANEG_ADV, &aneg_adv);
    return;
}

void smsc9220_establish_link(void)
{
    unsigned short bcr;

    smsc9220_phy_regread(SMSC9220_PHY_BCONTROL, &bcr);
    bcr |= (1 << 12) | (1 << 9);
    smsc9220_phy_regwrite(SMSC9220_PHY_BCONTROL, bcr);
    smsc9220_phy_regread(SMSC9220_PHY_BCONTROL, &bcr);

    {
        unsigned int hw_cfg;

        hw_cfg = 0;
        hw_cfg = SMSC9220->HW_CFG;

        hw_cfg &= 0xF0000;
        hw_cfg |= (1 << 20);
        SMSC9220->HW_CFG = hw_cfg;
    }

    return;
}

void smsc9220_enable_xmit(void)
{
    SMSC9220->TX_CFG = 0x2; // Enable trasmission
    return;
}

void smsc9220_enable_mac_xmit(void)
{
    unsigned int mac_cr;

    mac_cr = 0;
    smsc9220_mac_regread(SMSC9220_MAC_CR, &mac_cr);

    mac_cr |= (1 << 3);     // xmit enable
    mac_cr |= (1 << 28);    // Heartbeat disable

    smsc9220_mac_regwrite(SMSC9220_MAC_CR, mac_cr);
    return;
}

void smsc9220_enable_mac_recv(void)
{
    unsigned int mac_cr;

    mac_cr = 0;
    smsc9220_mac_regread(SMSC9220_MAC_CR, &mac_cr);
    mac_cr |= (1 << 2);     // Recv enable
    smsc9220_mac_regwrite(SMSC9220_MAC_CR, mac_cr);

    return;
}


unsigned int smsc9220_check_ready(void)
{
    return !(SMSC9220->PMT_CTRL & 1);
}

/* Generate a soft irq */
void smsc9220_set_soft_int(void)
{
    SMSC9220->INT_EN |= 0x80000000;
}

/* clear soft irq */
void smsc9220_clear_soft_int(void)
{
    SMSC9220->INT_STS |= 0x80000000;
}


unsigned int smsc9220_recv_packet(unsigned int *recvbuf, unsigned int *index)
{
    unsigned int rxfifo_inf;    // Tells us the status of rx payload and status fifos.
    unsigned int rxfifo_stat;

    unsigned int pktsize;
    unsigned int dwords_to_read;

    rxfifo_inf = SMSC9220->RX_FIFO_INF;

    if(rxfifo_inf & 0xFFFF) { // If there's data
        rxfifo_stat = SMSC9220->RX_STAT_PORT;
        if(rxfifo_stat != 0) {   // Fetch status of this packet
            pktsize = ((rxfifo_stat >> 16) & 0x3FFF);
            if(rxfifo_stat & (1 << 15)) {
                printf("Error occured during receiving of packets on the bus.\n");
                return 1;
            } else {
                /* Below formula (recommended by SMSC9220 code)
                 * gives 1 more than required. This is perhaps because
                 * a last word is needed for not word aligned packets.
                 */
                dwords_to_read = (pktsize + 3) >> 2;
                // PIO copy of data received:
                while(dwords_to_read > 0) {
                    recvbuf[*index] = SMSC9220->RX_DATA_PORT;
                    (*index)++;
                    dwords_to_read--;
                }
            }
        } else {
            return 1;
        }
    } else {
        return 1;
    }

    rxfifo_stat = SMSC9220->RX_STAT_PORT;
    rxfifo_inf = SMSC9220->RX_FIFO_INF;

    return 0;
}


// Does the actual transfer of data to FIFO, note it does no
// fifo availability checking. This should be done by caller.
// Assumes the whole frame is transferred at once as a single segment
void smsc9220_xmit_packet(unsigned char * pkt, unsigned int length)
{
    unsigned int txcmd_a, txcmd_b;
    unsigned int dwords_to_write;
    volatile unsigned int dwritten;
    unsigned int *pktptr;
    volatile unsigned int xmit_stat, xmit_stat2, xmit_inf;
    int i;

    pktptr = (unsigned int *) pkt;
    txcmd_a = 0;
    txcmd_b = 0;

    txcmd_a |= (1 << 12) | (1 << 13);   // First and last segments
    txcmd_a |= length & 0x7FF;          // [10:0] contains length

    txcmd_b |= ((length & 0xFFFF) << 16); // [31:16] contains length
    txcmd_b |= length & 0x7FF;          // [10:0] also contains length


    SMSC9220->TX_DATA_PORT = txcmd_a;
    SMSC9220->TX_DATA_PORT = txcmd_b;
    dwritten = dwords_to_write = (length + 3) >> 2;

    // PIO Copy to FIFO. Could replace this with DMA.
    while(dwords_to_write > 0) {
         SMSC9220->TX_DATA_PORT = *pktptr;
         pktptr++;
         dwords_to_write--;
    }

    xmit_stat = SMSC9220->TX_STAT_PORT;
    xmit_stat2 = SMSC9220->TX_STAT_PORT;
    xmit_inf = SMSC9220->TX_FIFO_INF;

    if(xmit_stat2 != 0 ) {
        for(i = 0; i < 6; i++) {
            xmit_stat2 = SMSC9220->TX_STAT_PORT;
        }
    }
}
