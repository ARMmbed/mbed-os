
/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#ifndef MBED_ETHERNET_API_H
#define MBED_ETHERNET_API_H

#include "device.h"
#include "platform/mbed_toolchain.h"

#if DEVICE_ETHERNET

#ifdef __cplusplus
extern "C" {
#endif

// Connection constants
MBED_DEPRECATED(
    "Ethernet drivers are provided by a class derived from `EMAC` attached to"
    " either lwIP or Nanostack."
)
int ethernet_init(void);
MBED_DEPRECATED(
    "Ethernet drivers are provided by a class derived from `EMAC` attached to"
    " either lwIP or Nanostack."
)
void ethernet_free(void);

// write size bytes from data to ethernet buffer
// return num bytes written
// or -1 if size is too big
MBED_DEPRECATED(
    "Ethernet drivers are provided by a class derived from `EMAC` attached to"
    " either lwIP or Nanostack."
)
int ethernet_write(const char *data, int size);

// send ethernet write buffer, returning the packet size sent
MBED_DEPRECATED(
    "Ethernet drivers are provided by a class derived from `EMAC` attached to"
    " either lwIP or Nanostack."
)
int ethernet_send(void);

// receive from ethernet buffer, returning packet size, or 0 if no packet
MBED_DEPRECATED(
    "Ethernet drivers are provided by a class derived from `EMAC` attached to"
    " either lwIP or Nanostack."
)
int ethernet_receive(void);

// read size bytes in to data, return actual num bytes read (0..size)
// if data == NULL, throw the bytes away
MBED_DEPRECATED(
    "Ethernet drivers are provided by a class derived from `EMAC` attached to"
    " either lwIP or Nanostack."
)
int ethernet_read(char *data, int size);

// get the ethernet address
MBED_DEPRECATED(
    "Ethernet drivers are provided by a class derived from `EMAC` attached to"
    " either lwIP or Nanostack."
)
void ethernet_address(char *mac);

// see if the link is up
MBED_DEPRECATED(
    "Ethernet drivers are provided by a class derived from `EMAC` attached to"
    " either lwIP or Nanostack."
)
int ethernet_link(void);

// force link settings
MBED_DEPRECATED(
    "Ethernet drivers are provided by a class derived from `EMAC` attached to"
    " either lwIP or Nanostack."
)
void ethernet_set_link(int speed, int duplex);

#ifdef __cplusplus
}
#endif

#endif

#endif


/** @}*/
