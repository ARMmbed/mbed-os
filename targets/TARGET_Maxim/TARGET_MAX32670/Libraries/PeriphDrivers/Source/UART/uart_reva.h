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

#include "mxc_device.h"
#include "mxc_assert.h"
#include "dma.h"
#include "uart_reva_regs.h"
#include "uart_regs.h"

typedef struct _mxc_uart_reva_req_t mxc_uart_reva_req_t;

struct _mxc_uart_reva_req_t {
    mxc_uart_reva_regs_t* uart;
    uint8_t        *txData;
    uint8_t        *rxData;
    uint32_t        txLen;
    uint32_t        rxLen;
    uint32_t        txCnt;
    uint32_t        rxCnt;
    mxc_uart_complete_cb_t callback;
};

int MXC_UART_RevA_Init (mxc_uart_reva_regs_t* uart, unsigned int baud);
int MXC_UART_RevA_Shutdown (mxc_uart_reva_regs_t* uart);
int MXC_UART_RevA_ReadyForSleep (mxc_uart_reva_regs_t* uart);
int MXC_UART_RevA_SetFrequency (mxc_uart_reva_regs_t* uart, unsigned int baud);
int MXC_UART_RevA_GetFrequency (mxc_uart_reva_regs_t* uart);
int MXC_UART_RevA_SetDataSize (mxc_uart_reva_regs_t* uart, int dataSize);
int MXC_UART_RevA_SetStopBits (mxc_uart_reva_regs_t* uart, mxc_uart_stop_t stopBits);
int MXC_UART_RevA_SetParity (mxc_uart_reva_regs_t* uart, mxc_uart_parity_t parity);
int MXC_UART_RevA_SetFlowCtrl (mxc_uart_reva_regs_t* uart, mxc_uart_flow_t flowCtrl, int rtsThreshold);
int MXC_UART_RevA_SetClockSource (mxc_uart_reva_regs_t* uart, int usePCLK);
int MXC_UART_RevA_SetNullModem (mxc_uart_reva_regs_t* uart, int nullModem);
int MXC_UART_RevA_SendBreak (mxc_uart_reva_regs_t* uart);
int MXC_UART_RevA_GetActive (mxc_uart_reva_regs_t* uart);
int MXC_UART_RevA_AbortTransmission (mxc_uart_reva_regs_t* uart);
int MXC_UART_RevA_ReadCharacterRaw (mxc_uart_reva_regs_t* uart);
int MXC_UART_RevA_WriteCharacterRaw (mxc_uart_reva_regs_t* uart, uint8_t character);
int MXC_UART_RevA_Read (mxc_uart_reva_regs_t* uart, uint8_t* buffer, int* len);
int MXC_UART_RevA_Write (mxc_uart_reva_regs_t* uart, uint8_t* byte, int* len);
unsigned int MXC_UART_RevA_ReadRXFIFO (mxc_uart_reva_regs_t* uart, unsigned char* bytes,
                                       unsigned int len);
int MXC_UART_RevA_ReadRXFIFODMA (mxc_uart_reva_regs_t* uart, mxc_dma_regs_t* dma, unsigned char* bytes,
        unsigned int len, mxc_uart_dma_complete_cb_t callback, mxc_dma_config_t config);
unsigned int MXC_UART_RevA_GetRXFIFOAvailable (mxc_uart_reva_regs_t* uart);
unsigned int MXC_UART_RevA_WriteTXFIFO (mxc_uart_reva_regs_t* uart, unsigned char* bytes,
                                        unsigned int len);
unsigned int MXC_UART_RevA_WriteTXFIFODMA (mxc_uart_reva_regs_t* uart, mxc_dma_regs_t* dma, unsigned char* bytes,
        unsigned int len, mxc_uart_dma_complete_cb_t callback, mxc_dma_config_t config);
unsigned int MXC_UART_RevA_GetTXFIFOAvailable (mxc_uart_reva_regs_t* uart);
int MXC_UART_RevA_ClearRXFIFO (mxc_uart_reva_regs_t* uart);
int MXC_UART_RevA_ClearTXFIFO (mxc_uart_reva_regs_t* uart);
int MXC_UART_RevA_SetRXThreshold (mxc_uart_reva_regs_t* uart, unsigned int numBytes);
unsigned int MXC_UART_RevA_GetRXThreshold (mxc_uart_reva_regs_t* uart);
int MXC_UART_RevA_SetTXThreshold (mxc_uart_reva_regs_t* uart, unsigned int numBytes);
unsigned int MXC_UART_RevA_GetTXThreshold (mxc_uart_reva_regs_t* uart);
unsigned int MXC_UART_RevA_GetFlags (mxc_uart_reva_regs_t* uart);
int MXC_UART_RevA_ClearFlags (mxc_uart_reva_regs_t* uart, unsigned int flags);
int MXC_UART_RevA_EnableInt (mxc_uart_reva_regs_t* uart, unsigned int mask);
int MXC_UART_RevA_DisableInt (mxc_uart_reva_regs_t* uart, unsigned int mask);
unsigned int MXC_UART_RevA_GetStatus (mxc_uart_reva_regs_t* uart);
int MXC_UART_RevA_Busy(mxc_uart_reva_regs_t* uart);
int MXC_UART_RevA_Transaction (mxc_uart_reva_req_t* req);
int MXC_UART_RevA_TransactionAsync (mxc_uart_reva_req_t* req);
int MXC_UART_RevA_TransactionDMA (mxc_uart_reva_req_t* req, mxc_dma_regs_t* dma);
int MXC_UART_RevA_AbortAsync (mxc_uart_reva_regs_t* uart);
int MXC_UART_RevA_AsyncHandler (mxc_uart_reva_regs_t* uart);
int MXC_UART_RevA_AsyncStop (mxc_uart_reva_regs_t* uart);
int MXC_UART_RevA_AsyncCallback (mxc_uart_reva_regs_t* uart, int retVal);
void MXC_UART_RevA_DMACallback (int ch, int error);
