/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
 * Copyright (c) 2023 Geehy Semiconductor Inc.
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
#include "mbed_error.h"

#if DEVICE_SPI
#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"

#define SPI_S(obj)    (( struct spi_s *)(obj))

/*!
 * @brief       Get the frequency of SPI clock source
 *              Configures the pins used by SPI, sets a default format and frequency, and enables the peripheral
 *
 * @param       spi_freq  The SPI clock source freguency
 *
 * @retval      obj  The SPI object
 */
static int spi_clock_source_frequency_get(spi_t *obj)
{
    uint32_t spi_freq;
    struct spi_s *spiobj = SPI_S(obj);

    switch (obj->spi)
    {
        case SPI_1:
            RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_SPI1);
            RCM_ReadPCLKFreq(0,&spi_freq);
            break;
        case SPI_2:
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_SPI2);
            RCM_ReadPCLKFreq(&spi_freq,0);
            break;
        case SPI_3:
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_SPI3);
            RCM_ReadPCLKFreq(&spi_freq,0);
            break;
        default:
            error("SPI clock source frequency get error");
            break;
    }

    return spi_freq;
}

/*!
 * @brief       Initialize the SPI structure
 *
 * @param       obj  The SPI object to initialize
 *
 * @retval      None
 */
static void spi_struct_init(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_T* _spi = (SPI_T*)(obj->spi);

    SPI_Disable(_spi);

    SPI_Config(_spi,&(obj->spi_struct));

    if (obj->spi_struct.nss != SPI_NSS_SOFT)
    {
        if (spiobj->spi_struct.mode == SPI_MODE_SLAVE)
        {
            SPI_DisableSSOutput(_spi);
        }
    }
    SPI_Enable(_spi);
}

/*!
 * @brief   Initialize the SPI peripheral
 *          Configures the pins used by SPI, sets a default format and frequency, and enables the peripheral
 *
 * @param   obj    The SPI object to initialize
 *
 * @param   mosi   The pin to use for MOSI
 *
 * @param   miso   The pin to use for MISO
 *
 * @param   sclk   The pin to use for SCLK
 *
 * @param   ssel   The pin to use for SSEL
 *
 * @retval  None
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

    switch (spiobj->spi)
    {
        case SPI_1:
            RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_SPI1);
            spiobj->spi_irq = SPI1_IRQn;
            break;
        case SPI_2:
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_SPI2);
            spiobj->spi_irq = SPI2_IRQn;
            break;
        case SPI_3:
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_SPI3);
            spiobj->spi_irq = SPI3_IRQn;
            break;
        default:
            error("SPI must be SPI1 ,SPI2 or SPI3");
    }

    pinmap_pinout(mosi,PinMap_SPI_MOSI);
    pinmap_pinout(miso,PinMap_SPI_MISO);
    pinmap_pinout(sclk,PinMap_SPI_SCLK);

    spiobj->pin_miso = miso;
    spiobj->pin_mosi = mosi;
    spiobj->pin_sclk = sclk;
    spiobj->pin_ssel = ssel;

    SPI_T *_spi = (SPI_T *)(spiobj->spi);

    spiobj->spi_struct.baudrateDiv = SPI_BAUDRATE_DIV_256;
    spiobj->spi_struct.mode = SPI_MODE_MASTER;
    spiobj->spi_struct.direction = SPI_DIRECTION_2LINES_FULLDUPLEX;
    spiobj->spi_struct.length = SPI_DATA_LENGTH_8B;
    spiobj->spi_struct.phase = SPI_CLKPHA_1EDGE;
    spiobj->spi_struct.firstBit = SPI_FIRSTBIT_MSB;
    spiobj->spi_struct.polarity = SPI_CLKPOL_LOW;
    spiobj->spi_struct.crcPolynomial = 7;
    
    if (ssel != NC)
    {
        pinmap_pinout(ssel,PinMap_SPI_SSEL);
        spiobj->spi_struct.nss = SPI_NSS_HARD;
        SPI_EnableSSOutput(_spi);
    }
    else
    {
        spiobj->spi_struct.nss = SPI_NSS_SOFT;
    }

    spi_struct_init(obj);
}

/*!
 * @brief       spi_free is currently unimplemented
 *              Return the pins owned by the SPI object to their reset state
 *              Disable the SPI peripheral
 *              Disable the SPI clock
 *
 * @param       obj The SPI object to deinitialize
 *
 * @retval      None
 */
void spi_free(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_T *_spi = (SPI_T*)(obj->spi);
    SPI_Disable(_spi);

    switch (obj->spi)
    {
        case SPI_1:
            SPI_I2S_Reset(SPI1);
            RCM_DisableAPB2PeriphClock(RCM_APB2_PERIPH_SPI1);
            break;
        case SPI_2:
            SPI_I2S_Reset(SPI2);
            RCM_DisableAPB1PeriphClock(RCM_APB1_PERIPH_SPI2);
            break;
        case SPI_3:
            SPI_I2S_Reset(SPI3);
            RCM_DisableAPB1PeriphClock(RCM_APB1_PERIPH_SPI3);
            break;
    }

    /* deinit GPIO mode of SPI pins */
    pin_function(spiobj->pin_miso, PIN_MODE_INPUT);
    pin_function(spiobj->pin_mosi, PIN_MODE_INPUT);
    pin_function(spiobj->pin_sclk, PIN_MODE_INPUT);
    if (spiobj->spi_struct.nss != SPI_NSS_SOFT)
    {
        pin_function(spiobj->pin_ssel, PIN_MODE_INPUT);
    }
}

/*!
 * @brief       Configure the SPI format
 *              Set the number of bits per frame, configure clock polarity and phase, shift order and master/slave mode.
 *              The default bit order is MSB.
 *
 * @param       obj   The SPI object to configure
 *
 * @param       bits  The number of bits per frame
 *
 * @param       mode  The SPI mode (clock polarity, phase, and shift direction)
 *
 * @param       slave Zero for master mode or non-zero for slave mode
 *
 * @retval      None
 */
void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_T *_spi = (SPI_T *)(spiobj->spi);
    spiobj->spi_struct.length = (bits == 16) ? SPI_DATA_LENGTH_16B : SPI_DATA_LENGTH_8B;

    switch (mode)
    {
        case 0:
            spiobj->spi_struct.phase = SPI_CLKPHA_1EDGE;
            spiobj->spi_struct.polarity = SPI_CLKPOL_LOW;
            break;
        case 1:
            spiobj->spi_struct.phase = SPI_CLKPHA_2EDGE;
            spiobj->spi_struct.polarity = SPI_CLKPOL_LOW;
            break;
        case 2:
            spiobj->spi_struct.phase = SPI_CLKPHA_1EDGE;
            spiobj->spi_struct.polarity = SPI_CLKPOL_HIGH;
            break;
        default:
            spiobj->spi_struct.phase = SPI_CLKPHA_2EDGE;
            spiobj->spi_struct.polarity = SPI_CLKPOL_HIGH;
            break;
    }

    spiobj->spi_struct.mode = (slave) ? SPI_MODE_SLAVE : SPI_MODE_MASTER;

    spi_struct_init(obj);
}

static const uint16_t baudrate_prescaler_table[] = {SPI_BAUDRATE_DIV_2,
                                                    SPI_BAUDRATE_DIV_4,
                                                    SPI_BAUDRATE_DIV_8,
                                                    SPI_BAUDRATE_DIV_16,
                                                    SPI_BAUDRATE_DIV_32,
                                                    SPI_BAUDRATE_DIV_64,
                                                    SPI_BAUDRATE_DIV_128,
                                                    SPI_BAUDRATE_DIV_256
                                                   };

/*!
 * @brief       Set the SPI baud rate
 *              Actual frequency may differ from the desired frequency due to available dividers and bus clock
 *              Configures the SPI peripheral's baud rate
 *
 * @param       obj The SPI object to configure
 *
 * @param       hz  The baud rate in Hz
 *
 * @retval      None
 */
void spi_frequency(spi_t *obj, int hz)
{
    struct spi_s *spiobj = SPI_S(obj);
    int spi_hz = 0;
    uint8_t prescaler_rank = 0;
    uint8_t last_index = (sizeof(baudrate_prescaler_table) / sizeof(baudrate_prescaler_table[0])) - 1;

    spi_hz = spi_clock_source_frequency_get(obj) / 2;

    /* configure SPI prescaler according to input frequency */
    while ((spi_hz > hz) && (prescaler_rank < last_index))
    {
        spi_hz = spi_hz / 2;
        prescaler_rank++;
    }

    spiobj->spi_struct.baudrateDiv = baudrate_prescaler_table[prescaler_rank];
    spi_struct_init(obj);

}

/*!
 * @brief       Write a block out in master mode and receive a value
 *              The total number of bytes sent and received will be the maximum of
 *              tx_length and rx_length. The bytes written will be padded with the
 *              value 0xff.
 *
 * @param obj        The SPI peripheral to use for sending
 *
 * @param tx_buffer  Pointer to the byte-array of data to write to the device
 *
 * @param tx_length  Number of bytes to write, may be zero
 *
 * @param rx_buffer  Pointer to the byte-array of data to read from the device
 *
 * @param rx_length  Number of bytes to read, may be zero
 *
 * @param write_fill Default data transmitted while performing a read
 *
 * @retval      The number of bytes written and read from the device. This is
 *              maximum of tx_length and rx_length.
 */
int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length, char write_fill)
{
    int total_length = (tx_length > rx_length) ? tx_length : rx_length;

    for (int i = 0; i < total_length; i++)
    {
        char out = (i < tx_length) ? tx_buffer[i] : write_fill;
        char in = spi_master_write(obj, out);
        if (i < rx_length) 
        {
            rx_buffer[i] = in;
        }
    }

    return total_length;
}

/*!
 * @brief       Write a byte out in master mode and receive a value
 *
 * @param       obj   The SPI peripheral to use for sending
 *
 * @param       value The value to send
 *
 * @retval      the value received during send
 */
int spi_master_write(spi_t *obj, int value)
{
    int timeout = 0;
    struct spi_s *spiobj = SPI_S(obj);
    SPI_T *_spi = (SPI_T *)(spiobj->spi);

    /* wait the SPI transmit buffer is empty */
    while ((SPI_I2S_ReadStatusFlag(_spi, SPI_FLAG_TXBE) == RESET) && (timeout++ < 1000));
    if (timeout < 1000)
    {
        SPI_I2S_TxData(_spi, value);
    }
    else
    {
        return -1;
    }

    timeout = 0;
    /* wait the SPI receive buffer is not empty */
    while ((SPI_I2S_ReadStatusFlag(_spi, SPI_FLAG_RXBNE) == RESET) && (timeout++ < 1000));
    if (timeout < 1000)
    {
        return SPI_I2S_RxData(_spi);
    }
    else
    {
        return -1;
    }
}

/*!
 * @brief       Check if a value is available to read
 *
 * @param       obj The SPI peripheral to check
 *
 * @retval      non-zero if a value is available
 */
int spi_slave_receive(spi_t *obj)
{
    int status;
    struct spi_s *spiobj = SPI_S(obj);
    SPI_T *_spi = (SPI_T *)(spiobj->spi);
    /* check whether or not the SPI receive buffer is empty */
    status = ((SPI_I2S_ReadStatusFlag(_spi, SPI_FLAG_RXBNE) != RESET) ? 1 : 0);
    return status;
}

/*!
 * @brief       Get a received value out of the SPI receive buffer in slave mode
 *              Blocks until a value is available
 *
 * @param       obj The SPI peripheral to read
 *
 * @retval      The value received
 */
int spi_slave_read(spi_t *obj)
{
    int count = 0;
    struct spi_s *spiobj = SPI_S(obj);
    SPI_T *_spi = (SPI_T *)(spiobj->spi);
    /* wait the SPI receive buffer is not empty */
    while ((SPI_I2S_ReadStatusFlag(_spi, SPI_FLAG_RXBNE) == RESET) && (count++ < 1000));
    if (count < 1000) 
    {
        return SPI_I2S_RxData(_spi);
    }
    else
    {
        return -1;
    }
}

/*!
 * @brief       Write a value to the SPI peripheral in slave mode
 *              Blocks until the SPI peripheral can be written to
 *
 * @param obj   The SPI peripheral to write
 * @param value The value to write
 *
 * @retval      None
 */
void spi_slave_write(spi_t *obj, int value)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_T *_spi = (SPI_T *)(spiobj->spi);
    /* wait the SPI transmit buffer is empty */
    while (SPI_I2S_ReadStatusFlag(_spi, SPI_FLAG_TXBE) == RESET);
    SPI_I2S_TxData(_spi, value);
}

/*!
 * @brief       Checks if the specified SPI peripheral is in use
 *
 * @param  obj  obj The SPI peripheral to check
 *
 * @retval      non-zero if the peripheral is currently transmitting
 */
int spi_busy(spi_t *obj)
{
    int status;
    struct spi_s *spiobj = SPI_S(obj);
    SPI_T *_spi = (SPI_T *)(spiobj->spi);
    /* check whether or not the SPI is busy */
    status = ((SPI_I2S_ReadStatusFlag(_spi, SPI_FLAG_BSY) != RESET) ? 1 : 0);
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
