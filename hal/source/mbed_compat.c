/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#include "analogin_api.h"
#include "i2c_api.h"
#include "spi_api.h"
#include "gpio_api.h"
#include "reset_reason_api.h"
#include "mbed_toolchain.h"

// To be re-implemented in the target layer if required
MBED_WEAK void gpio_free(gpio_t *obj)
{
    // Do nothing
}

#if DEVICE_I2C
// To be re-implemented in the target layer if required
MBED_WEAK void i2c_free(i2c_t *obj)
{
    // Do nothing
}
#endif

#if DEVICE_ANALOGIN
// To be re-implemented in the target layer if required
MBED_WEAK void analogin_free(analogin_t *obj)
{
    // Do nothing
}
#endif

#if DEVICE_SPI
// Default SPI capabilities. If specific target has different capabilities this function needs to be re-implemented.
MBED_WEAK void spi_get_capabilities(PinName ssel, bool slave, spi_capabilities_t *cap)
{
    if (slave) {
        cap->minimum_frequency = 200000;            // 200 kHz
        cap->maximum_frequency = 2000000;           // 2 MHz
        cap->word_length = 0x00008080;              // 8 and 16 bit symbols
        cap->support_slave_mode = false;            // to be determined later based on ssel
        cap->hw_cs_handle = false;                  // irrelevant in slave mode
        cap->slave_delay_between_symbols_ns = 2500; // 2.5 us
        cap->clk_modes = 0x0f;                      // all clock modes
        cap->tx_rx_buffers_equal_length = true;     // rx buffer size must be equal tx buffer size
#if DEVICE_SPI_ASYNCH
        cap->async_mode = true;
#else
        cap->async_mode = false;
#endif
    } else {
        cap->minimum_frequency = 200000;          // 200 kHz
        cap->maximum_frequency = 2000000;         // 2 MHz
        cap->word_length = 0x00008080;            // 8 and 16 bit symbols
        cap->support_slave_mode = false;          // to be determined later based on ssel
        cap->hw_cs_handle = false;                // to be determined later based on ssel
        cap->slave_delay_between_symbols_ns = 0;  // irrelevant in master mode
        cap->clk_modes = 0x0f;                    // all clock modes
        cap->tx_rx_buffers_equal_length = true;   // rx buffer size must be equal tx buffer size
#if DEVICE_SPI_ASYNCH
        cap->async_mode = true;
#else
        cap->async_mode = false;
#endif
    }

    // check if given ssel pin is in the cs pinmap
    const PinMap *cs_pins = spi_master_cs_pinmap();
    while (cs_pins->pin != NC) {
        if (cs_pins->pin == ssel) {
#if DEVICE_SPISLAVE
            cap->support_slave_mode = true;
#endif
            cap->hw_cs_handle = true;
            break;
        }
        cs_pins++;
    }
}

#endif

#if DEVICE_RESET_REASON
// To be re-implemented in the target layer if required
MBED_WEAK void hal_reset_reason_get_capabilities(reset_reason_capabilities_t *cap)
{
    cap->reasons = (1 << RESET_REASON_PIN_RESET) | (1 << RESET_REASON_SOFTWARE);
#if DEVICE_WATCHDOG
    cap->reasons |= 1 << RESET_REASON_WATCHDOG;
#endif
}
#endif
