/* mbed Microcontroller Library
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "objects.h"
#include "log_uart_api.h"

#include <string.h>

const u32 log_uart_support_rate[] = {
    UART_BAUD_RATE_2400,   UART_BAUD_RATE_4800,   UART_BAUD_RATE_9600,
    UART_BAUD_RATE_19200,  UART_BAUD_RATE_38400,  UART_BAUD_RATE_57600, 
    UART_BAUD_RATE_115200, UART_BAUD_RATE_921600, UART_BAUD_RATE_1152000,

    0xFFFFFFFF
};

extern HAL_TIMER_OP HalTimerOp;

extern u32 ConfigDebugErr;
extern u32 ConfigDebugWarn;
extern u32 ConfigDebugInfo;
extern u32 CfgSysDebugErr;
extern u32 CfgSysDebugInfo;
extern u32 CfgSysDebugWarn;

extern HAL_Status RuartIsTimeout (u32 StartCount, u32 TimeoutCnt);
extern u32 HalLogUartInitSetting(HAL_LOG_UART_ADAPTER *pUartAdapter);
extern VOID HalLogUartSetBaudRate(HAL_LOG_UART_ADAPTER *pUartAdapter);
extern VOID HalLogUartSetLineCtrl(HAL_LOG_UART_ADAPTER *pUartAdapter);
extern VOID HalLogUartIrqHandle(VOID * Data);

int32_t log_uart_init (log_uart_t *obj, int baudrate, int data_bits, SerialParity parity, int stop_bits)
{
    HAL_LOG_UART_ADAPTER *pUartAdapter;
    int i;

    _memset((void*)obj, 0, sizeof(log_uart_t));
    pUartAdapter = &obj->log_hal_uart;
    // Check Baud rate
    for (i=0; log_uart_support_rate[i]!=0xFFFFFF; i++) {
        if (log_uart_support_rate[i] == baudrate) {
            break;
        }
    }    
    if (log_uart_support_rate[i]== 0xFFFFFF) {
        DBG_UART_ERR("log_uart_init: Not support Baud Rate %d\n", baudrate);
        return -1;
    }
    // check word width
    if ((data_bits < 5) || (data_bits > 8)) {
        DBG_UART_ERR("log_uart_init: Not support Word Width %d\n", data_bits);
        return -1;
    }
    
    //4 Inital Log uart
    pUartAdapter->BaudRate = baudrate;
    pUartAdapter->DataLength = data_bits-5;
    pUartAdapter->FIFOControl = FCR_FIFO_EN | FCR_TX_TRIG_HF | FCR_RX_TRIG_HF;
    // only enable Rx linstatus at initial, 
    // Tx & Rx interrupt will be enabled @ transfer start time
    pUartAdapter->IntEnReg = IER_ELSI;  
    switch (parity) {
    case ParityNone:
        pUartAdapter->Parity = LCR_PARITY_NONE;
        break;

    case ParityOdd:
        pUartAdapter->Parity = LCR_PARITY_ODD;
        break;
        
    case ParityEven:
        pUartAdapter->Parity = LCR_PARITY_EVEN;
        break;

    default:
        DBG_UART_ERR("log_uart_init: Not support parity type %d\n", parity);
        return -1;
    }

    if (stop_bits > 1) {
        // if width is 5 bits, stop bit will be 1.5 bit
        pUartAdapter->Stop = LCR_STOP_2B;
    } else {
        pUartAdapter->Stop = LCR_STOP_1B;
    }

    //4 Initial Log Uart 
    HalLogUartInitSetting(pUartAdapter);    

    // disable all debug message
    ConfigDebugErr = 0;
    ConfigDebugWarn = 0;
    ConfigDebugInfo = 0;
    CfgSysDebugErr = 0;
    CfgSysDebugInfo = 0;
    CfgSysDebugWarn = 0;

    return 0;
}

void log_uart_free(log_uart_t *obj) 
{
    LOG_UART_ADAPTER UartAdapter;

    // Recover the Log UART for debug message printing
    //4 Release log uart reset and clock
    LOC_UART_FCTRL(OFF);
    LOC_UART_FCTRL(ON);
    ACTCK_LOG_UART_CCTRL(ON);

    //4 Inital Log uart
    UartAdapter.BaudRate = UART_BAUD_RATE_38400;
    UartAdapter.DataLength = UART_DATA_LEN_8BIT;
    UartAdapter.FIFOControl = 0xC1;
    UartAdapter.IntEnReg = 0x00;
    UartAdapter.Parity = UART_PARITY_DISABLE;
    UartAdapter.Stop = UART_STOP_1BIT;

    // un_register current IRQ first
    InterruptUnRegister(&(obj->log_hal_uart.IrqHandle)); 

    //4 Initial Log Uart
    HalLogUartInit(UartAdapter);
}

void log_uart_baud(log_uart_t *obj, int baudrate) 
{
    HAL_LOG_UART_ADAPTER *pUartAdapter;
    int i;

    pUartAdapter = &obj->log_hal_uart;
    // Check Baud rate
    for (i=0; log_uart_support_rate[i]!=0xFFFFFFFF; i++) {
        if (log_uart_support_rate[i] == baudrate) {
            break;
        }
    }
    if (log_uart_support_rate[i]== 0xFFFFFF) {
        DBG_UART_ERR("log_uart_baud: Not support Baud Rate %d\n", baudrate);
        return;
    }
    pUartAdapter->BaudRate = baudrate;
    HalLogUartSetBaudRate(pUartAdapter);
}

void log_uart_format(log_uart_t *obj, int data_bits, SerialParity parity, int stop_bits) 
{
    HAL_LOG_UART_ADAPTER *pUartAdapter;
    pUartAdapter = &obj->log_hal_uart;
    
    // check word width
    if ((data_bits < 5) || (data_bits > 8)) {
        DBG_UART_ERR("log_uart_format: Not support Word Width %d\n", data_bits);
        return;
    }
    
    //4 Inital Log uart
    pUartAdapter->DataLength = data_bits - 5;
    switch (parity) {
    case ParityNone:
        pUartAdapter->Parity = LCR_PARITY_NONE;
        break;

    case ParityOdd:
        pUartAdapter->Parity = LCR_PARITY_ODD;
        break;
        
    case ParityEven:
        pUartAdapter->Parity = LCR_PARITY_EVEN;
        break;

    default:
        DBG_UART_ERR("log_uart_format: Not support parity type %d\n", parity);
        return;
    }

    if (stop_bits > 1) {
        // if width is 5 bits, stop bit will be 1.5 bit
        pUartAdapter->Stop = LCR_STOP_2B;
    } else {
        pUartAdapter->Stop = LCR_STOP_1B;
    }

    HalLogUartSetLineCtrl(pUartAdapter);
}

/******************************************************************************
 * INTERRUPTS HANDLING
 ******************************************************************************/
void log_uart_irq_handler(log_uart_t *obj, loguart_irq_handler handler, uint32_t id) 
{
    HAL_LOG_UART_ADAPTER *pUartAdapter;

    pUartAdapter = &(obj->log_hal_uart);
    pUartAdapter->api_irq_handler = handler;
    pUartAdapter->api_irq_id = id;    
}

void log_uart_irq_set(log_uart_t *obj, LOG_UART_INT_ID irq, uint32_t enable) 
{
    HAL_LOG_UART_ADAPTER *pUartAdapter;
    u8 int_en=0;

    pUartAdapter = &(obj->log_hal_uart);

    switch (irq) {
    case IIR_RX_RDY:
        int_en = IER_ERBFI;
        break;

    case IIR_THR_EMPTY:
        int_en = IER_ETBEI;
        break;

    case IIR_RX_LINE_STATUS:
        int_en = IER_ELSI;
        break;

    case IIR_MODEM_STATUS:
        int_en = IER_EDSSI;
        break;

    default:
        DBG_UART_WARN("log_uart_irq_set: Unknown Irq Id\n");
        return;
    }
        
    if (enable) {
        pUartAdapter->IntEnReg |= int_en;
    } else {
        // disable
        pUartAdapter->IntEnReg &= (~int_en);
    }
    HalLogUartSetIntEn(pUartAdapter);
}

/******************************************************************************
 * READ/WRITE
 ******************************************************************************/

char log_uart_getc(log_uart_t *obj) 
{
    HAL_LOG_UART_ADAPTER *pUartAdapter=(PHAL_LOG_UART_ADAPTER)&(obj->log_hal_uart);

    while (!log_uart_readable(obj));
    return (char)(HAL_UART_READ32(UART_REV_BUF_OFF) & 0xFF);
}

void log_uart_putc(log_uart_t *obj, char c) 
{
    HAL_LOG_UART_ADAPTER *pUartAdapter=(PHAL_LOG_UART_ADAPTER)&(obj->log_hal_uart);
    
    while (!log_uart_writable(obj));
    HAL_UART_WRITE8(UART_TRAN_HOLD_OFF, c);
}

int log_uart_readable(log_uart_t *obj) 
{
    HAL_LOG_UART_ADAPTER *pUartAdapter=(PHAL_LOG_UART_ADAPTER)&(obj->log_hal_uart);
    volatile u8 line_status;

    line_status = HAL_UART_READ8(UART_LINE_STATUS_REG_OFF);

    if (line_status & LSR_DR) {
        return 1;
    } else {
        return 0;
    }
}

int log_uart_writable(log_uart_t *obj) 
{
    HAL_LOG_UART_ADAPTER *pUartAdapter=(PHAL_LOG_UART_ADAPTER)&(obj->log_hal_uart);
    volatile u8 line_status;

    line_status = HAL_UART_READ8(UART_LINE_STATUS_REG_OFF);
    if (line_status & LSR_THRE) {
        return 1;
    } else {
        return 0;
    }
}

void log_uart_clear(log_uart_t *obj) 
{
    HAL_LOG_UART_ADAPTER *pUartAdapter=(PHAL_LOG_UART_ADAPTER)&(obj->log_hal_uart);

    HalLogUartRstFIFO(pUartAdapter, (LOG_UART_RST_TX_FIFO|LOG_UART_RST_TX_FIFO));
    pUartAdapter->TxCount = 0;
    pUartAdapter->RxCount = 0;
}

void log_uart_clear_tx(log_uart_t *obj) 
{
    HAL_LOG_UART_ADAPTER *pUartAdapter=(PHAL_LOG_UART_ADAPTER)&(obj->log_hal_uart);

    HalLogUartRstFIFO(pUartAdapter, LOG_UART_RST_TX_FIFO);
    pUartAdapter->TxCount = 0;
}

void log_uart_clear_rx(log_uart_t *obj) 
{
    HAL_LOG_UART_ADAPTER *pUartAdapter=(PHAL_LOG_UART_ADAPTER)&(obj->log_hal_uart);

    HalLogUartRstFIFO(pUartAdapter, LOG_UART_RST_RX_FIFO);
    pUartAdapter->RxCount = 0;
}

void log_uart_break_set(log_uart_t *obj) 
{
    HAL_LOG_UART_ADAPTER *pUartAdapter=(PHAL_LOG_UART_ADAPTER)&(obj->log_hal_uart);
    u32 RegValue;

    RegValue = HAL_UART_READ32(UART_LINE_CTL_REG_OFF);
    RegValue |= LCR_BC;
    HAL_UART_WRITE32(UART_LINE_CTL_REG_OFF, RegValue);
}

void log_uart_break_clear(log_uart_t *obj) 
{
    HAL_LOG_UART_ADAPTER *pUartAdapter=(PHAL_LOG_UART_ADAPTER)&(obj->log_hal_uart);
    u32 RegValue;

    RegValue = HAL_UART_READ32(UART_LINE_CTL_REG_OFF);
    RegValue &= ~LCR_BC;
    HAL_UART_WRITE32(UART_LINE_CTL_REG_OFF, RegValue);
}

void log_uart_tx_comp_handler(log_uart_t *obj, void *handler, uint32_t id) 
{
    HAL_LOG_UART_ADAPTER *pUartAdapter=(PHAL_LOG_UART_ADAPTER)&(obj->log_hal_uart);

    pUartAdapter->TxCompCallback = (void(*)(void*))handler;
    pUartAdapter->TxCompCbPara = (void*)id;
}

void log_uart_rx_comp_handler(log_uart_t *obj, void *handler, uint32_t id) 
{
    HAL_LOG_UART_ADAPTER *pUartAdapter=&(obj->log_hal_uart);

    pUartAdapter->RxCompCallback = (void(*)(void*))handler;
    pUartAdapter->RxCompCbPara = (void*)id;
}

void log_uart_line_status_handler(log_uart_t *obj, void *handler, uint32_t id) 
{
    HAL_LOG_UART_ADAPTER *pUartAdapter=&(obj->log_hal_uart);

    pUartAdapter->LineStatusCallback = (void(*)(void*, u8))handler;
    pUartAdapter->LineStatusCbPara = (void*)id;
}

// Blocked(busy wait) receive, return received bytes count
int32_t log_uart_recv (log_uart_t *obj, char *prxbuf, uint32_t len, uint32_t timeout_ms)
{
    HAL_LOG_UART_ADAPTER *pUartAdapter=&(obj->log_hal_uart);
    int ret;

    ret = (int)HalLogUartRecv(pUartAdapter, prxbuf, len, timeout_ms);
    return (ret);
}

// Blocked(busy wait) send, return transmitted bytes count
int32_t log_uart_send (log_uart_t *obj, char *ptxbuf, uint32_t len, uint32_t timeout_ms)
{
    HAL_LOG_UART_ADAPTER *pUartAdapter=&(obj->log_hal_uart);
    int ret;

    ret = (int)HalLogUartSend(pUartAdapter, ptxbuf, len, timeout_ms);
    return (ret);
}

// Interrupt mode(no wait) receive, return HAL function result
int32_t log_uart_recv_stream (log_uart_t *obj, char *prxbuf, uint32_t len)
{
    HAL_LOG_UART_ADAPTER *pUartAdapter=&(obj->log_hal_uart);
    int ret;

    ret = (int)HalLogUartIntRecv(pUartAdapter, (u8*)prxbuf, len);
    return (ret);
}

// Interrupt Mode(no wait) send, return HAL function result
int32_t log_uart_send_stream (log_uart_t *obj, char *ptxbuf, uint32_t len)
{
    HAL_LOG_UART_ADAPTER *pUartAdapter=&(obj->log_hal_uart);
    int ret;

    ret = (int)HalLogUartIntSend(pUartAdapter, (u8*)ptxbuf, len);
    return (ret);
}

// Interrupt mode(no wait) receive with timeout
// return the byte count received before timeout, or error(<0)
int32_t log_uart_recv_stream_timeout (log_uart_t *obj, char *prxbuf, uint32_t len,
                                        uint32_t timeout_ms, void *force_cs)
{
    HAL_LOG_UART_ADAPTER *pUartAdapter=&(obj->log_hal_uart);
    uint32_t TimeoutCount=0, StartCount;
    int ret;
    void (*task_yield)(void);

    task_yield = NULL;
    ret = (int)HalLogUartIntRecv(pUartAdapter, (u8*)prxbuf, len);
    
    if ((ret == HAL_OK) && (timeout_ms > 0)) {
        TimeoutCount = (timeout_ms*1000/TIMER_TICK_US);
        StartCount = HalTimerOp.HalTimerReadCount(1);
        task_yield = (void (*)(void))force_cs;
        while (pUartAdapter->RxCount > 0) {
            if (HAL_TIMEOUT == RuartIsTimeout(StartCount, TimeoutCount)) {
                HalLogUartAbortIntRecv(pUartAdapter);
                break;
            }
            if (NULL != task_yield) {
               task_yield();
            }
        }
        return (len - pUartAdapter->RxCount);
    } else {
        return (-ret);
    }
}

// Abort Interrupt Mode TX and return how many bytes data has been sent
int32_t log_uart_send_stream_abort (log_uart_t *obj)
{
    HAL_LOG_UART_ADAPTER *pUartAdapter=&(obj->log_hal_uart);
    int ret;

    HalLogUartAbortIntSend(pUartAdapter);

    ret = (u32)pUartAdapter->pTxBuf - (u32)pUartAdapter->pTxStartAddr;
    return (ret);
}

// Abort Interrupt Mode RX and return how many bytes data has been received
int32_t log_uart_recv_stream_abort (log_uart_t *obj)
{
    HAL_LOG_UART_ADAPTER *pUartAdapter=&(obj->log_hal_uart);
    int ret;

    HalLogUartAbortIntRecv(pUartAdapter);

    ret = (u32)pUartAdapter->pRxBuf - (u32)pUartAdapter->pRxStartAddr;
    return (ret);
}

void log_uart_disable (log_uart_t *obj)
{
    HAL_LOG_UART_ADAPTER *pUartAdapter=&(obj->log_hal_uart);
    
    HalLogUartDisable(pUartAdapter);
}

void log_uart_enable (log_uart_t *obj)
{
    HAL_LOG_UART_ADAPTER *pUartAdapter=&(obj->log_hal_uart);
    
    HalLogUartEnable(pUartAdapter);
}

// to read Line-Status register
// Bit 0: RX Data Ready
// Bit 1: Overrun Error
// Bit 2: Parity Error
// Bit 3: Framing Error
// Bit 4: Break Interrupt (received data input is held in 0 state for a longer than a full word tx time)
// Bit 5: TX FIFO empty (THR empty)
// Bit 6: TX FIFO empty (THR & TSR both empty)
// Bit 7: Receiver FIFO Error (parity error, framing error or break indication)
uint8_t log_uart_raed_lsr(log_uart_t *obj) 
{
    uint8_t LineStatus;

    LineStatus = HAL_UART_READ8(UART_LINE_STATUS_REG_OFF);

    return LineStatus;
}

// to read Modem-Status register
// Bit 0: DCTS, The CTS line has changed its state
// Bit 1: DDSR, The DSR line has changed its state
// Bit 2: TERI, RI line has changed its state from low to high state
// Bit 3: DDCD, DCD line has changed its state
// Bit 4: Complement of the CTS input 
// Bit 5: Complement of the DSR input 
// Bit 6: Complement of the RI input 
// Bit 7: Complement of the DCD input 
uint8_t log_uart_raed_msr(log_uart_t *obj) 
{
    uint8_t RegValue;
    
    RegValue = HAL_UART_READ8(UART_MODEM_STATUS_REG_OFF);
    return RegValue;
}

