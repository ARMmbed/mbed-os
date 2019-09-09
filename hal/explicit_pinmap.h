/* mbed Microcontroller Library
 * Copyright (c) 2018-2019 ARM Limited
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
#ifndef EXPLICIT_PINMAP_H
#define EXPLICIT_PINMAP_H

#include "PinNames.h"
#include "spi_api.h"
#include "pwmout_api.h"
#include "analogin_api.h"
#include "analogout_api.h"
#include "i2c_api.h"
#include "serial_api.h"
#include <mstd_cstddef>

#if EXPLICIT_PINMAP_READY
#include "PeripheralPinMaps.h"

#if DEVICE_PWMOUT
MSTD_CONSTEXPR_FN_14 PinMap get_pwm_pinmap(const PinName pin)
{
    for (const PinMap &pinmap : PINMAP_PWM) {
        if (pinmap.pin == pin) {
            return {pin, pinmap.peripheral, pinmap.function};
        }
    }
    return {NC, NC, NC};
}
#endif // DEVICE_PWMOUT

#if DEVICE_ANALOGIN
MSTD_CONSTEXPR_FN_14 PinMap get_analogin_pinmap(const PinName pin)
{
    for (const PinMap &pinmap : PINMAP_ANALOGIN) {
        if (pinmap.pin == pin) {
            return {pin, pinmap.peripheral, pinmap.function};
        }
    }
    return {NC, NC, NC};
}
#endif // DEVICE_ANALOGIN

#if DEVICE_ANALOGOUT
MSTD_CONSTEXPR_FN_14 PinMap get_analogout_pinmap(const PinName pin)
{
    for (const PinMap &pinmap : PINMAP_ANALOGOUT) {
        if (pinmap.pin == pin) {
            return {pin, pinmap.peripheral, pinmap.function};
        }
    }
    return {NC, NC, NC};
}
#endif // DEVICE_ANALOGOUT

#if DEVICE_I2C
MSTD_CONSTEXPR_FN_14 i2c_pinmap_t get_i2c_pinmap(const PinName sda, const PinName scl)
{
    const PinMap *sda_map = nullptr;
    for (const PinMap &pinmap : PINMAP_I2C_SDA) {
        if (pinmap.pin == sda) {
            sda_map = &pinmap;
            break;
        }
    }

    const PinMap *scl_map = nullptr;
    for (const PinMap &pinmap : PINMAP_I2C_SCL) {
        if (pinmap.pin == scl) {
            scl_map = &pinmap;
            break;
        }
    }

    if (!sda_map || !scl_map || sda_map->peripheral != scl_map->peripheral) {
        return {NC, NC, NC, NC, NC};
    }

    return {sda_map->peripheral, sda_map->pin, sda_map->function, scl_map->pin, scl_map->function};
}
#endif //DEVICE_I2C

#if DEVICE_SERIAL
MSTD_CONSTEXPR_FN_14 serial_pinmap_t get_uart_pinmap(const PinName tx, const PinName rx)
{
    const PinMap *tx_map = nullptr;
    for (const PinMap &pinmap : PINMAP_UART_TX) {
        if (pinmap.pin == tx) {
            tx_map = &pinmap;
            break;
        }
    }

    const PinMap *rx_map = nullptr;
    for (const PinMap &pinmap : PINMAP_UART_RX) {
        if (pinmap.pin == rx) {
            rx_map = &pinmap;
            break;
        }
    }

    if (!tx_map || !rx_map || rx_map->peripheral != tx_map->peripheral) {
        return {NC, NC, NC, NC, NC, false};
    }

    if (tx_map->pin == STDIO_UART_TX && rx_map->pin == STDIO_UART_RX) {
        return {tx_map->peripheral, tx_map->pin, tx_map->function, rx_map->pin, rx_map->function, true};
    } else {
        return {tx_map->peripheral, tx_map->pin, tx_map->function, rx_map->pin, rx_map->function, false};
    }
}

#if DEVICE_SERIAL_FC
MSTD_CONSTEXPR_FN_14 serial_fc_pinmap_t get_uart_fc_pinmap(const PinName rxflow, const PinName txflow)
{
    const PinMap *rts_map = nullptr;
    for (const PinMap &pinmap : PINMAP_UART_RTS) {
        if (pinmap.pin == rxflow) {
            rts_map = &pinmap;
            break;
        }
    }

    const PinMap *cts_map = nullptr;
    for (const PinMap &pinmap : PINMAP_UART_CTS) {
        if (pinmap.pin == txflow) {
            cts_map = &pinmap;
            break;
        }
    }

    if ((!rts_map || !cts_map) || (rts_map->peripheral != cts_map->peripheral)) {
        return {NC, NC, NC, NC, NC};
    }

    return {cts_map->peripheral, cts_map->pin, cts_map->function, rts_map->pin, rts_map->function};
}
#endif // DEVICE_SERIAL_FC
#endif // DEVICE_SERIAL

#if DEVICE_SPI
MSTD_CONSTEXPR_FN_14 spi_pinmap_t get_spi_pinmap(const PinName mosi, const PinName miso, const PinName sclk, const PinName ssel)
{
    const PinMap *mosi_map = nullptr;
    for (const PinMap &pinmap : PINMAP_SPI_MOSI) {
        if (pinmap.pin == mosi) {
            mosi_map = &pinmap;
            break;
        }
    }

    const PinMap *miso_map = nullptr;
    for (const PinMap &pinmap : PINMAP_SPI_MISO) {
        if (pinmap.pin == miso) {
            miso_map = &pinmap;
            break;
        }
    }

    const PinMap *sclk_map = nullptr;
    for (const PinMap &pinmap : PINMAP_SPI_SCLK) {
        if (pinmap.pin == sclk) {
            sclk_map = &pinmap;
            break;
        }
    }

    const PinMap *ssel_map = nullptr;
    for (const PinMap &pinmap : PINMAP_SPI_SSEL) {
        if (pinmap.pin == ssel) {
            ssel_map = &pinmap;
            break;
        }
    }

    if ((!mosi_map || !miso_map || !sclk_map || !ssel_map) ||
        (mosi_map->peripheral != miso_map->peripheral || mosi_map->peripheral != sclk_map->peripheral) ||
        (ssel_map->pin != NC && mosi_map->peripheral != ssel_map->peripheral)) {
        return {NC, NC, NC, NC, NC, NC, NC, NC, NC};
    }

    return {mosi_map->peripheral, mosi_map->pin, mosi_map->function, miso_map->pin, miso_map->function, sclk_map->pin, sclk_map->function, ssel_map->pin, ssel_map->function};
}
#endif // DEVICE_SPI

#else // EXPLICIT_PINMAP_READY

#if DEVICE_PWMOUT
MSTD_CONSTEXPR_FN_14 PinMap get_pwm_pinmap(const PinName pin)
{
    return {pin, NC, NC};
}
#endif // DEVICE_PWMOUT

#if DEVICE_ANALOGIN
MSTD_CONSTEXPR_FN_14 PinMap get_analogin_pinmap(const PinName pin)
{
    return {pin, NC, NC};
}
#endif // DEVICE_ANALOGIN

#if DEVICE_ANALOGOUT
MSTD_CONSTEXPR_FN_14 PinMap get_analogout_pinmap(const PinName pin)
{
    return {pin, NC, NC};
}
#endif // DEVICE_ANALOGOUT

#if DEVICE_I2C
MSTD_CONSTEXPR_FN_14 i2c_pinmap_t get_i2c_pinmap(const PinName sda, const PinName scl)
{
    return {NC, sda, NC, scl, NC};
}
#endif //DEVICE_I2C

#if DEVICE_SERIAL
MSTD_CONSTEXPR_FN_14 serial_pinmap_t get_uart_pinmap(const PinName tx, const PinName rx)
{
    return {NC, tx, NC, rx, NC, false};
}

#if DEVICE_SERIAL_FC
MSTD_CONSTEXPR_FN_14 serial_fc_pinmap_t get_uart_fc_pinmap(const PinName rxflow, const PinName txflow)
{
    return {NC, txflow, NC, rxflow, NC};
}
#endif // DEVICE_SERIAL_FC
#endif // DEVICE_SERIAL

#if DEVICE_SPI
MSTD_CONSTEXPR_FN_14 spi_pinmap_t get_spi_pinmap(const PinName mosi, const PinName miso, const PinName sclk, const PinName ssel)
{
    return {NC, mosi, NC, miso, NC, sclk, NC, ssel, NC};
}
#endif // DEVICE_SERIAL

#endif // EXPLICIT_PINMAP_READY

#endif // EXPLICIT_PINMAP_H
