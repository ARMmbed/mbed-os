/* mbed Microcontroller Library
 * Copyright (c) 2015-2017 Nuvoton
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
    spi_t *     obj;
    void        (*vec)(void);
#if DEVICE_SPI_ASYNCH
    uint8_t     pdma_perp_tx;
    uint8_t     pdma_perp_rx;
#endif
};

/* NOTE: NANO130 doesn't support relocating vector table. ISR vector passed into NVIC_SetVector() can
 *       only be weak symbol defined in startup_Nano100Series.c. */
void SPI0_IRQHandler(void);
void SPI1_IRQHandler(void);
void SPI2_IRQHandler(void);
static void spi_irq(spi_t *obj);

static struct nu_spi_var spi0_var = {
    .obj                =   NULL,
    .vec                =   SPI0_IRQHandler,
#if DEVICE_SPI_ASYNCH
    .pdma_perp_tx       =   PDMA_SPI0_TX,
    .pdma_perp_rx       =   PDMA_SPI0_RX
#endif
};
static struct nu_spi_var spi1_var = {
    .obj                =   NULL,
    .vec                =   SPI1_IRQHandler,
#if DEVICE_SPI_ASYNCH
    .pdma_perp_tx       =   PDMA_SPI1_TX,
    .pdma_perp_rx       =   PDMA_SPI1_RX
#endif
};
static struct nu_spi_var spi2_var = {
    .obj                =   NULL,
    .vec                =   SPI2_IRQHandler,
#if DEVICE_SPI_ASYNCH
    .pdma_perp_tx       =   PDMA_SPI2_TX,
    .pdma_perp_rx       =   PDMA_SPI2_RX
#endif
};

/* Synchronous version of SPI_ENABLE()/SPI_DISABLE() macros
 *
 * The SPI peripheral clock is asynchronous with the system clock. In order to make sure the SPI
 * control logic is enabled/disabled, this bit indicates the real status of SPI controller.
 *
 * NOTE: All configurations shall be ready before calling SPI_ENABLE_SYNC().
 * NOTE: Before changing the configurations of SPIx_CTL, SPIx_CLKDIV, SPIx_SSCTL and SPIx_FIFOCTL registers,
 *       user shall clear the SPIEN (SPIx_CTL[0]) and confirm the SPIENSTS (SPIx_STATUS[15]) is 0
 *       (by SPI_DISABLE_SYNC here).
 *
 * NOTE:
 * 1. NUC472/M453/M487: SPI_CTL.SPIEN is controlled by software (in FIFO mode).
 * 2. NANO130: SPI_CTL.GO_BUSY is controlled by hardware in FIFO mode.
 */
__STATIC_INLINE void SPI_ENABLE_SYNC(SPI_T *spi_base)
{
    /* NOTE: On NANO130, FIFO mode defaults to disabled. */
    if (! (spi_base->CTL & SPI_CTL_FIFOM_Msk)) {
        SPI_EnableFIFO(spi_base, NU_SPI_FIFO_DEPTH / 2, NU_SPI_FIFO_DEPTH / 2);
    }
}
__STATIC_INLINE void SPI_DISABLE_SYNC(SPI_T *spi_base)
{
    /* NOTE: On NANO130, SPI_CTL.GO_BUSY always reads as 1 in slave/FIFO mode. So disable FIFO first. */
    if (spi_base->CTL & SPI_CTL_FIFOM_Msk) {
        SPI_DisableFIFO(spi_base);
    }
    
    /* NOTE: SPI H/W may get out of state without the busy check */
    while (SPI_IS_BUSY(spi_base));
}

#if DEVICE_SPI_ASYNCH
static void spi_enable_vector_interrupt(spi_t *obj, uint32_t handler, uint8_t enable);
static void spi_master_enable_interrupt(spi_t *obj, uint8_t enable, uint32_t mask);
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
    {SPI_0, SPI0_MODULE, CLK_CLKSEL2_SPI0_S_HCLK, MODULE_NoMsk, SPI0_RST, SPI0_IRQn, &spi0_var},
    {SPI_1, SPI1_MODULE, CLK_CLKSEL2_SPI1_S_HCLK, MODULE_NoMsk, SPI1_RST, SPI1_IRQn, &spi1_var},
    {SPI_2, SPI2_MODULE, CLK_CLKSEL2_SPI2_S_HCLK, MODULE_NoMsk, SPI2_RST, SPI2_IRQn, &spi2_var},
    
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
    // NOTE:
    // NANO130: Support two-port SPI MOSI/MISO 0/1
    if (NU_MODBASE(spi_data) == NU_MODBASE(spi_cntl)) {
        // NOTE: spi_data has subindex(port) encoded but spi_cntl hasn't.
        obj->spi.spi = (SPIName) spi_data;
    }
    else {
        obj->spi.spi = (SPIName) NC;
    }
    MBED_ASSERT((int)obj->spi.spi != NC);

    const struct nu_modinit_s *modinit = get_modinit(obj->spi.spi, spi_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->spi.spi);

    // Reset this module
    SYS_ResetModule(modinit->rsetidx);

    // Select IP clock source
    CLK_SetModuleClock(modinit->clkidx, modinit->clksrc, modinit->clkdiv);
    // Enable IP clock
    CLK_EnableModuleClock(modinit->clkidx);

    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    pinmap_pinout(ssel, PinMap_SPI_SSEL);

    obj->spi.pin_mosi = mosi;
    obj->spi.pin_miso = miso;
    obj->spi.pin_sclk = sclk;
    obj->spi.pin_ssel = ssel;


#if DEVICE_SPI_ASYNCH
    obj->spi.dma_usage = DMA_USAGE_NEVER;
    obj->spi.event = 0;
    obj->spi.dma_chn_id_tx = DMA_ERROR_OUT_OF_CHANNELS;
    obj->spi.dma_chn_id_rx = DMA_ERROR_OUT_OF_CHANNELS;
    
    /* NOTE: We use vector to judge if asynchronous transfer is on-going (spi_active).
     *       At initial time, asynchronous transfer is not on-going and so vector must
     *       be cleared to zero for correct judgement. */
    /* NOTE: On NANO130, vector table is fixed in ROM and cannot be modified. */
    obj->spi.hdlr_async = 0;
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
    MBED_ASSERT(modinit->modname == (int) obj->spi.spi);

    SPI_DisableInt(((SPI_T *) NU_MODBASE(obj->spi.spi)), (SPI_FIFO_RXOVR_INTEN_MASK | SPI_FIFO_RX_INTEN_MASK | SPI_FIFO_TX_INTEN_MASK));
    NVIC_DisableIRQ(modinit->irq_n);

    // Disable IP clock
    CLK_DisableModuleClock(modinit->clkidx);

    // Mark this module to be deinited.
    int i = modinit - spi_modinit_tab;
    spi_modinit_mask &= ~(1 << i);
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    MBED_ASSERT(bits >= NU_SPI_FRAME_MIN && bits <= NU_SPI_FRAME_MAX);

    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);

    SPI_DISABLE_SYNC(spi_base);

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
            switch (NU_MODSUBINDEX(obj->spi.spi)) {
                case 0:
                    SPI_EnableAutoSS(spi_base, SPI_SS0, SPI_SS0_ACTIVE_LOW);
                    break;
                    
                case 1:
                    SPI_EnableAutoSS(spi_base, SPI_SS1, SPI_SS1_ACTIVE_LOW);
                    break;
            }
        }
        else {
            SPI_DisableAutoSS(spi_base);
        }
    }
    else {
        // Slave
        // Configure SS as low active.
        switch (NU_MODSUBINDEX(obj->spi.spi)) {
            case 0:
                spi_base->SSR &= ~SPI_SS0_ACTIVE_HIGH;
                break;
            case 1:
                spi_base->SSR &= ~SPI_SS1_ACTIVE_HIGH;
                break;
        }
        // NOTE:
        // NANO130: Configure slave select signal to edge-trigger rather than level-trigger
        spi_base->SSR |= SPI_SSR_SS_LTRIG_Msk;
    }
    
    /* NOTE: M451's/M480's/M2351's SPI_Open() will enable SPI transfer (SPI_CTL_SPIEN_Msk).
     *       We cannot use SPI_CTL_SPIEN_Msk for judgement of spi_active().
     *       Judge with vector instead. */
}

void spi_frequency(spi_t *obj, int hz)
{
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);
    
    SPI_DISABLE_SYNC(spi_base);

    SPI_SetBusClock((SPI_T *) NU_MODBASE(obj->spi.spi), hz);
}


int spi_master_write(spi_t *obj, int value)
{
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);

    // NOTE: Data in receive FIFO can be read out via ICE.
    // NOTE:
    // NUC472/M453/M487: SPI_CTL.SPIEN is controlled by software (in FIFO mode).
    // NANO130: SPI_CTL.GO_BUSY is controlled by hardware in FIFO mode.
    SPI_ENABLE_SYNC(spi_base);

    // Wait for tx buffer empty
    while(! spi_writeable(obj));
    uint32_t TX = (NU_MODSUBINDEX(obj->spi.spi) == 0) ? ((uint32_t) &spi_base->TX0) : ((uint32_t) &spi_base->TX1);
    M32(TX) = value;

    // Wait for rx buffer full
    while (! spi_readable(obj));
    uint32_t RX = (NU_MODSUBINDEX(obj->spi.spi) == 0) ? ((uint32_t) &spi_base->RX0) : ((uint32_t) &spi_base->RX1);
    int value2 = M32(RX);

    /* We don't call SPI_DISABLE_SYNC here for performance. */

    return value2;
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length, char write_fill)
{
    int total = (tx_length > rx_length) ? tx_length : rx_length;

    for (int i = 0; i < total; i++) {
        char out = (i < tx_length) ? tx_buffer[i] : write_fill;
        char in = spi_master_write(obj, out);
        if (i < rx_length) {
            rx_buffer[i] = in;
        }
    }

    return total;
}

#if DEVICE_SPISLAVE
int spi_slave_receive(spi_t *obj)
{
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);

    SPI_ENABLE_SYNC(spi_base);

    return spi_readable(obj);
};

int spi_slave_read(spi_t *obj)
{
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);

    SPI_ENABLE_SYNC(spi_base);

    // Wait for rx buffer full
    while (! spi_readable(obj));
    uint32_t RX = (NU_MODSUBINDEX(obj->spi.spi) == 0) ? ((uint32_t) &spi_base->RX0) : ((uint32_t) &spi_base->RX1);
    int value = M32(RX);
    return value;
}

void spi_slave_write(spi_t *obj, int value)
{
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);

    SPI_ENABLE_SYNC(spi_base);

    // Wait for tx buffer empty
    while(! spi_writeable(obj));
    uint32_t TX = (NU_MODSUBINDEX(obj->spi.spi) == 0) ? ((uint32_t) &spi_base->TX0) : ((uint32_t) &spi_base->TX1);
    M32(TX) = value;
}
#endif

#if DEVICE_SPI_ASYNCH
void spi_master_transfer(spi_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint8_t bit_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
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

    SPI_ENABLE_SYNC(spi_base);

    if (obj->spi.dma_usage == DMA_USAGE_NEVER) {
        // Interrupt way
        spi_master_write_asynch(obj, NU_SPI_FIFO_DEPTH / 2);
        spi_enable_vector_interrupt(obj, handler, 1);
        spi_master_enable_interrupt(obj, 1, SPI_FIFO_RX_INTEN_MASK | SPI_FIFO_TX_INTEN_MASK);
    } else {
        // DMA way
        const struct nu_modinit_s *modinit = get_modinit(obj->spi.spi, spi_modinit_tab);
        MBED_ASSERT(modinit != NULL);
        MBED_ASSERT(modinit->modname == (int) obj->spi.spi);
        
        // Configure tx DMA
        dma_enable(obj->spi.dma_chn_id_tx, 1);                  // Enable this DMA channel
        PDMA_SetTransferMode(obj->spi.dma_chn_id_tx,
            ((struct nu_spi_var *) modinit->var)->pdma_perp_tx,    // Peripheral connected to this PDMA
            0,  // Scatter-gather disabled
            0); // Scatter-gather descriptor address
        PDMA_SetTransferCnt(obj->spi.dma_chn_id_tx, 
            (data_width == 8) ? PDMA_WIDTH_8 : (data_width == 16) ? PDMA_WIDTH_16 : PDMA_WIDTH_32, 
            tx_length);
        PDMA_SetTransferAddr(obj->spi.dma_chn_id_tx,
            (uint32_t) tx,  // NOTE:
                            // NUC472: End of source address
                            // M451: Start of source address
                            // NANO130: Start of destination address
            PDMA_SAR_INC,   // Source address incremental
            NU_MODSUBINDEX(obj->spi.spi) == 0 ? (uint32_t) &spi_base->TX0 : (uint32_t) &spi_base->TX1,  // Destination address
            PDMA_DAR_FIX);  // Destination address fixed
        PDMA_EnableInt(obj->spi.dma_chn_id_tx,
            PDMA_IER_TD_IE_Msk);    // Interrupt type
        // Register DMA event handler
        dma_set_handler(obj->spi.dma_chn_id_tx, (uint32_t) spi_dma_handler_tx, (uint32_t) obj, DMA_EVENT_ALL);
        
        // Configure rx DMA
        dma_enable(obj->spi.dma_chn_id_rx, 1);              // Enable this DMA channel
        PDMA_SetTransferMode(obj->spi.dma_chn_id_rx,
            ((struct nu_spi_var *) modinit->var)->pdma_perp_rx,    // Peripheral connected to this PDMA
            0,  // Scatter-gather disabled
            0); // Scatter-gather descriptor address
        PDMA_SetTransferCnt(obj->spi.dma_chn_id_rx, 
            (data_width == 8) ? PDMA_WIDTH_8 : (data_width == 16) ? PDMA_WIDTH_16 : PDMA_WIDTH_32, 
            rx_length);
        PDMA_SetTransferAddr(obj->spi.dma_chn_id_rx,
            NU_MODSUBINDEX(obj->spi.spi) == 0 ? (uint32_t) &spi_base->RX0 : (uint32_t) &spi_base->RX1,  // Source address
            PDMA_SAR_FIX,   // Source address fixed
            (uint32_t) rx,  // NOTE: 
                            // NUC472: End of destination address
                            // M451: Start of destination address
                            // NANO130: Start of destination address
            PDMA_DAR_INC);  // Destination address incremental
        PDMA_EnableInt(obj->spi.dma_chn_id_rx,
            PDMA_IER_TD_IE_Msk);    // Interrupt type
        // Register DMA event handler
        dma_set_handler(obj->spi.dma_chn_id_rx, (uint32_t) spi_dma_handler_rx, (uint32_t) obj, DMA_EVENT_ALL);

        /* Start tx/rx DMA transfer
         *
         * If we have both PDMA and SPI interrupts enabled and PDMA priority is lower than SPI priority,
         * we would trap in SPI interrupt handler endlessly with the sequence:
         *
         * 1. PDMA TX transfer done interrupt occurs and is well handled.
         * 2. SPI RX FIFO threshold interrupt occurs. Trap here because PDMA RX transfer done interrupt doesn't get handled.
         * 3. PDMA RX transfer done interrupt occurs but it cannot be handled due to above.
         *
         * To fix it, we don't enable SPI TX/RX threshold interrupts but keep SPI vector handler set to be called
         * in PDMA TX/RX transfer done interrupt handlers (spi_dma_handler_tx/spi_dma_handler_rx).
         */
        spi_enable_vector_interrupt(obj, handler, 1);
        /* No TX/RX FIFO threshold interrupt */
        spi_master_enable_interrupt(obj, 0, SPI_FIFO_RX_INTEN_MASK | SPI_FIFO_TX_INTEN_MASK);

        /* Order to enable PDMA TX/RX functions
         *
         * H/W spec: In SPI Master mode with full duplex transfer, if both TX and RX PDMA functions are
         *           enabled, RX PDMA function cannot be enabled prior to TX PDMA function. User can enable
         *           TX PDMA function firstly or enable both functions simultaneously.
         * Per real test, it is safer to start RX PDMA first and then TX PDMA. Otherwise, receive FIFO is 
         * subject to overflow by TX DMA.
         *
         * With the above conflicts, we enable PDMA TX/RX functions simultaneously.
         */
        spi_base->DMA |= (SPI_DMA_TX_DMA_EN_Msk | SPI_DMA_RX_DMA_EN_Msk);

        PDMA_Trigger(obj->spi.dma_chn_id_rx);
        PDMA_Trigger(obj->spi.dma_chn_id_tx);
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

    if (obj->spi.dma_usage != DMA_USAGE_NEVER) {
        // Receive FIFO Overrun in case of tx length > rx length on DMA way
        if (spi_base->STATUS & SPI_STATUS_RX_OVER_RUN_Msk) {
            spi_base->STATUS = SPI_STATUS_RX_OVER_RUN_Msk;
        }
        
        if (obj->spi.dma_chn_id_tx != DMA_ERROR_OUT_OF_CHANNELS) {
            PDMA_DisableInt(obj->spi.dma_chn_id_tx, PDMA_IER_TD_IE_Msk);
            // NOTE: On NUC472, next PDMA transfer will fail with PDMA_STOP() called.
            dma_enable(obj->spi.dma_chn_id_tx, 0);
        }
        spi_base->DMA &= ~SPI_DMA_TX_DMA_EN_Msk;

        if (obj->spi.dma_chn_id_rx != DMA_ERROR_OUT_OF_CHANNELS) {
            PDMA_DisableInt(obj->spi.dma_chn_id_rx, PDMA_IER_TD_IE_Msk);
            // NOTE: On NUC472, next PDMA transfer will fail with PDMA_STOP() called.
            dma_enable(obj->spi.dma_chn_id_rx, 0);
        }
        spi_base->DMA &= ~SPI_DMA_RX_DMA_EN_Msk;
    }

    // Necessary for both interrupt way and DMA way
    spi_enable_vector_interrupt(obj, 0, 0);
    spi_master_enable_interrupt(obj, 0, SPI_FIFO_RX_INTEN_MASK | SPI_FIFO_TX_INTEN_MASK);

    /* Necessary for accessing FIFOCTL below */
    SPI_DISABLE_SYNC(spi_base);

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
    const struct nu_modinit_s *modinit = get_modinit(obj->spi.spi, spi_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->spi.spi);

    /* Vector will be cleared when asynchronous transfer is finished or aborted.
       Use it to judge if asynchronous transfer is on-going. */
    /* NOTE: On NANO130, vector table is fixed in ROM and cannot be modified. */ 
    return obj->spi.hdlr_async ? 1 : 0;
}

void SPI0_IRQHandler(void)
{
    spi_irq(spi0_var.obj);
}
void SPI1_IRQHandler(void)
{
    spi_irq(spi1_var.obj);
}
void SPI2_IRQHandler(void)
{
    spi_irq(spi2_var.obj);
}
static void spi_irq(spi_t *obj)
{
    if (obj && obj->spi.hdlr_async) {
        void (*hdlr_async)(void) = (void(*)(void))(obj->spi.hdlr_async);
        hdlr_async();
    }
}

static int spi_writeable(spi_t * obj)
{
    // Receive FIFO must not be full to avoid receive FIFO overflow on next transmit/receive
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
        SPI_EnableInt((SPI_T *) NU_MODBASE(obj->spi.spi), SPI_FIFO_RXOVR_INTEN_MASK);
    }
}

static void spi_enable_vector_interrupt(spi_t *obj, uint32_t handler, uint8_t enable)
{
    const struct nu_modinit_s *modinit = get_modinit(obj->spi.spi, spi_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->spi.spi);
    
    struct nu_spi_var *var = (struct nu_spi_var *) modinit->var;
    
    if (enable) {
        var->obj = obj;
        obj->spi.hdlr_async = handler;
        /* NOTE: On NANO130, vector table is fixed in ROM and cannot be modified. */
        NVIC_EnableIRQ(modinit->irq_n);
    }
    else {
        NVIC_DisableIRQ(modinit->irq_n);
        /* NOTE: On NANO130, vector table is fixed in ROM and cannot be modified. */
        var->obj = NULL;
        obj->spi.hdlr_async = 0;
    }
}

static void spi_master_enable_interrupt(spi_t *obj, uint8_t enable, uint32_t mask)
{   
    SPI_T *spi_base = (SPI_T *) NU_MODBASE(obj->spi.spi);
    
    // NOTE:
    // NANO130: SPI_IE_MASK/SPI_STATUS_INTSTS_Msk are for unit transfer IE/EF. Don't get confused.
    if (enable) {
        // Enable tx/rx FIFO threshold interrupt
        SPI_EnableInt(spi_base, mask);
    }
    else {
        SPI_DisableInt(spi_base, mask);
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
    if (spi_base->STATUS & SPI_STATUS_RX_OVER_RUN_Msk) {
        spi_base->STATUS = SPI_STATUS_RX_OVER_RUN_Msk;
        // In case of tx length > rx length on DMA way
        if (obj->spi.dma_usage == DMA_USAGE_NEVER) {
            event |= SPI_EVENT_RX_OVERFLOW;
        }
    }

    // Receive Time-Out
    if (spi_base->STATUS & SPI_STATUS_TIME_OUT_STS_Msk) {
        spi_base->STATUS = SPI_STATUS_TIME_OUT_STS_Msk;
        // Not using this IF. Just clear it.
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
    uint32_t TX = (NU_MODSUBINDEX(obj->spi.spi) == 0) ? ((uint32_t) &spi_base->TX0) : ((uint32_t) &spi_base->TX1);
    
    while ((n_words < max_tx) && spi_writeable(obj)) {
        if (spi_is_tx_complete(obj)) {
            // Transmit dummy as transmit buffer is empty
            M32(TX) = 0;
        }
        else {
            switch (bytes_per_word) {
                case 4:
                    M32(TX) = nu_get32_le(tx);
                    tx += 4;
                    break;
                case 2:
                    M32(TX) = nu_get16_le(tx);
                    tx += 2;
                    break;
                case 1:
                    M32(TX) = *((uint8_t *) tx);
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
    uint32_t RX = (NU_MODSUBINDEX(obj->spi.spi) == 0) ? ((uint32_t) &spi_base->RX0) : ((uint32_t) &spi_base->RX1);
    
    while ((n_words < max_rx) && spi_readable(obj)) {
        if (spi_is_rx_complete(obj)) {
            // Disregard as receive buffer is full
            M32(RX);
        }
        else {
            switch (bytes_per_word) {
                case 4: {
                    uint32_t val = M32(RX);
                    nu_set32_le(rx, val);
                    rx += 4;
                    break;
                }
                case 2: {
                    uint16_t val = M32(RX);
                    nu_set16_le(rx, val);
                    rx += 2;
                    break;
                }
                case 1:
                    *rx ++ = M32(RX);
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

    uint32_t data_width = ((spi_base->CTL & SPI_CTL_TX_BIT_LEN_Msk) >> SPI_CTL_TX_BIT_LEN_Pos);
    if (data_width == 0) {
        data_width = 32;
    }

    return data_width;
}

static int spi_is_tx_complete(spi_t *obj)
{
    return (obj->tx_buff.pos == obj->tx_buff.length);
}

static int spi_is_rx_complete(spi_t *obj)
{
    return (obj->rx_buff.pos == obj->rx_buff.length);
}

static void spi_dma_handler_tx(uint32_t id, uint32_t event_dma)
{
    spi_t *obj = (spi_t *) id;

    // TODO: Pass this error to caller
    if (event_dma & DMA_EVENT_ABORT) {
    }
    // Expect SPI IRQ will catch this transfer done event
    if (event_dma & DMA_EVENT_TRANSFER_DONE) {
        obj->tx_buff.pos = obj->tx_buff.length;
    }
    // TODO: Pass this error to caller
    if (event_dma & DMA_EVENT_TIMEOUT) {
    }

    const struct nu_modinit_s *modinit = get_modinit(obj->spi.spi, spi_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->spi.spi);

    void (*vec)(void) = (void (*)(void)) NVIC_GetVector(modinit->irq_n);
    vec();
}

static void spi_dma_handler_rx(uint32_t id, uint32_t event_dma)
{
    spi_t *obj = (spi_t *) id;

    // TODO: Pass this error to caller
    if (event_dma & DMA_EVENT_ABORT) {
    }
    // Expect SPI IRQ will catch this transfer done event
    if (event_dma & DMA_EVENT_TRANSFER_DONE) {
        obj->rx_buff.pos = obj->rx_buff.length;
    }
    // TODO: Pass this error to caller
    if (event_dma & DMA_EVENT_TIMEOUT) {
    }

    const struct nu_modinit_s *modinit = get_modinit(obj->spi.spi, spi_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->spi.spi);

    void (*vec)(void) = (void (*)(void)) NVIC_GetVector(modinit->irq_n);
    vec();
}

#endif

#endif
