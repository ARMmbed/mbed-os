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

#define CRC16
#include "crc.h"
#include "fsl_clock_config.h"

// called before main
void mbed_sdk_init()
{
    BOARD_BootClockRUN();
}

// Change the NMI pin to an input. This allows NMI pin to
//  be used as a low power mode wakeup.  The application will
//  need to change the pin back to NMI_b or wakeup only occurs once!
void NMI_Handler(void)
{
    gpio_t gpio;
    gpio_init_in(&gpio, PTA4);
}

// Enable the RTC oscillator if available on the board
void rtc_setup_oscillator(RTC_Type *base)
{
    /* Enable the RTC oscillator */
    RTC->CR |= RTC_CR_OSCE_MASK;
}

// Provide ethernet devices with a semi-unique MAC address from the UUID
void mbed_mac_address(char *mac)
{
    uint16_t MAC[3];                        // 3 16 bits words for the MAC

    // get UID via SIM_UID macros defined in the K64F MCU CMSIS header file
    uint32_t UID[4];
    UID[0] = SIM->UIDH;
    UID[1] = SIM->UIDMH;
    UID[2] = SIM->UIDML;
    UID[3] = SIM->UIDL;

    // generate three CRC16's using different slices of the UUID
    MAC[0] = crcSlow((const uint8_t *)UID, 8);  // most significant half-word
    MAC[1] = crcSlow((const uint8_t *)UID, 12); 
    MAC[2] = crcSlow((const uint8_t *)UID, 16); // least significant half word

    // The network stack expects an array of 6 bytes
    // so we copy, and shift and copy from the half-word array to the byte array
    mac[0] = MAC[0] >> 8;
    mac[1] = MAC[0];
    mac[2] = MAC[1] >> 8;
    mac[3] = MAC[1];
    mac[4] = MAC[2] >> 8;
    mac[5] = MAC[2];

    // We want to force bits [1:0] of the most significant byte [0]
    // to be "10"
    // http://en.wikipedia.org/wiki/MAC_address

    mac[0] |= 0x02; // force bit 1 to a "1" = "Locally Administered"
    mac[0] &= 0xFE; // force bit 0 to a "0" = Unicast

}



