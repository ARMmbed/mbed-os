/* mbed Microcontroller Library

 * Copyright (c) 2006-2018 Arm Limited
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

#include "platform_devices.h"
#include "smsc9220_eth_drv.h"

/* Provide ethernet devices with a semi-unique MAC address from the UUID */
void mbed_mac_address(char *mac)
{
    smsc9220_read_mac_address(&SMSC9220_ETH_DEV, mac);
}

