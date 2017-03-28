/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
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

#include "spi_api.h"

#if DEVICE_SPI

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "nu_modutil.h"
#include "nu_miscutil.h"
#include "nu_bitutil.h"

#if DEVICE_SPI_ASYNCH
#include "dma_api.h"
#include "dma.h"
#endif

#define NU_SPI_FRAME_MIN    8
#define NU_SPI_FRAME_MAX    32
#define NU_SPI_FIFO_DEPTH   8

struct nu_spi_var {
#if DEVICE_SPI_ASYNCH
    uint8_t     pdma_perp_tx;
    uint8_t     pdma_perp_rx;
#endif
};

static struct nu_spi_var spi0_var = {
#if DEVICE_SPI_ASYNCH
    .pdma_perp_tx       =   PDMA_SPI0_TX,
    .pdma_perp_rx       =   PDMA_SPI0_RX
#endif
};
static struct nu_spi_var spi1_var = {
#if DEVICE_SPI_ASYNCH
    .pdma_perp_tx       =   PDMA_SPI1_TX,
    .pdma_perp_rx       =   PDMA_SPI1_RX
#endif
};
static struct nu_spi_var spi2_var = {
#if DEVICE_SPI_ASYNCH
    .pdma_perp_tx       =   PDMA_SPI2_TX,
    .pdma_perp_rx       =   PDMA_SPI2_RX
#endif
};
static struct nu_spi_var spi3_var = {
#if DEVICE_SPI_ASYNCH
    .pdma_perp_tx       =   PDMA_SPI3_TX,
    .pdma_perp_rx       =   PDMA_SPI3_RX
#endif
};

#if DEVICE_SPI_ASYNCH
static void spi_enable_vector_interrupt(spi_t *obj, uint32_t handler, uint8_t enable);
static void spi_master_enable_interrupt(spi_t *obj, uint8_t enable);
static uint32_t spi_master_write_asynch(spi_t *obj, uint32_t tx_limit);
static uint32_t spi_master_read_asynch(spi_t *obj);
static uint32_t spi_event_check(spi_t *obj);
static void spi_enable_event(spi_t *obj, uint32_t event, uint8_t enable);
static void spi_buffer_set(spi_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length);
static void spi_check_dma_usage(DMAUsage *dma_usage, int *dma_ch_tx, int *dma_ch_rx);
static uint8_t spi_get_data_width(spi_t *obj);
static int spi_is_tx_complete(spi_t *obj);
static int spi_is_rx_complete(spi_t *obj);
static int spi_writeable(spi_t * obj);
static int spi_readable(spi_t * obj);
static void spi_dma_handler_tx(uint32_t id, uint32_t event_dma);
static void spi_dma_handler_rx(uint32_t id, uint32_t event_dma);
#endif

static uint32_t spi_modinit_mask = 0;

static const struct nu_modinit_s spi_modinit_tab[] = {
    {SPI_0, SPI0_MODULE, 0, 0, SPI0_RST, SPI0_IRQn, &spi0_var},
    {SPI_1, SPI1_MODULE, 0, 0, SPI1_RST, SPI1_IRQn, &spi1_var},
    {SPI_2, SPI2_MODULE, 0, 0, SPI2_RST, SPI2_IRQn, &spi2_var},
    {SPI_3, SPI3_MODULE, 0, 0, SPI3_RST, SPI3_IRQn, &spi3_var},
    
    {NC, 0, 0, 0, 0, (IRQn_Type) 0, NULL}
};

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel) {
    // Determine which SPI_x the pins are used for
    uint32_t spi_mosi = pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    uint32_t spi_miso = pinmap_peripheral(miso, PinMap_SPI_MISO);
    uint32_t spi_sclk = pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    uint32_t spi_ssel = pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    uint32_t spi_data = pinmap_merge(spi_mosi, spi_miso);
    uint32_t spi_cntl = pinmap_merge(spi_sclk, spi_ssel);
    obj->spi.spi = (SPIName) pinmap_merge(spi_data, spi_cntl);
    MBED_ASSERT((int)obj->spi.spi != NC);

    const struct nu_modinit_s *modinit = get_modinit(obj->spi.spi, spi_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == obj->spi.spi);
    
    // Reset this module
    SYS_ResetModule(modinit->rsetidx);
    
    // Enable IP clock
    CLK_EnableModuleClock(modinit->clkidx);

    //SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);
    
    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    pinmap_pinout(ssel, PinMap_SPI_SSEL);
        
    obj->spi.pin_mosi = mosi;
    obj->spi.pin_miso = miso;
    obj->spi.pin_sclk = sclk;
    obj->spi.pin_ssel = ssel;
    
    // Configure the SPI data format and frequency
    //spi_format(obj, 8, 0, SPI_MSB); // 8 bits, mode 0
    //spi_frequency(obj, 1000000);
    
#if DEVICE_SPI_ASYNCH
    obj->spi.dma_usage = DMA_USAGE_NEVER;
    obj->spi.event = 0;
    obj->spi.dma_chn_id_tx = DMA_ERROR_OUT_OF_CHANNELS;
    obj->spi.dma_chn_id_rx = DMA_ERROR_OUT_OF_CHANNELS;
#endif

    // Mark this module to be inited.
    int i = modinit - spi_modinit_tab;
    spi_modinit_mask |= 1 << i;
}

void spi_free(spi_t *obj)
{
#if DEVICE_SPI_ASYNCH
    if (obj->spi.dma_chn_id_tx != DMA_ERROR_OUT_OF_CHANNELS) {
        dma_channel_free(obj->spi.dma_chn_id_tx);
        obj->spi.dma_chn_id_tx = DMA_ERROR_OUT_OF_CHANNELS;
    }
    if (obj->spi.dma_chn_id_rx != DMA_ERROR_OUT_OF_CHANNELS) {
        dma_channel_free(obj->spi.dma_chn_id_rx);
        obj->spi.dma_chn_id_rx = DMA_ERROR_OUT_OF_CHANNELS;
    }
#endif

    SPI_Close((SPI_T *) NU_MODBASE(obj->spi.spi));
    
    const struct nu_modinit_s *modinit = get_modinit(obj->spi.spi, spi_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == obj->spi.spi);
    
    SPI_DisableInt(((SPI_T *) NU_MODBASE(obj->spi.spi)), (SPI_FIFO_RXOVIEN_MASK | SPI_FIFO_RXTHIEN_MASK | SPI_FIFO_TXTHIEN_MASK));
    NVIC_DisableIRQ(modinit->irq_n);
    
    // Disable IP clock
    CLK_DisableModuleClock(modinit->clkidx);
    
    //((struct nu_spi_var *) modinit->var)->obj = NULL;
    
    // Mark this module to be deinited.
    int i = modinit - spi_modinit_tab;
    spi_modinit_mask &= ~(1 << i);
}
void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    MBED_ASSERT(bits >= NU_SPI_FRAME_MIN && bits <= NU_SPI_FRAME_MAX);
    
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);
    
    // NOTE 1: All configurations should be ready before enabling SPI peripheral.
    // NOTE 2: Re-configuration is allowed only as SPI peripheral is idle.
    while (SPI_IS_BUSY(spi_base));
    SPI_DISABLE(spi_base);
    
    SPI_Open(spi_base,
        slave ? SPI_SLAVE : SPI_MASTER,
        (mode == 0) ? SPI_MODE_0 : (mode == 1) ? SPI_MODE_1 : (mode == 2) ? SPI_MODE_2 : SPI_MODE_3,
        bits,
        SPI_GetBusClock(spi_base));
    // NOTE: Hardcode to be MSB first.
    SPI_SET_MSB_FIRST(spi_base);
    
    if (! slave) {
        // Master
        if (obj->spi.pin_ssel != NC) {
            // Configure SS as low active.            
            SPI_EnableAutoSS(spi_base, SPI_SS0, SPI_SS_ACTIVE_LOW);
            // NOTE: In NUC472 series, all SPI SS pins are SS0, so we can hardcode SS0 here.
        }
        else {
            SPI_DisableAutoSS(spi_base);
        }
    }
    else {
        // Slave
        // Configure SS as low active.
        spi_base->SSCTL &= ~SPI_SSCTL_SSACTPOL_Msk;
        // NOTE: SPI_SS0 is defined as the slave select input in Slave mode.
    }
}

void spi_frequency(spi_t *obj, int hz)
{
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);
    
    while (SPI_IS_BUSY(spi_base));
    SPI_DISABLE(spi_base);
    
    SPI_SetBusClock((SPI_T *) NU_MODBASE(obj->spi.spi), hz);
}


int spi_master_write(spi_t *obj, int value)
{
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);
    
    // NOTE: Data in receive FIFO can be read out via ICE.
    SPI_ENABLE(spi_base);
    
    // Wait for tx buffer empty
    while(! spi_writeable(obj));
    SPI_WRITE_TX(spi_base, value);

    // Wait for rx buffer full
    while (! spi_readable(obj));
    int value2 = SPI_READ_RX(spi_base);
    
    SPI_DISABLE(spi_base);
    
    return value2;
}

#if DEVICE_SPISLAVE
int spi_slave_receive(spi_t *obj)
{
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);
    
    SPI_ENABLE(spi_base);
    
    return spi_readable(obj);
};

int spi_slave_read(spi_t *obj)
{
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);
    
    SPI_ENABLE(spi_base);
    
    // Wait for rx buffer full
    while (! spi_readable(obj));
    int value = SPI_READ_RX(spi_base);
    return value;
}

void spi_slave_write(spi_t *obj, int value)
{
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);
    
    SPI_ENABLE(spi_base);
    
    // Wait for tx buffer empty
    while(! spi_writeable(obj));
    SPI_WRITE_TX(spi_base, value);
}
#endif

#if DEVICE_SPI_ASYNCH
void spi_master_transfer(spi_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint8_t bit_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
    //MBED_ASSERT(bits >= NU_SPI_FRAME_MIN && bits <= NU_SPI_FRAME_MAX);
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);
    SPI_SET_DATA_WIDTH(spi_base, bit_width);
    
    obj->spi.dma_usage = hint;
    spi_check_dma_usage(&obj->spi.dma_usage, &obj->spi.dma_chn_id_tx, &obj->spi.dma_chn_id_rx);
    uint32_t data_width = spi_get_data_width(obj);
    // Conditions to go DMA way:
    // (1) No DMA support for non-8 multiple data width.
    // (2) tx length >= rx length. Otherwise, as tx DMA is done, no bus activity for remaining rx.
    if ((data_width % 8) ||
        (tx_length < rx_length)) {
        obj->spi.dma_usage = DMA_USAGE_NEVER;
        dma_channel_free(obj->spi.dma_chn_id_tx);
        obj->spi.dma_chn_id_tx = DMA_ERROR_OUT_OF_CHANNELS;
        dma_channel_free(obj->spi.dma_chn_id_rx);
        obj->spi.dma_chn_id_rx = DMA_ERROR_OUT_OF_CHANNELS;
    }
    
    // SPI IRQ is necessary for both interrupt way and DMA way
    spi_enable_event(obj, event, 1);
    spi_buffer_set(obj, tx, tx_length, rx, rx_length);
            
    SPI_ENABLE(spi_base);
    
    if (obj->spi.dma_usage == DMA_USAGE_NEVER) {
        // Interrupt way
        spi_master_write_asynch(obj, NU_SPI_FIFO_DEPTH / 2);
        spi_enable_vector_interrupt(obj, handler, 1);
        spi_master_enable_interrupt(obj, 1);
    } else {
        // DMA way
        const struct nu_modinit_s *modinit = get_modinit(obj->spi.spi, spi_modinit_tab);
        MBED_ASSERT(modinit != NULL);
        MBED_ASSERT(modinit->modname == obj->spi.spi);
    
        PDMA_T *pdma_base = dma_modbase();
        
        // Configure tx DMA
        pdma_base->CHCTL |= 1 << obj->spi.dma_chn_id_tx;  // Enable this DMA channel
        PDMA_SetTransferMode(obj->spi.dma_chn_id_tx,
            ((struct nu_spi_var *) modinit->var)->pdma_perp_tx,    // Peripheral connected to this PDMA
            0,  // Scatter-gather disabled
            0); // Scatter-gather descriptor address
        PDMA_SetTransferCnt(obj->spi.dma_chn_id_tx, 
            (data_width == 8) ? PDMA_WIDTH_8 : (data_width == 16) ? PDMA_WIDTH_16 : PDMA_WIDTH_32, 
            tx_length);
        PDMA_SetTransferAddr(obj->spi.dma_chn_id_tx, 
            ((uint32_t) tx) + (data_width / 8) * tx_length,   // NOTE: End of source address
            PDMA_SAR_INC,   // Source address incremental
            (uint32_t) &spi_base->TX,   // Destination address
            PDMA_DAR_FIX);  // Destination address fixed
        PDMA_SetBurstType(obj->spi.dma_chn_id_tx, 
            PDMA_REQ_SINGLE,    // Single mode
            0); // Burst size
        PDMA_EnableInt(obj->spi.dma_chn_id_tx,
            0); // Interrupt type. No use here
        // Register DMA event handler
        dma_set_handler(obj->spi.dma_chn_id_tx, (uint32_t) spi_dma_handler_tx, (uint32_t) obj, DMA_EVENT_ALL);
        
        // Configure rx DMA
        pdma_base->CHCTL |= 1 << obj->spi.dma_chn_id_rx;  // Enable this DMA channel
        PDMA_SetTransferMode(obj->spi.dma_chn_id_rx,
            ((struct nu_spi_var *) modinit->var)->pdma_perp_rx,    // Peripheral connected to this PDMA
            0,  // Scatter-gather disabled
            0); // Scatter-gather descriptor address
        PDMA_SetTransferCnt(obj->spi.dma_chn_id_rx, 
            (data_width == 8) ? PDMA_WIDTH_8 : (data_width == 16) ? PDMA_WIDTH_16 : PDMA_WIDTH_32, 
            rx_length);
        PDMA_SetTransferAddr(obj->spi.dma_chn_id_rx,
            (uint32_t) &spi_base->RX,   // Source address
            PDMA_SAR_FIX,   // Source address fixed
            ((uint32_t) rx) + (data_width / 8) * rx_length,   // NOTE: End of destination address
            PDMA_DAR_INC);  // Destination address incremental
        PDMA_SetBurstType(obj->spi.dma_chn_id_rx, 
            PDMA_REQ_SINGLE,    // Single mode
            0); // Burst size
        PDMA_EnableInt(obj->spi.dma_chn_id_rx,
            0); // Interrupt type. No use here
        // Register DMA event handler
        dma_set_handler(obj->spi.dma_chn_id_rx, (uint32_t) spi_dma_handler_rx, (uint32_t) obj, DMA_EVENT_ALL);
        
        // Start tx/rx DMA transfer
        spi_enable_vector_interrupt(obj, handler, 1);
        // NOTE: It is safer to start rx DMA first and then tx DMA. Otherwise, receive FIFO is subject to overflow by tx DMA.
        SPI_TRIGGER_RX_PDMA(((SPI_T *) NU_MODBASE(obj->spi.spi)));
        SPI_TRIGGER_TX_PDMA(((SPI_T *) NU_MODBASE(obj->spi.spi)));
        spi_master_enable_interrupt(obj, 1);
    }
}

/**
 * Abort an SPI transfer
 * This is a helper function for event handling. When any of the events listed occurs, the HAL will abort any ongoing
 * transfers
 * @param[in] obj The SPI peripheral to stop
 */
void spi_abort_asynch(spi_t *obj)
{
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);
    PDMA_T *pdma_base = dma_modbase();
    
    if (obj->spi.dma_usage != DMA_USAGE_NEVER) {
        // Receive FIFO Overrun in case of tx length > rx length on DMA way
        if (spi_base->STATUS & SPI_STATUS_RXOVIF_Msk) {
            spi_base->STATUS = SPI_STATUS_RXOVIF_Msk;
        }
        
        if (obj->spi.dma_chn_id_tx != DMA_ERROR_OUT_OF_CHANNELS) {
            PDMA_DisableInt(obj->spi.dma_chn_id_tx, 0);
            // FIXME: Next PDMA transfer will fail with PDMA_STOP() called. Cause is unknown.
            //PDMA_STOP(obj->spi.dma_chn_id_tx);
            pdma_base->CHCTL &= ~(1 << obj->spi.dma_chn_id_tx);
        }
        SPI_DISABLE_TX_PDMA(((SPI_T *) NU_MODBASE(obj->spi.spi)));
        
        if (obj->spi.dma_chn_id_rx != DMA_ERROR_OUT_OF_CHANNELS) {
            PDMA_DisableInt(obj->spi.dma_chn_id_rx, 0);
            // FIXME: Next PDMA transfer will fail with PDMA_STOP() called. Cause is unknown.
            //PDMA_STOP(obj->spi.dma_chn_id_rx);
            pdma_base->CHCTL &= ~(1 << obj->spi.dma_chn_id_rx);
        }
        SPI_DISABLE_RX_PDMA(((SPI_T *) NU_MODBASE(obj->spi.spi)));
    }
    
    // Necessary for both interrupt way and DMA way
    spi_enable_vector_interrupt(obj, 0, 0);
    spi_master_enable_interrupt(obj, 0);
    
    // FIXME: SPI H/W may get out of state without the busy check.
    while (SPI_IS_BUSY(spi_base));
    SPI_DISABLE(spi_base);
    
    SPI_ClearRxFIFO(spi_base);
    SPI_ClearTxFIFO(spi_base);
}

/**
 * Handle the SPI interrupt
 * Read frames until the RX FIFO is empty.  Write at most as many frames as were read.  This way,
 * it is unlikely that the RX FIFO will overflow.
 * @param[in] obj The SPI peripheral that generated the interrupt
 * @return
 */
uint32_t spi_irq_handler_asynch(spi_t *obj)
{
    // Check for SPI events
    uint32_t event = spi_event_check(obj);
    if (event) {
        spi_abort_asynch(obj);
    }

    return (obj->spi.event & event) | ((event & SPI_EVENT_COMPLETE) ? SPI_EVENT_INTERNAL_TRANSFER_COMPLETE : 0);
}

uint8_t spi_active(spi_t *obj)
{
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);
    // FIXME
    /*
    if ((obj->rx_buff.buffer && obj->rx_buff.pos < obj->rx_buff.length)
            || (obj->tx_buff.buffer && obj->tx_buff.pos < obj->tx_buff.length) ){
        return 1;
    } else  {
        // interrupts are disabled, all transaction have been completed
        // TODO: checking rx fifo, it reports data eventhough RFDF is not set
        return DSPI_HAL_GetIntMode(obj->spi.address, kDspiRxFifoDrainRequest);
    }*/
    
    //return SPI_IS_BUSY(spi_base);
    return (spi_base->CTL & SPI_CTL_SPIEN_Msk);
}

int spi_allow_powerdown(void)
{
    uint32_t modinit_mask = spi_modinit_mask;
    while (modinit_mask) {
        int spi_idx = nu_ctz(modinit_mask);
        const struct nu_modinit_s *modinit = spi_modinit_tab + spi_idx;
        if (modinit->modname != NC) {
            SPI_T *spi_base = (SPI_T *) NU_MODBASE(modinit->modname);
            // Disallow entering power-down mode if SPI transfer is enabled.
            if (spi_base->CTL & SPI_CTL_SPIEN_Msk) {
                return 0;
            }
        }
        modinit_mask &= ~(1 << spi_idx);
    }
    
    return 1;
}

static int spi_writeable(spi_t * obj)
{
    // Receive FIFO must not be full to avoid receive FIFO overflow on next transmit/receive
    //return (! SPI_GET_TX_FIFO_FULL_FLAG(((SPI_T *) NU_MODBASE(obj->spi.spi)))) && (SPI_GET_RX_FIFO_COUNT(((SPI_T *) NU_MODBASE(obj->spi.spi))) < NU_SPI_FIFO_DEPTH);
    return (! SPI_GET_TX_FIFO_FULL_FLAG(((SPI_T *) NU_MODBASE(obj->spi.spi))));
}

static int spi_readable(spi_t * obj)
{
    return ! SPI_GET_RX_FIFO_EMPTY_FLAG(((SPI_T *) NU_MODBASE(obj->spi.spi)));
}

static void spi_enable_event(spi_t *obj, uint32_t event, uint8_t enable)
{   
    obj->spi.event &= ~SPI_EVENT_ALL;
    obj->spi.event |= (event & SPI_EVENT_ALL);
    if (event & SPI_EVENT_RX_OVERFLOW) {
        SPI_EnableInt((SPI_T *) NU_MODBASE(obj->spi.spi), SPI_FIFO_RXOVIEN_MASK);
    }
}

static void spi_enable_vector_interrupt(spi_t *obj, uint32_t handler, uint8_t enable)
{
    const struct nu_modinit_s *modinit = get_modinit(obj->spi.spi, spi_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == obj->spi.spi);
    
    if (enable) {
        NVIC_SetVector(modinit->irq_n, handler);
        NVIC_EnableIRQ(modinit->irq_n);
    }
    else {
        //NVIC_SetVector(modinit->irq_n, handler);
        NVIC_DisableIRQ(modinit->irq_n);
    }
}

static void spi_master_enable_interrupt(spi_t *obj, uint8_t enable)
{   
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);
    
    if (enable) {
        SPI_SetFIFOThreshold(spi_base, 4, 4);
        //SPI_SET_SUSPEND_CYCLE(spi_base, 4);
        // Enable tx/rx FIFO threshold interrupt
        SPI_EnableInt(spi_base, SPI_FIFO_RXTHIEN_MASK | SPI_FIFO_TXTHIEN_MASK);
    }
    else {
        SPI_DisableInt(spi_base, SPI_FIFO_RXTHIEN_MASK | SPI_FIFO_TXTHIEN_MASK);
    }
}

static uint32_t spi_event_check(spi_t *obj)
{
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);
    uint32_t event = 0;
    
    if (obj->spi.dma_usage == DMA_USAGE_NEVER) {
        uint32_t n_rec = spi_master_read_asynch(obj);
        spi_master_write_asynch(obj, n_rec);
    }
    
    if (spi_is_tx_complete(obj) && spi_is_rx_complete(obj)) {
        event |= SPI_EVENT_COMPLETE;
    }
    
    // Receive FIFO Overrun
    if (spi_base->STATUS & SPI_STATUS_RXOVIF_Msk) {
        spi_base->STATUS = SPI_STATUS_RXOVIF_Msk;
        // In case of tx length > rx length on DMA way
        if (obj->spi.dma_usage == DMA_USAGE_NEVER) {
            event |= SPI_EVENT_RX_OVERFLOW;
        }
    }
    
    // Receive Time-Out
    if (spi_base->STATUS & SPI_STATUS_RXTOIF_Msk) {
        spi_base->STATUS = SPI_STATUS_RXTOIF_Msk;
        //event |= SPI_EVENT_ERROR;
    }
    // Transmit FIFO Under-Run
    if (spi_base->STATUS & SPI_STATUS_TXUFIF_Msk) {
        spi_base->STATUS = SPI_STATUS_TXUFIF_Msk;
        event |= SPI_EVENT_ERROR;
    }
    
    return event;
}

/**
 * Send words from the SPI TX buffer until the send limit is reached or the TX FIFO is full
 * tx_limit is provided to ensure that the number of SPI frames (words) in flight can be managed.
 * @param[in] obj       The SPI object on which to operate
 * @param[in] tx_limit  The maximum number of words to send
 * @return The number of SPI words that have been transfered
 */
static uint32_t spi_master_write_asynch(spi_t *obj, uint32_t tx_limit)
{
    uint32_t n_words = 0;
    uint32_t tx_rmn = obj->tx_buff.length - obj->tx_buff.pos;
    uint32_t rx_rmn = obj->rx_buff.length - obj->rx_buff.pos;
    uint32_t max_tx = NU_MAX(tx_rmn, rx_rmn);
    max_tx = NU_MIN(max_tx, tx_limit);
    uint8_t data_width = spi_get_data_width(obj);
    uint8_t bytes_per_word = (data_width + 7) / 8;
    uint8_t *tx = (uint8_t *)(obj->tx_buff.buffer) + bytes_per_word * obj->tx_buff.pos;
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);
    
    while ((n_words < max_tx) && spi_writeable(obj)) {
        if (spi_is_tx_complete(obj)) {
            // Transmit dummy as transmit buffer is empty
            SPI_WRITE_TX(spi_base, 0);
        }
        else {
            switch (bytes_per_word) {
                case 4:
                    SPI_WRITE_TX(spi_base, nu_get32_le(tx));
                    tx += 4;
                    break;
                case 2:
                    SPI_WRITE_TX(spi_base, nu_get16_le(tx));
                    tx += 2;
                    break;
                case 1:
                    SPI_WRITE_TX(spi_base, *((uint8_t *) tx));
                    tx += 1;
                    break;
            }
        
            obj->tx_buff.pos ++;
        }
        n_words ++;
    }
    
    //Return the number of words that have been sent
    return n_words;
}

/**
 * Read SPI words out of the RX FIFO
 * Continues reading words out of the RX FIFO until the following condition is met:
 * o There are no more words in the FIFO
 * OR BOTH OF:
 * o At least as many words as the TX buffer have been received
 * o At least as many words as the RX buffer have been received
 * This way, RX overflows are not generated when the TX buffer size exceeds the RX buffer size
 * @param[in] obj The SPI object on which to operate
 * @return Returns the number of words extracted from the RX FIFO
 */
static uint32_t spi_master_read_asynch(spi_t *obj)
{
    uint32_t n_words = 0;
    uint32_t tx_rmn = obj->tx_buff.length - obj->tx_buff.pos;
    uint32_t rx_rmn = obj->rx_buff.length - obj->rx_buff.pos;
    uint32_t max_rx = NU_MAX(tx_rmn, rx_rmn);
    uint8_t data_width = spi_get_data_width(obj);
    uint8_t bytes_per_word = (data_width + 7) / 8;
    uint8_t *rx = (uint8_t *)(obj->rx_buff.buffer) + bytes_per_word * obj->rx_buff.pos;
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);
    
    while ((n_words < max_rx) && spi_readable(obj)) {
        if (spi_is_rx_complete(obj)) {
            // Disregard as receive buffer is full
            SPI_READ_RX(spi_base);
        }
        else {
            switch (bytes_per_word) {
                case 4: {
                    uint32_t val = SPI_READ_RX(spi_base);
                    nu_set32_le(rx, val);
                    rx += 4;
                    break;
                }
                case 2: {
                    uint16_t val = SPI_READ_RX(spi_base);
                    nu_set16_le(rx, val);
                    rx += 2;
                    break;
                }
                case 1:
                    *rx ++ = SPI_READ_RX(spi_base);
                    break;
            }
        
            obj->rx_buff.pos ++;
        }
        n_words ++;
    }
    
    // Return the number of words received
    return n_words;
}

static void spi_buffer_set(spi_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length)
{
    obj->tx_buff.buffer = (void *) tx;
    obj->tx_buff.length = tx_length;
    obj->tx_buff.pos = 0;
    obj->tx_buff.width = spi_get_data_width(obj);
    obj->rx_buff.buffer = rx;
    obj->rx_buff.length = rx_length;
    obj->rx_buff.pos = 0;
    obj->rx_buff.width = spi_get_data_width(obj);
}

static void spi_check_dma_usage(DMAUsage *dma_usage, int *dma_ch_tx, int *dma_ch_rx)
{
    if (*dma_usage != DMA_USAGE_NEVER) {
        if (*dma_ch_tx == DMA_ERROR_OUT_OF_CHANNELS) {
            *dma_ch_tx = dma_channel_allocate(DMA_CAP_NONE);
        }
        if (*dma_ch_rx == DMA_ERROR_OUT_OF_CHANNELS) {
            *dma_ch_rx = dma_channel_allocate(DMA_CAP_NONE);
        }
        
        if (*dma_ch_tx == DMA_ERROR_OUT_OF_CHANNELS || *dma_ch_rx == DMA_ERROR_OUT_OF_CHANNELS) {
            *dma_usage = DMA_USAGE_NEVER;
        }
    }
    
    if (*dma_usage == DMA_USAGE_NEVER) {
        dma_channel_free(*dma_ch_tx);
        *dma_ch_tx = DMA_ERROR_OUT_OF_CHANNELS;
        dma_channel_free(*dma_ch_rx);
        *dma_ch_rx = DMA_ERROR_OUT_OF_CHANNELS;
    }
}

static uint8_t spi_get_data_width(spi_t *obj)
{    
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);
    
    uint32_t data_width = ((spi_base->CTL & SPI_CTL_DWIDTH_Msk) >> SPI_CTL_DWIDTH_Pos);
    if (data_width == 0) {
        data_width = 32;
    }
    
    return data_width;
}

static int spi_is_tx_complete(spi_t *obj)
{
    // ???: Exclude tx fifo empty check due to no such interrupt on DMA way
    return (obj->tx_buff.pos == obj->tx_buff.length);
    //return (obj->tx_buff.pos == obj->tx_buff.length && SPI_GET_TX_FIFO_EMPTY_FLAG(((SPI_T *) NU_MODBASE(obj->spi.spi))));
}

static int spi_is_rx_complete(spi_t *obj)
{
    return (obj->rx_buff.pos == obj->rx_buff.length);
}

static void spi_dma_handler_tx(uint32_t id, uint32_t event_dma)
{
    spi_t *obj = (spi_t *) id;
    
    // FIXME: Pass this error to caller
    if (event_dma & DMA_EVENT_ABORT) {
    }
    // Expect SPI IRQ will catch this transfer done event
    if (event_dma & DMA_EVENT_TRANSFER_DONE) {
        obj->tx_buff.pos = obj->tx_buff.length;
    }
    // FIXME: Pass this error to caller
    if (event_dma & DMA_EVENT_TIMEOUT) {
    }
    
    const struct nu_modinit_s *modinit = get_modinit(obj->spi.spi, spi_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == obj->spi.spi);
    
    void (*vec)(void) = (void (*)(void)) NVIC_GetVector(modinit->irq_n);
    vec();
}

static void spi_dma_handler_rx(uint32_t id, uint32_t event_dma)
{
    spi_t *obj = (spi_t *) id;
    
    // FIXME: Pass this error to caller
    if (event_dma & DMA_EVENT_ABORT) {
    }
    // Expect SPI IRQ will catch this transfer done event
    if (event_dma & DMA_EVENT_TRANSFER_DONE) {
        obj->rx_buff.pos = obj->rx_buff.length;
    }
    // FIXME: Pass this error to caller
    if (event_dma & DMA_EVENT_TIMEOUT) {
    }
    
    const struct nu_modinit_s *modinit = get_modinit(obj->spi.spi, spi_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == obj->spi.spi);
    
    void (*vec)(void) = (void (*)(void)) NVIC_GetVector(modinit->irq_n);
    vec();
}

#endif

#endif
