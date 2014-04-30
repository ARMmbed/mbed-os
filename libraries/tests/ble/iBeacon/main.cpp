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

#include "mbed.h"
#include "nRF51822n.h"

nRF51822n   nrf;                /* BLE radio driver */

DigitalOut  led1(LED1);
DigitalOut  led2(LED2);
Ticker      flipper;
Serial      pc(USBTX,USBRX);

void tickerCallback(void);

/**************************************************************************/
/*!
    @brief  Program entry point
*/
/**************************************************************************/
int main(void)
{
    *(uint32_t *)0x40000504 = 0xC007FFDF;
    *(uint32_t *)0x40006C18 = 0x00008000;
    
    /* Setup blinky: led1 is toggled in main, led2 is toggled via Ticker */
    led1=1;
    led2=1;
    flipper.attach(&tickerCallback, 1.0);

    /* Initialise the nRF51822 */
    pc.printf("Initialising the nRF51822\n\r");
    nrf.init();
    
    GapAdvertisingParams advParams ( GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED );
    GapAdvertisingData   advData;
    GapAdvertisingData   scanResponse;

    /* Define an iBeacon payload
       --------------------------------------------------------------
       128-Bit UUID = E2 0A 39 F4 73 F5 4B C4 A1 2F 17 D1 AD 07 A9 61
       Major/Minor  = 0000 / 0000
       Tx Power     = C8
    */
    uint8_t iBeaconPayload[25] = { 0x4C, 0x00, 0x02, 0x15, 0xE2, 0x0A, 0x39, 0xF4, 0x73, 0xF5, 0x4B, 0xC4, 0xA1, 0x2F, 0x17, 0xD1, 0xAD, 0x07, 0xA9, 0x61, 0x00, 0x00, 0x00, 0x00, 0xC8 };

    /* Make sure we get a clean start */
    nrf.reset();

    /* iBeacon includes the FLAG and MSD fields */
    advData.addFlags(GapAdvertisingData::BREDR_NOT_SUPPORTED);
    advData.addData(GapAdvertisingData::MANUFACTURER_SPECIFIC_DATA, iBeaconPayload, sizeof(iBeaconPayload));

    /* Start advertising! */
    nrf.getGap().setAdvertisingData(advData, scanResponse);
    nrf.getGap().startAdvertising(advParams);

    /* Do blinky on LED1 while we're waiting for BLE events */
    for (;;)
    {
      led1 = !led1;
      wait(1);
    }
}

/**************************************************************************/
/*!
    @brief  Ticker callback to switch led2 state
*/
/**************************************************************************/
void tickerCallback(void)
{
    led2 = !led2;
}
