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

#include "mxc_device.h"
#include "mxc_assert.h"
#include "dma.h"
#include "uart_revb_regs.h"
#include "uart_regs.h"

typedef struct _mxc_uart_revb_req_t mxc_uart_revb_req_t;

/**
 * @brief      Clock settings */
typedef enum {
	MXC_UART_REVB_APB_CLK = 0,		// PCLK
	MXC_UART_REVB_EXT_CLK = 1,
	MXC_UART_REVB_CLK2 = 2,
	MXC_UART_REVB_CLK3 = 3,
        // For UART3, ERTCO and INRO clocks
        MXC_UART_REVB_LPCLK2 = 4,
        MXC_UART_REVB_LPCLK3 = 5
} mxc_uart_revb_clock_t;

struct _mxc_uart_revb_req_t {
    mxc_uart_revb_regs_t* uart;
    uint8_t        *txData;
    uint8_t        *rxData;
    uint32_t        txLen;
    uint32_t        rxLen;
    uint32_t        txCnt;
    uint32_t        rxCnt;
    mxc_uart_complete_cb_t callback;
};

int MXC_UART_RevB_Init (mxc_uart_revb_regs_t* uart, unsigned int baud, mxc_uart_revb_clock_t clock);
int MXC_UART_RevB_Shutdown (mxc_uart_revb_regs_t* uart);
int MXC_UART_RevB_ReadyForSleep (mxc_uart_revb_regs_t* uart);
int MXC_UART_RevB_SetFrequency (mxc_uart_revb_regs_t* uart, unsigned int baud, mxc_uart_revb_clock_t clock);
int MXC_UART_RevB_GetFrequency (mxc_uart_revb_regs_t* uart);
int MXC_UART_RevB_SetDataSize (mxc_uart_revb_regs_t* uart, int dataSize);
int MXC_UART_RevB_SetStopBits (mxc_uart_revb_regs_t* uart, mxc_uart_stop_t stopBits);
int MXC_UART_RevB_SetParity (mxc_uart_revb_regs_t* uart, mxc_uart_parity_t parity);
int MXC_UART_RevB_SetFlowCtrl (mxc_uart_revb_regs_t* uart, mxc_uart_flow_t flowCtrl, int rtsThreshold);
int MXC_UART_RevB_SetClockSource (mxc_uart_revb_regs_t* uart, mxc_uart_revb_clock_t clock);
int MXC_UART_RevB_GetActive (mxc_uart_revb_regs_t* uart);
int MXC_UART_RevB_AbortTransmission (mxc_uart_revb_regs_t* uart);
int MXC_UART_RevB_ReadCharacterRaw (mxc_uart_revb_regs_t* uart);
int MXC_UART_RevB_WriteCharacterRaw (mxc_uart_revb_regs_t* uart, uint8_t character);
int MXC_UART_RevB_ReadCharacter (mxc_uart_revb_regs_t* uart);
int MXC_UART_RevB_WriteCharacter (mxc_uart_revb_regs_t* uart, uint8_t character);
int MXC_UART_RevB_Read (mxc_uart_revb_regs_t* uart, uint8_t* buffer, int* len);
int MXC_UART_RevB_Write (mxc_uart_revb_regs_t* uart, const uint8_t* byte, int* len);
unsigned int MXC_UART_RevB_ReadRXFIFO (mxc_uart_revb_regs_t* uart, unsigned char* bytes,
                                       unsigned int len);
int MXC_UART_RevB_ReadRXFIFODMA (mxc_uart_revb_regs_t* uart, unsigned char* bytes,
        unsigned int len, mxc_uart_dma_complete_cb_t callback, mxc_dma_config_t config);
unsigned int MXC_UART_RevB_GetRXFIFOAvailable (mxc_uart_revb_regs_t* uart);
unsigned int MXC_UART_RevB_WriteTXFIFO (mxc_uart_revb_regs_t* uart, const unsigned char* bytes,
                                        unsigned int len);
int MXC_UART_RevB_WriteTXFIFODMA (mxc_uart_revb_regs_t* uart, const unsigned char* bytes,
        unsigned int len, mxc_uart_dma_complete_cb_t callback, mxc_dma_config_t config);
unsigned int MXC_UART_RevB_GetTXFIFOAvailable (mxc_uart_revb_regs_t* uart);
int MXC_UART_RevB_ClearRXFIFO (mxc_uart_revb_regs_t* uart);
int MXC_UART_RevB_ClearTXFIFO (mxc_uart_revb_regs_t* uart);
int MXC_UART_RevB_SetRXThreshold (mxc_uart_revb_regs_t* uart, unsigned int numBytes);
unsigned int MXC_UART_RevB_GetRXThreshold (mxc_uart_revb_regs_t* uart);
int MXC_UART_RevB_SetTXThreshold (mxc_uart_revb_regs_t* uart, unsigned int numBytes);
unsigned int MXC_UART_RevB_GetTXThreshold (mxc_uart_revb_regs_t* uart);
unsigned int MXC_UART_RevB_GetFlags (mxc_uart_revb_regs_t* uart);
int MXC_UART_RevB_ClearFlags (mxc_uart_revb_regs_t* uart, unsigned int flags);
int MXC_UART_RevB_EnableInt (mxc_uart_revb_regs_t* uart, unsigned int mask);
int MXC_UART_RevB_DisableInt (mxc_uart_revb_regs_t* uart, unsigned int mask);
unsigned int MXC_UART_RevB_GetStatus (mxc_uart_revb_regs_t* uart);
int MXC_UART_RevB_Transaction (mxc_uart_revb_req_t* req);
int MXC_UART_RevB_TransactionAsync (mxc_uart_revb_req_t* req);
int MXC_UART_RevB_TransactionDMA (mxc_uart_revb_req_t* req);
int MXC_UART_RevB_AbortAsync (mxc_uart_revb_regs_t* uart);
int MXC_UART_RevB_AsyncHandler (mxc_uart_revb_regs_t* uart);
int MXC_UART_RevB_AsyncStop (mxc_uart_revb_regs_t* uart);
int MXC_UART_RevB_AsyncCallback (mxc_uart_revb_regs_t* uart, int retVal);
void MXC_UART_RevB_DMACallback (int ch, int error);
