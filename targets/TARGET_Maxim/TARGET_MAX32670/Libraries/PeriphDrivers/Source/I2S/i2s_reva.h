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

#ifndef LIBRARIES_PERIPHDRIVERS_SOURCE_I2S_I2S_REVA_H_
#define LIBRARIES_PERIPHDRIVERS_SOURCE_I2S_I2S_REVA_H_

/* **** Includes **** */
#include <stdint.h>
#include "i2s.h"
#include "i2s_reva_regs.h"

/****** Definitions *****/

/* **** Functions **** */
int MXC_I2S_RevA_Init(mxc_i2s_reva_regs_t *i2s, mxc_i2s_req_t *req);

int MXC_I2S_RevA_Shutdown(mxc_i2s_reva_regs_t *i2s);

int MXC_I2S_RevA_ConfigData(mxc_i2s_reva_regs_t *i2s, mxc_i2s_req_t *req);

void MXC_I2S_RevA_TXEnable(mxc_i2s_reva_regs_t *i2s);

void MXC_I2S_RevA_TXDisable(mxc_i2s_reva_regs_t *i2s);

void MXC_I2S_RevA_RXEnable(mxc_i2s_reva_regs_t *i2s);

void MXC_I2S_RevA_RXDisable(mxc_i2s_reva_regs_t *i2s);

int MXC_I2S_RevA_SetRXThreshold(mxc_i2s_reva_regs_t *i2s, uint8_t threshold);

int MXC_I2S_RevA_SetFrequency(mxc_i2s_reva_regs_t *i2s, mxc_i2s_ch_mode_t mode, uint16_t clkdiv);

int MXC_I2S_RevA_SetSampleRate(mxc_i2s_reva_regs_t *i2s, uint32_t smpl_rate,
                               mxc_i2s_wsize_t smpl_sz, uint32_t src_clk);

int MXC_I2S_RevA_GetSampleRate(mxc_i2s_reva_regs_t *i2s, uint32_t src_clk);

int MXC_I2S_RevA_CalculateClockDiv(mxc_i2s_reva_regs_t *i2s, uint32_t smpl_rate,
                                   mxc_i2s_wsize_t smpl_sz, uint32_t src_clk);

void MXC_I2S_RevA_Flush(mxc_i2s_reva_regs_t *i2s);

int MXC_I2S_RevA_FillTXFIFO(mxc_i2s_reva_regs_t *i2s, void *txData, mxc_i2s_wsize_t wordSize,
                            int len, int smpl_cnt);

int MXC_I2S_RevA_ReadRXFIFO(mxc_i2s_reva_regs_t *i2s, void *rxData, mxc_i2s_wsize_t wordSize,
                            int len, int smpl_cnt);

void MXC_I2S_RevA_EnableInt(mxc_i2s_reva_regs_t *i2s, uint32_t flags);

void MXC_I2S_RevA_DisableInt(mxc_i2s_reva_regs_t *i2s, uint32_t flags);

int MXC_I2S_RevA_GetFlags(mxc_i2s_reva_regs_t *i2s);

void MXC_I2S_RevA_ClearFlags(mxc_i2s_reva_regs_t *i2s, uint32_t flags);

int MXC_I2S_RevA_Transaction(mxc_i2s_reva_regs_t *i2s, mxc_i2s_req_t *i2s_req);

int MXC_I2S_RevA_TransactionAsync(mxc_i2s_reva_regs_t *i2s, mxc_i2s_req_t *i2s_req);

int MXC_I2S_RevA_TXDMAConfig(mxc_i2s_reva_regs_t *i2s, void *src_addr, int len);

int MXC_I2S_RevA_RXDMAConfig(mxc_i2s_reva_regs_t *i2s, void *dest_addr, int len);

void MXC_I2S_RevA_Handler(mxc_i2s_reva_regs_t *i2s);

void MXC_I2S_RevA_RegisterDMACallback(void (*callback)(int, int));

void MXC_I2S_RevA_RegisterAsyncCallback(void (*callback)(int));

#endif // LIBRARIES_PERIPHDRIVERS_SOURCE_I2S_I2S_REVA_H_
