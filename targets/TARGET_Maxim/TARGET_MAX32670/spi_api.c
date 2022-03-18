/*******************************************************************************
 * Copyright (c) 2022 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */

#include <stdio.h>
#include "mbed_assert.h"
#include "mbed_critical.h"
#include "spi_api.h"       // mbed HAL

#include "mxc_sys.h"
#include "spi_regs.h"
#include "mxc_spi.h"

#include "pinmap.h"
#include "PeripheralPins.h"


static unsigned int g_drv_ssel = 1;

//******************************************************************************
void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    // Make sure pins are pointing to the same SPI instance
    SPIName spi_mosi = (SPIName)pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    SPIName spi_ssel = (SPIName)pinmap_peripheral(ssel, PinMap_SPI_SSEL);

    SPIName spi_data = (SPIName)pinmap_merge(spi_mosi, spi_miso);
    SPIName spi_cntl;

    // Control is SCK and optionaly SS
    if ((SPIName)spi_ssel != (SPIName)NC) {
        g_drv_ssel = 1; // Driver will drive SSEL pin
        spi_cntl = (SPIName)pinmap_merge(spi_sclk, spi_ssel);
    } else {
        g_drv_ssel = 0; // Driver will NOT drive SSEL pin
        spi_cntl = spi_sclk;
    }

    SPIName spi = (SPIName)pinmap_merge(spi_data, spi_cntl);

    MBED_ASSERT((SPIName)spi != (SPIName)NC);

    obj->spi = (mxc_spi_regs_t*)spi;

    int  masterMode   = 1;
    int  quadModeUsed = 0;
    int  numSlaves    = 1;
    int  ssPolarity   = 0;
    unsigned int spi_speed = 1000000;

    MXC_SPI_Init(obj->spi, masterMode, quadModeUsed, numSlaves, ssPolarity, spi_speed, g_drv_ssel);
    MXC_SPI_SetDataSize(obj->spi, 8);
    MXC_SPI_SetWidth(obj->spi, SPI_WIDTH_STANDARD);

    // Configure Slave Slect Pin if defined
    if ((SPIName)spi_ssel != (SPIName)NC) {
        uint32_t function = pinmap_function(ssel, PinMap_SPI_SSEL);
        if (function != (uint32_t)NC) {
            pin_function(ssel, function+1);// index start from 1;
            pin_mode(ssel, PullNone);
        }
    }
}

//******************************************************************************
void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    // Only supports master mode
    MBED_ASSERT(!slave);

    MXC_SPI_SetDataSize(obj->spi, bits);
    MXC_SPI_SetMode(obj->spi, (mxc_spi_mode_t)mode);
}

////******************************************************************************
void spi_frequency(spi_t *obj, int hz)
{
    // Maximum frequency is the PeripheralClock
    MBED_ASSERT((unsigned int)hz <= PeripheralClock);

    MXC_SPI_SetFrequency(obj->spi, (unsigned int)hz);
}

////******************************************************************************
int spi_master_write(spi_t *obj, int value)
{
    mxc_spi_req_t req;
    uint8_t out, in;

    // Prepare req structure for transfer
    out = value;
    //SPI Request
    req.spi = obj->spi;
    req.txData = &out;
    req.rxData = &in;
    req.txLen = 1;
    req.rxLen = 1;
    req.ssIdx = 0;
    req.ssDeassert = 1;
    req.txCnt = 0;
    req.rxCnt = 0;
    req.completeCB = NULL;

    // Start transfer
    MXC_SPI_MasterTransaction(&req);

    return in;
}

////******************************************************************************
int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length, char write_fill)
{
    mxc_spi_req_t req;

    if (!(tx_length | rx_length) ||
        (tx_length < 0) ||
        (rx_length < 0)) {
        return 0;
    }

    req.spi = obj->spi;
    req.ssIdx = 0;
    req.ssDeassert = 1;
    req.txCnt = 0;
    req.rxCnt = 0;
    req.completeCB = NULL;

    MXC_SPI_SetDefaultTXData(obj->spi, write_fill);

    core_util_critical_section_enter();
    if (tx_length == rx_length) {
        req.txData = (uint8_t *)tx_buffer;
        req.txLen  = tx_length;
        req.rxData = (uint8_t *)rx_buffer;
        req.rxLen  = rx_length;

        MXC_SPI_MasterTransaction(&req);
    } else if (tx_length < rx_length) {
        if (tx_length == 0) {
            req.txData = NULL;
            req.txLen  = 0;
            req.rxData = (uint8_t *)rx_buffer;
            req.rxLen  = rx_length;

            MXC_SPI_MasterTransaction(&req);
        } else {
            req.txData = (uint8_t *)tx_buffer;
            req.txLen  = tx_length;
            req.rxData = (uint8_t *)rx_buffer;
            req.rxLen  = tx_length;

            MXC_SPI_MasterTransaction(&req);

            req.txData = NULL;
            req.txLen  = 0;
            req.rxData = (uint8_t *)&rx_buffer[tx_length];
            req.rxLen  = rx_length-tx_length;
            req.txCnt = 0;
            req.rxCnt = 0;

            MXC_SPI_MasterTransaction(&req);
        }
    } else {
        if (rx_length == 0) {
            req.txData = (uint8_t *)tx_buffer;
            req.txLen  = tx_length;
            req.rxData = NULL;
            req.rxLen  = 0;

            MXC_SPI_MasterTransaction(&req);
        } else {
            req.txData = (uint8_t *)tx_buffer;
            req.txLen  = rx_length;
            req.rxData = (uint8_t *)rx_buffer;
            req.rxLen  = rx_length;

            MXC_SPI_MasterTransaction(&req); 

            req.txData = (uint8_t *)&tx_buffer[rx_length];
            req.txLen  = tx_length-rx_length;
            req.rxData = NULL;
            req.rxLen  = 0;
            req.txCnt = 0;
            req.rxCnt = 0;

            MXC_SPI_MasterTransaction(&req);
        }
    }
    core_util_critical_section_exit();

    return tx_length > rx_length ? tx_length : rx_length;
}

//******************************************************************************
int spi_busy(spi_t *obj)
{
    return (((mxc_spi_regs_t *)obj->spi)->stat & MXC_F_SPI_STAT_BUSY);
}

//******************************************************************************
uint8_t spi_get_module(spi_t *obj)
{
    return obj->index;
}

//******************************************************************************
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
