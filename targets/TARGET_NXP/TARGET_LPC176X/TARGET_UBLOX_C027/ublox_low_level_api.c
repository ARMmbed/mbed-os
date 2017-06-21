/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#include <stdbool.h>
#include "hal/us_ticker_api.h"
#include "platform/mbed_wait_api.h"
#include "gpio_api.h"
#include "ublox_low_level_api.h"

static bool modemOn;
static bool gpsOn;

void ublox_mdm_init(void)
{
    gpio_t gpio;
    // start with modem disabled 
    gpio_init_out_ex(&gpio, MDMEN,     0);
    gpio_init_out_ex(&gpio, MDMRST,    1);
    gpio_init_out_ex(&gpio, MDMPWRON,  1);
    gpio_init_out_ex(&gpio, MDMLVLOE,  1); // LVLEN:  1=disabled
    gpio_init_out_ex(&gpio, MDMILVLOE, 0); // ILVLEN: 0=disabled
    gpio_init_out_ex(&gpio, MDMUSBDET, 0);
    gpio_init_out_ex(&gpio, MDMRTS,    0);
    // start with gps disabled 
    gpio_init_out_ex(&gpio, GPSEN,     0);
    gpio_init_out_ex(&gpio, GPSRST,    1);
    // led should be off
    gpio_init_out_ex(&gpio, LED,       0);
    
    // Can't use wait_ms() as RTOS isn't initialised yet
    //wait_ms(50); // when USB cable is inserted the interface chip issues
    // Here's the code from the non-RTOS version
    uint32_t start = us_ticker_read();
    while ((us_ticker_read() - start) < 50000);
}

// For forwards compatibility
void ublox_mdm_power_on(int usb)
{
    ublox_mdm_powerOn(usb);
}

void ublox_mdm_powerOn(int usb)
{
    gpio_t gpio;
    // turn on the mode by enabling power with power on pin low and correct USB detect level
    gpio_init_out_ex(&gpio, MDMUSBDET,  usb ? 1 : 0);  // USBDET: 0=disabled, 1=enabled
    if (!modemOn) { // enable modem
        gpio_init_out_ex(&gpio, MDMEN, 1);        // LDOEN:  1=on
        wait_ms(1);                   // wait until supply switched off
        // now we can safely enable the level shifters
        gpio_init_out_ex(&gpio, MDMLVLOE, 0);      // LVLEN:  0=enabled (uart/gpio)
        if (gpsOn)
            gpio_init_out_ex(&gpio, MDMILVLOE, 1); // ILVLEN: 1=enabled (i2c)
    }
}

// For forwards compatibility
void ublox_mdm_power_off()
{
    ublox_mdm_powerOff();
}

void ublox_mdm_powerOff(void)
{
    gpio_t gpio;
    if (modemOn) {
        // diable all level shifters
        gpio_init_out_ex(&gpio, MDMILVLOE, 0);  // ILVLEN: 0=disabled (i2c)
        gpio_init_out_ex(&gpio, MDMLVLOE, 1);   // LVLEN:  1=disabled (uart/gpio)
        gpio_init_out_ex(&gpio,MDMUSBDET, 0);  // USBDET: 0=disabled
        // now we can savely switch off the ldo
        gpio_init_out_ex(&gpio, MDMEN, 0);      // LDOEN:  0=off
        modemOn = false;
    }
}        

void ublox_gps_powerOn(void)
{
    gpio_t gpio;
    if (!gpsOn) {
        // switch on power supply
        gpio_init_out_ex(&gpio, GPSEN, 1);          // LDOEN: 1=on
        wait_ms(1);                     // wait until supply switched off
        if (modemOn)
            gpio_init_out_ex(&gpio, MDMILVLOE, 1);  // ILVLEN: 1=enabled (i2c)
    }
}

void ublox_gps_powerOff(void)
{
    gpio_t gpio;
    if (gpsOn) {
        gpio_init_out_ex(&gpio, MDMILVLOE, 0);   // ILVLEN: 0=disabled (i2c)
        gpio_init_out_ex(&gpio, GPSEN, 0);       // LDOEN: 0=off
    }
}
