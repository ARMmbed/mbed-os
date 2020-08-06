/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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

#include "mbed_error.h"
#include "static_pinmap.h"


/*
 * This file provides default definitions of xxx_direct() functions for peripherals.
 * in all cases standard init function is called, which won't give any ROM memory savings.
 *
 */

#if DEVICE_SPI
MBED_WEAK void spi_init_direct(spi_t *obj, const spi_pinmap_t *pinmap)
{
    spi_init(obj, pinmap->mosi_pin, pinmap->miso_pin, pinmap->sclk_pin, pinmap->ssel_pin);
}
#endif

#if DEVICE_PWMOUT
MBED_WEAK void pwmout_init_direct(pwmout_t *obj, const PinMap *pinmap)
{
    pwmout_init(obj, pinmap->pin);
}
#endif

#if DEVICE_ANALOGIN
MBED_WEAK void analogin_init_direct(analogin_t *obj, const PinMap *pinmap)
{
    analogin_init(obj, pinmap->pin);
}
#endif

#if DEVICE_ANALOGOUT
MBED_WEAK void analogout_init_direct(dac_t *obj, const PinMap *pinmap)
{
    analogout_init(obj, pinmap->pin);
}
#endif

#if DEVICE_I2C
MBED_WEAK void i2c_init_direct(i2c_t *obj, const i2c_pinmap_t *pinmap)
{
    i2c_init(obj, pinmap->sda_pin, pinmap->scl_pin);
}
#endif

#if DEVICE_SERIAL
MBED_WEAK void serial_init_direct(serial_t *obj, const serial_pinmap_t *pinmap)
{
    serial_init(obj, pinmap->tx_pin, pinmap->rx_pin);
}

#if DEVICE_SERIAL_FC
MBED_WEAK void serial_set_flow_control_direct(serial_t *obj, FlowControl type, const serial_fc_pinmap_t *pinmap)
{
    serial_set_flow_control(obj, type, pinmap->rx_flow_pin, pinmap->tx_flow_pin);
}
#endif

#if DEVICE_CAN
MBED_WEAK void can_init_freq_direct(can_t *obj, const can_pinmap_t *pinmap, int hz)
{
    can_init_freq(obj, pinmap->rd_pin, pinmap->td_pin, hz);
}

MBED_WEAK void can_init_direct(can_t *obj, const can_pinmap_t *pinmap)
{
    can_init(obj, pinmap->rd_pin, pinmap->td_pin);
}

#endif

#if DEVICE_QSPI
MBED_WEAK qspi_status_t qspi_init_direct(qspi_t *obj, const qspi_pinmap_t *pinmap, uint32_t hz, uint8_t mode)
{
    return qspi_init(obj, pinmap->data0_pin, pinmap->data1_pin, pinmap->data2_pin, pinmap->data3_pin, pinmap->sclk_pin, pinmap->ssel_pin, hz, mode);
}
#endif

#endif
