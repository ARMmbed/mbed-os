/*
 * Copyright (c) 2020 SparkFun Electronics
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#if DEVICE_SPI

#include "spi_api.h"
#include "iom_api.h"
#include "PeripheralPins.h"
#include "mbed_assert.h"

#include <string.h>

#define DEFAULT_CLK_FREQ (4000000)
#define DEFAULT_SPI_MODE (AM_HAL_IOM_SPI_MODE_0)

static am_hal_iom_transfer_t xfer = {0};

SPIName spi_get_peripheral_name(PinName mosi, PinName miso, PinName sclk)
{
    uint32_t iom_mosi = pinmap_peripheral(mosi, spi_master_mosi_pinmap());
    uint32_t iom_miso = pinmap_peripheral(miso, spi_master_miso_pinmap());
    uint32_t iom_sclk = pinmap_peripheral(sclk, spi_master_clk_pinmap());

    uint32_t iom;

    if (miso == NC) {
        iom = pinmap_merge(iom_mosi, iom_sclk);
    } else if (mosi == NC) {
        iom = pinmap_merge(iom_miso, iom_sclk);
    } else {
        uint32_t iom_data = pinmap_merge(iom_mosi, iom_miso);
        iom = pinmap_merge(iom_data, iom_sclk);
    }

    if ((int)iom == NC) {
        return IOM_NUM;
    }

    return (SPIName)iom;
}

void spi_get_capabilities(PinName ssel, bool slave, spi_capabilities_t *cap)
{
    MBED_ASSERT(cap);

    SPIName iom_ssel = (SPIName)pinmap_peripheral(ssel, spi_master_cs_pinmap());

    cap->minimum_frequency = 0;
    cap->maximum_frequency = AM_HAL_IOM_MAX_FREQ;
    cap->word_length = 0x00000080;
    cap->slave_delay_between_symbols_ns = 0;
    cap->clk_modes = 0x0F;
    cap->support_slave_mode = (iom_ssel == IOM_ANY) ? true : false;
    cap->hw_cs_handle = false;
    cap->async_mode = false;
    cap->tx_rx_buffers_equal_length = false;
}

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    MBED_ASSERT(obj);

    MBED_ASSERT((int)ssel == NC);

    // iom determination
    SPIName iom = spi_get_peripheral_name(mosi, miso, sclk);
    MBED_ASSERT((int)iom != IOM_NUM);
    MBED_ASSERT((int)iom != IOM_ANY);

    // iom configuration
    obj->spi.iom_obj.iom.inst = (uint32_t)iom;
    obj->spi.iom_obj.iom.cfg.eInterfaceMode = AM_HAL_IOM_SPI_MODE;
    obj->spi.iom_obj.iom.cfg.ui32ClockFreq = DEFAULT_CLK_FREQ;
    obj->spi.iom_obj.iom.cfg.eSpiMode = DEFAULT_SPI_MODE;
    obj->spi.iom_obj.iom.cfg.pNBTxnBuf = NULL;
    obj->spi.iom_obj.iom.cfg.ui32NBTxnBufLength = 0;

    // invariant xfer settings
    xfer.ui32InstrLen = 0;
    xfer.ui32Instr = 0;
    xfer.bContinue = false;
    xfer.ui8RepeatCount = 0;
    xfer.ui8Priority = 1;
    xfer.ui32PauseCondition = 0;
    xfer.ui32StatusSetClr = 0;

    // pin configuration
    pinmap_config(sclk, spi_master_clk_pinmap());
    if ((int)mosi != NC) {
        pinmap_config(mosi, spi_master_mosi_pinmap());
    }
    if ((int)miso != NC) {
        pinmap_config(miso, spi_master_miso_pinmap());
    }
    if ((int)ssel != NC) {
        pinmap_config(ssel, spi_master_cs_pinmap());
    }

    // initialization
    iom_init(&obj->spi.iom_obj);
}

void spi_free(spi_t *obj)
{
    iom_deinit(&obj->spi.iom_obj);
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    MBED_ASSERT(obj);
    obj->spi.iom_obj.iom.cfg.eSpiMode = (am_hal_iom_spi_mode_e)mode;
    iom_init(&obj->spi.iom_obj);
}

void spi_frequency(spi_t *obj, int hz)
{
    MBED_ASSERT(obj);
    obj->spi.iom_obj.iom.cfg.ui32ClockFreq = (uint32_t)hz;
    iom_init(&obj->spi.iom_obj);
}

int spi_master_write(spi_t *obj, int value)
{
    uint32_t rxval = 0;
    spi_master_block_write(obj, (const char *)&value, 1, (char *)&rxval, 1, SPI_FILL_CHAR);
    return rxval;
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length, char write_fill)
{
    MBED_ASSERT(obj);

    int chars_handled = 0;
    uint32_t status = AM_HAL_STATUS_SUCCESS;

    // always perform a duplex xfer
    xfer.eDirection = AM_HAL_IOM_FULLDUPLEX;

    if (tx_length == rx_length) {
        xfer.pui32RxBuffer = (uint32_t *)rx_buffer;
        xfer.pui32TxBuffer = (uint32_t *)tx_buffer;
        xfer.ui32NumBytes = tx_length;
        status = am_hal_iom_spi_blocking_fullduplex(obj->spi.iom_obj.iom.handle, &xfer);
    }

    // handle difference between buffers
    else if (tx_length < rx_length) {
        xfer.pui32RxBuffer = (uint32_t *)rx_buffer;
        xfer.ui32NumBytes = rx_length - tx_length;
        uint8_t fill[xfer.ui32NumBytes];
        memset(fill, write_fill, xfer.ui32NumBytes);
        xfer.pui32TxBuffer = (uint32_t *)&fill;
        status = am_hal_iom_spi_blocking_fullduplex(obj->spi.iom_obj.iom.handle, &xfer);
    }

    else {
        xfer.pui32TxBuffer = (uint32_t *)tx_buffer;
        xfer.ui32NumBytes = tx_length - rx_length;
        uint8_t fill[xfer.ui32NumBytes];
        memset(fill, write_fill, xfer.ui32NumBytes);
        xfer.pui32RxBuffer = (uint32_t *)&fill;
        status = am_hal_iom_spi_blocking_fullduplex(obj->spi.iom_obj.iom.handle, &xfer);
    }
    
    if (AM_HAL_STATUS_SUCCESS != status) {
        return 0;
    }

    chars_handled += xfer.ui32NumBytes;
    return chars_handled;
}

int spi_slave_receive(spi_t *obj)
{
    MBED_ASSERT(0);
    return 0;
}

int spi_slave_read(spi_t *obj)
{
    MBED_ASSERT(0);
    return 0;
}

void spi_slave_write(spi_t *obj, int value)
{
    MBED_ASSERT(0);
}

int spi_busy(spi_t *obj)
{
    MBED_ASSERT(0);
    return 0;
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

#endif // DEVICE_SPI
