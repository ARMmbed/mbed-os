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
#ifdef __CC_ARM //Keil 
#pragma diag_suppress 188  // enumerated type mixed with another type
#pragma diag_suppress 68  // integer conversion resulted in a change of sign
#endif



#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "mxc_device.h"
#include "mxc_assert.h"
#include "mxc_lock.h"
#include "mxc_sys.h"
#include "mxc_delay.h"
#include "mxc_spi.h"
#include "spi_reva.h"
#include "dma_reva.h"

/* **** Definitions **** */
typedef struct {
    mxc_spi_reva_req_t *req;
    int started;
    unsigned last_size;
    unsigned ssDeassert;
    unsigned defaultTXData;
    int channelTx;
    int channelRx;
    unsigned drv_ssel;
} spi_req_reva_state_t;

/* states whether to use call back or not */
uint8_t async;

static spi_req_reva_state_t states[MXC_SPI_INSTANCES];

static uint32_t MXC_SPI_RevA_MasterTransHandler (mxc_spi_reva_regs_t *spi, mxc_spi_reva_req_t *req);
static uint32_t MXC_SPI_RevA_TransHandler (mxc_spi_reva_regs_t *spi, mxc_spi_reva_req_t *req);
static uint32_t MXC_SPI_RevA_SlaveTransHandler (mxc_spi_reva_req_t *req);
static void MXC_SPI_RevA_SwapByte (uint8_t * arr, size_t length);
static int MXC_SPI_RevA_TransSetup (mxc_spi_reva_req_t * req);


int MXC_SPI_RevA_Init (mxc_spi_reva_regs_t* spi, int masterMode, int quadModeUsed, int numSlaves,
                       unsigned ssPolarity, unsigned int hz, unsigned drv_ssel)
{
    int spi_num;
    
    spi_num = MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi);
    MXC_ASSERT (spi_num >= 0);
    
    states[spi_num].req = NULL;
    states[spi_num].last_size = 0;
    states[spi_num].ssDeassert = 1;
    states[spi_num].defaultTXData = 0;
    states[spi_num].drv_ssel = drv_ssel;
    
    spi->ctrl0 = (MXC_F_SPI_REVA_CTRL0_EN);
    spi->sstime = ( (0x1 << MXC_F_SPI_REVA_SSTIME_PRE_POS) |
                     (0x1 << MXC_F_SPI_REVA_SSTIME_POST_POS) |
                     (0x1 << MXC_F_SPI_REVA_SSTIME_INACT_POS));
                                      
    //set master
    if (masterMode) {
        spi->ctrl0 |= MXC_F_SPI_REVA_CTRL0_MST_MODE;
    }
    else {
        spi->ctrl0 &= ~ (MXC_F_SPI_REVA_CTRL0_MST_MODE);
    }
    
    MXC_SPI_SetFrequency ((mxc_spi_regs_t*) spi, hz);
    
    //set slave select polarity
    spi->ctrl2 |= ( (!!ssPolarity) << MXC_F_SPI_REVA_CTRL2_SS_POL_POS);
    
    // Clear the interrupts
    spi->intfl = spi->intfl;
    
    // Driver will drive SS pin?
    if (states[spi_num].drv_ssel) {
        spi->ctrl0 |= MXC_S_SPI_REVA_CTRL0_SS_ACTIVE_SS0;
    }

    //set quad mode
    if (quadModeUsed) {
        spi->ctrl2 |= MXC_S_SPI_REVA_CTRL2_DATA_WIDTH_QUAD;
    }
    
    return E_NO_ERROR;
}

int MXC_SPI_RevA_Shutdown (mxc_spi_reva_regs_t* spi)
{
    int spi_num;
    mxc_spi_reva_req_t * temp_req;
    spi_num = MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi);
    MXC_ASSERT (spi_num >= 0);
    
    //disable and clear interrupts
    spi->inten = 0;
    spi->intfl = spi->intfl;
    
    // Disable SPI and FIFOS
    spi->ctrl0 &= ~ (MXC_F_SPI_REVA_CTRL0_EN);
    spi->dma &= ~ (MXC_F_SPI_REVA_DMA_TX_FIFO_EN | MXC_F_SPI_REVA_DMA_RX_FIFO_EN);
    
    //call all of the pending callbacks for this spi
    spi_num = MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi);
    
    if (states[spi_num].req != NULL) {
        //save the request
        temp_req = states[spi_num].req;
        MXC_FreeLock((uint32_t*)(uint32_t*) &states[spi_num].req);
        
        // Callback if not NULL
        if (states[spi_num].req->completeCB != NULL) {
            states[spi_num].req->completeCB(temp_req, E_SHUTDOWN);
        }
        
    }
    
    // Clear registers
    spi->ctrl0 = 0;
    spi->ctrl1 = 0;
    spi->ctrl2 = 0;
    spi->sstime = 0;
    
    return E_NO_ERROR;
}

int MXC_SPI_RevA_ReadyForSleep (mxc_spi_reva_regs_t* spi)
{
    MXC_ASSERT (MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi) >= 0);
    
    if (spi->stat & MXC_F_SPI_REVA_STAT_BUSY || (spi->dma & MXC_F_SPI_REVA_DMA_TX_LVL) || (spi->dma & MXC_F_SPI_REVA_DMA_RX_LVL)) {
        return E_BUSY;
    }
    else {
        return E_NO_ERROR;
    }
}

int MXC_SPI_RevA_SetFrequency (mxc_spi_reva_regs_t* spi, unsigned int hz)
{
    int hi_clk, lo_clk, scale;
    uint32_t freq_div;
    
    // Check if frequency is too high
    if (hz > PeripheralClock) {
        return E_BAD_PARAM;
    }
    
    // Set the clock high and low
    freq_div = MXC_SPI_GetPeripheralClock((mxc_spi_regs_t*) spi);
  #if TARGET_NUM == 32570 || TARGET_NUM == 32680
    freq_div = (freq_div / hz / 2);
  #else
    freq_div = (freq_div / hz);
  #endif

    hi_clk = freq_div / 2;
    lo_clk = freq_div / 2;
    scale = 0;
    
    if (freq_div % 2) {
        hi_clk += 1;
    }
    
    while (hi_clk >= 16 && scale < 8) {
        hi_clk /= 2;
        lo_clk /= 2;
        scale ++;
    }
    
    if (scale == 8) {
        lo_clk = 15;
        hi_clk = 15;
    }
    
    MXC_SETFIELD (spi->clkctrl, MXC_F_SPI_REVA_CLKCTRL_LO, (lo_clk << MXC_F_SPI_REVA_CLKCTRL_LO_POS));
    MXC_SETFIELD (spi->clkctrl, MXC_F_SPI_REVA_CLKCTRL_HI, (hi_clk << MXC_F_SPI_REVA_CLKCTRL_HI_POS));
    MXC_SETFIELD (spi->clkctrl, MXC_F_SPI_REVA_CLKCTRL_CLKDIV, (scale << MXC_F_SPI_REVA_CLKCTRL_CLKDIV_POS));
    
    return E_NO_ERROR;
}

unsigned int MXC_SPI_RevA_GetFrequency (mxc_spi_reva_regs_t* spi)
{
    if (MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi) < 0) {
        return E_BAD_PARAM;
    }
    
    unsigned scale, lo_clk, hi_clk;
    
    scale = (spi->clkctrl & MXC_F_SPI_REVA_CLKCTRL_CLKDIV) >> MXC_F_SPI_REVA_CLKCTRL_CLKDIV_POS;
    hi_clk= (spi->clkctrl & MXC_F_SPI_REVA_CLKCTRL_HI) >> MXC_F_SPI_REVA_CLKCTRL_HI_POS;
    lo_clk= (spi->clkctrl & MXC_F_SPI_REVA_CLKCTRL_LO) >> MXC_F_SPI_REVA_CLKCTRL_LO_POS;
    
    return (PeripheralClock / (1 << scale)) / (lo_clk + hi_clk);
}

int MXC_SPI_RevA_SetDataSize (mxc_spi_reva_regs_t* spi, int dataSize)
{
    int spi_num;
    
    // HW has problem with these two character sizes
    if (dataSize == 1 || dataSize > 16) {
        return E_BAD_PARAM;
    }
    
    spi_num = MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi);
    MXC_ASSERT (spi_num >= 0);
    
    // Setup the character size
    if (! (spi->stat & MXC_F_SPI_REVA_STAT_BUSY) && states[spi_num].ssDeassert == 1) {
        //disable spi to change transfer size
        spi->ctrl0 &= ~ (MXC_F_SPI_REVA_CTRL0_EN);
        // set bit size
        states[spi_num].last_size = dataSize;
        
        if (dataSize < 16) {
            MXC_SETFIELD (spi->ctrl2, MXC_F_SPI_REVA_CTRL2_NUMBITS, dataSize << MXC_F_SPI_REVA_CTRL2_NUMBITS_POS);
        }
        
        else {
            MXC_SETFIELD (spi->ctrl2, MXC_F_SPI_REVA_CTRL2_NUMBITS, 0 << MXC_F_SPI_REVA_CTRL2_NUMBITS_POS);    //may not be neccessary
        }
        
        //enable spi to change transfer size
        spi->ctrl0 |= (MXC_F_SPI_REVA_CTRL0_EN);
    }
    else {
        return E_BAD_STATE;
    }
    
    return E_NO_ERROR;
}

int MXC_SPI_RevA_GetDataSize (mxc_spi_reva_regs_t* spi)
{
    int spi_num;
    spi_num = MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi);
    MXC_ASSERT (spi_num >= 0);
    (void)spi_num;
    
    if (!(spi->ctrl2 & MXC_F_SPI_REVA_CTRL2_NUMBITS)) {
        return 16;
    }
    
    return (spi->ctrl2 & MXC_F_SPI_REVA_CTRL2_NUMBITS) >> MXC_F_SPI_REVA_CTRL2_NUMBITS_POS;
}

int MXC_SPI_RevA_SetSlave (mxc_spi_reva_regs_t* spi, int ssIdx)
{
    int spi_num;
    
    // HW has problem with these two character sizes
    if (ssIdx >= MXC_SPI_SS_INSTANCES) {
        return E_BAD_PARAM;
    }
    
    //check if in master mode
    if (! (spi->ctrl0 & MXC_F_SPI_REVA_CTRL0_MST_MODE)) {
        return E_BAD_STATE;
    }
    
    spi_num = MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi);
    MXC_ASSERT (spi_num >= 0);
    (void)spi_num;
    
    if (states[spi_num].drv_ssel) {
        // Setup the slave select
        // Activate chosen SS pin
        spi->ctrl0 |= (1 << ssIdx) << MXC_F_SPI_REVA_CTRL0_SS_ACTIVE_POS;   
    }
    return E_NO_ERROR;
}

int MXC_SPI_RevA_GetSlave (mxc_spi_reva_regs_t* spi)
{
    int spi_num = MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi);
    MXC_ASSERT (spi_num >= 0);
    (void)spi_num;
    
    return ( (spi->ctrl0 & MXC_F_SPI_REVA_CTRL0_SS_ACTIVE) >> MXC_F_SPI_REVA_CTRL0_SS_ACTIVE_POS) >> 1;
}

int MXC_SPI_RevA_SetWidth (mxc_spi_reva_regs_t* spi, mxc_spi_reva_width_t spiWidth)
{
    int spi_num;
    spi_num = MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi);
    MXC_ASSERT (spi_num >= 0);
    (void)spi_num;
    
    spi->ctrl2 &= ~ (MXC_F_SPI_REVA_CTRL2_THREE_WIRE | MXC_F_SPI_REVA_CTRL2_DATA_WIDTH);
    
    switch (spiWidth) {
    
    case SPI_REVA_WIDTH_3WIRE:
        spi->ctrl2 |= MXC_F_SPI_REVA_CTRL2_THREE_WIRE;
        break;
        
    case SPI_REVA_WIDTH_STANDARD:
        spi->ctrl2 |= MXC_S_SPI_REVA_CTRL2_DATA_WIDTH_MONO;
        break;
        
    case SPI_REVA_WIDTH_DUAL:
        spi->ctrl2 |= MXC_S_SPI_REVA_CTRL2_DATA_WIDTH_DUAL;
        break;
        
    case SPI_REVA_WIDTH_QUAD:
        spi->ctrl2 |= MXC_S_SPI_REVA_CTRL2_DATA_WIDTH_QUAD;
        break;
    }
    
    return E_NO_ERROR;
}

mxc_spi_reva_width_t MXC_SPI_RevA_GetWidth (mxc_spi_reva_regs_t* spi)
{
    if (spi->ctrl2 & MXC_F_SPI_REVA_CTRL2_THREE_WIRE) {
        return SPI_REVA_WIDTH_3WIRE;
    }
    
    if (spi->ctrl2 & MXC_S_SPI_REVA_CTRL2_DATA_WIDTH_DUAL) {
        return SPI_REVA_WIDTH_DUAL;
    }
    
    if (spi->ctrl2 & MXC_S_SPI_REVA_CTRL2_DATA_WIDTH_QUAD) {
        return SPI_REVA_WIDTH_QUAD;
    }
    
    return SPI_REVA_WIDTH_STANDARD;
}

int MXC_SPI_RevA_SetMode (mxc_spi_reva_regs_t* spi, mxc_spi_reva_mode_t spiMode)
{
    int spi_num = MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi);
    MXC_ASSERT (spi_num >= 0);
    (void)spi_num;

    switch(spiMode) {
    case SPI_REVA_MODE_0:
        spi->ctrl2 &= ~MXC_F_SPI_REVA_CTRL2_CLKPHA;
        spi->ctrl2 &= ~MXC_F_SPI_REVA_CTRL2_CLKPOL;
        break;

    case SPI_REVA_MODE_1:
        spi->ctrl2 &= ~MXC_F_SPI_REVA_CTRL2_CLKPHA;
        spi->ctrl2 |=  MXC_F_SPI_REVA_CTRL2_CLKPOL;
        break;

    case SPI_REVA_MODE_2:
        spi->ctrl2 |=  MXC_F_SPI_REVA_CTRL2_CLKPHA;
        spi->ctrl2 &= ~MXC_F_SPI_REVA_CTRL2_CLKPOL;
        break;

    case SPI_REVA_MODE_3:
        spi->ctrl2 |=  MXC_F_SPI_REVA_CTRL2_CLKPHA;
        spi->ctrl2 |=  MXC_F_SPI_REVA_CTRL2_CLKPOL;
        break;

    default:
        break;                
    }

    return E_NO_ERROR;
}

mxc_spi_reva_mode_t MXC_SPI_RevA_GetMode (mxc_spi_reva_regs_t* spi)
{
    int spi_num = MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi);
    MXC_ASSERT (spi_num >= 0);
    (void)spi_num;

    if(spi->ctrl2 & MXC_F_SPI_REVA_CTRL2_CLKPHA) {
        if(spi->ctrl2 & MXC_F_SPI_REVA_CTRL2_CLKPOL) {
            return SPI_REVA_MODE_3;
        }
        else {
            return SPI_REVA_MODE_2;
        }
    }
    else {
        if(spi->ctrl2 & MXC_F_SPI_REVA_CTRL2_CLKPOL) {
            return SPI_REVA_MODE_1;
        }
    }

    return SPI_REVA_MODE_0;
}

int MXC_SPI_RevA_StartTransmission (mxc_spi_reva_regs_t* spi)
{
    int spi_num = MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi);
    MXC_ASSERT (spi_num >= 0);
    (void)spi_num;
    
    if (MXC_SPI_GetActive ((mxc_spi_regs_t*) spi) == E_BUSY) {
        return E_BUSY;
    }
    
    spi->ctrl0 |= MXC_F_SPI_REVA_CTRL0_START;
    
    return E_NO_ERROR;
}

int MXC_SPI_RevA_GetActive (mxc_spi_reva_regs_t* spi)
{
    if (spi->stat & MXC_F_SPI_REVA_STAT_BUSY) {
        return E_BUSY;
    }
    
    return E_NO_ERROR;
}

int MXC_SPI_RevA_AbortTransmission (mxc_spi_reva_regs_t* spi)
{
    int spi_num;
    spi_num = MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi);
    MXC_ASSERT (spi_num >= 0);

    // Disable interrupts, clear the flags
    spi->inten = 0;
    spi->intfl = spi->intfl;
    
    // Reset the SPI17Y to cancel the on ongoing transaction
    spi->ctrl0 &= ~ (MXC_F_SPI_REVA_CTRL0_EN);
    spi->ctrl0 |= (MXC_F_SPI_REVA_CTRL0_EN);
    
    // Unlock this SPI
    mxc_spi_reva_req_t * temp = states[spi_num].req;
    MXC_FreeLock ((uint32_t*) &states[spi_num].req);
    
    // Callback if not NULL
    if (temp->completeCB != NULL) {
        temp->completeCB(states[spi_num].req, E_ABORT);
    }
    
    return E_NO_ERROR;
}

unsigned int MXC_SPI_RevA_ReadRXFIFO (mxc_spi_reva_regs_t* spi, unsigned char* bytes,
                                      unsigned int len)
{
    unsigned rx_avail,bits;
    MXC_ASSERT (MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi) >= 0);
    
    if (!bytes || !len) {
        return 0;
    }
    
    rx_avail = MXC_SPI_GetRXFIFOAvailable ((mxc_spi_regs_t*) spi);
    bits = MXC_SPI_GetDataSize ((mxc_spi_regs_t*) spi);
    
    if (len > rx_avail) {
        len = rx_avail;
    }
    
    if (bits > 8) {
        len &= ~(unsigned) 0x1;
    }
    
    unsigned cnt = 0;
    
    if (bits <= 8 || len >= 2) {
        // Read from the FIFO
        while (len) {
            if (len > 3) {
                memcpy(& ((uint8_t*) bytes) [cnt], (void*) &spi->fifo32, 4);
                len -= 4;
                cnt += 4;
            }
            else if (len > 1) {
                memcpy(& ((uint8_t*) bytes) [cnt], (void*) &spi->fifo16[0], 2);
                len -= 2;
                cnt += 2;
                
            }
            else {
                ((uint8_t*) bytes) [cnt++] = spi->fifo8[0];
                len -= 1;
            }
            
            // Don't read less than 2 bytes if we are using greater than 8 bit characters
            if (len == 1 && bits > 8) {
                break;
            }
        }
    }
    
    return cnt;
}

unsigned int MXC_SPI_RevA_GetRXFIFOAvailable (mxc_spi_reva_regs_t* spi)
{
    return (spi->dma & MXC_F_SPI_REVA_DMA_RX_LVL) >> MXC_F_SPI_REVA_DMA_RX_LVL_POS;
}

unsigned int MXC_SPI_RevA_WriteTXFIFO (mxc_spi_reva_regs_t* spi, unsigned char* bytes,
                                       unsigned int len)
{
    unsigned tx_avail,bits;
    MXC_ASSERT (MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi) >= 0);
    
    if (!bytes || !len) {
        return 0;
    }
    
    tx_avail = MXC_SPI_GetTXFIFOAvailable ((mxc_spi_regs_t*) spi);
    bits = MXC_SPI_GetDataSize ((mxc_spi_regs_t*) spi);
    
    if (len > tx_avail) {
        len = tx_avail;
    }
    
    if (bits > 8) {
        len &= ~(unsigned) 0x1;
    }
    
    unsigned cnt = 0;
    
    while (len) {
        if (len > 3) {
            memcpy((void*) &spi->fifo32, & ((uint8_t*) bytes) [cnt], 4);
            
            len -= 4;
            cnt += 4;
            
        }
        else if (len > 1) {
            memcpy((void*) &spi->fifo16[0], & ((uint8_t*) bytes) [cnt], 2);
            
            len -= 2;
            cnt += 2;
            
        }
        else if (bits <= 8) {
            spi->fifo8[0] = ((uint8_t*) bytes) [cnt++];
            len--;
        }
        
    }
    
    return cnt;
}

unsigned int MXC_SPI_RevA_GetTXFIFOAvailable (mxc_spi_reva_regs_t* spi)
{
    MXC_ASSERT (MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi) >= 0);
    return MXC_SPI_FIFO_DEPTH - ((spi->dma & MXC_F_SPI_REVA_DMA_TX_LVL) >> MXC_F_SPI_REVA_DMA_TX_LVL_POS);
}

void MXC_SPI_RevA_ClearRXFIFO (mxc_spi_reva_regs_t* spi)
{
    MXC_ASSERT (MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi) >= 0);
    spi->dma |= MXC_F_SPI_REVA_DMA_RX_FLUSH;
}

void MXC_SPI_RevA_ClearTXFIFO (mxc_spi_reva_regs_t* spi)
{
    MXC_ASSERT (MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi) >= 0);
    spi->dma |= MXC_F_SPI_REVA_DMA_TX_FLUSH;
}

int MXC_SPI_RevA_SetRXThreshold (mxc_spi_reva_regs_t* spi, unsigned int numBytes)
{
    MXC_ASSERT (MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi) >= 0);
    
    if (numBytes > 32) {
        return E_BAD_PARAM;
    }
    
    MXC_SETFIELD (spi->dma, MXC_F_SPI_REVA_DMA_RX_THD_VAL, numBytes << MXC_F_SPI_REVA_DMA_RX_THD_VAL_POS);
    
    return E_NO_ERROR;
}

unsigned int MXC_SPI_RevA_GetRXThreshold (mxc_spi_reva_regs_t* spi)
{
    MXC_ASSERT (MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi) >= 0);
    return (spi->dma & MXC_F_SPI_REVA_DMA_RX_THD_VAL) >> MXC_F_SPI_REVA_DMA_RX_THD_VAL_POS;
}

int MXC_SPI_RevA_SetTXThreshold (mxc_spi_reva_regs_t* spi, unsigned int numBytes)
{
    MXC_ASSERT (MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi) >= 0);
    
    if (numBytes > 32) {
        return E_BAD_PARAM;
    }
    
    MXC_SETFIELD (spi->dma, MXC_F_SPI_REVA_DMA_TX_THD_VAL, numBytes << MXC_F_SPI_REVA_DMA_TX_THD_VAL_POS);
    
    return E_NO_ERROR;
}

unsigned int MXC_SPI_RevA_GetTXThreshold (mxc_spi_reva_regs_t* spi)
{
    MXC_ASSERT (MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi) >= 0);
    return (spi->dma & MXC_F_SPI_REVA_DMA_TX_THD_VAL) >> MXC_F_SPI_REVA_DMA_TX_THD_VAL_POS;
}

unsigned int MXC_SPI_RevA_GetFlags (mxc_spi_reva_regs_t* spi)
{
    MXC_ASSERT (MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi) >= 0);
    return spi->intfl;
}

void MXC_SPI_RevA_ClearFlags (mxc_spi_reva_regs_t* spi)
{
    MXC_ASSERT (MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi) >= 0);
    spi->intfl = spi->intfl;
}

void MXC_SPI_RevA_EnableInt (mxc_spi_reva_regs_t* spi, unsigned int intEn)
{
    MXC_ASSERT (MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi) >= 0);
    spi->inten |= intEn;
}

void MXC_SPI_RevA_DisableInt (mxc_spi_reva_regs_t* spi, unsigned int intDis)
{
    MXC_ASSERT (MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi) >= 0);
    spi->inten &= ~ (intDis);
}

int MXC_SPI_RevA_TransSetup (mxc_spi_reva_req_t * req)
{
    int spi_num;
    uint8_t bits;
    
    if ((!req) || ((req->txData == NULL) && (req->rxData == NULL))) {
        return E_BAD_PARAM;
    }
    
    // Setup the number of characters to transact
    if (req->txLen > (MXC_F_SPI_REVA_CTRL1_TX_NUM_CHAR >> MXC_F_SPI_REVA_CTRL1_TX_NUM_CHAR_POS)) {
        return E_BAD_PARAM;
    }
    
    spi_num = MXC_SPI_GET_IDX ((mxc_spi_regs_t*)(req->spi));
    MXC_ASSERT (spi_num >= 0);
    MXC_ASSERT (req->ssIdx < MXC_SPI_SS_INSTANCES);
    
    bits = MXC_SPI_GetDataSize ((mxc_spi_regs_t*) req->spi);
    req->txCnt = 0;
    req->rxCnt = 0;
    
    states[spi_num].req = req;
    states[spi_num].started = 0;
    
    // HW requires disabling/renabling SPI block at end of each transaction (when SS is inactive).
    if (states[spi_num].ssDeassert == 1) {
        (req->spi)->ctrl0 &= ~ (MXC_F_SPI_REVA_CTRL0_EN);
    }
    
    //if  master
    if ((req->spi)->ctrl0 & MXC_F_SPI_REVA_CTRL0_MST_MODE) {
        // Setup the slave select
        MXC_SPI_SetSlave ((mxc_spi_regs_t*) req->spi, req->ssIdx);
        
    }
    
    if (req->rxData != NULL && req->rxLen > 0) {
        MXC_SETFIELD ( (req->spi)->ctrl1, MXC_F_SPI_REVA_CTRL1_RX_NUM_CHAR,
                       req->rxLen << MXC_F_SPI_REVA_CTRL1_RX_NUM_CHAR_POS);
        (req->spi)->dma |= MXC_F_SPI_REVA_DMA_RX_FIFO_EN;
    }
    else {
        (req->spi)->ctrl1 &= ~ (MXC_F_SPI_REVA_CTRL1_RX_NUM_CHAR);
        (req->spi)->dma &= ~ (MXC_F_SPI_REVA_DMA_RX_FIFO_EN);
    }
    
    // Must use TXFIFO and NUM in full duplex//start  editing here
    if ((mxc_spi_reva_width_t) MXC_SPI_GetWidth ((mxc_spi_regs_t*) req->spi) == SPI_REVA_WIDTH_STANDARD
            && ! ( ( (req->spi)->ctrl2 & MXC_F_SPI_REVA_CTRL2_THREE_WIRE) >> MXC_F_SPI_REVA_CTRL2_THREE_WIRE_POS)) {
        if (req->txData == NULL) {
            // Must have something to send, so we'll use the rx_data buffer initialized to 0.
            //SPI_SetDefaultTXData(spi, 0);
            memset(req->rxData, states[spi_num].defaultTXData, (bits > 8 ? req->rxLen << 1 : req->rxLen));
            req->txData = req->rxData;
            req->txLen = req->rxLen;
        }
    }
        
    if(req->txData != NULL && req->txLen > 0) {
        MXC_SETFIELD ( (req->spi)->ctrl1, MXC_F_SPI_REVA_CTRL1_TX_NUM_CHAR,
                       req->txLen << MXC_F_SPI_REVA_CTRL1_TX_NUM_CHAR_POS);
        (req->spi)->dma |= MXC_F_SPI_REVA_DMA_TX_FIFO_EN;
    }
    else {
        (req->spi)->ctrl1 &= ~(MXC_F_SPI_REVA_CTRL1_TX_NUM_CHAR);
        (req->spi)->dma &= ~(MXC_F_SPI_REVA_DMA_TX_FIFO_EN);
    }
    
    (req->spi)->dma |= (MXC_F_SPI_REVA_DMA_TX_FLUSH | MXC_F_SPI_REVA_DMA_RX_FLUSH);
    (req->spi)->ctrl0 |= (MXC_F_SPI_REVA_CTRL0_EN);
    
    states[spi_num].ssDeassert = req->ssDeassert;
    // Clear master done flag
    (req->spi)->intfl = MXC_F_SPI_REVA_INTFL_MST_DONE;
    
    
    return E_NO_ERROR;
}

uint32_t MXC_SPI_RevA_MasterTransHandler (mxc_spi_reva_regs_t *spi, mxc_spi_reva_req_t *req)
{
    uint32_t retval;
    int spi_num;
    
    spi_num = MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi);
    
    // Leave slave select asserted at the end of the transaction
    if (!req->ssDeassert) {
        spi->ctrl0 |= MXC_F_SPI_REVA_CTRL0_SS_CTRL;
    }
    
    retval = MXC_SPI_RevA_TransHandler(spi, req);
    
    if (!states[spi_num].started) {
        MXC_SPI_StartTransmission ((mxc_spi_regs_t*) spi);
        states[spi_num].started = 1;
    }
    
    // Deassert slave select at the end of the transaction
    if (req->ssDeassert) {
        spi->ctrl0 &= ~MXC_F_SPI_REVA_CTRL0_SS_CTRL;
    }
    
    return retval;
}

uint32_t MXC_SPI_RevA_SlaveTransHandler (mxc_spi_reva_req_t *req)
{
    return MXC_SPI_RevA_TransHandler(req->spi, req);
}

uint32_t MXC_SPI_RevA_TransHandler (mxc_spi_reva_regs_t *spi, mxc_spi_reva_req_t *req)
{
    int remain, spi_num;
    uint32_t int_en = 0;
    uint32_t tx_length = 0, rx_length = 0;
    uint8_t bits;
    spi_num = MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi);
    if (spi_num < 0) {
        MXC_ASSERT(0);
    }

    bits = MXC_SPI_GetDataSize ((mxc_spi_regs_t*) req->spi);
    
    //MXC_F_SPI_REVA_CTRL2_NUMBITS data bits
    // Read/write 2x number of bytes if larger character size
    if (bits > 8) {
        tx_length = req->txLen * 2;
        rx_length = req->rxLen * 2;
    }
    else {
        tx_length = req->txLen;
        rx_length = req->rxLen;
    }
    
    if (req->txData != NULL) {
    
        req->txCnt += MXC_SPI_WriteTXFIFO ((mxc_spi_regs_t*) spi, & (req->txData[req->txCnt]),tx_length - req->txCnt);
    }
    
    remain = tx_length - req->txCnt;
    
    // Set the TX interrupts
    // Write the FIFO //starting here
    if (remain) {
        if (remain > MXC_SPI_FIFO_DEPTH) {
            MXC_SPI_SetTXThreshold ((mxc_spi_regs_t*) spi,MXC_SPI_FIFO_DEPTH);
        }
        else {
            MXC_SPI_SetTXThreshold ((mxc_spi_regs_t*) spi,remain);
        }
        
        int_en |= MXC_F_SPI_REVA_INTEN_TX_THD;
        
    }
    // Break out if we've transmitted all the bytes and not receiving
    if ((req->rxData == NULL) && (req->txCnt == tx_length)) {
        spi->inten = 0;
        int_en = 0;
        MXC_FreeLock((uint32_t*) &states[spi_num].req);
        
        // Callback if not NULL
        if (async && req->completeCB != NULL) {
            req->completeCB(req, E_NO_ERROR);
        }
    }

    // Read the RX FIFO
    if (req->rxData != NULL) {
    
        req->rxCnt += MXC_SPI_ReadRXFIFO ((mxc_spi_regs_t*) spi,& (req->rxData[req->rxCnt]),rx_length - req->rxCnt);
        
        
        remain = rx_length - req->rxCnt;
        
        if (remain) {
            if (remain > MXC_SPI_FIFO_DEPTH) {
                MXC_SPI_SetRXThreshold ((mxc_spi_regs_t*) spi, 2);
            }
            else {
                MXC_SPI_SetRXThreshold ((mxc_spi_regs_t*) spi, remain - 1);
            }
            
            int_en |= MXC_F_SPI_REVA_INTEN_RX_THD;
        }
        
        // Break out if we've received all the bytes and we're not transmitting
        if ((req->txData == NULL) && (req->rxCnt == rx_length)) {
            spi->inten = 0;
            int_en = 0;
            MXC_FreeLock((uint32_t*) &states[spi_num].req);
            
            // Callback if not NULL
            if (async && req->completeCB != NULL) {
                req->completeCB(req, E_NO_ERROR);
            }
        }
    }
    // Break out once we've transmitted and received all of the data
    if ((req->rxCnt == rx_length)  && (req->txCnt == tx_length)) {
        spi->inten = 0;
        int_en = 0;
        MXC_FreeLock((uint32_t*) &states[spi_num].req);
        
        // Callback if not NULL
        if (async && req->completeCB != NULL) {
            req->completeCB(req, E_NO_ERROR);
        }
    }
    
    return int_en;
}

int MXC_SPI_RevA_MasterTransaction (mxc_spi_reva_req_t* req)
{
    int error;
    
    if ((error = MXC_SPI_RevA_TransSetup(req)) != E_NO_ERROR) {
        return error;
    }
    
    async = 0;
    
    //call master transHandler
    while (MXC_SPI_RevA_MasterTransHandler(req->spi, req) != 0);
    
    while (!((req->spi)->intfl & MXC_F_SPI_REVA_INTFL_MST_DONE));
    
    return E_NO_ERROR;
}

int MXC_SPI_RevA_MasterTransactionAsync (mxc_spi_reva_req_t* req)
{
    int error;
    
    if ((error = MXC_SPI_RevA_TransSetup(req)) != E_NO_ERROR) {
        return error;
    }
    
    async = 1;
    
    MXC_SPI_EnableInt ((mxc_spi_regs_t*) req->spi, MXC_SPI_RevA_MasterTransHandler (req->spi,req));
    
    return E_NO_ERROR;
}

int MXC_SPI_RevA_MasterTransactionDMA (mxc_spi_reva_req_t* req, int reqselTx, int reqselRx, mxc_dma_regs_t* dma)
{
    int spi_num;
    uint8_t channel,error,bits;
    mxc_dma_config_t config;
    mxc_dma_srcdst_t srcdst;
    mxc_dma_adv_config_t advConfig = {0, 0, 0, 0, 0, 0};

    spi_num = MXC_SPI_GET_IDX ((mxc_spi_regs_t*)(req->spi));
    MXC_ASSERT (spi_num >= 0);

    if (req->txData == NULL && req->rxData == NULL) {
        return E_BAD_PARAM;
    }

    if ((error = MXC_SPI_RevA_TransSetup (req)) != E_NO_ERROR) {
        return error;
    }

    // Leave slave select asserted at the end of the transaction
    if (!req->ssDeassert) {
        req->spi->ctrl0 |= MXC_F_SPI_REVA_CTRL0_SS_CTRL;
    }
    
    bits = MXC_SPI_GetDataSize ((mxc_spi_regs_t*) req->spi);
    
    MXC_SPI_RevA_TransHandler(req->spi, req);
    
    if (bits <= 8) {
        MXC_SPI_SetTXThreshold ((mxc_spi_regs_t*) req->spi, 1);       //set threshold to 1 byte
        MXC_SPI_SetRXThreshold ((mxc_spi_regs_t*) req->spi, 0);       //set threshold to 0 bytes
    }
    else {
        MXC_SPI_SetTXThreshold ((mxc_spi_regs_t*) req->spi, 2);
        MXC_SPI_SetRXThreshold ((mxc_spi_regs_t*) req->spi, 0);
    }
    
  #if TARGET_NUM == 32665
    MXC_DMA_Init(dma);
  #else
    MXC_DMA_Init();
  #endif
    
    //tx
    if (req->txData != NULL) {
      #if TARGET_NUM == 32665
        channel = MXC_DMA_AcquireChannel(dma);
      #else
        channel = MXC_DMA_AcquireChannel();
      #endif

        config.reqsel = reqselTx;
        config.ch = channel;
        advConfig.ch = channel;
        advConfig.burst_size = 2;
        
        if (bits <= 8) {
            config.srcwd = MXC_DMA_WIDTH_BYTE;
            config.dstwd = MXC_DMA_WIDTH_BYTE;
        }
        else {
            config.srcwd = MXC_DMA_WIDTH_HALFWORD;
            config.dstwd = MXC_DMA_WIDTH_HALFWORD;
        }
        
        config.srcinc_en = 1;
        config.dstinc_en = 0;
        
        srcdst.ch = channel;
        srcdst.source = & (req->txData[req->txCnt]);
        
        if (bits > 8) {
            srcdst.len = (req->txLen * 2) - req->txCnt;
        }
        else {
            srcdst.len = (req->txLen) - req->txCnt;
        }
        
        states[spi_num].channelTx = channel;
        MXC_DMA_ConfigChannel (config,srcdst);
        MXC_DMA_SetCallback (channel, MXC_SPI_RevA_DMACallback);
        MXC_DMA_EnableInt (channel);
        MXC_DMA_Start (channel);
        MXC_DMA_SetChannelInterruptEn(channel, false, true);
        //MXC_DMA->ch[channel].ctrl |= MXC_F_DMA_CTRL_CTZ_IE;

        if (bits > 8) {
            MXC_DMA_AdvConfigChannel(advConfig);
            //MXC_SETFIELD (MXC_DMA->ch[channel].ctrl, MXC_F_DMA_CTRL_BURST_SIZE, 1 << MXC_F_DMA_CTRL_BURST_SIZE_POS);
        }
    }
    
    if (req->rxData != NULL) {
      #if TARGET_NUM == 32665
        channel = MXC_DMA_AcquireChannel(dma);
      #else
        channel = MXC_DMA_AcquireChannel();
      #endif

        config.reqsel = reqselRx;
        config.ch = channel;
        config.srcinc_en = 0;
        config.dstinc_en = 1;
        advConfig.ch = channel;
        advConfig.burst_size = 1;
        
        if (bits <= 8) {
            config.srcwd = MXC_DMA_WIDTH_BYTE;
            config.dstwd = MXC_DMA_WIDTH_BYTE;
        }
        else {
            config.srcwd = MXC_DMA_WIDTH_HALFWORD;
            config.dstwd = MXC_DMA_WIDTH_HALFWORD;
        }
        
        
        srcdst.ch = channel;
        srcdst.dest = req->rxData;
        
        if (bits <= 8) {
            srcdst.len = req->rxLen;
        }
        else {
            srcdst.len = req->rxLen * 2;
        }
        
        states[spi_num].channelRx = channel;
        
        MXC_DMA_ConfigChannel (config,srcdst);
        MXC_DMA_SetCallback (channel, MXC_SPI_RevA_DMACallback);
        MXC_DMA_EnableInt (channel);
        MXC_DMA_Start (channel);
        MXC_DMA_SetChannelInterruptEn(channel, false, true);
        //MXC_DMA->ch[channel].ctrl |= MXC_F_DMA_CTRL_CTZ_IE;

        if (bits > 8) {
            MXC_DMA_AdvConfigChannel(advConfig);
            //MXC_SETFIELD (MXC_DMA->ch[channel].ctrl, MXC_F_DMA_CTRL_BURST_SIZE, 0 << MXC_F_DMA_CTRL_BURST_SIZE_POS);
        }
        
    }
    
    (req->spi)->dma |= (MXC_F_SPI_REVA_DMA_DMA_TX_EN | MXC_F_SPI_REVA_DMA_DMA_RX_EN);
    
    
    if (!states[spi_num].started) {
        MXC_SPI_StartTransmission ((mxc_spi_regs_t*) req->spi);
        states[spi_num].started = 1;
    }
    
    // Deassert slave select at the end of the transaction
    if (req->ssDeassert) {
        req->spi->ctrl0 &= ~MXC_F_SPI_REVA_CTRL0_SS_CTRL;
    }
    
    return E_NO_ERROR;
}

int MXC_SPI_RevA_SlaveTransaction (mxc_spi_reva_req_t* req)
{
    int error;
    
    if ((error = MXC_SPI_RevA_TransSetup(req)) != E_NO_ERROR) {
        return error;
    }
    
    
    async = 0;
    
    while (MXC_SPI_RevA_SlaveTransHandler(req) != 0);
    
    return E_NO_ERROR;
}

int MXC_SPI_RevA_SlaveTransactionAsync (mxc_spi_reva_req_t* req)
{
    int error;
    
    if ((error = MXC_SPI_RevA_TransSetup(req)) != E_NO_ERROR) {
        return error;
    }
    
    async = 1;
    
    MXC_SPI_EnableInt ((mxc_spi_regs_t*) req->spi,MXC_SPI_RevA_SlaveTransHandler (req));
    
    return E_NO_ERROR;
}

int MXC_SPI_RevA_SlaveTransactionDMA (mxc_spi_reva_req_t* req, int reqselTx, int reqselRx, mxc_dma_regs_t* dma)
{
    int spi_num;
    uint8_t channel, error, bits;
    mxc_dma_config_t config;
    mxc_dma_srcdst_t srcdst;
    mxc_dma_adv_config_t advConfig = {0, 0, 0, 0, 0, 0};
    
    if (req->txData == NULL && req->rxData == NULL) {
        return E_BAD_PARAM;
    }
    
    if ((error = MXC_SPI_RevA_TransSetup(req)) != E_NO_ERROR) {
        return error;
    }
    
    bits = MXC_SPI_GetDataSize ((mxc_spi_regs_t*) req->spi);
    
    MXC_SPI_RevA_TransHandler(req->spi, req);
    
    if (bits <= 8) {
        MXC_SPI_SetTXThreshold ((mxc_spi_regs_t*) req->spi, 1);
        MXC_SPI_SetRXThreshold ((mxc_spi_regs_t*) req->spi, 0);
    }
    else {
    
        MXC_SPI_SetTXThreshold ((mxc_spi_regs_t*) req->spi, 2);
        MXC_SPI_SetRXThreshold ((mxc_spi_regs_t*) req->spi, 0);
    }
    
  #if TARGET_NUM == 32665
    MXC_DMA_Init(dma);
  #else
    MXC_DMA_Init();
  #endif
    
    spi_num = MXC_SPI_GET_IDX ((mxc_spi_regs_t*)(req->spi));
    MXC_ASSERT (spi_num >= 0);
    
    //tx
    if (req->txData != NULL) {
      #if TARGET_NUM == 32665
        channel = MXC_DMA_AcquireChannel(dma);
      #else
        channel = MXC_DMA_AcquireChannel();
      #endif

        config.reqsel = reqselTx;
        config.ch = channel;
        advConfig.ch = channel;
        advConfig.burst_size = 2;
        
        if (bits <= 8) {
            config.srcwd = MXC_DMA_WIDTH_BYTE;
            config.dstwd = MXC_DMA_WIDTH_BYTE;
        }
        else {
            config.srcwd = MXC_DMA_WIDTH_HALFWORD;
            config.dstwd = MXC_DMA_WIDTH_HALFWORD;
        }
        
        config.srcinc_en = 1;
        config.dstinc_en = 0;
        
        srcdst.ch = channel;
        srcdst.source = & (req->txData[req->txCnt]);
        
        if (bits > 8) {
            srcdst.len = (req->txLen * 2) - req->txCnt;
        }
        else {
            srcdst.len = (req->txLen) - req->txCnt;
        }
        
        states[spi_num].channelTx = channel;
        
        MXC_DMA_ConfigChannel (config,srcdst);
        MXC_DMA_SetCallback (channel, MXC_SPI_RevA_DMACallback);
        MXC_DMA_EnableInt (channel);
        MXC_DMA_Start (channel);
        MXC_DMA_SetChannelInterruptEn(channel, false, true);
        //MXC_DMA->ch[channel].ctrl |= MXC_F_DMA_CTRL_CTZ_IE;
        
        if (bits > 8) {
            MXC_DMA_AdvConfigChannel(advConfig);
            //MXC_SETFIELD (MXC_DMA->ch[channel].ctrl, MXC_F_DMA_CTRL_BURST_SIZE, 1 << MXC_F_DMA_CTRL_BURST_SIZE_POS);
        }
    }
    
    if (req->rxData != NULL) {
      #if TARGET_NUM == 32665
        channel = MXC_DMA_AcquireChannel(dma);
      #else
        channel = MXC_DMA_AcquireChannel();
      #endif

        config.reqsel = reqselRx;
        config.ch = channel;
        config.srcinc_en = 0;
        config.dstinc_en = 1;
        advConfig.ch = channel;
        advConfig.burst_size = 1;
        
        if (bits <= 8) {
            config.srcwd = MXC_DMA_WIDTH_BYTE;
            config.dstwd = MXC_DMA_WIDTH_BYTE;
        }
        else {
            config.srcwd = MXC_DMA_WIDTH_HALFWORD;
            config.dstwd = MXC_DMA_WIDTH_HALFWORD;
        }
        
        
        srcdst.ch = channel;
        srcdst.dest = req->rxData;
        
        if (bits <= 8) {
            srcdst.len = req->rxLen;
        }
        else {
            srcdst.len = req->rxLen * 2;
        }
        
        states[spi_num].channelRx = channel;
        
        MXC_DMA_ConfigChannel (config,srcdst);
        MXC_DMA_SetCallback (channel, MXC_SPI_RevA_DMACallback);
        MXC_DMA_EnableInt (channel);
        MXC_DMA_Start (channel);
        MXC_DMA_SetChannelInterruptEn(channel, false, true);
        //MXC_DMA->ch[channel].ctrl |= MXC_F_DMA_CTRL_CTZ_IE;
        
        if (bits > 8) {
            MXC_DMA_AdvConfigChannel(advConfig);
            //MXC_SETFIELD (MXC_DMA->ch[channel].ctrl, MXC_F_DMA_CTRL_BURST_SIZE, 0 << MXC_F_DMA_CTRL_BURST_SIZE_POS);
        }
    }
    
    (req->spi)->dma |= (MXC_F_SPI_REVA_DMA_DMA_TX_EN | MXC_F_SPI_REVA_DMA_DMA_RX_EN);
    
    return E_NO_ERROR;
}

void MXC_SPI_RevA_DMACallback(int ch, int error)
{
    mxc_spi_reva_req_t * temp_req;
    
    for (int i = 0; i < MXC_SPI_INSTANCES; i ++) {
        if (states[i].channelTx == ch) {
            //save the request
            temp_req = states[i].req;
            MXC_FreeLock((uint32_t*) &states[i].req);
            // Callback if not NULL
            if (temp_req->completeCB != NULL) {
                temp_req->completeCB(temp_req, E_NO_ERROR);
            }            
            break;
        }
        
        else if (states[i].channelRx == ch) {
            //save the request
            temp_req = states[i].req;
            MXC_FreeLock((uint32_t*) &states[i].req);
            
            if (MXC_SPI_GetDataSize ((mxc_spi_regs_t*) temp_req->spi) > 8) {
                MXC_SPI_RevA_SwapByte (temp_req->rxData, temp_req->rxLen);
            }

            // Callback if not NULL
            if (temp_req->completeCB != NULL) {
                temp_req->completeCB(temp_req, E_NO_ERROR);
            } 
                        
            break;
        }
    }
}

int MXC_SPI_RevA_SetDefaultTXData (mxc_spi_reva_regs_t* spi, unsigned int defaultTXData)
{
    int spi_num = MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi);
    MXC_ASSERT (spi_num >= 0);
    states[spi_num].defaultTXData = defaultTXData;
    return E_NO_ERROR;
}

void MXC_SPI_RevA_AbortAsync (mxc_spi_reva_regs_t* spi)
{
    MXC_SPI_AbortTransmission ((mxc_spi_regs_t*) spi);
}

void MXC_SPI_RevA_AsyncHandler (mxc_spi_reva_regs_t* spi)
{
    int spi_num;
    unsigned rx_avail;
    uint32_t flags;
    
    // Clear the interrupt flags
    spi->inten = 0;
    flags = spi->intfl;
    spi->intfl = flags;
    
    spi_num = MXC_SPI_GET_IDX ((mxc_spi_regs_t*) spi);
    
    // Figure out if this SPI has an active request
    if ( (states[spi_num].req != NULL) && (flags)) {
        if ( (spi->ctrl0 & MXC_F_SPI_REVA_CTRL0_MST_MODE) >> MXC_F_SPI_REVA_CTRL0_MST_MODE_POS) {
            do {
                spi->inten = MXC_SPI_RevA_MasterTransHandler(spi,  states[spi_num].req);
                rx_avail = MXC_SPI_RevA_GetRXFIFOAvailable(spi);
            }
            while (rx_avail > MXC_SPI_RevA_GetRXThreshold(spi));
            
        }
        else {
            do {
                spi->inten = MXC_SPI_RevA_SlaveTransHandler(states[spi_num].req);
                rx_avail = MXC_SPI_RevA_GetRXFIFOAvailable(spi);
            }
            while (rx_avail > MXC_SPI_RevA_GetRXThreshold(spi));
            
        }
    }
}

//call in DMA IRQHANDLER with rxData for transmissions with bits > 8
void MXC_SPI_RevA_SwapByte(uint8_t* arr, size_t length)
{
    MXC_ASSERT(arr != NULL);
    
    for (size_t i = 0 ; i < (length * 2); i+=2) {
        uint8_t tmp = arr[i];
        arr[i] = arr[i + 1];
        arr[i + 1] = tmp;
    }
}
