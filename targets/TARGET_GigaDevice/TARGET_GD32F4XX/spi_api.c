/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
 *
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
#include "mbed_assert.h"
#include "mbed_error.h"
#include "spi_api.h"

#if DEVICE_SPI
#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"

#define SPI_S(obj)    (( struct spi_s *)(obj))

/** Get the frequency of SPI clock source
 *
 * Configures the pins used by SPI, sets a default format and frequency, and enables the peripheral
 * @param[out] spi_freq  The SPI clock source freguency
 * @param[in] obj  The SPI object
 */
static int dev_spi_clock_source_frequency_get(spi_t *obj)
{
    int spi_freq = 0;
    struct spi_s *spiobj = SPI_S(obj);

    switch ((int)spiobj->spi) {
        case SPI0:
            /* clock source is APB2 */
            spi_freq = rcu_clock_freq_get(CK_APB2);
            break;
        case SPI1:
            /* clock source is APB1 */
            spi_freq = rcu_clock_freq_get(CK_APB1);
            break;
        case SPI2:
            /* clock source is APB1 */
            spi_freq = rcu_clock_freq_get(CK_APB1);
            break;
        case SPI3:
            /* clock source is APB2 */
            spi_freq = rcu_clock_freq_get(CK_APB2);
            break;
        case SPI4:
            /* clock source is APB2 */
            spi_freq = rcu_clock_freq_get(CK_APB2);
            break;
        case SPI5:
            /* clock source is APB2 */
            spi_freq = rcu_clock_freq_get(CK_APB2);
            break;
        default:
            error("SPI clock source frequency get error");
            break;
    }
    return spi_freq;
}

/** Initialize the SPI structure
 *
 * Configures the pins used by SPI, sets a default format and frequency, and enables the peripheral
 * @param[out] obj  The SPI object to initialize
 */
static void dev_spi_struct_init(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);

    spi_disable(spiobj->spi);
    spi_para_init(spiobj->spi, &obj->spi_struct);
    spi_enable(spiobj->spi);
}

/** Initialize the SPI peripheral
 *
 * Configures the pins used by SPI, sets a default format and frequency, and enables the peripheral
 * @param[out] obj  The SPI object to initialize
 * @param[in]  mosi The pin to use for MOSI
 * @param[in]  miso The pin to use for MISO
 * @param[in]  sclk The pin to use for SCLK
 * @param[in]  ssel The pin to use for SSEL
 */
void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    struct spi_s *spiobj = SPI_S(obj);

    SPIName spi_mosi = (SPIName)pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    SPIName spi_ssel = (SPIName)pinmap_peripheral(ssel, PinMap_SPI_SSEL);

    /* return SPIName according to PinName */
    SPIName spi_data = (SPIName)pinmap_merge(spi_mosi, spi_miso);
    SPIName spi_cntl = (SPIName)pinmap_merge(spi_sclk, spi_ssel);

    spiobj->spi = (SPIName)pinmap_merge(spi_data, spi_cntl);
    MBED_ASSERT(spiobj->spi != (SPIName)NC);

    /* set iqr type */
    if (spiobj->spi == SPI0) {
        rcu_periph_clock_enable(RCU_SPI0);
        spiobj->spi_irq = SPI0_IRQn;
    }
    if (spiobj->spi == SPI1) {
        rcu_periph_clock_enable(RCU_SPI1);
        spiobj->spi_irq = SPI1_IRQn;
    }
    if (spiobj->spi == SPI2) {
        rcu_periph_clock_enable(RCU_SPI2);
        spiobj->spi_irq = SPI2_IRQn;
    }
    if (spiobj->spi == SPI3) {
        rcu_periph_clock_enable(RCU_SPI3);
        spiobj->spi_irq = SPI3_IRQn;
    }
    if (spiobj->spi == SPI4) {
        rcu_periph_clock_enable(RCU_SPI4);
        spiobj->spi_irq = SPI4_IRQn;
    }
    if (spiobj->spi == SPI5) {
        rcu_periph_clock_enable(RCU_SPI5);
        spiobj->spi_irq = SPI5_IRQn;
    }

    /* configure GPIO mode of SPI pins */
    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    spiobj->pin_miso = miso;
    spiobj->pin_mosi = mosi;
    spiobj->pin_sclk = sclk;
    spiobj->pin_ssel = ssel;
    if (ssel != NC) {
        pinmap_pinout(ssel, PinMap_SPI_SSEL);
        spiobj->spi_struct.nss = SPI_NSS_HARD;
        spi_nss_output_enable(spiobj->spi);
    } else {
        spiobj->spi_struct.nss = SPI_NSS_SOFT;
    }

    /* initilize spiobj with default values */
    spiobj->spi_struct.device_mode          = SPI_MASTER;
    spiobj->spi_struct.prescale             = SPI_PSC_256;
    spiobj->spi_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spiobj->spi_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spiobj->spi_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spiobj->spi_struct.endian               = SPI_ENDIAN_MSB;

    dev_spi_struct_init(obj);
}

/** Release a SPI object
 *
 * TODO: spi_free is currently unimplemented
 * This will require reference counting at the C++ level to be safe
 *
 * Return the pins owned by the SPI object to their reset state
 * Disable the SPI peripheral
 * Disable the SPI clock
 * @param[in] obj The SPI object to deinitialize
 */
void spi_free(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);
    spi_disable(spiobj->spi);

    /* disable and deinit SPI */
    if (spiobj->spi == SPI0) {
        spi_i2s_deinit(SPI0);
        rcu_periph_clock_disable(RCU_SPI0);
    }
    if (spiobj->spi == SPI1) {
        spi_i2s_deinit(SPI1);
        rcu_periph_clock_disable(RCU_SPI1);
    }
    if (spiobj->spi == SPI2) {
        spi_i2s_deinit(SPI2);
        rcu_periph_clock_disable(RCU_SPI2);
    }
    if (spiobj->spi == SPI3) {
        spi_i2s_deinit(SPI3);
        rcu_periph_clock_disable(RCU_SPI3);
    }
    if (spiobj->spi == SPI4) {
        spi_i2s_deinit(SPI4);
        rcu_periph_clock_disable(RCU_SPI4);
    }
    if (spiobj->spi == SPI5) {
        spi_i2s_deinit(SPI5);
        rcu_periph_clock_disable(RCU_SPI5);
    }

    /* deinit GPIO mode of SPI pins */
    pin_function(spiobj->pin_miso, GPIO_MODE_INPUT);
    pin_function(spiobj->pin_mosi, GPIO_MODE_INPUT);
    pin_function(spiobj->pin_sclk, GPIO_MODE_INPUT);
    if (spiobj->spi_struct.nss != SPI_NSS_SOFT) {
        pin_function(spiobj->pin_ssel, GPIO_MODE_INPUT);
    }
}

/** Configure the SPI format
 *
 * Set the number of bits per frame, configure clock polarity and phase, shift order and master/slave mode.
 * The default bit order is MSB.
 * @param[in,out] obj   The SPI object to configure
 * @param[in]     bits  The number of bits per frame
 * @param[in]     mode  The SPI mode (clock polarity, phase, and shift direction)
 * @param[in]     slave Zero for master mode or non-zero for slave mode
 */
void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    struct spi_s *spiobj = SPI_S(obj);

    spiobj->spi_struct.frame_size = (bits == 16) ? SPI_FRAMESIZE_16BIT : SPI_FRAMESIZE_8BIT;
    /* configure polarity and phase of SPI */
    switch (mode) {
        case 0:
            spiobj->spi_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
            break;
        case 1:
            spiobj->spi_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_2EDGE;
            break;
        case 2:
            spiobj->spi_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_1EDGE;
            break;
        default:
            spiobj->spi_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
            break;
    }

    if (spiobj->spi_struct.nss != SPI_NSS_SOFT) {
        if (slave) {
            spi_nss_output_disable(spiobj->spi);
        }
    }
    /* select SPI as master or slave */
    spiobj->spi_struct.device_mode = (slave) ? SPI_SLAVE : SPI_MASTER;

    dev_spi_struct_init(obj);
}

static const uint16_t baudrate_prescaler_table[] = {SPI_PSC_2,
                                                    SPI_PSC_4,
                                                    SPI_PSC_8,
                                                    SPI_PSC_16,
                                                    SPI_PSC_32,
                                                    SPI_PSC_64,
                                                    SPI_PSC_128,
                                                    SPI_PSC_256
                                                   };

/** Set the SPI baud rate
 *
 * Actual frequency may differ from the desired frequency due to available dividers and bus clock
 * Configures the SPI peripheral's baud rate
 * @param[in,out] obj The SPI object to configure
 * @param[in]     hz  The baud rate in Hz
 */
void spi_frequency(spi_t *obj, int hz)
{
    struct spi_s *spiobj = SPI_S(obj);
    int spi_hz = 0;
    uint8_t prescaler_rank = 0;
    uint8_t last_index = (sizeof(baudrate_prescaler_table) / sizeof(baudrate_prescaler_table[0])) - 1;

    spi_hz = dev_spi_clock_source_frequency_get(obj) / 2;

    /* configure SPI prescaler according to input frequency */
    while ((spi_hz > hz) && (prescaler_rank < last_index)) {
        spi_hz = spi_hz / 2;
        prescaler_rank++;
    }

    spiobj->spi_struct.prescale = baudrate_prescaler_table[prescaler_rank];
    dev_spi_struct_init(obj);
}

/** Write a block out in master mode and receive a value
 *
 *  The total number of bytes sent and received will be the maximum of
 *  tx_length and rx_length. The bytes written will be padded with the
 *  value 0xff.
 *
 * @param[in] obj        The SPI peripheral to use for sending
 * @param[in] tx_buffer  Pointer to the byte-array of data to write to the device
 * @param[in] tx_length  Number of bytes to write, may be zero
 * @param[in] rx_buffer  Pointer to the byte-array of data to read from the device
 * @param[in] rx_length  Number of bytes to read, may be zero
 * @param[in] write_fill Default data transmitted while performing a read
 * @returns
 *      The number of bytes written and read from the device. This is
 *      maximum of tx_length and rx_length.
 */
int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length, char write_fill)
{
    int total = (tx_length > rx_length) ? tx_length : rx_length;

    for (int i = 0; i < total; i++) {
        char out = (i < tx_length) ? tx_buffer[i] : write_fill;
        char in = spi_master_write(obj, out);
        if (i < rx_length) {
            rx_buffer[i] = in;
        }
    }

    return total;
}

/** Write a byte out in master mode and receive a value
 *
 * @param[in] obj   The SPI peripheral to use for sending
 * @param[in] value The value to send
 * @return Returns the value received during send
 */
int spi_master_write(spi_t *obj, int value)
{
    int count = 0;
    struct spi_s *spiobj = SPI_S(obj);

    /* wait the SPI transmit buffer is empty */
    while ((RESET == spi_i2s_flag_get(spiobj->spi, SPI_FLAG_TBE)) && (count++ < 1000));
    if (count >= 1000) {
        return -1;
    } else {
        spi_i2s_data_transmit(spiobj->spi, value);
    }

    count = 0;
    /* wait the SPI receive buffer is not empty */
    while ((RESET == spi_i2s_flag_get(spiobj->spi, SPI_FLAG_RBNE)) && (count++ < 1000));
    if (count >= 1000) {
        return -1;
    } else {
        return spi_i2s_data_receive(spiobj->spi);
    }
}

/** Check if a value is available to read
 *
 * @param[in] obj The SPI peripheral to check
 * @return non-zero if a value is available
 */
int spi_slave_receive(spi_t *obj)
{
    int status;
    struct spi_s *spiobj = SPI_S(obj);
    /* check whether or not the SPI receive buffer is empty */
    status = ((spi_i2s_flag_get(spiobj->spi, SPI_FLAG_RBNE) != RESET) ? 1 : 0);
    return status;
}

/** Get a received value out of the SPI receive buffer in slave mode
 *
 * Blocks until a value is available
 * @param[in] obj The SPI peripheral to read
 * @return The value received
 */
int spi_slave_read(spi_t *obj)
{
    int count = 0;
    struct spi_s *spiobj = SPI_S(obj);
    /* wait the SPI receive buffer is not empty */
    while ((RESET == spi_i2s_flag_get(spiobj->spi, SPI_FLAG_RBNE)) && (count++ < 1000));
    if (count >= 1000) {
        return -1;
    } else {
        return spi_i2s_data_receive(spiobj->spi);
    }
}

/** Write a value to the SPI peripheral in slave mode
 *
 * Blocks until the SPI peripheral can be written to
 * @param[in] obj   The SPI peripheral to write
 * @param[in] value The value to write
 */
void spi_slave_write(spi_t *obj, int value)
{
    struct spi_s *spiobj = SPI_S(obj);
    /* wait the SPI transmit buffer is empty */
    while (RESET == spi_i2s_flag_get(spiobj->spi, SPI_FLAG_TBE));
    spi_i2s_data_transmit(spiobj->spi, value);
}

/** Checks if the specified SPI peripheral is in use
 *
 * @param[in] obj The SPI peripheral to check
 * @return non-zero if the peripheral is currently transmitting
 */
int spi_busy(spi_t *obj)
{
    int status;
    struct spi_s *spiobj = SPI_S(obj);
    /* check whether or not the SPI is busy */
    status = ((spi_i2s_flag_get(spiobj->spi, SPI_FLAG_TRANS) != RESET) ? 1 : 0);
    return status;
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
    return PinMap_SPI_SCLK;
}

const PinMap *spi_master_cs_pinmap()
{
    return PinMap_SPI_SSEL;
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
    return PinMap_SPI_SCLK;
}

const PinMap *spi_slave_cs_pinmap()
{
    return PinMap_SPI_SSEL;
}

#endif
