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

#include "ethernet_api.h"
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

/*----------------------------------------------------------------------------
  Ethernet Device initialize
 *----------------------------------------------------------------------------*/
int ethernet_init()
{
      int error;
    error = 0;

    if(smsc9220_check_id()) {
        error = TRUE;
    }

    if(smsc9220_soft_reset()) {
        error = TRUE;
    }

    smsc9220_set_txfifo(5);

    // Sets automatic flow control thresholds, and backpressure
    // threshold to defaults specified.
    SMSC9220->AFC_CFG = 0x006E3740;

    if(smsc9220_wait_eeprom()) {
        error = TRUE;
    }

    // Configure GPIOs as LED outputs.
    SMSC9220->GPIO_CFG = 0x70070000;

    smsc9220_init_irqs();

    /* Configure MAC addresses here if needed. */

    if(smsc9220_check_phy()) {
        error = TRUE;
    }

    if(smsc9220_reset_phy()) {
        error = TRUE;
        return error;
    }

    wait_ms(100);
    // Checking whether phy reset completed successfully.
    {
        unsigned short phyreset;
        phyreset = 0;
        smsc9220_phy_regread(SMSC9220_PHY_BCONTROL, &phyreset);
        if(phyreset & (1 << 15)) {
            error = TRUE;
            return error;
        }
    }

    /* Advertise capabilities */
    smsc9220_advertise_cap();


    /* Begin to establish link */
    smsc9220_establish_link();      // bit [12] of BCONTROL seems self-clearing.
                                    // Although it's not so in the manual.

    /* Interrupt threshold */
    SMSC9220->FIFO_INT = 0xFF000000;

    smsc9220_enable_mac_xmit();

    smsc9220_enable_xmit();

    SMSC9220->RX_CFG = 0;

    smsc9220_enable_mac_recv();

    // Rx status FIFO level irq threshold
    SMSC9220->FIFO_INT &= ~(0xFF);  // Clear 2 bottom nibbles

    // This sleep is compulsory otherwise txmit/receive will fail.
    wait_ms(2000);
    return error;

}

/*----------------------------------------------------------------------------
  Ethernet Device Uninitialize
 *----------------------------------------------------------------------------*/
void ethernet_free() {
}

int ethernet_write(const char *data, int size)
{
        return 0;
}

int ethernet_send() 
{
 return 0;
}

int ethernet_receive() 
{
    return 0;
}

// Read from an recevied ethernet packet.
// After receive returnd a number bigger than 0 it is
// possible to read bytes from this packet.
// Read will write up to size bytes into data.
// It is possible to use read multible times.
// Each time read will start reading after the last read byte before.

int ethernet_read(char *data, int dlen) 
{
    return 0;
}

void ethernet_address(char *mac) {
    mbed_mac_address(mac);
}

int ethernet_link(void) 
{
    return 0;
}

void ethernet_set_link(int speed, int duplex)
{
    smsc9220_establish_link();
}

