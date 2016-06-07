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

#ifndef MPS2_ETHERNET_API_H
#define MPS2_ETHERNET_API_H

#include "device.h"

#if DEVICE_ETHERNET

#ifdef __cplusplus
extern "C" {
#endif

// Connection constants

// send ethernet write buffer, returning the packet size sent
int ethernet_transmission(unsigned char * pkt, unsigned int length);

// recieve from ethernet buffer, returning packet size, or 0 if no packet
int ethernet_reception(unsigned int *recvbuf, unsigned int *index);

// get the ethernet address
int ethernet_mac_address(char *mac);

unsigned int ethernet_check_ready(void);

unsigned int ethernet_intf(void);

int smsc9220_check_id(void);

int smsc9220_check_macaddress(void);

void smsc9220_print_mac_registers(void);
void smsc9220_print_phy_registers(void);

#ifdef __cplusplus
}
#endif

#endif

#endif

