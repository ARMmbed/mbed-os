/*
 * Copyright (c) 2020 SparkFun Electronics
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#if DEVICE_I2C

#include "i2c_api.h"
#include "iom_api.h"
#include "PeripheralPins.h"
#include "mbed_assert.h"

#define DEFAULT_CLK_FREQ (AM_HAL_IOM_400KHZ)

static am_hal_iom_transfer_t xfer = {0};

#if DEVICE_I2C_ASYNCH

#define DMATCBBuffer_size 256

static uint32_t DMA0TCBBuffer[DMATCBBuffer_size];
static uint32_t DMA1TCBBuffer[DMATCBBuffer_size];
static uint32_t DMA2TCBBuffer[DMATCBBuffer_size];
static uint32_t DMA3TCBBuffer[DMATCBBuffer_size];
static uint32_t DMA4TCBBuffer[DMATCBBuffer_size];
static uint32_t DMA5TCBBuffer[DMATCBBuffer_size];

static volatile bool g_IOM0DMAComplete = true;
static volatile bool g_IOM1DMAComplete = true;
static volatile bool g_IOM2DMAComplete = true;
static volatile bool g_IOM3DMAComplete = true;
static volatile bool g_IOM4DMAComplete = true;
static volatile bool g_IOM5DMAComplete = true;

static i2c_t * _dma_i2c_obj;
static void (*_dma0_isr_handler)(int) = NULL;
static void (*_dma1_isr_handler)(int) = NULL;
static void (*_dma2_isr_handler)(int) = NULL;
static void (*_dma3_isr_handler)(int) = NULL;
static void (*_dma4_isr_handler)(int) = NULL;
static void (*_dma5_isr_handler)(int) = NULL;

static uint32_t iom0Status, iom1Status, iom2Status, iom3Status, iom4Status, iom5Status;

#endif

I2CName i2c_get_peripheral_name(PinName sda, PinName scl)
{
    uint32_t iom_sda = pinmap_peripheral(sda, i2c_master_sda_pinmap());
    uint32_t iom_scl = pinmap_peripheral(scl, i2c_master_scl_pinmap());

    uint32_t iom = pinmap_merge(iom_sda, iom_scl);

    if ((int)iom == NC) {
        return IOM_NUM;
    }

    return (I2CName)iom;
}

void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{

    MBED_ASSERT(obj);
    // iom determination
    I2CName iom = i2c_get_peripheral_name(sda, scl);
    MBED_ASSERT((int)iom != IOM_NUM);

    // iom configuration
    obj->i2c.iom_obj.iom.inst = (uint32_t)iom;
    obj->i2c.iom_obj.iom.cfg.eInterfaceMode = AM_HAL_IOM_I2C_MODE;
    obj->i2c.iom_obj.iom.cfg.ui32ClockFreq = DEFAULT_CLK_FREQ;
#if DEVICE_I2C_ASYNCH
    if(iom == 0) obj->i2c.iom_obj.iom.cfg.pNBTxnBuf = &DMA0TCBBuffer[0];
    else if(iom == 1) obj->i2c.iom_obj.iom.cfg.pNBTxnBuf = &DMA1TCBBuffer[0];
    else if(iom == 2) obj->i2c.iom_obj.iom.cfg.pNBTxnBuf = &DMA2TCBBuffer[0];
    else if(iom == 3) obj->i2c.iom_obj.iom.cfg.pNBTxnBuf = &DMA3TCBBuffer[0];
    else if(iom == 4) obj->i2c.iom_obj.iom.cfg.pNBTxnBuf = &DMA4TCBBuffer[0];
    else if(iom == 5) obj->i2c.iom_obj.iom.cfg.pNBTxnBuf = &DMA5TCBBuffer[0];
    obj->i2c.iom_obj.iom.cfg.ui32NBTxnBufLength = DMATCBBuffer_size / 4;
#else
    obj->i2c.iom_obj.iom.cfg.pNBTxnBuf = NULL;
    obj->i2c.iom_obj.iom.cfg.ui32NBTxnBufLength = 0;
#endif

    // pin configuration
    if ((int)sda != NC) {
        pinmap_config(sda, i2c_master_sda_pinmap());
    }
    if ((int)scl != NC) {
        pinmap_config(scl, i2c_master_scl_pinmap());
    }

    // invariant xfer settings
    xfer.ui32InstrLen = 0;
    xfer.ui32Instr = 0;
    xfer.ui8RepeatCount = 0;
    xfer.ui8Priority = 1;
    xfer.ui32PauseCondition = 0;
    xfer.ui32StatusSetClr = 0;

    // initialization
    iom_init(&obj->i2c.iom_obj);
#if DEVICE_I2C_ASYNCH
    // Enable interrupts for NB send to work
    am_hal_iom_interrupt_enable(obj->i2c.iom_obj.iom.handle, 0x7FFF);
    if(iom == 0) NVIC_EnableIRQ(IOMSTR0_IRQn);
    else if(iom == 1) NVIC_EnableIRQ(IOMSTR1_IRQn);
    else if(iom == 2) NVIC_EnableIRQ(IOMSTR2_IRQn);
    else if(iom == 3) NVIC_EnableIRQ(IOMSTR3_IRQn);
    else if(iom == 4) NVIC_EnableIRQ(IOMSTR4_IRQn);
    else if(iom == 5) NVIC_EnableIRQ(IOMSTR4_IRQn);
#endif
}

void i2c_free(i2c_t *obj)
{
    MBED_ASSERT(obj);
    iom_deinit(&obj->i2c.iom_obj);
}

void i2c_frequency(i2c_t *obj, int hz)
{
    MBED_ASSERT(obj);
    if (hz > AM_HAL_IOM_MAX_FREQ) {
        hz = AM_HAL_IOM_MAX_FREQ;
    }
    obj->i2c.iom_obj.iom.cfg.ui32ClockFreq = hz;
    iom_init(&obj->i2c.iom_obj);
}

int  i2c_start(i2c_t *obj)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(0);
    return I2C_ERROR_NO_SLAVE;
}

int  i2c_stop(i2c_t *obj)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(0);
    return I2C_ERROR_NO_SLAVE;
}

int i2c_read(i2c_t *obj, int address8bit, char *data, int length, int stop)
{
    MBED_ASSERT(obj);
    int handled_chars = 0;

    xfer.uPeerInfo.ui32I2CDevAddr = (address8bit >> 1);
    xfer.eDirection = AM_HAL_IOM_RX;
    xfer.ui32NumBytes = length;
    xfer.pui32RxBuffer = (uint32_t *)data;
    xfer.pui32TxBuffer = NULL;
    xfer.bContinue = (stop) ? false : true;
    uint32_t status = am_hal_iom_blocking_transfer(obj->i2c.iom_obj.iom.handle, &xfer);
    if (AM_HAL_STATUS_SUCCESS != status) {
        return I2C_ERROR_NO_SLAVE;
    }
    handled_chars += xfer.ui32NumBytes;

    return handled_chars;
}

int i2c_write(i2c_t *obj, int address8bit, const char *data, int length, int stop)
{
    MBED_ASSERT(obj);
    int handled_chars = 0;

    xfer.uPeerInfo.ui32I2CDevAddr = (address8bit >> 1);
    xfer.eDirection = AM_HAL_IOM_TX;
    xfer.ui32NumBytes = length;
    xfer.pui32TxBuffer = (uint32_t *)data;
    xfer.pui32RxBuffer = NULL;
    xfer.bContinue = (stop) ? false : true;
    uint32_t status = am_hal_iom_blocking_transfer(obj->i2c.iom_obj.iom.handle, &xfer);
    if (AM_HAL_STATUS_SUCCESS != status) {
        return I2C_ERROR_NO_SLAVE;
    }
    handled_chars += xfer.ui32NumBytes;

    return handled_chars;
}

void i2c_reset(i2c_t *obj)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(0);
}

int i2c_byte_read(i2c_t *obj, int last)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(0);
    return I2C_ERROR_NO_SLAVE;
}

int i2c_byte_write(i2c_t *obj, int data)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(0);
    return I2C_ERROR_NO_SLAVE;
}

const PinMap *i2c_master_sda_pinmap(void)
{
    return PinMap_I2C_SDA;
}

const PinMap *i2c_master_scl_pinmap(void)
{
    return PinMap_I2C_SCL;
}

const PinMap *i2c_slave_sda_pinmap(void)
{
    return PinMap_I2C_SDA;
}

const PinMap *i2c_slave_scl_pinmap(void)
{
    return PinMap_I2C_SCL;
}

#if DEVICE_I2C_ASYNCH

void
am_iomaster0_isr(void)
{
    uint32_t iomStatus = 0;
    if (!am_hal_iom_interrupt_status_get(_dma_i2c_obj->i2c.iom_obj.iom.handle, true, &iomStatus))
    {
        if ( iomStatus )
        {
            iom0Status = iomStatus;
            am_hal_iom_interrupt_clear(_dma_i2c_obj->i2c.iom_obj.iom.handle, iomStatus);
            am_hal_iom_interrupt_service(_dma_i2c_obj->i2c.iom_obj.iom.handle, iomStatus);
            if(_dma0_isr_handler) _dma0_isr_handler(iomStatus);
        }
    }
}

void
am_iomaster1_isr(void)
{
    uint32_t iomStatus = 0;
    if (!am_hal_iom_interrupt_status_get(_dma_i2c_obj->i2c.iom_obj.iom.handle, true, &iomStatus))
    {
        if ( iomStatus )
        {
            iom1Status = iomStatus;
            am_hal_iom_interrupt_clear(_dma_i2c_obj->i2c.iom_obj.iom.handle, iomStatus);
            am_hal_iom_interrupt_service(_dma_i2c_obj->i2c.iom_obj.iom.handle, iomStatus);
            if(_dma1_isr_handler) _dma1_isr_handler(iomStatus);
        }
    }
}

void
am_iomaster2_isr(void)
{
    uint32_t iomStatus = 0;
    if (!am_hal_iom_interrupt_status_get(_dma_i2c_obj->i2c.iom_obj.iom.handle, true, &iomStatus))
    {
        if ( iomStatus )
        {
            iom2Status = iomStatus;
            am_hal_iom_interrupt_clear(_dma_i2c_obj->i2c.iom_obj.iom.handle, iomStatus);
            am_hal_iom_interrupt_service(_dma_i2c_obj->i2c.iom_obj.iom.handle, iomStatus);
            if(_dma2_isr_handler) _dma2_isr_handler(iomStatus);
        }
    }
}

void
am_iomaster3_isr(void)
{
    uint32_t iomStatus = 0;
    if (!am_hal_iom_interrupt_status_get(_dma_i2c_obj->i2c.iom_obj.iom.handle, true, &iomStatus))
    {
        if ( iomStatus )
        {
            iom3Status = iomStatus;
            am_hal_iom_interrupt_clear(_dma_i2c_obj->i2c.iom_obj.iom.handle, iomStatus);
            am_hal_iom_interrupt_service(_dma_i2c_obj->i2c.iom_obj.iom.handle, iomStatus);
            if(_dma3_isr_handler) _dma3_isr_handler(iomStatus);
        }
    }
}

void
am_iomaster4_isr(void)
{
    uint32_t iomStatus = 0;
    if (!am_hal_iom_interrupt_status_get(_dma_i2c_obj->i2c.iom_obj.iom.handle, true, &iomStatus))
    {
        if ( iomStatus )
        {
            iom4Status = iomStatus;
            am_hal_iom_interrupt_clear(_dma_i2c_obj->i2c.iom_obj.iom.handle, iomStatus);
            am_hal_iom_interrupt_service(_dma_i2c_obj->i2c.iom_obj.iom.handle, iomStatus);
            if(_dma4_isr_handler) _dma4_isr_handler(iomStatus);
        }
    }
}

void
am_iomaster5_isr(void)
{
    uint32_t iomStatus = 0;
    if (!am_hal_iom_interrupt_status_get(_dma_i2c_obj->i2c.iom_obj.iom.handle, true, &iomStatus))
    {
        if ( iomStatus )
        {
            iom5Status = iomStatus;
            am_hal_iom_interrupt_clear(_dma_i2c_obj->i2c.iom_obj.iom.handle, iomStatus);
            am_hal_iom_interrupt_service(_dma_i2c_obj->i2c.iom_obj.iom.handle, iomStatus);
            if(_dma5_isr_handler) _dma5_isr_handler(iomStatus);
        }
    }
}

static void pfnIOM0_DMA_Callback(void *pCallbackCtxt, uint32_t status)
{
    // Set the DMA complete flag.
    g_IOM0DMAComplete = true;
}

static void pfnIOM1_DMA_Callback(void *pCallbackCtxt, uint32_t status)
{
    // Set the DMA complete flag.
    g_IOM1DMAComplete = true;
}

static void pfnIOM2_DMA_Callback(void *pCallbackCtxt, uint32_t status)
{
    // Set the DMA complete flag.
    g_IOM2DMAComplete = true;
}

static void pfnIOM3_DMA_Callback(void *pCallbackCtxt, uint32_t status)
{
    // Set the DMA complete flag.
    g_IOM3DMAComplete = true;
}

static void pfnIOM4_DMA_Callback(void *pCallbackCtxt, uint32_t status)
{
    // Set the DMA complete flag.
    g_IOM4DMAComplete = true;
}

static void pfnIOM5_DMA_Callback(void *pCallbackCtxt, uint32_t status)
{
    // Set the DMA complete flag.
    g_IOM5DMAComplete = true;
}

/** Start i2c asynchronous transfer.
 *  @param obj     The I2C object
 *  @param tx        The buffer to send - set the first byte the register address
 *  @param tx_length The number of words to transmit - 0: when the I2C transaction is read
 *  @param rx        The buffer to receive - set the first byte the register address
 *  @param rx_length The number of words to receive - 0: when the I2C transaction is write
 *  @param address The address to be set - 7bit or 9 bit
 *  @param stop    If true, stop will be generated after the transfer is done
 *  @param handler The I2C IRQ handler to be set
 *  @param hint    DMA hint usage
 */
void i2c_transfer_asynch(i2c_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint32_t address, uint32_t stop, uint32_t handler, uint32_t event, DMAUsage hint)
{
    MBED_ASSERT(obj);

    _dma_i2c_obj = obj;

    if( (tx_length == 0) && (rx_length == 0)) return;

    am_hal_iom_transfer_t       Transaction;

    Transaction.ui8Priority     = 1;        // High priority for now.
    Transaction.ui32InstrLen    = 1;
    if(rx == NULL) {
        Transaction.eDirection      = AM_HAL_IOM_TX;
        char * ptr = (char *)tx;
        Transaction.ui32Instr       = *ptr;
        Transaction.ui32NumBytes    = tx_length;
        ptr++;
        Transaction.pui32TxBuffer   = (const uint32_t *)ptr;
    }
    else if(tx == NULL) {
        Transaction.eDirection      = AM_HAL_IOM_RX;
        Transaction.ui32Instr       = *(char *)rx;
        Transaction.ui32NumBytes    = rx_length;
        Transaction.pui32RxBuffer   = (uint32_t *)rx;
    }
    Transaction.bContinue       = false;
    Transaction.ui8RepeatCount  = 0;
    Transaction.ui32PauseCondition = 0;
    Transaction.ui32StatusSetClr = 0;
    Transaction.uPeerInfo.ui32I2CDevAddr = address;

    void *pCallbackCtxt;
    bool * g_IOMDMAComplete;

    if(obj->i2c.iom_obj.iom.inst == 0) {
        pCallbackCtxt = pfnIOM0_DMA_Callback;
        g_IOM0DMAComplete = false ;
        g_IOMDMAComplete = &g_IOM0DMAComplete;
        _dma0_isr_handler =  handler;
    }
    else if(obj->i2c.iom_obj.iom.inst == 1)
    {
        pCallbackCtxt = pfnIOM1_DMA_Callback;
        g_IOM1DMAComplete = false ;
        g_IOMDMAComplete = &g_IOM1DMAComplete;
        _dma1_isr_handler =  handler;
    }
    else if(obj->i2c.iom_obj.iom.inst == 2)
    {
        pCallbackCtxt = pfnIOM2_DMA_Callback;
        g_IOM2DMAComplete = false ;
        g_IOMDMAComplete = &g_IOM2DMAComplete;
        _dma2_isr_handler =  handler;
    }
    else if(obj->i2c.iom_obj.iom.inst == 3)
    {
        pCallbackCtxt = pfnIOM3_DMA_Callback;
        g_IOM3DMAComplete = false ;
        g_IOMDMAComplete = &g_IOM3DMAComplete;
        _dma3_isr_handler =  handler;
    }
    else if(obj->i2c.iom_obj.iom.inst == 4)
    {
        pCallbackCtxt = pfnIOM4_DMA_Callback;
        g_IOM4DMAComplete = false ;
        g_IOMDMAComplete = &g_IOM4DMAComplete;
        _dma4_isr_handler =  handler;
    }
    else if(obj->i2c.iom_obj.iom.inst == 5)
    {
        pCallbackCtxt = pfnIOM5_DMA_Callback;
        g_IOM4DMAComplete = false ;
        g_IOMDMAComplete = &g_IOM5DMAComplete;
        _dma4_isr_handler =  handler;
    }

    am_hal_iom_nonblocking_transfer(obj->i2c.iom_obj.iom.handle, &Transaction,pCallbackCtxt, NULL);

#if 1
    while (!(*g_IOMDMAComplete)) {
        //am_util_stdio_printf("# trans: %d\n", Transaction.ui32NumBytes );
        uint32_t ui32Status;
        if (!am_hal_iom_interrupt_status_get(_dma_i2c_obj->i2c.iom_obj.iom.handle, true, &ui32Status)) {
            //am_util_stdio_printf("dma Int.: %x!\n",ui32Status);
            am_hal_iom_status_t psStatus;
            am_hal_iom_status_get(_dma_i2c_obj->i2c.iom_obj.iom.handle, &psStatus);
            //am_util_stdio_printf("dma state: \n %x \n %x \n %x \n %x \n %x  \n %x \n",
            //psStatus.bStatIdle,
            //psStatus.bStatCmdAct,
            //psStatus.bStatErr,
            //psStatus.ui32DmaStat,
            //psStatus.ui32MaxTransactions,
            //psStatus.ui32NumPendTransactions);
        }
    }

#endif
}

uint32_t i2c_irq_handler_asynch(i2c_t *obj)
{
    MBED_ASSERT(obj);
    uint32_t iomStatus;
    if(obj->i2c.iom_obj.iom.inst == 0) iomStatus = iom0Status;
    else if(obj->i2c.iom_obj.iom.inst == 1) iomStatus = iom1Status;
    else if(obj->i2c.iom_obj.iom.inst == 2) iomStatus = iom2Status;
    else if(obj->i2c.iom_obj.iom.inst == 3) iomStatus = iom3Status;
    else if(obj->i2c.iom_obj.iom.inst == 4) iomStatus = iom4Status;
    else if(obj->i2c.iom_obj.iom.inst == 5) iomStatus = iom5Status;

    if(iomStatus & AM_HAL_IOM_INT_DCMP) // DMA transfer complete
    {
        return 1;
    }
    return 0;
}

uint8_t i2c_active(i2c_t *obj)
{
    MBED_ASSERT(obj);

    int active;

    if(obj->i2c.iom_obj.iom.inst == 0) active = g_IOM0DMAComplete;
    else if(obj->i2c.iom_obj.iom.inst == 1) active = g_IOM1DMAComplete;
    else if(obj->i2c.iom_obj.iom.inst == 2) active = g_IOM2DMAComplete;
    else if(obj->i2c.iom_obj.iom.inst == 3) active = g_IOM3DMAComplete;
    else if(obj->i2c.iom_obj.iom.inst == 4) active = g_IOM4DMAComplete;
    else if(obj->i2c.iom_obj.iom.inst == 5) active = g_IOM5DMAComplete;

    if(active)
        return 0;  // no transaction ongoing
    else
        return 1;   // transaction ongoing
}

void i2c_abort_asynch(i2c_t *obj)
{
    MBED_ASSERT(obj);

}


#endif
#endif // DEVICE_I2C
