/* ****************************************************************************
 * Copyright (C) 2022 Maxim Integrated Products, Inc., All Rights Reserved.
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
#ifdef __CC_ARM  // Keil 
 #pragma diag_suppress 188  // enumerated type mixed with another type
#endif

#include <stdlib.h>
#include <string.h>
#include "mxc_sys.h"
#include "mxc_device.h"
#include "mxc_errors.h"
#include "mxc_assert.h"
#include "mxc_lock.h"

#include "dma.h"
#include "aes_regs.h"
#include "aes_key_regs.h"
#include "aes_revb.h"
#include "trng_revb.h"

/* **** Variable Declaration **** */
typedef struct {
    uint8_t enc;
    uint8_t channelRX;
    uint8_t channelTX;
    uint32_t remain;    
    uint32_t* inputText;
    uint32_t* outputText;
} mxc_aes_revb_dma_req_t;

static mxc_aes_revb_dma_req_t dma_state;

#define SWAP_BYTES(x) ((((x) >> 24) & 0x000000FF) | (((x) >>  8) & 0x0000FF00) | (((x) <<  8) & 0x00FF0000) | (((x) << 24) & 0xFF000000))

static void memcpy32r(uint32_t * dst, const uint32_t * src, unsigned int len)
{
  uint32_t * dstr = dst + (len/4) - 1;
  while (len) {
    *dstr = SWAP_BYTES(*src);
    dstr--;
    src++;
    len -= 4;
  }
}

int MXC_AES_RevB_Init(mxc_aes_revb_regs_t* aes)
{
  aes->ctrl = 0x00;

  while (MXC_AES_RevB_IsBusy(aes) != E_NO_ERROR);

  aes->ctrl |= MXC_F_AES_REVB_CTRL_EN;

  return E_NO_ERROR;
}

int MXC_AES_RevB_Shutdown (mxc_aes_revb_regs_t* aes)
{
    MXC_AES_RevB_FlushInputFIFO(aes);
	MXC_AES_RevB_FlushOutputFIFO(aes);

    while(MXC_AES_RevB_IsBusy(aes) != E_NO_ERROR) ;
    
    aes->ctrl = 0x00;

  return E_NO_ERROR;
}

int MXC_AES_RevB_IsBusy (mxc_aes_revb_regs_t* aes)
{
    if(aes->status & MXC_F_AES_REVB_STATUS_BUSY)
    {
        return E_BUSY;
    }

  return E_NO_ERROR;
}

void MXC_AES_RevB_SetKeySize(mxc_aes_revb_regs_t* aes, mxc_aes_revb_keys_t key)
{
    while(MXC_AES_IsBusy() != E_NO_ERROR);
    aes->ctrl |= key;
}

mxc_aes_keys_t MXC_AES_RevB_GetKeySize(mxc_aes_revb_regs_t* aes)
{
    return (aes->ctrl & MXC_F_AES_REVB_CTRL_KEY_SIZE);
}

void MXC_AES_RevB_FlushInputFIFO(mxc_aes_revb_regs_t* aes)
{
    while(MXC_AES_IsBusy() != E_NO_ERROR);
    aes->ctrl |= MXC_F_AES_REVB_CTRL_INPUT_FLUSH;
}

void MXC_AES_RevB_FlushOutputFIFO(mxc_aes_revb_regs_t* aes)
{
    while(MXC_AES_IsBusy() != E_NO_ERROR);
    aes->ctrl |= MXC_F_AES_REVB_CTRL_OUTPUT_FLUSH;
}

void MXC_AES_RevB_Start(mxc_aes_revb_regs_t* aes)
{
    while(MXC_AES_IsBusy() != E_NO_ERROR);  
    aes->ctrl |= MXC_F_AES_REVB_CTRL_START;  
}

void MXC_AES_RevB_EnableInt (mxc_aes_revb_regs_t* aes, uint32_t interrupt)
{
    aes->inten |= (interrupt & (MXC_F_AES_REVB_INTEN_DONE | MXC_F_AES_REVB_INTEN_KEY_CHANGE | \
                        MXC_F_AES_REVB_INTEN_KEY_ZERO | MXC_F_AES_REVB_INTEN_OV));
}

void MXC_AES_RevB_DisableInt (mxc_aes_revb_regs_t* aes, uint32_t interrupt)
{
    aes->inten &= ~(interrupt & (MXC_F_AES_REVB_INTEN_DONE | MXC_F_AES_REVB_INTEN_KEY_CHANGE | \
                        MXC_F_AES_REVB_INTEN_KEY_ZERO | MXC_F_AES_REVB_INTEN_OV));
}

uint32_t MXC_AES_RevB_GetFlags(mxc_aes_revb_regs_t* aes)
{
    return aes->intfl;
}

void MXC_AES_RevB_ClearFlags(mxc_aes_revb_regs_t* aes, uint32_t flags)
{
    aes->intfl = (flags & (MXC_F_AES_REVB_INTFL_DONE | MXC_F_AES_REVB_INTFL_KEY_CHANGE | \
                        MXC_F_AES_REVB_INTFL_KEY_ZERO | MXC_F_AES_REVB_INTFL_OV));
}

int MXC_AES_RevB_Generic(mxc_aes_revb_regs_t* aes, mxc_aes_revb_req_t* req)
{
    int i;
    int remain;

    if(req == NULL) {
        return E_NULL_PTR;
    }

    if(req->inputData == NULL || req->resultData == NULL) {
        return E_NULL_PTR;
    }

    if(req->length == 0) {
        return E_BAD_PARAM;
    }

    remain = req->length;
    
    MXC_AES_RevB_FlushInputFIFO(aes);
	MXC_AES_RevB_FlushOutputFIFO(aes);

    MXC_AES_RevB_SetKeySize(aes, req->keySize);

    while(MXC_AES_IsBusy() != E_NO_ERROR);
    
    MXC_SETFIELD (aes->ctrl, MXC_F_AES_REVB_CTRL_TYPE, req->encryption << MXC_F_AES_REVB_CTRL_TYPE_POS);

    while(remain/4)
    {
        for(i = 0; i < 4; i++) {
            aes->fifo = SWAP_BYTES(req->inputData[3-i]);
        }
        req->inputData += 4;

        while(!(aes->intfl & MXC_F_AES_REVB_INTFL_DONE));
        aes->intfl |= MXC_F_AES_REVB_INTFL_DONE;

        for(i = 0; i < 4; i++) {
            uint32_t tmp = aes->fifo;
            req->resultData[3-i] = SWAP_BYTES(tmp);
        }        
        req->resultData += 4;

        remain -= 4;
    }

    if(remain%4)
    {
        for(i = 0; i < remain; i++) {
            aes->fifo = SWAP_BYTES(req->inputData[remain-1-i]);
        }
        req->inputData += remain;

        // Pad last block with 0's
        for(i = remain; i < 4; i++) {
            aes->fifo = 0;
        }

        while(!(aes->intfl & MXC_F_AES_REVB_INTFL_DONE));
        aes->intfl |= MXC_F_AES_REVB_INTFL_DONE;

        for(i = 0; i < 4; i++) {
            uint32_t tmp = aes->fifo;
            req->resultData[3-i] = SWAP_BYTES(tmp);
        }        
        req->resultData += 4;
    }
    return E_NO_ERROR;
}

int MXC_AES_RevB_Encrypt(mxc_aes_revb_regs_t* aes, mxc_aes_revb_req_t* req)
{
    return MXC_AES_RevB_Generic(aes, req);
}

int MXC_AES_RevB_Decrypt(mxc_aes_revb_regs_t* aes, mxc_aes_revb_req_t* req)
{
    return MXC_AES_RevB_Generic(aes, req);
}

int MXC_AES_RevB_TXDMAConfig(void* src_addr, int len)
{
    uint8_t channel;
    mxc_dma_config_t config;
    mxc_dma_srcdst_t srcdst;

    if (src_addr == NULL) {
      return E_NULL_PTR;
    }

    if (len == 0) {
      return E_BAD_PARAM;
    }

    MXC_DMA_Init();

    channel = MXC_DMA_AcquireChannel();
    dma_state.channelTX = channel;

    config.reqsel = MXC_DMA_REQUEST_AESTX;
    
    config.ch = channel;
    
    config.srcwd = MXC_DMA_WIDTH_WORD;
    config.dstwd = MXC_DMA_WIDTH_WORD;
    
    config.srcinc_en = 1;
    config.dstinc_en = 0;
    
    srcdst.ch = channel;
    srcdst.source = src_addr;
    
    if(dma_state.enc == 1) {
        srcdst.len = 4;
    }
    else if(len > 4) {
        srcdst.len = 4;
    }
    else{
        srcdst.len = len;
    }

    MXC_DMA_ConfigChannel (config, srcdst);
    MXC_DMA_SetCallback (channel, MXC_AES_RevB_DMACallback);
    
    MXC_DMA_EnableInt (channel);
    MXC_DMA_Start (channel);
    //MXC_DMA->ch[channel].ctrl |= MXC_F_DMA_CTRL_CTZ_IE;
    MXC_DMA_SetChannelInterruptEn(channel, 0, 1);

    return E_NO_ERROR;
}

int MXC_AES_RevB_RXDMAConfig(void* dest_addr, int len)
{
    if (dest_addr == NULL) {
      return E_NULL_PTR;
    }

    if (len == 0) {
      return E_BAD_PARAM;
    }

    uint8_t channel;
    mxc_dma_config_t config;
    mxc_dma_srcdst_t srcdst;

    MXC_DMA_Init();

    channel = MXC_DMA_AcquireChannel();
    dma_state.channelRX = channel;

    config.reqsel = MXC_DMA_REQUEST_AESRX;
    
    config.ch = channel;
    
    config.srcwd = MXC_DMA_WIDTH_WORD;
    config.dstwd = MXC_DMA_WIDTH_WORD;
    
    config.srcinc_en = 0;
    config.dstinc_en = 1;
    
    srcdst.ch = channel;
    srcdst.dest = dest_addr;

    if(dma_state.enc == 0) {
        srcdst.len = 4;
    }
    else if(len > 4) {
        srcdst.len = 4;
    }
    else{
        srcdst.len = len;
    }
    
    MXC_DMA_ConfigChannel (config, srcdst);
    MXC_DMA_SetCallback (channel, MXC_AES_RevB_DMACallback);
    
    MXC_DMA_EnableInt (channel);
    MXC_DMA_Start (channel);
    //MXC_DMA->ch[channel].ctrl |= MXC_F_DMA_CTRL_CTZ_IE;
    MXC_DMA_SetChannelInterruptEn(channel, 0, 1);

    return E_NO_ERROR;
}

int MXC_AES_RevB_GenericAsync(mxc_aes_revb_regs_t* aes, mxc_aes_revb_req_t* req, uint8_t enc)
{
    if(req == NULL) {
        return E_NULL_PTR;
    }

    if(req->inputData == NULL || req->resultData == NULL) {
        return E_NULL_PTR;
    }

    if(req->length == 0) {
        return E_BAD_PARAM;
    }

    MXC_AES_RevB_FlushInputFIFO(aes);
	  MXC_AES_RevB_FlushOutputFIFO(aes);

    MXC_AES_RevB_SetKeySize(aes, req->keySize);

    MXC_AES_IsBusy();
    MXC_SETFIELD (aes->ctrl, MXC_F_AES_REVB_CTRL_TYPE, req->encryption << MXC_F_AES_REVB_CTRL_TYPE_POS);

    dma_state.enc = enc;
    dma_state.remain = req->length;
    dma_state.inputText = req->inputData;
    dma_state.outputText = req->resultData;

    aes->ctrl |= MXC_F_AES_REVB_CTRL_DMA_RX_EN;              //Enable AES DMA    
    aes->ctrl |= MXC_F_AES_REVB_CTRL_DMA_TX_EN;              //Enable AES DMA    

    if(MXC_AES_RevB_TXDMAConfig(dma_state.inputText, dma_state.remain) != E_NO_ERROR) {
        return E_BAD_PARAM;
    }

    return E_NO_ERROR;
}
 
int MXC_AES_RevB_EncryptAsync(mxc_aes_revb_regs_t* aes, mxc_aes_revb_req_t* req)
{
    return MXC_AES_RevB_GenericAsync(aes, req, 0);
}

int MXC_AES_RevB_DecryptAsync(mxc_aes_revb_regs_t* aes, mxc_aes_revb_req_t* req)
{
    return MXC_AES_RevB_GenericAsync(aes, req, 1);
}

void MXC_AES_RevB_DMACallback(int ch, int error)
{
    if(error != E_NO_ERROR) {

    } 
    else {
      if (dma_state.channelTX == ch) {
        MXC_DMA_ReleaseChannel(dma_state.channelTX);
        if (dma_state.remain < 4) {
          MXC_AES_Start();
        }
        MXC_AES_RevB_RXDMAConfig(dma_state.outputText, dma_state.remain);
        } 
      else if (dma_state.channelRX == ch) {
        if (dma_state.remain > 4) {
          dma_state.remain -= 4;
        } else if (dma_state.remain > 0) {
          dma_state.remain = 0;
        }
        MXC_DMA_ReleaseChannel(dma_state.channelRX);
        if (dma_state.remain > 0) {
          MXC_AES_RevB_TXDMAConfig(dma_state.inputText, dma_state.remain);
        }
      }
    }
}

void MXC_AES_RevB_SetExtKey(mxc_aes_key_revb_regs_t* aeskey, const void* key, mxc_aes_keys_t len)
{
  int numBytes;

  if(len == MXC_AES_128BITS) {
    numBytes = 16;
  } else if (len == MXC_AES_192BITS) {
    numBytes = 24;
  } else {
    numBytes = 32;
  }

  /* TODO: Figure out if this is the correct byte ordering */
  memcpy32r((void*)&(aeskey->aes_key0), key, numBytes);
}
