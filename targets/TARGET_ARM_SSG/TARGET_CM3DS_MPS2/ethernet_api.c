/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
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
#include "cmsis.h"
#include "mbed_interface.h"
#include "mbed_toolchain.h"
#include "mbed_error.h"
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
    return 0;

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
}

