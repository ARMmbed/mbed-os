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
 *************************************************************************** */

/****** Includes *******/
#include "dma_reva_regs.h"
#include <stdbool.h>

/***** Definitions *****/

/******* Globals *******/

/****** Functions ******/
int MXC_DMA_RevA_Init(mxc_dma_reva_regs_t *dma);
int MXC_DMA_RevA_AcquireChannel(mxc_dma_reva_regs_t* dma);
int MXC_DMA_RevA_ReleaseChannel(int ch);
int MXC_DMA_RevA_ConfigChannel(mxc_dma_config_t config, mxc_dma_srcdst_t srcdst);
int MXC_DMA_RevA_AdvConfigChannel(mxc_dma_adv_config_t advConfig);
int MXC_DMA_RevA_SetSrcDst(mxc_dma_srcdst_t srcdst);
int MXC_DMA_RevA_GetSrcDst(mxc_dma_srcdst_t *srcdst);
int MXC_DMA_RevA_SetSrcReload(mxc_dma_srcdst_t srcdst);
int MXC_DMA_RevA_GetSrcReload(mxc_dma_srcdst_t *srcdst);
int MXC_DMA_RevA_SetCallback(int ch, void(*callback)(int, int));
int MXC_DMA_RevA_SetChannelInterruptEn(int ch, bool chdis, bool ctz);
int MXC_DMA_RevA_ChannelEnableInt(int ch, int flags);
int MXC_DMA_RevA_ChannelDisableInt(int ch, int flags);
int MXC_DMA_RevA_ChannelGetFlags(int ch);
int MXC_DMA_RevA_ChannelClearFlags(int ch, int flags);
int MXC_DMA_RevA_EnableInt(mxc_dma_reva_regs_t *dma, int ch);
int MXC_DMA_RevA_DisableInt(mxc_dma_reva_regs_t *dma, int ch);
int MXC_DMA_RevA_Start(int ch);
int MXC_DMA_RevA_Stop(int ch);
mxc_dma_ch_regs_t* MXC_DMA_RevA_GetCHRegs(int ch);
void MXC_DMA_RevA_Handler(mxc_dma_reva_regs_t *dma);
int MXC_DMA_RevA_MemCpy(mxc_dma_reva_regs_t* dma, void* dest, void* src, int len, mxc_dma_complete_cb_t callback);
int MXC_DMA_RevA_DoTransfer(mxc_dma_reva_regs_t* dma, mxc_dma_config_t config, mxc_dma_srcdst_t firstSrcDst, mxc_dma_trans_chain_t callback);
