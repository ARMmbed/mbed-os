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
#include "mxc_device.h"
#include "mxc_assert.h"
#include "uart.h"
#include "uart_reva.h"
#include "dma.h"

/* **** Definitions **** */
#define MXC_UART_REVA_ERRINT_EN (MXC_F_UART_REVA_INT_EN_RX_FRAME_ERROR | \
                            MXC_F_UART_REVA_INT_EN_RX_PARITY_ERROR | \
                            MXC_F_UART_REVA_INT_EN_RX_OVERRUN)

#define MXC_UART_REVA_ERRINT_FL (MXC_F_UART_REVA_INT_FL_RX_FRAME_ERROR | \
                            MXC_F_UART_REVA_INT_FL_RX_PARITY_ERROR | \
                            MXC_F_UART_REVA_INT_FL_RX_OVERRUN)

/* **** Variable Declaration **** */
static void* AsyncRequests[MXC_UART_INSTANCES];

// Structure to save DMA state
typedef struct {
    mxc_uart_reva_req_t *req;
    int channelTx;
    int channelRx;
} uart_reva_req_state_t;

uart_reva_req_state_t states[MXC_UART_INSTANCES];

/* **** Function Prototypes **** */

/* ************************************************************************* */
/* Control/Configuration functions                                           */
/* ************************************************************************* */
int MXC_UART_RevA_Init (mxc_uart_reva_regs_t* uart, unsigned int baud)
{
    int err;

    // Initialize UART
    // Set RX threshold to 1 byte
    if ((err = (MXC_UART_SetRXThreshold ((mxc_uart_regs_t*) uart, 1))) != E_NO_ERROR) {
        return err;
    }

    // Set TX threshold to 2 byte
    if ((err = (MXC_UART_SetTXThreshold ((mxc_uart_regs_t*) uart, 2))) != E_NO_ERROR) {
        return err;
    }        
    
    // Set Datasize to 8 bits
    if ((err = (MXC_UART_SetDataSize ((mxc_uart_regs_t*) uart, 8))) != E_NO_ERROR) {
        return err;
    }         
    
    if ((err = (MXC_UART_SetParity ((mxc_uart_regs_t*) uart, MXC_UART_PARITY_DISABLE))) != E_NO_ERROR) {
        return err;
    }
    
    if ((err = (MXC_UART_SetStopBits ((mxc_uart_regs_t*) uart, MXC_UART_STOP_1))) != E_NO_ERROR) {
        return err;
    }
    
    uart->ctrl |= MXC_F_UART_REVA_CTRL_ENABLE;
    
    MXC_UART_SetFrequency ((mxc_uart_regs_t*) uart, baud);

    return E_NO_ERROR;
}

int MXC_UART_RevA_ReadyForSleep (mxc_uart_reva_regs_t* uart)
{
    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
        return E_BAD_PARAM;
    }
    
    if (AsyncRequests[MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart)] != NULL) {
        return E_BUSY;
    }
    
    return MXC_UART_GetActive ((mxc_uart_regs_t*) uart);
}

int MXC_UART_RevA_SetFrequency (mxc_uart_reva_regs_t* uart, unsigned int baud)
{
    float uartDiv;
    int periphClock;
    int prescale;
    int decimalDiv;
    
    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
        return E_BAD_PARAM;
    }
    
    if (uart->ctrl & MXC_F_UART_REVA_CTRL_CLKSEL) {
#ifdef IBRO_FREQ
        periphClock = IBRO_FREQ;
#else
        return E_BAD_PARAM;
#endif
    }
    else {
        periphClock = PeripheralClock;
    }
    
    uartDiv = (float) periphClock / baud;
    
    // Find the largest value of prescale that keeps div > 1
    for (prescale = 8; prescale < 128; prescale = prescale << 1) {
        if (uartDiv / (float) prescale < 1) {
            prescale = prescale >> 1;
            break;
        }
    }
    
    if (prescale < 8) {
        return E_BAD_PARAM;
    }
    
    uartDiv /= prescale;
    decimalDiv = (int)((uartDiv - (int) uartDiv) * 128);
    
    // Work around for Jira Bug: ME10-650
    if (decimalDiv > 3) {
        decimalDiv -= 3;
    }
    else {
        decimalDiv += 3;
    }
    
    switch (prescale) {
    case 8:
        prescale = 4;
        break;
        
    case 16:
        prescale = 3;
        break;
        
    case 32:
        prescale = 2;
        break;
        
    case 64:
        prescale = 1;
        break;
        
    case 128:
        prescale = 0;
        break;
        
    default:
        return E_UNKNOWN;
        break;
    }
    
    prescale <<= MXC_F_UART_REVA_BAUD0_FACTOR_POS;
    decimalDiv <<= MXC_F_UART_REVA_BAUD1_DBAUD_POS;
    
    MXC_SETFIELD (uart->baud0, MXC_F_UART_REVA_BAUD0_FACTOR, prescale);
    MXC_SETFIELD (uart->baud0, MXC_F_UART_REVA_BAUD0_IBAUD, (((int) uartDiv) << MXC_F_UART_REVA_BAUD0_IBAUD_POS));
    MXC_SETFIELD (uart->baud1, MXC_F_UART_REVA_BAUD1_DBAUD, decimalDiv);
    
    return MXC_UART_GetFrequency ((mxc_uart_regs_t*) uart);
}

int MXC_UART_RevA_GetFrequency (mxc_uart_reva_regs_t* uart)
{
    int periphClock = 0;
    float uartDiv = 0;
    float decimalDiv = 0;
    
    if (uart->ctrl & MXC_F_UART_REVA_CTRL_CLKSEL) {
#ifdef IBRO_FREQ
        periphClock = IBRO_FREQ;
#else
        return E_BAD_PARAM;
#endif
    }
    else {
        periphClock = PeripheralClock;
    }
    
    uartDiv += uart->baud0 & MXC_F_UART_REVA_BAUD0_IBAUD;
    decimalDiv = uart->baud1 & MXC_F_UART_REVA_BAUD1_DBAUD;

    // Based on work around for Jira Bug: ME10-650
    // No way to tell if the SetFrequency function added or 
    //      subtracted 3 in this range
    if(decimalDiv > 3 && decimalDiv <= 6) {
    	decimalDiv -= 3;
    }
    else {
    	decimalDiv += 3;
    }

    uartDiv += decimalDiv / (float)128;
    uartDiv *= (1 << (7 - (uart->baud0 & MXC_F_UART_REVA_BAUD0_FACTOR)));

    return (int) ( (float) periphClock / uartDiv);
}

int MXC_UART_RevA_SetDataSize (mxc_uart_reva_regs_t* uart, int dataSize)
{
    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
        return E_BAD_PARAM;
    }
    
    if (dataSize < 5 || dataSize > 8) {
        return E_BAD_PARAM;
    }
    
    dataSize = (dataSize - 5) << MXC_F_UART_REVA_CTRL_CHAR_SIZE_POS;
    
    MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_CHAR_SIZE, dataSize);
    
    return E_NO_ERROR;
}

int MXC_UART_RevA_SetStopBits (mxc_uart_reva_regs_t* uart, mxc_uart_stop_t stopBits)
{
    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
        return E_BAD_PARAM;
    }
    
    switch (stopBits) {
    case MXC_UART_STOP_1:
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_STOPBITS, 0 << MXC_F_UART_REVA_CTRL_STOPBITS_POS);
        break;
        
    case MXC_UART_STOP_2:
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_STOPBITS, 1 << MXC_F_UART_REVA_CTRL_STOPBITS_POS);
        break;
        
    default:
        return E_BAD_PARAM;
        break;
    }
    
    return E_NO_ERROR;
}

int MXC_UART_RevA_SetParity (mxc_uart_reva_regs_t* uart, mxc_uart_parity_t parity)
{
    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
        return E_BAD_PARAM;
    }
    
    switch (parity) {
    case MXC_UART_PARITY_DISABLE:
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARITY_EN, 0 << MXC_F_UART_REVA_CTRL_PARITY_EN_POS );
        break;
        
    case MXC_UART_PARITY_EVEN:
    case MXC_UART_PARITY_EVEN_0:
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARITY_EN, 1 << MXC_F_UART_REVA_CTRL_PARITY_EN_POS);
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARITY, MXC_S_UART_REVA_CTRL_PARITY_EVEN);
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARMD, 0 << MXC_F_UART_REVA_CTRL_PARMD_POS);
        break;
        
    case MXC_UART_PARITY_EVEN_1:
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARITY_EN, 1 << MXC_F_UART_REVA_CTRL_PARITY_EN_POS);
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARITY, MXC_S_UART_REVA_CTRL_PARITY_EVEN);
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARMD, 1 << MXC_F_UART_REVA_CTRL_PARMD_POS);
        break;
        
    case MXC_UART_PARITY_ODD:
    case MXC_UART_PARITY_ODD_0:
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARITY_EN, 1 << MXC_F_UART_REVA_CTRL_PARITY_EN_POS);
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARITY, MXC_S_UART_REVA_CTRL_PARITY_ODD);
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARMD, 0 << MXC_F_UART_REVA_CTRL_PARMD_POS);
        break;
        
    case MXC_UART_PARITY_ODD_1:
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARITY_EN, 1 << MXC_F_UART_REVA_CTRL_PARITY_EN_POS);
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARITY, MXC_S_UART_REVA_CTRL_PARITY_ODD);
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARMD, 1 << MXC_F_UART_REVA_CTRL_PARMD_POS);
        break;
        
    case MXC_UART_PARITY_MARK:
    case MXC_UART_PARITY_MARK_0:
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARITY_EN, 1 << MXC_F_UART_REVA_CTRL_PARITY_EN_POS);
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARITY, MXC_S_UART_REVA_CTRL_PARITY_MARK);
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARMD, 0 << MXC_F_UART_REVA_CTRL_PARMD_POS);
        break;
        
    case MXC_UART_PARITY_MARK_1:
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARITY_EN, 1 << MXC_F_UART_REVA_CTRL_PARITY_EN_POS);
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARITY, MXC_S_UART_REVA_CTRL_PARITY_MARK);
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARMD, 1 << MXC_F_UART_REVA_CTRL_PARMD_POS);
        break;
        
    case MXC_UART_PARITY_SPACE:
    case MXC_UART_PARITY_SPACE_0:
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARITY_EN, 1 << MXC_F_UART_REVA_CTRL_PARITY_EN_POS);
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARITY, MXC_S_UART_REVA_CTRL_PARITY_SPACE);
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARMD, 0 << MXC_F_UART_REVA_CTRL_PARMD_POS);
        break;
        
    case MXC_UART_PARITY_SPACE_1:
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARITY_EN, 1 << MXC_F_UART_REVA_CTRL_PARITY_EN_POS);
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARITY, MXC_S_UART_REVA_CTRL_PARITY_SPACE);
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_PARMD, 1 << MXC_F_UART_REVA_CTRL_PARMD_POS);
        break;
        
    default:
        return E_BAD_PARAM;
        break;
    }
    
    return E_NO_ERROR;
}

int MXC_UART_RevA_SetFlowCtrl (mxc_uart_reva_regs_t* uart, mxc_uart_flow_t flowCtrl, int rtsThreshold)
{
    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
        return E_BAD_PARAM;
    }
    
    switch (flowCtrl) {
    case MXC_UART_FLOW_DIS:
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_FLOW_CTRL, 0 << MXC_F_UART_REVA_CTRL_FLOW_CTRL_POS);
        break;
        
    case MXC_UART_FLOW_EN_LOW:
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_FLOW_CTRL, 1 << MXC_F_UART_REVA_CTRL_FLOW_CTRL_POS);
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_FLOW_POL, 0 << MXC_F_UART_REVA_CTRL_FLOW_POL_POS);
        break;
        
    case MXC_UART_FLOW_EN_HIGH:
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_FLOW_CTRL, 1 << MXC_F_UART_REVA_CTRL_FLOW_CTRL_POS);
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_FLOW_POL, 1 << MXC_F_UART_REVA_CTRL_FLOW_POL_POS);
        break;
        
    default:
        return E_BAD_PARAM;
        break;
    }
    
    if (rtsThreshold < 1 || rtsThreshold > MXC_UART_FIFO_DEPTH) {
        return E_BAD_PARAM;
    }
    
    rtsThreshold <<= MXC_F_UART_REVA_THRESH_CTRL_RX_FIFO_THRESH_POS;
    MXC_SETFIELD (uart->thresh_ctrl, MXC_F_UART_REVA_THRESH_CTRL_RTS_FIFO_THRESH, rtsThreshold);
    
    return E_NO_ERROR;
}

int MXC_UART_RevA_SetClockSource (mxc_uart_reva_regs_t* uart, int usePCLK)
{
	int baudRate;

    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
        return E_BAD_PARAM;
    }
    
    baudRate = MXC_UART_GetFrequency ((mxc_uart_regs_t*) uart);
    if(baudRate < 0) {	// return error code
    	return baudRate;
    }

    if (usePCLK) {
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_CLKSEL, 0 << MXC_F_UART_REVA_CTRL_CLKSEL_POS);
    }
    else {
        MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_CLKSEL, 1 << MXC_F_UART_REVA_CTRL_CLKSEL_POS);
    }

    return MXC_UART_SetFrequency ((mxc_uart_regs_t*) uart, baudRate);
}

int MXC_UART_RevA_SetNullModem (mxc_uart_reva_regs_t* uart, int nullModem)
{
    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
        return E_BAD_PARAM;
    }
    
    nullModem = (nullModem > 0) << MXC_F_UART_REVA_CTRL_NULL_MODEM_POS;
    
    MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_NULL_MODEM, nullModem);
    
    return E_NO_ERROR;
}

int MXC_UART_RevA_SendBreak (mxc_uart_reva_regs_t* uart)
{
    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
        return E_BAD_PARAM;
    }
    
    MXC_SETFIELD (uart->ctrl, MXC_F_UART_REVA_CTRL_BREAK, 1 << MXC_F_UART_REVA_CTRL_BREAK_POS);
    
    return E_NO_ERROR;
}

int MXC_UART_RevA_GetActive (mxc_uart_reva_regs_t* uart)
{
    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
        return E_BAD_PARAM;
    }
    
    if (uart->status & (MXC_F_UART_REVA_STATUS_TX_BUSY | MXC_F_UART_REVA_STATUS_RX_BUSY)) {
        return E_BUSY;
    }
    
    return E_NO_ERROR;
}

int MXC_UART_RevA_AbortTransmission (mxc_uart_reva_regs_t* uart)
{
    MXC_UART_ClearTXFIFO ((mxc_uart_regs_t*) uart);
    return E_NO_ERROR;
}

int MXC_UART_RevA_ReadCharacterRaw (mxc_uart_reva_regs_t* uart)
{
    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
        return E_BAD_PARAM;
    }
    
    if (uart->status & MXC_F_UART_REVA_STATUS_RX_EMPTY) {
        return E_UNDERFLOW;
    }
    
    return uart->fifo;
}

int MXC_UART_RevA_WriteCharacterRaw (mxc_uart_reva_regs_t* uart, uint8_t character)
{
    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
        return E_BAD_PARAM;
    }
    
    // Return error if the FIFO is full
    if (uart->status & MXC_F_UART_REVA_STATUS_TX_FULL) {
        return E_OVERFLOW;
    }
    
    uart->fifo = character;
    
    return E_NO_ERROR;
}

int MXC_UART_RevA_Read (mxc_uart_reva_regs_t* uart, uint8_t* buffer, int* len)
{
    int read = 0;
    int retVal;
    
    for (; read < *len; read++) {
        retVal = MXC_UART_ReadCharacter ((mxc_uart_regs_t*) uart);
        
        if (retVal < 0) {
            *len = read;
            return retVal;
        }
        else {
            buffer[read] = retVal;
        }
    }
    
    *len = read;
    return E_NO_ERROR;
}

int MXC_UART_RevA_Write (mxc_uart_reva_regs_t* uart, uint8_t* byte, int* len)
{
    int written = 0;
    int retVal;
    
    for (; written < *len; written++) {
        retVal = MXC_UART_WriteCharacter ((mxc_uart_regs_t*) uart, byte[written]);
        
        if (retVal != E_NO_ERROR) {
            *len = written;
            return retVal;
        }
    }
    
    *len = written;
    return E_NO_ERROR;
}

unsigned int MXC_UART_RevA_ReadRXFIFO (mxc_uart_reva_regs_t* uart, unsigned char* bytes, unsigned int len)
{
    unsigned int read = 0;
    
    for (; read < len; read++) {
        if (uart->status & MXC_F_UART_REVA_STATUS_RX_EMPTY) {
            break;
        }
        
        bytes[read] = uart->fifo;
    }
    
    return read;
}

int MXC_UART_RevA_ReadRXFIFODMA (mxc_uart_reva_regs_t* uart, mxc_dma_regs_t* dma, unsigned char* bytes, unsigned int len,
                                    mxc_uart_dma_complete_cb_t callback, mxc_dma_config_t config)
{
    uint8_t channel;
    mxc_dma_srcdst_t srcdst;
    
    int uart_num = MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart);
    
    if (uart_num < 0) {
        return E_BAD_PARAM;
    }
    
    if (bytes == NULL) {
        return E_NULL_PTR;
    }
    
  #if TARGET_NUM == 32665
    channel = MXC_DMA_AcquireChannel(dma);
  #else
    channel = MXC_DMA_AcquireChannel();
  #endif
    
    config.ch = channel;
    
    config.srcwd = MXC_DMA_WIDTH_BYTE;
    config.dstwd = MXC_DMA_WIDTH_BYTE;
    
    config.srcinc_en = 0;
    config.dstinc_en = 1;
    
    srcdst.ch = channel;
    srcdst.dest = bytes;
    srcdst.len = len;
    
    
    states[uart_num].channelRx = channel;
    MXC_DMA_ConfigChannel (config,srcdst);
    MXC_DMA_SetCallback (channel, MXC_UART_DMACallback);
    MXC_DMA_EnableInt (channel);
    MXC_DMA_Start (channel);
    //MXC_DMA->ch[channel].ctrl |= MXC_F_DMA_CTRL_CTZ_IE;
    MXC_DMA_SetChannelInterruptEn(channel, 0, 1);
    uart->dma |= MXC_F_UART_REVA_DMA_RXDMA_EN;
    
    return E_NO_ERROR;
}

unsigned int MXC_UART_RevA_GetRXFIFOAvailable (mxc_uart_reva_regs_t* uart)
{
    return (uart->status & MXC_F_UART_REVA_STATUS_RX_FIFO_CNT) >> MXC_F_UART_REVA_STATUS_RX_FIFO_CNT_POS;
}

unsigned int MXC_UART_RevA_WriteTXFIFO (mxc_uart_reva_regs_t* uart, unsigned char* bytes, unsigned int len)
{
    unsigned int written = 0;
    
    for (; written < len; written++) {
        if (uart->status & MXC_F_UART_REVA_STATUS_TX_FULL) {
            break;
        }
        
        uart->fifo = bytes[written];
    }
    
    return written;
}

unsigned int MXC_UART_RevA_WriteTXFIFODMA (mxc_uart_reva_regs_t* uart, mxc_dma_regs_t* dma, unsigned char* bytes, unsigned int len,
                                    mxc_uart_dma_complete_cb_t callback, mxc_dma_config_t config)
{
    uint8_t channel;
    mxc_dma_srcdst_t srcdst;
    
    int uart_num = MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart);
    
    if (uart_num < 0) {
        return E_BAD_PARAM;
    }
    
    if (bytes == NULL) {
        return E_NULL_PTR;
    }
    
  #if TARGET_NUM == 32665
    channel = MXC_DMA_AcquireChannel(dma);
  #else
    channel = MXC_DMA_AcquireChannel();
  #endif
    
    config.ch = channel;
    
    config.srcwd = MXC_DMA_WIDTH_BYTE;
    config.dstwd = MXC_DMA_WIDTH_BYTE;
    
    config.srcinc_en = 1;
    config.dstinc_en = 0;
    
    srcdst.ch = channel;
    srcdst.source = bytes;
    srcdst.len = len;
    
    
    states[uart_num].channelTx = channel;
    MXC_DMA_ConfigChannel (config,srcdst);
    MXC_DMA_SetCallback (channel, MXC_UART_DMACallback);
    MXC_DMA_EnableInt (channel);
    MXC_DMA_Start (channel);
    //MXC_DMA->ch[channel].ctrl |= MXC_F_DMA_CTRL_CTZ_IE;
    MXC_DMA_SetChannelInterruptEn(channel, 0, 1);
    
    uart->dma |= MXC_F_UART_REVA_DMA_TXDMA_EN;
    
    return E_NO_ERROR;
}

unsigned int MXC_UART_RevA_GetTXFIFOAvailable (mxc_uart_reva_regs_t* uart)
{
    int txCnt = (uart->status & MXC_F_UART_REVA_STATUS_TX_FIFO_CNT) >> MXC_F_UART_REVA_STATUS_TX_FIFO_CNT_POS;
    return MXC_UART_FIFO_DEPTH - txCnt;
}

int MXC_UART_RevA_ClearRXFIFO (mxc_uart_reva_regs_t* uart)
{
    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
        return E_BAD_PARAM;
    }

    uart->ctrl |= MXC_F_UART_REVA_CTRL_RX_FLUSH;
    
    while (uart->ctrl & MXC_F_UART_REVA_CTRL_RX_FLUSH);

    return E_NO_ERROR;
}

int MXC_UART_RevA_ClearTXFIFO (mxc_uart_reva_regs_t* uart)
{
    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
        return E_BAD_PARAM;
    }

    uart->ctrl |= MXC_F_UART_REVA_CTRL_TX_FLUSH;
    
    while (uart->ctrl & MXC_F_UART_REVA_CTRL_TX_FLUSH);

    return E_NO_ERROR;
}

int MXC_UART_RevA_SetRXThreshold (mxc_uart_reva_regs_t* uart, unsigned int numBytes)
{
    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
        return E_BAD_PARAM;
    }
    
    if (numBytes < 1 || numBytes > MXC_UART_FIFO_DEPTH) {
        return E_BAD_PARAM;
    }
    
    numBytes <<= MXC_F_UART_REVA_THRESH_CTRL_RX_FIFO_THRESH_POS;
    MXC_SETFIELD (uart->thresh_ctrl, MXC_F_UART_REVA_THRESH_CTRL_RX_FIFO_THRESH, numBytes);
    
    return E_NO_ERROR;
}

unsigned int MXC_UART_RevA_GetRXThreshold (mxc_uart_reva_regs_t* uart)
{
    return (uart->thresh_ctrl & MXC_F_UART_REVA_THRESH_CTRL_RX_FIFO_THRESH) >> MXC_F_UART_REVA_THRESH_CTRL_RX_FIFO_THRESH_POS;
}

int MXC_UART_RevA_SetTXThreshold (mxc_uart_reva_regs_t* uart, unsigned int numBytes)
{
    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
        return E_BAD_PARAM;
    }
    
    if (numBytes < 1 || numBytes > MXC_UART_FIFO_DEPTH) {
        return E_BAD_PARAM;
    }
    
    numBytes <<= MXC_F_UART_REVA_THRESH_CTRL_TX_FIFO_THRESH_POS;
    MXC_SETFIELD (uart->thresh_ctrl, MXC_F_UART_REVA_THRESH_CTRL_TX_FIFO_THRESH, numBytes);
    
    return E_NO_ERROR;
}

unsigned int MXC_UART_RevA_GetTXThreshold (mxc_uart_reva_regs_t* uart)
{
    return (uart->thresh_ctrl & MXC_F_UART_REVA_THRESH_CTRL_TX_FIFO_THRESH) >> MXC_F_UART_REVA_THRESH_CTRL_TX_FIFO_THRESH_POS;
}

unsigned int MXC_UART_RevA_GetFlags (mxc_uart_reva_regs_t* uart)
{
    return uart->int_fl;
}

int MXC_UART_RevA_ClearFlags (mxc_uart_reva_regs_t* uart, unsigned int flags)
{
    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
        return E_BAD_PARAM;
    }
    
    uart->int_fl = flags;
    
    return E_NO_ERROR;
}

int MXC_UART_RevA_EnableInt (mxc_uart_reva_regs_t* uart, unsigned int intEn)
{
    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
        return E_BAD_PARAM;
    }
    
    uart->int_en |= intEn;
    
    return E_NO_ERROR;
}

int MXC_UART_RevA_DisableInt (mxc_uart_reva_regs_t* uart, unsigned int intDis)
{
    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
        return E_BAD_PARAM;
    }
    
    uart->int_en &= ~intDis;
    
    return E_NO_ERROR;
}

unsigned int MXC_UART_RevA_GetStatus (mxc_uart_reva_regs_t* uart)
{
    return uart->status;
}

int MXC_UART_RevA_Busy(mxc_uart_reva_regs_t* uart)
{
    int uart_num = MXC_UART_GET_IDX((mxc_uart_regs_t*) uart);  // Holds the current index of tx_states
    MXC_ASSERT(uart_num >= 0);
    if ((uart->status & MXC_F_UART_REVA_STATUS_TX_BUSY) || (uart->status & MXC_F_UART_REVA_STATUS_RX_BUSY)) {
        return E_BUSY;
    }
    // Check to see if there are any ongoing transactions and the UART has room in its FIFO
    if ((states[uart_num].req == NULL) &&
            !(uart->status & MXC_F_UART_REVA_STATUS_TX_FULL)) {

        return E_NO_ERROR;
    }

    return E_BUSY;
}

int MXC_UART_RevA_Transaction (mxc_uart_reva_req_t* req)
{
    unsigned int numToWrite, numToRead;

    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*)(req->uart)) < 0) {
        return E_BAD_PARAM;
    }
    
    MXC_UART_DisableInt ((mxc_uart_regs_t*)(req->uart), 0xFFFFFFFF);
    MXC_UART_ClearFlags ((mxc_uart_regs_t*)(req->uart), 0xFFFFFFFF);
    
    MXC_UART_ClearRXFIFO ((mxc_uart_regs_t*)(req->uart));
    MXC_UART_ClearTXFIFO ((mxc_uart_regs_t*)(req->uart));
    
    req->txCnt = 0;
    req->rxCnt = 0;
    
    if (req->rxLen) {
        if (req->rxData == NULL) {
            return E_BAD_PARAM;
        }
    }
    
    if (req->txLen) {
        if (req->txData == NULL) {
            return E_BAD_PARAM;
        }
        
        numToWrite = MXC_UART_GetTXFIFOAvailable ((mxc_uart_regs_t*)(req->uart));
        numToWrite = numToWrite > (req->txLen-req->txCnt) ? req->txLen-req->txCnt : numToWrite;
        req->txCnt += MXC_UART_WriteTXFIFO ((mxc_uart_regs_t*)(req->uart), &req->txData[req->txCnt], numToWrite);
        
        while (req->txCnt < req->txLen) {
            while ( !(MXC_UART_GetFlags ((mxc_uart_regs_t*)(req->uart)) & MXC_F_UART_REVA_INT_FL_TX_FIFO_THRESH));
            
            numToWrite = MXC_UART_GetTXFIFOAvailable ((mxc_uart_regs_t*)(req->uart));
            numToWrite = numToWrite > (req->txLen-req->txCnt) ? req->txLen-req->txCnt : numToWrite;
            req->txCnt += MXC_UART_WriteTXFIFO ((mxc_uart_regs_t*)(req->uart), &req->txData[req->txCnt], numToWrite);
            MXC_UART_ClearFlags ((mxc_uart_regs_t*)(req->uart), MXC_F_UART_REVA_INT_FL_TX_FIFO_THRESH);
        }
    }
    
    if (req->rxLen) {
        numToRead = MXC_UART_GetRXFIFOAvailable ((mxc_uart_regs_t*)(req->uart));
        numToRead = numToRead > (req->rxLen-req->rxCnt) ? req->rxLen-req->rxCnt : numToRead;
        req->rxCnt += MXC_UART_ReadRXFIFO ((mxc_uart_regs_t*)(req->uart), &req->rxData[req->rxCnt], numToRead);
        
        while (req->rxCnt < req->rxLen) {
            while ( !(MXC_UART_GetFlags ((mxc_uart_regs_t*)(req->uart)) & MXC_F_UART_REVA_INT_FL_RX_FIFO_THRESH));
            
            numToRead = MXC_UART_GetRXFIFOAvailable ((mxc_uart_regs_t*)(req->uart));
            numToRead = numToRead > (req->rxLen-req->rxCnt) ? req->rxLen-req->rxCnt : numToRead;
            req->rxCnt += MXC_UART_ReadRXFIFO ((mxc_uart_regs_t*)(req->uart), &req->rxData[req->rxCnt], numToRead);
            MXC_UART_ClearFlags ((mxc_uart_regs_t*)(req->uart), MXC_F_UART_REVA_INT_FL_RX_FIFO_THRESH);
        }
    }
    
    return E_NO_ERROR;
}

int MXC_UART_RevA_TransactionAsync (mxc_uart_reva_req_t* req)
{
    unsigned int numToWrite, numToRead;

    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*)(req->uart)) < 0) {
        return E_BAD_PARAM;
    }
    
    MXC_UART_DisableInt ((mxc_uart_regs_t*)(req->uart), 0xFFFFFFFF);
    MXC_UART_ClearFlags ((mxc_uart_regs_t*)(req->uart), 0xFFFFFFFF);
    
    MXC_UART_ClearRXFIFO ((mxc_uart_regs_t*)(req->uart));
    MXC_UART_ClearTXFIFO ((mxc_uart_regs_t*)(req->uart));
    
    req->txCnt = 0;
    req->rxCnt = 0;
    
    if (req->txLen) {
        if (req->txData == NULL) {
            return E_BAD_PARAM;
        }
        
        MXC_UART_EnableInt ((mxc_uart_regs_t*)(req->uart), MXC_F_UART_REVA_INT_EN_TX_FIFO_THRESH);
        numToWrite = MXC_UART_GetTXFIFOAvailable ((mxc_uart_regs_t*)(req->uart));
        numToWrite = numToWrite > (req->txLen-req->txCnt) ? req->txLen-req->txCnt : numToWrite;
        req->txCnt += MXC_UART_WriteTXFIFO ((mxc_uart_regs_t*)(req->uart), &req->txData[req->txCnt], numToWrite);
    }
    
    if (req->rxLen) {
        // All error interrupts are related to RX
        MXC_UART_EnableInt ((mxc_uart_regs_t*)(req->uart), MXC_UART_REVA_ERRINT_EN);
        
        if (req->rxData == NULL) {
            MXC_UART_DisableInt ((mxc_uart_regs_t*)(req->uart), 0xFFFFFFFF);
            MXC_UART_ClearTXFIFO ((mxc_uart_regs_t*)(req->uart));
            return E_BAD_PARAM;
        }
        
        MXC_UART_EnableInt ((mxc_uart_regs_t*)(req->uart), MXC_F_UART_REVA_INT_EN_RX_FIFO_THRESH);
        numToRead = MXC_UART_GetRXFIFOAvailable ((mxc_uart_regs_t*)(req->uart));
        numToRead = numToRead > (req->rxLen-req->rxCnt) ? req->rxLen-req->rxCnt : numToRead;
        req->rxCnt += MXC_UART_ReadRXFIFO ((mxc_uart_regs_t*)(req->uart), &req->rxData[req->rxCnt], numToRead);
        MXC_UART_ClearFlags ((mxc_uart_regs_t*)(req->uart), MXC_F_UART_REVA_INT_FL_RX_FIFO_THRESH);
    }
    
    AsyncRequests[MXC_UART_GET_IDX ((mxc_uart_regs_t*)(req->uart))] = (void *) req;
    return E_NO_ERROR;
}

int MXC_UART_RevA_TransactionDMA (mxc_uart_reva_req_t* req, mxc_dma_regs_t* dma)
{
    int uart_num = MXC_UART_GET_IDX ((mxc_uart_regs_t*)(req->uart));
    
    if (uart_num < 0) {
        return E_BAD_PARAM;
    }
    
    if (req->txLen) {
        if (req->txData == NULL) {
            return E_BAD_PARAM;
        }
    }
    
    if (req->rxLen) {
        if (req->rxData == NULL) {
            return E_BAD_PARAM;
        }
    }
    
    MXC_UART_DisableInt ((mxc_uart_regs_t*)(req->uart), 0xFFFFFFFF);
    MXC_UART_ClearFlags ((mxc_uart_regs_t*)(req->uart), 0xFFFFFFFF);
    
    MXC_UART_ClearRXFIFO ((mxc_uart_regs_t*)(req->uart));
    MXC_UART_ClearTXFIFO ((mxc_uart_regs_t*)(req->uart));
    
    (req->uart)->dma |= (1 << MXC_F_UART_REVA_DMA_RXDMA_LEVEL_POS);      // Set RX DMA threshold to 1 byte
    (req->uart)->dma |= (2 << MXC_F_UART_REVA_DMA_TXDMA_LEVEL_POS);      // Set TX DMA threshold to 2 bytes
    
  #if TARGET_NUM == 32665
    MXC_DMA_Init(dma);
  #else
    MXC_DMA_Init();
  #endif
    
    //tx
    if ( (req->txData != NULL) && (req->txLen)) {
      #if TARGET_NUM == 32665
        if (MXC_UART_WriteTXFIFODMA ((mxc_uart_regs_t*)(req->uart), dma, req->txData, req->txLen, NULL) != E_NO_ERROR) {
            return E_BAD_PARAM;
        }
      #else
        if (MXC_UART_WriteTXFIFODMA ((mxc_uart_regs_t*)(req->uart), req->txData, req->txLen, NULL) != E_NO_ERROR) {
            return E_BAD_PARAM;
        }
      #endif
    }
    
    if ( (req->rxData != NULL) && (req->rxLen)) {
      #if TARGET_NUM == 32665
        if (MXC_UART_ReadRXFIFODMA ((mxc_uart_regs_t*)(req->uart), dma, req->rxData, req->rxLen, NULL) != E_NO_ERROR) {
            return E_BAD_PARAM;
        }
      #else
        if (MXC_UART_ReadRXFIFODMA ((mxc_uart_regs_t*)(req->uart), req->rxData, req->rxLen, NULL) != E_NO_ERROR) {
            return E_BAD_PARAM;
        }
      #endif
    }
    
    return E_NO_ERROR;
}

void MXC_UART_RevA_DMACallback (int ch, int error)
{
    mxc_uart_reva_req_t * temp_req;
    
    for (int i = 0; i < MXC_UART_INSTANCES; i ++) {
        if (states[i].channelTx == ch) {
            //save the request
            temp_req = states[i].req;
            // Callback if not NULL
            if (temp_req->callback != NULL) {
                temp_req->callback((mxc_uart_req_t*)temp_req, E_NO_ERROR);
            }            
            break;
        }
        
        else if (states[i].channelRx == ch) {
            //save the request
            temp_req = states[i].req;
            // Callback if not NULL
            if (temp_req->callback != NULL) {
                temp_req->callback((mxc_uart_req_t*)temp_req, E_NO_ERROR);
            }              
            break;
        }
    }
}

int MXC_UART_RevA_AsyncCallback (mxc_uart_reva_regs_t* uart, int retVal)
{
    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
        return E_BAD_PARAM;
    }

    mxc_uart_reva_req_t* req = (mxc_uart_reva_req_t*) AsyncRequests[MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart)];
    
    if (req->callback != NULL) {
        req->callback ((mxc_uart_req_t*) req, retVal);
    }
    
    return E_NO_ERROR;
}

int MXC_UART_RevA_AsyncStop (mxc_uart_reva_regs_t* uart)
{
    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
        return E_BAD_PARAM;
    }

    MXC_UART_DisableInt ((mxc_uart_regs_t*) uart, 0xFFFFFFFF);
    AsyncRequests[MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart)] = NULL;

    return E_NO_ERROR;
}

int MXC_UART_RevA_AbortAsync (mxc_uart_reva_regs_t* uart)
{
    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
        return E_BAD_PARAM;
    }

    MXC_UART_AsyncCallback ((mxc_uart_regs_t*) uart, E_ABORT);
    MXC_UART_AsyncStop ((mxc_uart_regs_t*) uart);

    return E_NO_ERROR;
}

int MXC_UART_RevA_AsyncHandler (mxc_uart_reva_regs_t* uart)
{
    int uartNum = MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart);
    unsigned int flags, numToWrite, numToRead;
    mxc_uart_reva_req_t* req;
    
    if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
        return E_BAD_PARAM;
    }
    
    req = (mxc_uart_reva_req_t*) AsyncRequests[uartNum];
    
    flags = MXC_UART_GetFlags ((mxc_uart_regs_t*) uart);
    
    if (flags & MXC_UART_REVA_ERRINT_FL & uart->int_en) {
        MXC_UART_AsyncCallback ((mxc_uart_regs_t*) uart, E_COMM_ERR);
        MXC_UART_AsyncStop ((mxc_uart_regs_t*) uart);
        return E_INVALID;
    }
    
    if ( (flags & MXC_F_UART_REVA_INT_FL_TX_FIFO_THRESH) && (req->txLen)) {
        numToWrite = MXC_UART_GetTXFIFOAvailable ((mxc_uart_regs_t*)(req->uart));
        numToWrite = numToWrite > (req->txLen-req->txCnt) ? req->txLen-req->txCnt : numToWrite;
        req->txCnt += MXC_UART_WriteTXFIFO ((mxc_uart_regs_t*)(req->uart), &req->txData[req->txCnt], numToWrite);
        MXC_UART_ClearFlags ((mxc_uart_regs_t*)(req->uart), MXC_F_UART_REVA_INT_FL_TX_FIFO_THRESH);
    }
    
    if ( (flags & MXC_F_UART_REVA_INT_FL_RX_FIFO_THRESH) && (req->rxLen)) {
        numToRead = MXC_UART_GetRXFIFOAvailable ((mxc_uart_regs_t*)(req->uart));
        numToRead = numToRead > (req->rxLen-req->rxCnt) ? req->rxLen-req->rxCnt : numToRead;
        req->rxCnt += MXC_UART_ReadRXFIFO ((mxc_uart_regs_t*)(req->uart), &req->rxData[req->rxCnt], numToRead);
        
        if ( (req->rxLen - req->rxCnt) < MXC_UART_GetRXThreshold ((mxc_uart_regs_t*)(req->uart))) {
            MXC_UART_SetRXThreshold ((mxc_uart_regs_t*)(req->uart), req->rxLen - req->rxCnt);
        }
        
        MXC_UART_ClearFlags ((mxc_uart_regs_t*)(req->uart), MXC_F_UART_REVA_INT_FL_RX_FIFO_THRESH);
    }
    
    if ( (req->rxCnt == req->rxLen) && (req->txCnt == req->txLen)) {
        MXC_UART_AsyncCallback ((mxc_uart_regs_t*) uart, E_NO_ERROR);
        MXC_UART_AsyncStop ((mxc_uart_regs_t*) uart);
    }
    
    return E_NO_ERROR;
}
