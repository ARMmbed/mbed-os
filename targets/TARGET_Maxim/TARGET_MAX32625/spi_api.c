/*******************************************************************************
 * Copyright (c) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
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

#include "mbed_assert.h"
#include "mbed_critical.h"
#include "spi_api.h"        // mbed HAL
#include "spim_regs.h"      // bare metal
#include "spim.h"           // Maxim CMSIS driver
#include "pinmap.h"
#include "PeripheralPins.h"

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
        spi_cntl = (SPIName)pinmap_merge(spi_sclk, spi_ssel);
    } else {
        spi_cntl = spi_sclk;
    }

    SPIName spi = (SPIName)pinmap_merge(spi_data, spi_cntl);

    MBED_ASSERT((SPIName)spi != (SPIName)NC);

    obj->spi = (mxc_spim_regs_t *)spi;

    // Merge pin function requests for use with CMSIS init func
    ioman_req_t io_req;
    pin_function_t *pin_func;
    pin_func = (pin_function_t *)pinmap_find_function(mosi, PinMap_SPI_MOSI);
    io_req.value  = pin_func->req_val;
    pin_func = (pin_function_t *)pinmap_find_function(miso, PinMap_SPI_MISO);
    io_req.value |= pin_func->req_val;
    pin_func = (pin_function_t *)pinmap_find_function(sclk, PinMap_SPI_SCLK);
    io_req.value |= pin_func->req_val;
    if ((SPIName)spi_ssel != (SPIName)NC) {
        pin_func = (pin_function_t *)pinmap_find_function(ssel, PinMap_SPI_SSEL);
        io_req.value |= pin_func->req_val;
    }

    // Using req and ack pointers of last pin function lookup
    sys_cfg_spim_t sys_cfg;
    sys_cfg.io_cfg.req_reg = pin_func->reg_req;
    sys_cfg.io_cfg.ack_reg = pin_func->reg_ack;
    sys_cfg.io_cfg.req_val = io_req;
    sys_cfg.clk_scale = CLKMAN_SCALE_AUTO;

    // Defaults
    spim_cfg_t spim_cfg;
    spim_cfg.mode = 0;
    spim_cfg.ssel_pol = 0;
    spim_cfg.baud = 1000000;

    SPIM_Init(obj->spi, &spim_cfg, &sys_cfg);

    obj->index = MXC_SPIM_GET_IDX(obj->spi);
}

//******************************************************************************
void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    // Check the validity of the inputs
    MBED_ASSERT(bits == 8);

    // Only supports master mode
    MBED_ASSERT(!slave);

    // Set the mode
    obj->spi->mstr_cfg &= ~(MXC_F_SPIM_MSTR_CFG_SPI_MODE);
    obj->spi->mstr_cfg |= (mode << MXC_F_SPIM_MSTR_CFG_SPI_MODE_POS);
}

//******************************************************************************
void spi_frequency(spi_t *obj, int hz)
{
    // Maximum frequency is half the system frequency
    MBED_ASSERT((unsigned int)hz <= (SystemCoreClock / 2));
    unsigned clocks = ((SystemCoreClock / 2) / hz);

    // Figure out the divider ratio
    int clk_div = 1;
    while(clk_div < 10) {
        if(clocks < 0x10) {
            break;
        }
        clk_div++;
        clocks = clocks >> 1;
    }

    // Turn on the SPI clock
    if(obj->index == 0) {
        MXC_CLKMAN->sys_clk_ctrl_11_spi0 = clk_div;
    } else if(obj->index == 1) {
        MXC_CLKMAN->sys_clk_ctrl_12_spi1 = clk_div;
    } else if(obj->index == 2) {
        MXC_CLKMAN->sys_clk_ctrl_13_spi2 = clk_div;
    } else {
        MBED_ASSERT(0);
    }

    // Set the number of clocks to hold sclk high and low
    MXC_SET_FIELD(&obj->spi->mstr_cfg,
                  (MXC_F_SPIM_MSTR_CFG_SCK_HI_CLK | MXC_F_SPIM_MSTR_CFG_SCK_LO_CLK),
                  ((clocks << MXC_F_SPIM_MSTR_CFG_SCK_HI_CLK_POS) | (clocks << MXC_F_SPIM_MSTR_CFG_SCK_LO_CLK_POS)));
}

//******************************************************************************
int spi_master_write(spi_t *obj, int value)
{
    spim_req_t req;
    uint8_t out;
    uint8_t in;

    out = value;

    req.ssel = 0;
    req.deass = 0;
    req.tx_data = &out;
    req.rx_data = &in;
    req.width = SPIM_WIDTH_1;
    req.len = 1;
    req.ssel = 0;
    req.deass = 1;
    req.callback = NULL;

    SPIM_Trans(obj->spi, &req);

    return *req.rx_data;
}

//******************************************************************************
int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length, char write_fill)
{
    spim_req_t req;

    if (!(tx_length | rx_length) ||
        (tx_length < 0) ||
        (rx_length < 0)) {
        return 0;
    }

    req.width = SPIM_WIDTH_1;
    req.ssel = 0;
    req.deass = 1;
    req.callback = NULL;

    core_util_critical_section_enter();
    if (tx_length == rx_length) {
        req.tx_data = (uint8_t *)tx_buffer;
        req.rx_data = (uint8_t *)rx_buffer;
        req.len = tx_length;
        SPIM_Trans(obj->spi, &req);
    } else if (tx_length < rx_length) {
        req.tx_data = (tx_length > 0) ? (uint8_t *)tx_buffer : NULL;
        req.rx_data = (uint8_t *)rx_buffer;
        req.len = (tx_length > 0) ? tx_length : rx_length;
        SPIM_Trans(obj->spi, &req);

        if (tx_length) {
            req.tx_data = NULL;
            req.rx_data = (uint8_t *)(rx_buffer + tx_length);
            req.len = rx_length - tx_length;
            SPIM_Trans(obj->spi, &req);
        }
    } else {
        req.tx_data = (uint8_t *)tx_buffer;
        req.rx_data = (rx_length > 0) ? (uint8_t *)rx_buffer : NULL;
        req.len = (rx_length > 0) ? rx_length : tx_length;
        SPIM_Trans(obj->spi, &req);

        if (rx_length) {
            req.tx_data = (uint8_t *)(tx_buffer + rx_length);
            req.rx_data = NULL;
            req.len = tx_length - rx_length;
            SPIM_Trans(obj->spi, &req);
        }
    }
    core_util_critical_section_exit();

    while (SPIM_Busy(obj->spi));

    return tx_length > rx_length ? tx_length : rx_length;
}

//******************************************************************************
int spi_busy(spi_t *obj)
{
    return SPIM_Busy(obj->spi);
}

//******************************************************************************
uint8_t spi_get_module(spi_t *obj)
{
    return obj->index;
}
