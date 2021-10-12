/*******************************************************************************
 * Copyright (c) Maxim Integrated Products, Inc., All Rights Reserved.
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
#include "spimss_regs.h"
#include "spimss.h"
#include "mxc_spi.h"

#include "pinmap.h"
#include "PeripheralPins.h"


#define SPI_NUM_0   0 // SPI17Y
#define SPI_NUM_1A  1 // SPIMSS MAPA
#define SPI_NUM_1B  2 // SPIMSS MAPB
#define SPI_NUM_MAX 3 // 

static unsigned int spi_speed = 1000000;
static unsigned int spi_mode = 0;
static unsigned int g_drv_ssel = 1;

static int master_send(int spi_num, void *req)
{
    int error = E_NO_ERROR;

    if (spi_num == SPI_NUM_0) {
        error = MXC_SPI_MasterTransaction((mxc_spi_req_t *)req);
    } else if (spi_num < SPI_NUM_MAX) {
        mxc_spimss_req_t spimss_req;
        mxc_spi_req_t *req_tmp = (mxc_spi_req_t *)req;

        spimss_req.ssel     = req_tmp->ssIdx;       /**< Not Used*/
        spimss_req.deass    = req_tmp->ssDeassert;  /**< Not Used*/
        spimss_req.tx_data  = req_tmp->txData;      /**< Pointer to a buffer to transmit data from. NULL if undesired. */
        spimss_req.rx_data  = req_tmp->rxData;      /**< Pointer to a buffer to store data received. NULL if undesired.*/
        spimss_req.width    = DUMMY_1;          /**< Not Used */
        /**< Number of transfer units to send from the \p tx_data buffer. */
        spimss_req.len      = (req_tmp->txLen > req_tmp->rxLen)? req_tmp->txLen: req_tmp->rxLen;
        spimss_req.bits     = 8;                /**< Number of bits in transfer unit (e.g. 8 for byte, 16 for short) */
        spimss_req.rx_num   = req_tmp->rxCnt;       /**< Number of bytes actually read into the \p rx_data buffer. */
        spimss_req.tx_num   = req_tmp->txCnt;       /**< Number of bytes actually sent from the \p tx_data buffer */
        spimss_req.callback = NULL;             /**< Callback function if desired, NULL otherwise */

        error = MXC_SPIMSS_MasterTrans(MXC_SPIMSS, &spimss_req);
    } else {
        return E_BAD_PARAM;
    }

    return error;
}

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

    if (spi == SPI_0) {
    	obj->spi = MXC_SPI0;
        obj->index = SPI_NUM_0;

        MXC_SPI_Init(obj->spi, 1, 0, 1, 0, spi_speed, g_drv_ssel);
        MXC_SPI_SetDataSize(obj->spi, 8);
        MXC_SPI_SetWidth(obj->spi, SPI_WIDTH_STANDARD);
    }
    else if(spi == SPI_1) {
    	obj->spi = (void *)MXC_SPIMSS;
        
        if (pinmap_function(mosi, PinMap_SPI_MOSI) == 1) {
            obj->index = SPI_NUM_1A;
            MXC_SPIMSS_Init(obj->spi, spi_mode, spi_speed, MAP_A, g_drv_ssel);
        } else {
            obj->index = SPI_NUM_1B;
            MXC_SPIMSS_Init(obj->spi, spi_mode, spi_speed, MAP_B, g_drv_ssel);
        }
    }

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

    spi_mode = mode;

    if (obj->index == SPI_NUM_0) {
        MXC_SPI_SetDataSize(obj->spi, bits);
        MXC_SPI_SetMode(obj->spi, (mxc_spi_mode_t)mode);
    }
    else if (obj->index < SPI_NUM_MAX) {
        MXC_SPIMSS_Shutdown(obj->spi);
        if (obj->index == SPI_NUM_1A) {
            MXC_SPIMSS_Init(obj->spi, spi_mode, spi_speed, MAP_A, g_drv_ssel);
        } else {
            MXC_SPIMSS_Init(obj->spi, spi_mode, spi_speed, MAP_B, g_drv_ssel);
        }
    }
}

////******************************************************************************
void spi_frequency(spi_t *obj, int hz)
{
    // Maximum frequency is the PeripheralClock
    MBED_ASSERT((unsigned int)hz <= PeripheralClock);
 
    if (obj->index == SPI_NUM_0) {
        spi_speed = hz;
        MXC_SPI_SetFrequency(obj->spi, spi_speed);
    }
    else if (obj->index < SPI_NUM_MAX) {
        spi_speed = hz;
        MXC_SPIMSS_Shutdown(obj->spi);
        if (obj->index == SPI_NUM_1A) {
            MXC_SPIMSS_Init(obj->spi, spi_mode, spi_speed, MAP_A, g_drv_ssel);
        } else {
            MXC_SPIMSS_Init(obj->spi, spi_mode, spi_speed, MAP_B, g_drv_ssel);
        }
    }
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
    master_send(obj->index, &req);

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

    if (obj->index == SPI_NUM_0) {
        MXC_SPI_SetDefaultTXData(obj->spi, write_fill);
    } else {
        MXC_SPIMSS_SetDefaultTXData(obj->spi, write_fill);
    }

    core_util_critical_section_enter();
    if (tx_length == rx_length) {
        req.txData = (uint8_t *)tx_buffer;
        req.txLen  = tx_length;
        req.rxData = (uint8_t *)rx_buffer;
        req.rxLen  = rx_length;

        master_send(obj->index, &req);
    } else if (tx_length < rx_length) {
        if (tx_length == 0) {
            req.txData = NULL;
            req.txLen  = 0;
            req.rxData = (uint8_t *)rx_buffer;
            req.rxLen  = rx_length;

            master_send(obj->index, &req);  
        } else {
            req.txData = (uint8_t *)tx_buffer;
            req.txLen  = tx_length;
            req.rxData = (uint8_t *)rx_buffer;
            req.rxLen  = tx_length;

            master_send(obj->index, &req); 

            req.txData = NULL;
            req.txLen  = 0;
            req.rxData = (uint8_t *)&rx_buffer[tx_length];
            req.rxLen  = rx_length-tx_length;
            req.txCnt = 0;
            req.rxCnt = 0;

            master_send(obj->index, &req); 
        }
    } else {
        if (rx_length == 0) {
            req.txData = (uint8_t *)tx_buffer;
            req.txLen  = tx_length;
            req.rxData = NULL;
            req.rxLen  = 0;

            master_send(obj->index, &req);  
        } else {
            req.txData = (uint8_t *)tx_buffer;
            req.txLen  = rx_length;
            req.rxData = (uint8_t *)rx_buffer;
            req.rxLen  = rx_length;

            master_send(obj->index, &req); 

            req.txData = (uint8_t *)&tx_buffer[rx_length];
            req.txLen  = tx_length-rx_length;
            req.rxData = NULL;
            req.rxLen  = 0;
            req.txCnt = 0;
            req.rxCnt = 0;

            master_send(obj->index, &req); 
        }
    }
    core_util_critical_section_exit();

    return tx_length > rx_length ? tx_length : rx_length;
}

//******************************************************************************
int spi_busy(spi_t *obj)
{
    if (obj->index == SPI_NUM_0) {
    	return (((mxc_spi_regs_t *)obj->spi)->stat & MXC_F_SPI_STAT_BUSY);
	}
	else if (obj->index < SPI_NUM_MAX) {
        unsigned int reg =  ((mxc_spimss_regs_t *)obj->spi)->int_fl; 
        return (reg & MXC_F_SPIMSS_INT_FL_TXST) >> MXC_F_SPIMSS_INT_FL_TXST_POS;
	}

    return 1;
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
