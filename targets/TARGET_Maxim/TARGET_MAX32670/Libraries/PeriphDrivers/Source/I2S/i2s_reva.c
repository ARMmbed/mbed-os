/******************************************************************************
 * Copyright (C) 2023 Maxim Integrated Products, Inc., All Rights Reserved.
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
 ******************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include "mxc_device.h"
#include "mxc_assert.h"
#include "mxc_lock.h"
#include "mxc_sys.h"
#include "mxc_delay.h"
#include "dma.h"
#include "i2s_reva.h"
#include "i2s.h"

/* ***** Definitions ***** */
#define DATALENGTH_EIGHT (8 - 1)
#define DATALENGTH_SIXTEEN (16 - 1)
#define DATALENGTH_TWENTY (20 - 1)
#define DATALENGTH_TWENTYFOUR (24 - 1)
#define DATALENGTH_THIRTYTWO (32 - 1)

// #define USE_LEGACY_I2S_DMA_CFG

typedef struct {
    int rxCnt;
    int txCnt;
    bool async;
} mxc_i2s_reva_txn_t;

/* ****** Globals ****** */
static mxc_i2s_req_t *request;
static void (*dma_cb)(int, int) = NULL;
static void (*async_cb)(int) = NULL;

static mxc_i2s_req_t txn_req;
static mxc_i2s_reva_txn_t txn_state;
static uint32_t txn_lock = 0;

/* ****** Functions ****** */
int MXC_I2S_RevA_Init(mxc_i2s_reva_regs_t *i2s, mxc_i2s_req_t *req)
{
    if (((req->txData == NULL) || (req->rawData == NULL)) && (req->rxData == NULL)) {
        return E_NULL_PTR;
    }

    if (req->length == 0) {
        return E_BAD_PARAM;
    }

    request = req;

    if (req->stereoMode) {
        i2s->ctrl0ch0 |= (req->stereoMode << MXC_F_I2S_REVA_CTRL0CH0_STEREO_POS);
    }

    //Set RX Threshold 2 (default)
    i2s->ctrl0ch0 |= (2 << MXC_F_I2S_REVA_CTRL0CH0_RX_THD_VAL_POS);

    //Set justify
    MXC_SETFIELD(i2s->ctrl0ch0, MXC_F_I2S_REVA_CTRL0CH0_ALIGN,
                 (req->justify) << MXC_F_I2S_REVA_CTRL0CH0_ALIGN_POS);

    if (MXC_I2S_ConfigData((mxc_i2s_req_t *)req) != E_NO_ERROR) {
        return E_BAD_PARAM;
    }

    MXC_I2S_SetFrequency(req->channelMode, req->clkdiv);

    return E_NO_ERROR;
}

int MXC_I2S_RevA_Shutdown(mxc_i2s_reva_regs_t *i2s)
{
    MXC_I2S_DisableInt(0xFF);

    //Disable I2S TX and RX channel
    MXC_I2S_TXDisable();
    MXC_I2S_RXDisable();

    MXC_I2S_Flush();

    //Clear all the registers. Not cleared on reset
    i2s->ctrl0ch0 = 0x00;
    i2s->dmach0 = 0x00;
    i2s->ctrl1ch0 = 0x00;

    i2s->ctrl0ch0 |= MXC_F_I2S_REVA_CTRL0CH0_RST; //Reset channel

    return E_NO_ERROR;
}

int MXC_I2S_RevA_ConfigData(mxc_i2s_reva_regs_t *i2s, mxc_i2s_req_t *req)
{
    uint32_t dataMask;

    //Data pointers
    uint8_t *txdata_8 = (uint8_t *)req->txData;
    uint16_t *txdata_16 = (uint16_t *)req->txData;
    uint32_t *txdata_32 = (uint32_t *)req->txData;
    uint8_t *rawdata_8 = (uint8_t *)req->rawData;
    uint16_t *rawdata_16 = (uint16_t *)req->rawData;
    uint32_t *rawdata_32 = (uint32_t *)req->rawData;

    if ((req->txData == NULL) && (req->rxData == NULL)) {
        return E_NULL_PTR;
    }

    if (req->length == 0) {
        return E_BAD_PARAM;
    }

    // Clear configuration bits
    i2s->ctrl0ch0 &= ~MXC_F_I2S_REVA_CTRL0CH0_WSIZE;
    i2s->ctrl1ch0 &= ~MXC_F_I2S_REVA_CTRL1CH0_BITS_WORD;
    i2s->ctrl1ch0 &= ~MXC_F_I2S_REVA_CTRL1CH0_SMP_SIZE;

    switch (req->sampleSize) {
    case MXC_I2S_SAMPLESIZE_EIGHT:
        if (req->wordSize == MXC_I2S_DATASIZE_WORD) {
            //Set word length
            i2s->ctrl1ch0 |= (DATALENGTH_THIRTYTWO << MXC_F_I2S_REVA_CTRL1CH0_BITS_WORD_POS);
        } else if (req->wordSize == MXC_I2S_DATASIZE_HALFWORD) {
            //Set word length
            i2s->ctrl1ch0 |= (DATALENGTH_SIXTEEN << MXC_F_I2S_REVA_CTRL1CH0_BITS_WORD_POS);
        } else {
            //Set word length
            i2s->ctrl1ch0 |= (DATALENGTH_EIGHT << MXC_F_I2S_REVA_CTRL1CH0_BITS_WORD_POS);
        }

        //Set sample length
        i2s->ctrl1ch0 |= (DATALENGTH_EIGHT << MXC_F_I2S_REVA_CTRL1CH0_SMP_SIZE_POS);

        //Set datasize to load in FIFO
        MXC_SETFIELD(i2s->ctrl0ch0, MXC_F_I2S_REVA_CTRL0CH0_WSIZE,
                     (MXC_I2S_DATASIZE_BYTE) << MXC_F_I2S_REVA_CTRL0CH0_WSIZE_POS);

        dataMask = 0x000000ff;

        if ((req->rawData != NULL) && (req->txData != NULL)) {
            for (uint32_t i = 0; i < req->length; i++) {
                *txdata_8++ = *rawdata_8++ & dataMask;
            }
        }

        break;

    case MXC_I2S_SAMPLESIZE_SIXTEEN:
        if (req->wordSize == MXC_I2S_DATASIZE_WORD) {
            //Set word length
            i2s->ctrl1ch0 |= (DATALENGTH_THIRTYTWO << MXC_F_I2S_REVA_CTRL1CH0_BITS_WORD_POS);
        } else {
            //Set word length
            i2s->ctrl1ch0 |= (DATALENGTH_SIXTEEN << MXC_F_I2S_REVA_CTRL1CH0_BITS_WORD_POS);
        }

        //Set sample length
        i2s->ctrl1ch0 |= (DATALENGTH_SIXTEEN << MXC_F_I2S_REVA_CTRL1CH0_SMP_SIZE_POS);

        //Set datasize
        MXC_SETFIELD(i2s->ctrl0ch0, MXC_F_I2S_REVA_CTRL0CH0_WSIZE,
                     (MXC_I2S_DATASIZE_HALFWORD) << MXC_F_I2S_REVA_CTRL0CH0_WSIZE_POS);

        dataMask = 0x0000ffff;

        if ((req->rawData != NULL) && (req->txData != NULL)) {
            for (uint32_t i = 0; i < req->length; i++) {
                *txdata_16++ = *rawdata_16++ & dataMask;
            }
        }

        break;

    case MXC_I2S_SAMPLESIZE_TWENTY:
        //Set word length
        i2s->ctrl1ch0 |= (DATALENGTH_THIRTYTWO << MXC_F_I2S_REVA_CTRL1CH0_BITS_WORD_POS);

        //Set sample length
        i2s->ctrl1ch0 |= (DATALENGTH_TWENTY << MXC_F_I2S_REVA_CTRL1CH0_SMP_SIZE_POS);

        //Set datasize
        MXC_SETFIELD(i2s->ctrl0ch0, MXC_F_I2S_REVA_CTRL0CH0_WSIZE,
                     (MXC_I2S_DATASIZE_WORD) << MXC_F_I2S_REVA_CTRL0CH0_WSIZE_POS);

        dataMask = 0x00fffff;

        if ((req->rawData != NULL) && (req->txData != NULL)) {
            for (uint32_t i = 0; i < req->length; i++) {
                *txdata_32++ = (*rawdata_32++ & dataMask) << 12;
            }
        }

        break;

    case MXC_I2S_SAMPLESIZE_TWENTYFOUR:
        //Set word length
        i2s->ctrl1ch0 |= (DATALENGTH_THIRTYTWO << MXC_F_I2S_REVA_CTRL1CH0_BITS_WORD_POS);

        //Set sample length
        i2s->ctrl1ch0 |= (DATALENGTH_TWENTYFOUR << MXC_F_I2S_REVA_CTRL1CH0_SMP_SIZE_POS);

        //Set datasize
        MXC_SETFIELD(i2s->ctrl0ch0, MXC_F_I2S_REVA_CTRL0CH0_WSIZE,
                     (MXC_I2S_DATASIZE_WORD) << MXC_F_I2S_REVA_CTRL0CH0_WSIZE_POS);

        dataMask = 0x00ffffff;

        if ((req->rawData != NULL) && (req->txData != NULL)) {
            for (uint32_t i = 0; i < req->length; i++) {
                *txdata_32++ = (*rawdata_32++ & dataMask) << 8;
            }
        }

        break;

    case MXC_I2S_SAMPLESIZE_THIRTYTWO:
        //Set word length
        i2s->ctrl1ch0 |= (DATALENGTH_THIRTYTWO << MXC_F_I2S_REVA_CTRL1CH0_BITS_WORD_POS);

        //Set sample length
        i2s->ctrl1ch0 |= (DATALENGTH_THIRTYTWO << MXC_F_I2S_REVA_CTRL1CH0_SMP_SIZE_POS);

        //Set datasize
        MXC_SETFIELD(i2s->ctrl0ch0, MXC_F_I2S_REVA_CTRL0CH0_WSIZE,
                     (MXC_I2S_DATASIZE_WORD) << MXC_F_I2S_REVA_CTRL0CH0_WSIZE_POS);

        dataMask = 0xffffffff;

        if ((req->rawData != NULL) && (req->txData != NULL)) {
            for (uint32_t i = 0; i < req->length; i++) {
                *txdata_32++ = *rawdata_32++ & dataMask;
            }
        }

        break;

    default:
        return E_BAD_PARAM;
        break;
    }

    return E_NO_ERROR;
}

void MXC_I2S_RevA_TXEnable(mxc_i2s_reva_regs_t *i2s)
{
    MXC_SETFIELD(i2s->ctrl0ch0, MXC_F_I2S_REVA_CTRL0CH0_TX_EN,
                 1 << MXC_F_I2S_REVA_CTRL0CH0_TX_EN_POS);
}

void MXC_I2S_RevA_TXDisable(mxc_i2s_reva_regs_t *i2s)
{
    MXC_SETFIELD(i2s->ctrl0ch0, MXC_F_I2S_REVA_CTRL0CH0_TX_EN,
                 0 << MXC_F_I2S_REVA_CTRL0CH0_TX_EN_POS);
}

void MXC_I2S_RevA_RXEnable(mxc_i2s_reva_regs_t *i2s)
{
    MXC_SETFIELD(i2s->ctrl0ch0, MXC_F_I2S_REVA_CTRL0CH0_RX_EN,
                 1 << MXC_F_I2S_REVA_CTRL0CH0_RX_EN_POS);
}

void MXC_I2S_RevA_RXDisable(mxc_i2s_reva_regs_t *i2s)
{
    MXC_SETFIELD(i2s->ctrl0ch0, MXC_F_I2S_REVA_CTRL0CH0_RX_EN,
                 0 << MXC_F_I2S_REVA_CTRL0CH0_RX_EN_POS);
}

int MXC_I2S_RevA_SetRXThreshold(mxc_i2s_reva_regs_t *i2s, uint8_t threshold)
{
    if ((threshold == 0) || (threshold > 8)) {
        return E_NOT_SUPPORTED;
    }

    i2s->ctrl0ch0 |= (threshold << MXC_F_I2S_REVA_CTRL0CH0_RX_THD_VAL_POS);

    return E_NO_ERROR;
}

int MXC_I2S_RevA_SetFrequency(mxc_i2s_reva_regs_t *i2s, mxc_i2s_ch_mode_t mode, uint16_t clkdiv)
{
    i2s->ctrl1ch0 &= ~MXC_F_I2S_REVA_CTRL1CH0_EN;

    MXC_SETFIELD(i2s->ctrl0ch0, MXC_F_I2S_REVA_CTRL0CH0_CH_MODE,
                 (mode) << MXC_F_I2S_REVA_CTRL0CH0_CH_MODE_POS);

    i2s->ctrl1ch0 |= ((uint32_t)clkdiv) << MXC_F_I2S_REVA_CTRL1CH0_CLKDIV_POS;

    i2s->ctrl1ch0 |= MXC_F_I2S_REVA_CTRL1CH0_EN;

    return E_NO_ERROR;
}

int MXC_I2S_RevA_SetSampleRate(mxc_i2s_reva_regs_t *i2s, uint32_t smpl_rate,
                               mxc_i2s_wsize_t smpl_sz, uint32_t src_clk)
{
    int clk_div;

    clk_div = MXC_I2S_RevA_CalculateClockDiv(i2s, smpl_rate, smpl_sz, src_clk);
    if (clk_div < 0) {
        return clk_div;
    }

    i2s->ctrl1ch0 &= ~MXC_F_I2S_REVA_CTRL1CH0_EN;
    i2s->ctrl1ch0 |= ((uint32_t)clk_div) << MXC_F_I2S_REVA_CTRL1CH0_CLKDIV_POS;
    i2s->ctrl1ch0 |= MXC_F_I2S_REVA_CTRL1CH0_EN;

    return MXC_I2S_RevA_GetSampleRate(i2s, src_clk);
}

int MXC_I2S_RevA_GetSampleRate(mxc_i2s_reva_regs_t *i2s, uint32_t src_clk)
{
    uint16_t word_sz, clk_div;
    uint32_t bclk;

    word_sz = (i2s->ctrl0ch0 & MXC_F_I2S_REVA_CTRL0CH0_WSIZE) >> MXC_F_I2S_REVA_CTRL0CH0_WSIZE_POS;
    clk_div = (i2s->ctrl1ch0 & MXC_F_I2S_REVA_CTRL1CH0_CLKDIV) >>
              MXC_F_I2S_REVA_CTRL1CH0_CLKDIV_POS; // Get clock divider value

    switch (word_sz) { // Get word size
    case MXC_I2S_DATASIZE_BYTE:
        word_sz = 8;
        break;
    case MXC_I2S_DATASIZE_HALFWORD:
        word_sz = 16;
        break;
    case MXC_I2S_DATASIZE_WORD:
    default:
        word_sz = 32;
        break;
    }

    bclk = (src_clk / (clk_div + 1)) >>
           1; // bclk_frequency = src_clk_frequency / (clk_divider + 1) / 2
    return (bclk / word_sz) >>
           1; // return sample rate (sample_rate = bclk_frequency / word_size / 2)
}

int MXC_I2S_RevA_CalculateClockDiv(mxc_i2s_reva_regs_t *i2s, uint32_t smpl_rate,
                                   mxc_i2s_wsize_t smpl_sz, uint32_t src_clk)
{
    uint32_t bclk;

    switch (smpl_sz) { // Get word size
    case MXC_I2S_DATASIZE_BYTE:
        bclk = 8;
        break;
    case MXC_I2S_DATASIZE_HALFWORD:
        bclk = 16;
        break;
    case MXC_I2S_DATASIZE_WORD:
        bclk = 32;
        break;
    default:
        return E_BAD_PARAM;
    }

    bclk *= smpl_rate * 4; // bclk_frequency = sample_rate * word_size * 2

    if (bclk > src_clk) {
        return E_INVALID;
    }

    return (src_clk / bclk) - 1; // clk_divider = src_clk_frequency / (bclk_frequency * 2) - 1
}

void MXC_I2S_RevA_Flush(mxc_i2s_reva_regs_t *i2s)
{
    i2s->ctrl0ch0 |= MXC_F_I2S_REVA_CTRL0CH0_FLUSH;

    while (i2s->ctrl0ch0 & MXC_F_I2S_REVA_CTRL0CH0_FLUSH) {}
}

static uint32_t write_tx_fifo(void *tx, mxc_i2s_wsize_t wordSize, int smpl_cnt)
{
    uint32_t write_val = 0;

    if (wordSize == MXC_I2S_DATASIZE_BYTE) {
        uint8_t *tx8 = (uint8_t *)tx;
        for (int i = 0; i < 4; i++) {
            write_val |= (tx8[smpl_cnt++] << (i * 8));
        }
    } else if (wordSize == MXC_I2S_DATASIZE_HALFWORD) {
        uint16_t *tx16 = (uint16_t *)tx;
        for (int i = 0; i < 2; i++) {
            write_val |= (tx16[smpl_cnt++] << (i * 16));
        }
    } else if (wordSize == MXC_I2S_DATASIZE_WORD) {
        uint32_t *tx32 = (uint32_t *)tx;
        write_val = tx32[smpl_cnt];
    }

    return write_val;
}

int MXC_I2S_RevA_FillTXFIFO(mxc_i2s_reva_regs_t *i2s, void *txData, mxc_i2s_wsize_t wordSize,
                            int len, int smpl_cnt)
{
    int num_smpl = 0x4 >> wordSize; // Number of samples per FIFO write
    int sent = 0; // Total number of samples transmitted
    uint32_t fifo_write, fifo_avail; // Value to write to I2S TX FIFO

    if (txData == NULL) { // Check for bad parameters
        return E_NULL_PTR;
    } else if (wordSize < MXC_I2S_DATASIZE_BYTE || wordSize > MXC_I2S_DATASIZE_WORD) {
        return E_BAD_PARAM;
    } else if (len == 0) {
        return E_NO_ERROR;
    }

    len -= len % num_smpl; // TEST
    fifo_avail =
        8 - ((i2s->dmach0 & MXC_F_I2S_REVA_DMACH0_TX_LVL) >> MXC_F_I2S_REVA_DMACH0_TX_LVL_POS);
    fifo_avail *= num_smpl;
    while (sent < len && sent < fifo_avail) {
        fifo_write = write_tx_fifo(txData, wordSize, sent + smpl_cnt);
        sent += num_smpl;

        i2s->fifoch0 = fifo_write;
    }

    return sent;
}

static void read_rx_fifo(mxc_i2s_reva_regs_t *i2s, void *rxData, mxc_i2s_wsize_t wordSize, int cnt)
{
    uint32_t fifo_val = i2s->fifoch0;

    if (wordSize == MXC_I2S_DATASIZE_BYTE) {
        uint8_t *rx8 = (uint8_t *)rxData;
        for (int i = 0; i < 4; i++) {
            rx8[cnt++] = fifo_val & 0xFF;
            fifo_val = fifo_val >> 8;
        }
    } else if (wordSize == MXC_I2S_DATASIZE_HALFWORD) {
        uint16_t *rx16 = (uint16_t *)rxData;
        for (int i = 0; i < 2; i++) {
            rx16[cnt++] = fifo_val & 0xFFFF;
            fifo_val = fifo_val >> 16;
        }
    } else if (wordSize == MXC_I2S_DATASIZE_WORD) {
        uint32_t *rx32 = (uint32_t *)rxData;
        rx32[cnt] = fifo_val;
    }
}

int MXC_I2S_RevA_ReadRXFIFO(mxc_i2s_reva_regs_t *i2s, void *rxData, mxc_i2s_wsize_t wordSize,
                            int len, int smpl_cnt)
{
    int received = 0;
    int num_smpl = 0x4 >> wordSize;
    uint32_t fifo_avail;

    if (rxData == NULL) { // Check for bad parameters
        return E_NULL_PTR;
    } else if (wordSize < MXC_I2S_DATASIZE_BYTE || wordSize > MXC_I2S_DATASIZE_WORD) {
        return E_BAD_PARAM;
    } else if (len == 0) {
        return E_NO_ERROR;
    }

    len -= len % num_smpl;
    fifo_avail = (i2s->dmach0 & MXC_F_I2S_REVA_DMACH0_RX_LVL) >> MXC_F_I2S_REVA_DMACH0_RX_LVL_POS;
    while (received < len && fifo_avail) {
        read_rx_fifo(i2s, rxData, wordSize, received + smpl_cnt);
        received += num_smpl;
        fifo_avail = (i2s->dmach0 & MXC_F_I2S_REVA_DMACH0_RX_LVL) >>
                     MXC_F_I2S_REVA_DMACH0_RX_LVL_POS;
    }

    return received;
}

void MXC_I2S_RevA_EnableInt(mxc_i2s_reva_regs_t *i2s, uint32_t flags)
{
    i2s->inten |= flags;
}

void MXC_I2S_RevA_DisableInt(mxc_i2s_reva_regs_t *i2s, uint32_t flags)
{
    i2s->inten &= ~flags;
}

int MXC_I2S_RevA_GetFlags(mxc_i2s_reva_regs_t *i2s)
{
    return (i2s->intfl & 0xF);
}

void MXC_I2S_RevA_ClearFlags(mxc_i2s_reva_regs_t *i2s, uint32_t flags)
{
    i2s->intfl |= flags;
}

int MXC_I2S_RevA_Transaction(mxc_i2s_reva_regs_t *i2s, mxc_i2s_req_t *i2s_req)
{
    int err;

    if (i2s_req->rawData != NULL && i2s_req->txData == NULL) {
        return E_INVALID;
    } else if (i2s_req->length == 0) {
        return E_INVALID;
    } else if (MXC_GetLock(&txn_lock, 1) != E_NO_ERROR) {
        return E_BUSY;
    }

    i2s->ctrl1ch0 &= ~MXC_F_I2S_REVA_CTRL1CH0_EN; // Disable I2S while it's being configured

    txn_req = *i2s_req; // Initialize transaction request state variables
    txn_state.rxCnt = txn_req.length;
    txn_state.txCnt = txn_req.length;
    txn_state.async = false;

    MXC_I2S_Flush();

    if (txn_req.rawData != NULL &&
        txn_req.txData != NULL) { // Set up transmit if transmit parameters valid
        txn_state.txCnt = 0;

        err = MXC_I2S_ConfigData(&txn_req);
        if (err) {
            MXC_FreeLock(&txn_lock);
            return err;
        }

        err = MXC_I2S_FillTXFIFO(txn_req.txData, txn_req.wordSize, txn_req.length,
                                 txn_state.txCnt); // Initialize TX FIFO
        if (err < E_NO_ERROR) {
            MXC_FreeLock(&txn_lock);
            return err;
        }
        txn_state.txCnt = err;

        MXC_I2S_TXEnable(); // Enable I2S transmit (Do this before Fill FIFO?)
    }

    if (txn_req.rxData != NULL) { // Setup I2S receive if receive parameters valid
        txn_state.rxCnt = 0;

        MXC_SETFIELD(i2s->ctrl0ch0, MXC_F_I2S_REVA_CTRL0CH0_RX_THD_VAL,
                     (6 << MXC_F_I2S_REVA_CTRL0CH0_RX_THD_VAL_POS)); // Set RX threshold

        MXC_I2S_RXEnable(); // Enable I2S Receive
    }

    i2s->ctrl1ch0 |= MXC_F_I2S_REVA_CTRL1CH0_EN; // Enable I2S RX/TX

    while (MXC_GetLock(&txn_lock, 1) != E_NO_ERROR) {
        MXC_I2S_RevA_Handler(i2s);
    }

    MXC_FreeLock(&txn_lock);

    return E_NO_ERROR;
}

int MXC_I2S_RevA_TransactionAsync(mxc_i2s_reva_regs_t *i2s, mxc_i2s_req_t *i2s_req)
{
    int err;
    uint32_t int_en = 0;

    if (i2s_req->rawData != NULL && i2s_req->txData == NULL) {
        return E_INVALID;
    } else if (i2s_req->length == 0) {
        return E_INVALID;
    } else if (MXC_GetLock(&txn_lock, 1) != E_NO_ERROR) {
        return E_BUSY;
    }

    i2s->ctrl1ch0 &= ~MXC_F_I2S_REVA_CTRL1CH0_EN; // Disable I2S while it's being configured

    txn_req = *i2s_req; // Initialize transacion request state variables
    txn_state.rxCnt = txn_req.length;
    txn_state.txCnt = txn_req.length;
    txn_state.async = true;

    MXC_I2S_Flush();

    if (txn_req.rawData != NULL &&
        txn_req.txData != NULL) { // Set up transmit if transmit parameters valid
        txn_state.txCnt = 0;

        err = MXC_I2S_ConfigData(&txn_req);
        if (err) {
            MXC_FreeLock(&txn_lock);
            return err;
        }

        int_en |= MXC_F_I2S_REVA_INTFL_TX_OB_CH0; // Enable TX one entry remaining interrupt

        err = MXC_I2S_FillTXFIFO(txn_req.txData, txn_req.wordSize, txn_req.length,
                                 txn_state.txCnt); // Initialize TX FIFO
        if (err < E_NO_ERROR) {
            MXC_FreeLock(&txn_lock);
            return err;
        }
        txn_state.txCnt = err;

        MXC_I2S_TXEnable(); // Enable I2S transmit (Do this before Fill FIFO?)
    }

    if (txn_req.rxData != NULL) { // Setup I2S receive if receive parameters valid
        txn_state.rxCnt = 0;

        int_en |= MXC_F_I2S_REVA_INTEN_RX_THD_CH0; // Enable RX threshold interrupt
        MXC_SETFIELD(i2s->ctrl0ch0, MXC_F_I2S_REVA_CTRL0CH0_RX_THD_VAL,
                     (6 << MXC_F_I2S_REVA_CTRL0CH0_RX_THD_VAL_POS)); // Set RX threshold

        MXC_I2S_RXEnable(); // Enable I2S Receive
    }

    MXC_I2S_DisableInt(0xF); // Configure interrupts
    MXC_I2S_ClearFlags(0xF);
    MXC_I2S_EnableInt(int_en);

    i2s->ctrl1ch0 |= MXC_F_I2S_REVA_CTRL1CH0_EN; // Enable I2S RX/TX

    return E_NO_ERROR;
}

int MXC_I2S_RevA_TXDMAConfig(mxc_i2s_reva_regs_t *i2s, void *src_addr, int len)
{
    int channel;
    mxc_dma_config_t config;
    mxc_dma_adv_config_t advConfig;
    mxc_dma_srcdst_t srcdst;

    MXC_DMA_Init();

    i2s->dmach0 |= (2 << MXC_F_I2S_REVA_DMACH0_DMA_TX_THD_VAL_POS); //TX DMA Threshold

    channel = MXC_DMA_AcquireChannel();
    if (channel < E_NO_ERROR) {
        return channel;
    }

    config.reqsel = MXC_DMA_REQUEST_I2STX;

    config.ch = channel;

    switch (request->wordSize) {
    case MXC_I2S_DATASIZE_WORD:
        config.srcwd = MXC_DMA_WIDTH_WORD;
        config.dstwd = MXC_DMA_WIDTH_WORD;
        advConfig.burst_size = 4;
        break;

    case MXC_I2S_DATASIZE_HALFWORD:
        config.srcwd = MXC_DMA_WIDTH_HALFWORD;
        config.dstwd = MXC_DMA_WIDTH_WORD;
        advConfig.burst_size = 2;
        break;

    case MXC_I2S_DATASIZE_BYTE:
        config.srcwd = MXC_DMA_WIDTH_BYTE;
        config.dstwd = MXC_DMA_WIDTH_WORD;
        advConfig.burst_size = 1;
        break;

    default:
        config.srcwd = MXC_DMA_WIDTH_BYTE;
        config.dstwd = MXC_DMA_WIDTH_WORD;
        advConfig.burst_size = 1;
        break;
    }

#ifndef USE_LEGACY_I2S_DMA_CFG
    advConfig.burst_size = 4;
#endif

    config.srcinc_en = 1;
    config.dstinc_en = 0;

    advConfig.ch = channel;
    advConfig.prio = 0;
    advConfig.reqwait_en = 0;
    advConfig.tosel = 0;
    advConfig.pssel = 0;

    srcdst.ch = channel;
    srcdst.source = src_addr;
    srcdst.len = len;

    MXC_DMA_ConfigChannel(config, srcdst);
    MXC_DMA_AdvConfigChannel(advConfig);
    MXC_DMA_SetCallback(channel, dma_cb);

    MXC_I2S_TXEnable(); //Enable I2S TX
    i2s->dmach0 |= MXC_F_I2S_REVA_DMACH0_DMA_TX_EN; //Enable I2S DMA

    MXC_DMA_EnableInt(channel);
    MXC_DMA_Start(channel);
    MXC_DMA->ch[channel].ctrl |= MXC_F_DMA_CTRL_CTZ_IE;

    return channel;
}

int MXC_I2S_RevA_RXDMAConfig(mxc_i2s_reva_regs_t *i2s, void *dest_addr, int len)
{
    int channel;
    mxc_dma_config_t config;
    mxc_dma_adv_config_t advConfig;
    mxc_dma_srcdst_t srcdst;

    MXC_DMA_Init();

    i2s->dmach0 |= (6 << MXC_F_I2S_REVA_DMACH0_DMA_RX_THD_VAL_POS); //RX DMA Threshold

    channel = MXC_DMA_AcquireChannel();
    if (channel < E_NO_ERROR) {
        return channel;
    }

    config.reqsel = MXC_DMA_REQUEST_I2SRX;

    config.ch = channel;

    switch (request->wordSize) {
    case MXC_I2S_DATASIZE_WORD:
        config.srcwd = MXC_DMA_WIDTH_WORD;
        config.dstwd = MXC_DMA_WIDTH_WORD;
        advConfig.burst_size = 4;
        break;

    case MXC_I2S_DATASIZE_HALFWORD:
        config.srcwd = MXC_DMA_WIDTH_WORD;
        config.dstwd = MXC_DMA_WIDTH_HALFWORD;
        advConfig.burst_size = 2;
        break;

    case MXC_I2S_DATASIZE_BYTE:
        config.srcwd = MXC_DMA_WIDTH_WORD;
        config.dstwd = MXC_DMA_WIDTH_BYTE;
        advConfig.burst_size = 1;
        break;

    default:
        config.srcwd = MXC_DMA_WIDTH_WORD;
        config.dstwd = MXC_DMA_WIDTH_BYTE;
        advConfig.burst_size = 1;
        break;
    }

#ifndef USE_LEGACY_I2S_DMA_CFG
    advConfig.burst_size = 4;
#endif

    config.srcinc_en = 0;
    config.dstinc_en = 1;

    advConfig.ch = channel;
    advConfig.prio = 0;
    advConfig.reqwait_en = 0;
    advConfig.tosel = 0;
    advConfig.pssel = 0;

    srcdst.ch = channel;
    srcdst.dest = dest_addr;
    srcdst.len = len;

    MXC_DMA_ConfigChannel(config, srcdst);
    MXC_DMA_AdvConfigChannel(advConfig);
    MXC_DMA_SetCallback(channel, dma_cb);

    MXC_I2S_RXEnable(); //Enable I2S RX
    i2s->dmach0 |= MXC_F_I2S_REVA_DMACH0_DMA_RX_EN; //Enable I2S DMA

    MXC_DMA_EnableInt(channel);
    MXC_DMA_Start(channel);
    MXC_DMA->ch[channel].ctrl |= MXC_F_DMA_CTRL_CTZ_IE;

    return channel;
}

void MXC_I2S_RevA_Handler(mxc_i2s_reva_regs_t *i2s)
{
    uint32_t flags = MXC_I2S_GetFlags();

    if (txn_state.txCnt == txn_req.length && txn_state.rxCnt == txn_req.length) {
        MXC_I2S_DisableInt(MXC_F_I2S_REVA_INTEN_TX_OB_CH0 | MXC_F_I2S_REVA_INTEN_RX_THD_CH0);
        MXC_I2S_ClearFlags(MXC_F_I2S_REVA_INTFL_TX_OB_CH0 | MXC_F_I2S_REVA_INTFL_RX_THD_CH0);

        while (i2s->dmach0 & MXC_F_I2S_REVA_DMACH0_TX_LVL) {}

        MXC_I2S_TXDisable();
        MXC_I2S_RXDisable();

        if (async_cb != NULL && txn_state.async) {
            async_cb(E_NO_ERROR);
        }

        MXC_FreeLock(&txn_lock);
    } else if (txn_req.txData != NULL && (flags & MXC_F_I2S_REVA_INTFL_TX_OB_CH0)) {
        MXC_I2S_ClearFlags(MXC_F_I2S_REVA_INTFL_TX_OB_CH0);

        if (txn_state.txCnt < txn_req.length) {
            txn_state.txCnt += MXC_I2S_FillTXFIFO(txn_req.txData, txn_req.wordSize,
                                                  (txn_req.length - txn_state.txCnt),
                                                  txn_state.txCnt);
        }
    } else if (txn_req.rxData != NULL && (flags & MXC_F_I2S_REVA_INTFL_RX_THD_CH0)) {
        MXC_I2S_ClearFlags(MXC_F_I2S_REVA_INTFL_RX_THD_CH0);

        if (txn_state.rxCnt < txn_req.length) {
            txn_state.rxCnt += MXC_I2S_ReadRXFIFO(txn_req.rxData, txn_req.wordSize,
                                                  (txn_req.length - txn_state.rxCnt),
                                                  txn_state.rxCnt);
        }
    }
}

void MXC_I2S_RevA_RegisterDMACallback(void (*callback)(int, int))
{
    dma_cb = callback;
}

void MXC_I2S_RevA_RegisterAsyncCallback(void (*callback)(int))
{
    async_cb = callback;
}
