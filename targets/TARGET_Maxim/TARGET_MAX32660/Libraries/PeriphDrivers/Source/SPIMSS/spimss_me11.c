/* ****************************************************************************
 * Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
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
 *
 *************************************************************************** */

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include "mxc_device.h"
#include "mxc_assert.h"
#include "mxc_lock.h"
#include "mxc_sys.h"
#include "mxc_delay.h"
#include "spimss_reva_regs.h"
#include "spimss_reva.h"

 /* **** Functions **** */

/* ************************************************************************** */
int MXC_SPIMSS_Init(mxc_spimss_regs_t *spi, unsigned mode, unsigned freq, const sys_map_t sys_cfg, unsigned drv_ssel)
{
    if (mode > 3) {
        return E_BAD_PARAM;
    }

    // Check if frequency is too high
    if (freq > PeripheralClock) {
        return E_BAD_PARAM;
    }

    // Configure GPIO for spimss
    if (spi == MXC_SPIMSS) {
        MXC_GCR->rst0 |= MXC_F_GCR_RST0_SPI1;
        while (MXC_GCR->rst0 & MXC_F_GCR_RST0_SPI1);
        MXC_GCR->pclk_dis0 &= ~ (MXC_F_GCR_PCLK_DIS0_SPI1D);
        if(sys_cfg == MAP_A){
            MXC_GPIO_Config(&gpio_cfg_spi1a);  // SPI1A chosen
        }else if(sys_cfg == MAP_B){
            MXC_GPIO_Config(&gpio_cfg_spi1b);  // SPI1B chosen
        }else{
            return E_BAD_PARAM;
        }
    } else {
        return E_NO_DEVICE;
    }

    return MXC_SPIMSS_RevA_Init((mxc_spimss_reva_regs_t*) spi, mode, freq, drv_ssel);
}
/* ************************************************************************* */
int MXC_SPIMSS_Shutdown(mxc_spimss_regs_t *spi)
{
    if(spi != MXC_SPIMSS) {
        return E_NO_DEVICE;
    }

    MXC_SPIMSS_RevA_Shutdown((mxc_spimss_reva_regs_t*) spi);
    //
    MXC_GCR->pclk_dis0 |= (MXC_F_GCR_PCLK_DIS0_SPI1D);

    return E_NO_ERROR;
}
/* ************************************************************************** */
void MXC_SPIMSS_Handler(mxc_spimss_regs_t *spi)  // From the IRQ
{
    MXC_SPIMSS_RevA_Handler((mxc_spimss_reva_regs_t*) spi);
}

/* ************************************************************************** */
int MXC_SPIMSS_MasterTrans(mxc_spimss_regs_t *spi, mxc_spimss_req_t *req)
{
    return MXC_SPIMSS_RevA_MasterTrans((mxc_spimss_reva_regs_t*) spi, (spimss_reva_req_t*) req);
}


/* ************************************************************************** */
int MXC_SPIMSS_SlaveTrans(mxc_spimss_regs_t *spi, mxc_spimss_req_t *req)
{
    return MXC_SPIMSS_RevA_SlaveTrans((mxc_spimss_reva_regs_t*) spi, (spimss_reva_req_t*) req);
}

/* ************************************************************************** */
int MXC_SPIMSS_MasterTransAsync(mxc_spimss_regs_t *spi, mxc_spimss_req_t *req)
{
    return MXC_SPIMSS_RevA_MasterTransAsync((mxc_spimss_reva_regs_t*) spi, (spimss_reva_req_t*) req);
}

/* ************************************************************************** */
int MXC_SPIMSS_SlaveTransAsync(mxc_spimss_regs_t *spi, mxc_spimss_req_t *req)
{
    return MXC_SPIMSS_RevA_SlaveTransAsync((mxc_spimss_reva_regs_t*) spi, (spimss_reva_req_t*) req);
}

/* ************************************************************************* */
int MXC_SPIMSS_SetDefaultTXData(mxc_spimss_req_t* spi, unsigned int defaultTXData)
{
    return MXC_SPIMSS_RevA_SetDefaultTXData((spimss_reva_req_t*) spi, defaultTXData);
}

/* ************************************************************************* */
int MXC_SPIMSS_AbortAsync(mxc_spimss_req_t *req)
{
    return MXC_SPIMSS_RevA_AbortAsync((spimss_reva_req_t*) req);
}
