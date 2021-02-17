/***************************************************************************//**
 * @file spi_api.c
 *******************************************************************************
 * @section License
 * <b>Copyright (c) 2021 Microchip Technology Inc. and its subsidiaries.</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include "device.h"
#include "clocking.h"

#if DEVICE_SPI

#include "mbed_assert.h"
#include "PeripheralPins.h"
#include "pinmap.h"
#include "mbed_error.h"

#include "spi_api.h"
#include "spia.h"
#include "pdc.h"
#include "status_codes.h"

void spi_get_capabilities(PinName ssel, bool slave, spi_capabilities_t *cap)
{
    if (slave) {
        cap->minimum_frequency = 200000;            // 200 kHz
        cap->maximum_frequency = 2000000;           // 2 MHz
        cap->word_length = 0x0000FF80;              // from 8 to 16 bit symbols
        cap->support_slave_mode = false;            // to be determined later based on ssel
        cap->hw_cs_handle = false;                  // irrelevant in slave mode
        cap->slave_delay_between_symbols_ns = 2500; // 2.5 us
        cap->clk_modes = 0x0f;                      // all clock modes
        cap->tx_rx_buffers_equal_length = true;     // rx buffer size must be equal tx buffer size
        cap->async_mode = false;
    } else {
        cap->minimum_frequency = 200000;          // 200 kHz
        cap->maximum_frequency = 2000000;         // 2 MHz
        cap->word_length = 0x0000FF80;            // from 8 to 16 bit symbols
        cap->support_slave_mode = false;          // to be determined later based on ssel
        cap->hw_cs_handle = true;                 // to be determined later based on ssel
        cap->slave_delay_between_symbols_ns = 0;  // irrelevant in master mode
        cap->clk_modes = 0x0f;                    // all clock modes
        cap->tx_rx_buffers_equal_length = true;   // rx buffer size must be equal tx buffer size
        cap->async_mode = false;
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

void spi_init_direct(spi_t *obj, const spi_pinmap_t *pinmap)
{
    // TBD!!!!!!!!!!!!!!!!!!!!!
}

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName clk, PinName cs)
{
    uint32_t spi_obj;
    SPIName spi_mosi = (SPIName) pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName) pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_clk = (SPIName) pinmap_peripheral(clk, PinMap_SPI_CLK);
    SPIName spi_cs = (SPIName) pinmap_peripheral(cs, PinMap_SPI_CS);

    /* By default CS0 */
    obj->cs = 0;

    if (cs == NC) {
        SPIName spi_data = (SPIName) pinmap_merge(spi_mosi, spi_miso);
        spi_obj = ((uint32_t)pinmap_merge(spi_clk, spi_data)) << 12;
        /* Master mode */
        obj->is_slave = false;
    } else {
        SPIName spi_data = (SPIName) pinmap_merge(spi_mosi, spi_miso);
        SPIName spi_ctrl = (SPIName) pinmap_merge(spi_clk, spi_cs);
        spi_obj = ((uint32_t)pinmap_merge(spi_clk, spi_ctrl)) << 12;

        /* Detect CS0 or CS1 */
        spi_cs = (SPIName) pinmap_peripheral(cs, PinMap_SPI_CS0);
        if (spi_cs == (SPIName)NC) {
            spi_cs = (SPIName) pinmap_peripheral(cs, PinMap_SPI_CS1);
            if (spi_cs == (SPIName)NC) {
                /* Select CS0 By default */
                obj->cs = 0;
            } else {
                /* Select CS0 */
                obj->cs = 1;
            }
        } else {
            /* Select CS0 */
            obj->cs = 0;
        }
        /* Slave mode */
        obj->is_slave = true;
    }

    spi_obj |= 0x40000400;
    obj->spi = (Spi *)spi_obj;

    MBED_ASSERT((unsigned int) obj->spi != NC);

    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(clk, PinMap_SPI_CLK);

    if (cs != NC) {
        pinmap_pinout(cs, PinMap_SPI_CS);
    }

    /* Enable the peripheral and set SPI mode. */
    if (obj->spi == SPI0) {
		sysclk_enable_peripheral_clock(ID_FLEXCOM0);
        flexcom_enable(FLEXCOM0);
        flexcom_set_opmode(FLEXCOM0, FLEXCOM_SPI);
	} else if (obj->spi == SPI2) {
		sysclk_enable_peripheral_clock(ID_FLEXCOM2);
        flexcom_enable(FLEXCOM2);
        flexcom_set_opmode(FLEXCOM2, FLEXCOM_SPI);
	} else if (obj->spi == SPI3) {
		sysclk_enable_peripheral_clock(ID_FLEXCOM3);
        flexcom_enable(FLEXCOM3);
        flexcom_set_opmode(FLEXCOM3, FLEXCOM_SPI);
	} else if (obj->spi == SPI4) {
		sysclk_enable_peripheral_clock(ID_FLEXCOM4);
        flexcom_enable(FLEXCOM4);
        flexcom_set_opmode(FLEXCOM4, FLEXCOM_SPI);
	} else if (obj->spi == SPI5) {
		sysclk_enable_peripheral_clock(ID_FLEXCOM5);
        flexcom_enable(FLEXCOM5);
        flexcom_set_opmode(FLEXCOM5, FLEXCOM_SPI);
	} else if (obj->spi == SPI6) {
		sysclk_enable_peripheral_clock(ID_FLEXCOM6);
        flexcom_enable(FLEXCOM6);
        flexcom_set_opmode(FLEXCOM6, FLEXCOM_SPI);
	} else if (obj->spi == SPI7) {
		sysclk_enable_peripheral_clock(ID_FLEXCOM7);
        flexcom_enable(FLEXCOM7);
        flexcom_set_opmode(FLEXCOM7, FLEXCOM_SPI);
	} else {
		// Do Nothing
	}
}

void spi_free(spi_t *obj)
{
    spi_reset(obj->spi);
    spi_disable(obj->spi);

    /* Disable the peripheral clock. */
    if (obj->spi == SPI0) {
		sysclk_disable_peripheral_clock(ID_FLEXCOM0);
	} else if (obj->spi == SPI2) {
		sysclk_disable_peripheral_clock(ID_FLEXCOM2);
	} else if (obj->spi == SPI3) {
		sysclk_disable_peripheral_clock(ID_FLEXCOM3);
	} else if (obj->spi == SPI4) {
		sysclk_disable_peripheral_clock(ID_FLEXCOM4);
	} else if (obj->spi == SPI5) {
		sysclk_disable_peripheral_clock(ID_FLEXCOM5);
	} else if (obj->spi == SPI6) {
		sysclk_disable_peripheral_clock(ID_FLEXCOM6);
	} else if (obj->spi == SPI7) {
		sysclk_disable_peripheral_clock(ID_FLEXCOM7);
	} else {
		// Do Nothing
	}
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
	/* Enable SPI peripheral. */
	spi_enable_clock(obj->spi);

	/* Reset SPI */
	spi_disable(obj->spi);
	spi_reset(obj->spi);

	/* Configure SPI by default */
	spi_disable_mode_fault_detect(obj->spi);
	spi_set_fixed_peripheral_select(obj->spi);
	spi_set_transfer_delay(obj->spi, obj->cs, 1, 0);
    spi_configure_cs_behavior(obj->spi, obj->cs, SPI_CS_RISE_NO_TX);

    if (obj->cs != (uint8_t)NC) {
        spi_set_peripheral_chip_select_value(obj->spi, spi_get_pcs(obj->cs));
    } else {
        /* Select CS0 by default */
        spi_set_peripheral_chip_select_value(obj->spi, spi_get_pcs(0));
    }

    /* Bits: values between 8 and 16 are valid */
    MBED_ASSERT(bits >= 8 && bits <= 16);
	spi_set_bits_per_transfer(obj->spi, obj->cs, SPI_CSR_BITS(bits - 8));

    MBED_ASSERT(mode >= 0 && mode <= 3);
    switch (mode) {
        case 0:
            spi_set_clock_polarity(obj->spi, obj->cs, 0);
            spi_set_clock_phase(obj->spi, obj->cs, 1);
            break;
        case 1:
            spi_set_clock_polarity(obj->spi, obj->cs, 0);
            spi_set_clock_phase(obj->spi, obj->cs, 0);
            break;
        case 2:
            spi_set_clock_polarity(obj->spi, obj->cs, 1);
            spi_set_clock_phase(obj->spi, obj->cs, 1);
            break;
        case 3:
            spi_set_clock_polarity(obj->spi, obj->cs, 1);
            spi_set_clock_phase(obj->spi, obj->cs, 0);
            break;
        default:
            spi_set_clock_polarity(obj->spi, obj->cs, 0);
            spi_set_clock_phase(obj->spi, obj->cs, 1);
    }

    if (slave) {
        /* Configure Slave Mode */
        spi_set_slave_mode(obj->spi);
        obj->is_slave = true;
    } else {
        /* Configure Master Mode */
        spi_set_master_mode(obj->spi);
        obj->is_slave = false;
    }

    spi_enable(obj->spi);
}

void spi_frequency(spi_t *obj, int hz)
{
    uint32_t ul_cpuhz;
	uint8_t uc_div;

    spi_disable(obj->spi);

    /* Get DIV */
    ul_cpuhz = sysclk_get_peripheral_hz();
	uc_div = ul_cpuhz / hz;

	if (ul_cpuhz % hz) {
		uc_div++;
	}

	spi_set_baudrate_div(obj->spi, obj->cs, uc_div);

    spi_enable(obj->spi);
}

int spi_master_write(spi_t *obj, int value)
{
	uint32_t timeout = SPI_TIMEOUT;
    int data;

    timeout = SPI_TIMEOUT;
    while (!spi_is_tx_ready(obj->spi)) {
        if (!timeout--) {
            return 0;
        }
    }

    spi_put(obj->spi, (uint16_t)value);

    timeout = SPI_TIMEOUT;
    while (!spi_is_rx_ready(obj->spi)) {
        if (!timeout--) {
            return 0;
        }
    }

    data = (int)spi_get(obj->spi);

	return data;
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length, char write_fill)
{
	uint32_t timeout = SPI_TIMEOUT;
	uint16_t val;
    int total = (tx_length > rx_length) ? tx_length : rx_length;

	for (int i = 0; i < total; i++) {
        val = (i < tx_length) ? (uint16_t)tx_buffer[i] : (uint16_t)write_fill;

		timeout = SPI_TIMEOUT;
		while (!spi_is_tx_ready(obj->spi)) {
			if (!timeout--) {
				return ERR_TIME_OUT;
			}
		}
		spi_put(obj->spi, val);

        if (i < rx_length) {
            timeout = SPI_TIMEOUT;
            while (!spi_is_rx_ready(obj->spi)) {
                if (!timeout--) {
                    return ERR_TIME_OUT;
                }
            }

            rx_buffer[i] = (char)spi_get(obj->spi);
        }
    }

    return total;
}

int spi_slave_receive(spi_t *obj)
{
    return (int)spi_is_rx_full(obj->spi);
}

int spi_slave_read(spi_t *obj)
{
    return (int)spi_get(obj->spi);
}

void spi_slave_write(spi_t *obj, int value)
{
    spi_put(obj->spi, (uint16_t)value);
}

int spi_busy(spi_t *obj)
{
    if (spi_is_tx_empty(obj->spi)) {
        return 0;
    } else {
        return 1;
    }
}

uint8_t spi_get_module(spi_t *obj)
{
    return (uint8_t)((uint32_t)obj->spi >> 12);
}

const PinMap *spi_master_mosi_pinmap()
{
    return PinMap_SPI_MOSI;
}

const PinMap *spi_master_miso_pinmap()
{
    return PinMap_SPI_MISO;
}

const PinMap *spi_master_clk_pinmap()
{
    return PinMap_SPI_CLK;
}

const PinMap *spi_master_cs_pinmap()
{
    return PinMap_SPI_CS;
}

const PinMap *spi_slave_mosi_pinmap()
{
    return PinMap_SPI_MOSI;
}

const PinMap *spi_slave_miso_pinmap()
{
    return PinMap_SPI_MISO;
}

const PinMap *spi_slave_clk_pinmap()
{
    return PinMap_SPI_CLK;
}

const PinMap *spi_slave_cs_pinmap()
{
    return PinMap_SPI_CS;
}

#endif
