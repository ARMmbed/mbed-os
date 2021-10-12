/**
 * @file    i2s.c
 * @brief   Inter-Integrated Sound(I2S) driver implementation.
 */

/* ****************************************************************************
 * Copyright(C) Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files(the "Software"),
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
 *
 *************************************************************************** */

#include <stddef.h>
#include <stdint.h>
#include "mxc_errors.h"
#include "mxc_device.h"
#include "mxc_assert.h"
#include "mxc_lock.h"
#include "mxc_sys.h"
#include "dma.h"
#include "i2s.h"
#include "i2s_reva.h"
#include "spimss.h"
#include "spimss_reva.h"

#define I2S_CHANNELS 2
#define I2S_WIDTH    16

static int dma_channel = -1;

int MXC_I2S_RevA_Init(mxc_spimss_reva_regs_t *spimss, const mxc_i2s_config_t *config, void(*dma_ctz_cb)(int, int))
{
    unsigned int baud;
    uint16_t clkdiv;
    uint8_t ctz_en;
    int err;

    mxc_dma_config_t dma_config;
    mxc_dma_srcdst_t srcdst;

    /* Setup SPI_MSS as master, mode 0, 16 bit transfers as I2S Requires */
    spimss->ctrl = MXC_F_SPIMSS_REVA_CTRL_MMEN;
    spimss->mode = MXC_V_SPIMSS_REVA_MODE_NUMBITS_BITS16 | MXC_F_SPIMSS_REVA_MODE_SS_IO;
    spimss->dma = MXC_S_SPIMSS_REVA_DMA_TX_FIFO_LVL_ENTRIES8;

    /* Setup I2S register from i2s_cfg_t */
    spimss->i2s_ctrl =  config->justify << MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_LJ_POS |
                            config->audio_mode << MXC_F_SPIMSS_I2S_CTRL_I2S_MONO_POS;

    /* Determine divisor for baud rate generator */
    baud = config->sample_rate * I2S_CHANNELS * I2S_WIDTH;

    if((PeripheralClock / 4) < baud) {
        return E_BAD_PARAM;
    }

    clkdiv = PeripheralClock / (2 * baud);      // Peripheral clock in system_max*****.h

    if(clkdiv < 2) {
        return E_BAD_PARAM;
    }

    spimss->brg = clkdiv;

    /* Prepare SPIMSS DMA register for DMA setup */
    if(dma_ctz_cb == NULL) {
        ctz_en = 0;
    } else {
        ctz_en = 1;
    }

    /* Initialize DMA */
    if(config->audio_direction % 2) {
        spimss->dma |= MXC_F_SPIMSS_REVA_DMA_TX_DMA_EN | MXC_F_SPIMSS_REVA_DMA_TX_FIFO_CLR;
        if((err = MXC_DMA_Init()) != E_NO_ERROR) {
            if(err != E_BAD_STATE) {
                return err;
            }
        }

        if((err = MXC_DMA_AcquireChannel()) < 0) {
            return err;
        }

        dma_channel = err;

        dma_config.ch = dma_channel;

        dma_config.srcwd = MXC_DMA_WIDTH_HALFWORD;
        dma_config.dstwd = MXC_DMA_WIDTH_WORD;
	  #if TARGET_NUM == 32650
        dma_config.reqsel = MXC_DMA_REQUEST_SPIMSSTX;
	  #endif
        
        dma_config.srcinc_en = 1;
        dma_config.dstinc_en = 0;

        srcdst.ch = dma_channel;
        srcdst.source = config->src_addr;
        srcdst.len = config->length;

        MXC_DMA_ConfigChannel(dma_config, srcdst);
        MXC_DMA_SetChannelInterruptEn(dma_channel, 0, 1);

        MXC_DMA->ch[dma_channel].cfg &= ~MXC_F_DMA_CFG_BRST;
        MXC_DMA->ch[dma_channel].cfg |= (0x1f << MXC_F_DMA_CFG_BRST_POS);

        if(ctz_en) {
            MXC_DMA_SetCallback(dma_channel, dma_ctz_cb);
            MXC_DMA_EnableInt(dma_channel);
        }
    }
    if(config->audio_direction / 2) {
        spimss->dma = MXC_F_SPIMSS_REVA_DMA_RX_DMA_EN | MXC_F_SPIMSS_REVA_DMA_RX_FIFO_CLR;
        if((err = MXC_DMA_Init()) != E_NO_ERROR) {
            if(err != E_BAD_STATE) {    //DMA already initialized
                return err;
            }
        }

        if((err = MXC_DMA_AcquireChannel()) < 0) {
            return err;
        }

        dma_channel = err;

        dma_config.ch = dma_channel;
        
        dma_config.srcwd = MXC_DMA_WIDTH_WORD;
        dma_config.dstwd = MXC_DMA_WIDTH_HALFWORD;
	  #if TARGET_NUM == 32650
        dma_config.reqsel = MXC_DMA_REQUEST_SPIMSSRX;
	  #endif

        dma_config.srcinc_en = 0;
        dma_config.dstinc_en = 1;

        srcdst.ch = dma_channel;
        srcdst.dest = config->dst_addr;
        srcdst.len = config->length;

        MXC_DMA_ConfigChannel(dma_config, srcdst);
        MXC_DMA_SetChannelInterruptEn(dma_channel, 0, 1);

        MXC_DMA->ch[dma_channel].cfg &= ~MXC_F_DMA_CFG_BRST;
        MXC_DMA->ch[dma_channel].cfg |= (0x1f << MXC_F_DMA_CFG_BRST_POS);

        if(ctz_en) {
            MXC_DMA_SetCallback(dma_channel, dma_ctz_cb);
            MXC_DMA_EnableInt(dma_channel);
        }
    }

    MXC_I2S_DMA_SetAddrCnt(config->src_addr, config->dst_addr, config->length);
    if(config->dma_reload_en) {
        MXC_I2S_DMA_SetReload(config->src_addr, config->dst_addr, config->length);
    }

    if(config->start_immediately) {
        return MXC_I2S_Start();
    }
    return E_NO_ERROR;
}

int MXC_I2S_RevA_Shutdown(mxc_spimss_reva_regs_t *spimss)
{
    spimss->ctrl = 0;
    spimss->i2s_ctrl = 0;
    spimss->brg = 0;
    spimss->mode = 0;
    spimss->dma = 0;
    return MXC_DMA_ReleaseChannel(dma_channel);
}

int MXC_I2S_RevA_Mute(mxc_spimss_reva_regs_t *spimss)
{
    spimss->i2s_ctrl |= MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_MUTE;
    return E_NO_ERROR;
}

int MXC_I2S_RevA_Unmute(mxc_spimss_reva_regs_t *spimss)
{
    spimss->i2s_ctrl &= ~MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_MUTE;
    return E_NO_ERROR;
}

int MXC_I2S_RevA_Pause(mxc_spimss_reva_regs_t *spimss)
{
    spimss->i2s_ctrl |= MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_PAUSE;
    return E_NO_ERROR;
}

int MXC_I2S_RevA_Unpause(mxc_spimss_reva_regs_t *spimss)
{
    spimss->i2s_ctrl &= ~MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_PAUSE;
    return E_NO_ERROR;
}

int MXC_I2S_RevA_Stop(mxc_spimss_reva_regs_t *spimss)
{
    spimss->ctrl &= ~MXC_F_SPIMSS_REVA_CTRL_ENABLE;
    spimss->i2s_ctrl &= ~MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_EN;
    return MXC_DMA_Stop(dma_channel);
}

int MXC_I2S_RevA_Start(mxc_spimss_reva_regs_t *spimss)
{
    spimss->ctrl |= MXC_F_SPIMSS_REVA_CTRL_ENABLE;
    spimss->i2s_ctrl |= MXC_F_SPIMSS_REVA_I2S_CTRL_I2S_EN;
    return MXC_DMA_Start(dma_channel);
}

int MXC_I2S_RevA_DMA_ClearFlags(void)
{
    int flags = MXC_DMA_ChannelGetFlags(dma_channel);
    return MXC_DMA_ChannelClearFlags(dma_channel, flags);
}

int MXC_I2S_RevA_DMA_SetAddrCnt(void *src_addr, void *dst_addr, unsigned int count)
{
    mxc_dma_srcdst_t srcdst;
    srcdst.ch = dma_channel;
    srcdst.source = src_addr;
    srcdst.dest = dst_addr;
    srcdst.len = count;
    return MXC_DMA_SetSrcDst(srcdst);
}

int MXC_I2S_RevA_DMA_SetReload(void *src_addr, void *dst_addr, unsigned int count)
{
    mxc_dma_srcdst_t srcdst;
    srcdst.ch = dma_channel;
    srcdst.source = src_addr;
    srcdst.dest = dst_addr;
    srcdst.len = count;
    return MXC_DMA_SetSrcReload(srcdst);
}
