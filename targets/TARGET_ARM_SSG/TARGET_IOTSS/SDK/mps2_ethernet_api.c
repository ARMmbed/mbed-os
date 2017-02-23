/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#include <string.h>

#include "mps2_ethernet_api.h"
#include "cmsis.h"
#include "mbed_interface.h"
#include "mbed_toolchain.h"
#include "mbed_error.h"
#include "ETH_MPS2.h"
#include "mbed_wait_api.h"

#define TX_PKT_SIZE 256
#define RX_PKT_SIZE 300

// Types
#undef FALSE
#undef TRUE
#define FALSE   0
#define TRUE    1


int smsc9220_check_id(void)
{
    int error;
    unsigned int id;
    error = 0;

    id = smsc9220_read_id();

    // If bottom and top halves of the word are the same
    if(((id >> 16) & 0xFFFF) == (id & 0xFFFF)) {
        error = 1;
        return error;
    }
    switch(((id >> 16) & 0xFFFF)) {
        case 0x9220:
            break;

        default:
            error = 1;
            break;
    }

    return error;
}

int smsc9220_check_macaddress(void)
{
    int error;
    const unsigned int mac_valid_high = 0xC00A;
    const unsigned int mac_valid_low = 0x00F70200;
    unsigned int mac_low;
    unsigned int mac_high;

    error = 0;

    // Read current mac address.
    smsc9220_mac_regread(SMSC9220_MAC_ADDRH, &mac_high);
    smsc9220_mac_regread(SMSC9220_MAC_ADDRL, &mac_low);

    // Writing temporary address:
    smsc9220_mac_regwrite(SMSC9220_MAC_ADDRH, mac_valid_high);
    smsc9220_mac_regwrite(SMSC9220_MAC_ADDRL, mac_valid_low);

    // Verify write was correct:
    smsc9220_mac_regread(SMSC9220_MAC_ADDRH, &mac_high);
    smsc9220_mac_regread(SMSC9220_MAC_ADDRL, &mac_low);


    if(mac_high != mac_valid_high || mac_low != mac_valid_low) {
        error = TRUE;
        return error;
    }

    return error;
}

void smsc9220_print_mac_registers()
{
    unsigned int read;
    int i;

    i = 0;
    read = 0;

    for(i = 1; i <= 0xC; i++) {
        smsc9220_mac_regread(i, &read);
    }
    return;
}


void smsc9220_print_phy_registers()
{
    unsigned short read;
    unsigned int i;

    i = 0;
    read = 0;
    for(i = 0; i <= 6; i++) {
        smsc9220_phy_regread(i, &read);
    }
    smsc9220_phy_regread(i = 17, &read);

    smsc9220_phy_regread(i = 18, &read);

    smsc9220_phy_regread(i = 27, &read);

    smsc9220_phy_regread(i = 29, &read);

    smsc9220_phy_regread(i = 30, &read);

    smsc9220_phy_regread(i = 31, &read);

    return;
}

/*----------------------------------------------------------------------------
  Ethernet Device initialize
 *----------------------------------------------------------------------------*/

int ethernet_transmission(unsigned char * pkt, unsigned int length)
{
    smsc9220_xmit_packet(pkt, length);
    return 0;
}

int ethernet_reception(unsigned int *recvbuf, unsigned int *index) 
{
    return smsc9220_recv_packet((unsigned int *)recvbuf, index);
}

int ethernet_mac_address(char *mac) 
{
    return smsc9220_check_macaddress();
}

unsigned int ethernet_check_ready(void)
{
    return smsc9220_check_ready();
}

unsigned int ethernet_intf()
{
  unsigned int txfifo_inf;
 
    txfifo_inf = SMSC9220->TX_FIFO_INF;
    
    return txfifo_inf;

}

