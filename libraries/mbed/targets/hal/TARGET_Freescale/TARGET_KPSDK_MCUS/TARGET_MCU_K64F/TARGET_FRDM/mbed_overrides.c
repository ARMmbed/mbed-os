/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "gpio_api.h"

// called before main - implement here if board needs it ortherwise, let
//  the application override this if necessary
//void mbed_sdk_init()
//{
//
//}

// Change the NMI pin to an input. This allows NMI pin to 
//  be used as a low power mode wakeup.  The application will
//  need to change the pin back to NMI_b or wakeup only occurs once!
void NMI_Handler(void)
{
    gpio_t gpio;
    gpio_init_in(&gpio, PTA4);
}

// Provide ethernet devices with a semi-unique MAC address from the UUID
void mbed_mac_address(char *mac)
{
    // Making a random-as-possible MAC from 3 16 bit seeds and load of XOR'ing
    uint16_t MAC[3]; // 3 16 bits words for the MAC
    uint32_t UUID_LOC_BASE = 0x40048054;

    // three random seeds to make a MAC
    MAC[0] = 0x2055;  
    MAC[1] = 0x5c44;
    MAC[2] = 0x79fe;

    // XOR in each halfword of the UUID into each seed 
    for (int i=0;i<3;i++) {
        for (int j=0;j<8;j++) {
            MAC[i] ^= (*(uint32_t *)(UUID_LOC_BASE+(0x2*j)));    
        }
    }

    // we only want bottom 16 bits of word1 (MAC bits 32-47)
    // and bit 9 forced to 1, bit 8 forced to 0
    // Locally administered MAC, reduced conflicts
    // http://en.wikipedia.org/wiki/MAC_address
    // uint32_t word1 = *(uint32_t *)UUID_LOC_WORD1;
    MAC[2] |= 0x0002;
    MAC[2] &= 0xFFFE;
    
    mac[0] = MAC[0];
    mac[1] = MAC[0] >> 8;
    mac[2] = MAC[1];
    mac[3] = MAC[1] >> 8;
    mac[4] = MAC[2];
    mac[5] = MAC[2] >> 8;    
}




