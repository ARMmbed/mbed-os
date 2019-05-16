/* 
 * Copyright (c) 2013 Nordic Semiconductor ASA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this list 
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA 
 *      integrated circuit in a product or a software update for such product, must reproduce 
 *      the above copyright notice, this list of conditions and the following disclaimer in 
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be 
 *      used to endorse or promote products derived from this software without specific prior 
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a 
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse 
 *      engineered, decompiled, modified and/or disassembled. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */


#include "spi_api.h"

#if DEVICE_SPI

#include "cmsis.h"
#include "pinmap.h"
#include "mbed_assert.h"
#include "mbed_error.h"
#include "nrf_drv_spi.h"
#include "nrf_drv_spis.h"
#include "app_util_platform.h"
#include "sdk_config.h"

#if DEVICE_SPI_ASYNCH
    #define SPI_IDX(obj)    ((obj)->spi.spi_idx)
#else
    #define SPI_IDX(obj)    ((obj)->spi_idx)
#endif
#define SPI_INFO(obj)       (&m_spi_info[SPI_IDX(obj)])
#define MASTER_INST(obj)    (&m_instances[SPI_IDX(obj)].master)
#define SLAVE_INST(obj)     (&m_instances[SPI_IDX(obj)].slave)

typedef struct {
    bool    initialized;
    bool    master;
    uint8_t sck_pin;
    uint8_t mosi_pin;
    uint8_t miso_pin;
    uint8_t ss_pin;
    uint8_t spi_mode;
    nrf_drv_spi_frequency_t frequency;
    volatile union {
        bool busy;     // master
        bool readable; // slave
    } flag;
    volatile uint8_t tx_buf;
    volatile uint8_t rx_buf;

#if DEVICE_SPI_ASYNCH
    uint32_t handler;
    uint32_t event;
#endif
} spi_info_t;
static spi_info_t m_spi_info[SPI_COUNT];

typedef struct {
    nrf_drv_spi_t  master;
    nrf_drv_spis_t slave;
} sdk_driver_instances_t;

void SPI0_TWI0_IRQHandler(void);
void SPI1_TWI1_IRQHandler(void);
void SPIM2_SPIS2_SPI2_IRQHandler(void);

// Pinmap used for testing only
static const PinMap PinMap_SPI_testing[] = {
    {p0,  0, 0},
    {p1,  0, 0},
    {p2,  0, 0},
    {p3,  0, 0},
    {p4,  0, 0},
    {p5,  0, 0},
    {p6,  0, 0},
    {p7,  0, 0},
    {p8,  0, 0},
    {p9,  0, 0},
    {p10, 0, 0},
    {p11, 0, 0},
    {p12, 0, 0},
    {p13, 0, 0},
    {p14, 0, 0},
    {p15, 0, 0},
    {p16, 0, 0},
    {p17, 0, 0},
    {p18, 0, 0},
    {p19, 0, 0},
    {p20, 0, 0},
    {p21, 0, 0},
    {p22, 0, 0},
    {p23, 0, 0},
    {p24, 0, 0},
    {p25, 0, 0},

    {p28, 0, 0},
    {p29, 0, 0},
    {p30, 0, 0},

    {NC, NC, 0}
};

static const peripheral_handler_desc_t spi_handler_desc[SPI_COUNT] = {
#if SPI0_ENABLED
    {
        SPI0_IRQ,
        (uint32_t) SPI0_TWI0_IRQHandler
    },
#endif
#if SPI1_ENABLED
    {
        SPI1_IRQ,
        (uint32_t) SPI1_TWI1_IRQHandler
    },
#endif
#if SPI2_ENABLED
    {
        SPI2_IRQ,
        (uint32_t) SPIM2_SPIS2_SPI2_IRQHandler
    },
#endif    
};


static sdk_driver_instances_t m_instances[SPI_COUNT] = {
#if SPI0_ENABLED
    {
        NRF_DRV_SPI_INSTANCE(0),
        NRF_DRV_SPIS_INSTANCE(0)
    },
#endif
#if SPI1_ENABLED
    {
        NRF_DRV_SPI_INSTANCE(1),
        NRF_DRV_SPIS_INSTANCE(1)
    },
#endif
#if SPI2_ENABLED
    {
        NRF_DRV_SPI_INSTANCE(2),
        NRF_DRV_SPIS_INSTANCE(2)
    },
#endif
};

static void master_event_handler(uint8_t spi_idx,
                                 nrf_drv_spi_evt_t const *p_event)
{
    spi_info_t *p_spi_info = &m_spi_info[spi_idx];

    if (p_event->type == NRF_DRV_SPI_EVENT_DONE) {
        p_spi_info->flag.busy = false;
#if DEVICE_SPI_ASYNCH
        if (p_spi_info->handler) {
            void (*handler)(void) = (void (*)(void))p_spi_info->handler;
            p_spi_info->handler = 0;
            handler();
        }
#endif
    }
}
#define MASTER_EVENT_HANDLER(idx) \
    static void master_event_handler_##idx(nrf_drv_spi_evt_t const *p_event) { \
        master_event_handler(SPI##idx##_INSTANCE_INDEX, p_event); \
    }
#if SPI0_ENABLED
    MASTER_EVENT_HANDLER(0)
#endif
#if SPI1_ENABLED
    MASTER_EVENT_HANDLER(1)
#endif
#if SPI2_ENABLED
    MASTER_EVENT_HANDLER(2)
#endif

static nrf_drv_spi_handler_t const m_master_event_handlers[SPI_COUNT] = {
#if SPI0_ENABLED
    master_event_handler_0,
#endif
#if SPI1_ENABLED
    master_event_handler_1,
#endif
#if SPI2_ENABLED
    master_event_handler_2,
#endif
};


static void slave_event_handler(uint8_t spi_idx,
                                nrf_drv_spis_event_t event)
{
    spi_info_t *p_spi_info = &m_spi_info[spi_idx];

    if (event.evt_type == NRF_DRV_SPIS_XFER_DONE) {
        // Signal that there is some data received that could be read.
        p_spi_info->flag.readable = true;

        // And prepare for the next transfer.
        // Previous data set in 'spi_slave_write' (if any) has been transmitted,
        // now use the default one, until some new is set by 'spi_slave_write'.
        p_spi_info->tx_buf = SPIS_DEFAULT_ORC;
        nrf_drv_spis_buffers_set(&m_instances[spi_idx].slave,
            (uint8_t const *)&p_spi_info->tx_buf, 1,
            (uint8_t *)&p_spi_info->rx_buf, 1);
    }
}
#define SLAVE_EVENT_HANDLER(idx) \
    static void slave_event_handler_##idx(nrf_drv_spis_event_t event) { \
        slave_event_handler(SPIS##idx##_INSTANCE_INDEX, event); \
    }
#if SPIS0_ENABLED
    SLAVE_EVENT_HANDLER(0)
#endif
#if SPIS1_ENABLED
    SLAVE_EVENT_HANDLER(1)
#endif
#if SPIS2_ENABLED
    SLAVE_EVENT_HANDLER(2)
#endif

static nrf_drv_spis_event_handler_t const m_slave_event_handlers[SPIS_COUNT] = {
#if SPIS0_ENABLED
    slave_event_handler_0,
#endif
#if SPIS1_ENABLED
    slave_event_handler_1,
#endif
#if SPIS2_ENABLED
    slave_event_handler_2,
#endif
};

static void prepare_master_config(nrf_drv_spi_config_t *p_config,
                                  spi_info_t const *p_spi_info)
{
    p_config->sck_pin   = p_spi_info->sck_pin;
    p_config->mosi_pin  = p_spi_info->mosi_pin;
    p_config->miso_pin  = p_spi_info->miso_pin;
    p_config->ss_pin    = p_spi_info->ss_pin;
    p_config->frequency = p_spi_info->frequency;
    p_config->mode      = (nrf_drv_spi_mode_t)p_spi_info->spi_mode;

    p_config->irq_priority = SPI_DEFAULT_CONFIG_IRQ_PRIORITY;
    p_config->orc          = 0xFF;
    p_config->bit_order    = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST;
}

static void prepare_slave_config(nrf_drv_spis_config_t *p_config,
                                 spi_info_t const *p_spi_info)
{
    p_config->sck_pin   = p_spi_info->sck_pin;
    p_config->mosi_pin  = p_spi_info->mosi_pin;
    p_config->miso_pin  = p_spi_info->miso_pin;
    p_config->csn_pin   = p_spi_info->ss_pin;
    p_config->mode      = (nrf_drv_spis_mode_t)p_spi_info->spi_mode;

    p_config->irq_priority = SPIS_DEFAULT_CONFIG_IRQ_PRIORITY;
    p_config->orc          = SPIS_DEFAULT_ORC;
    p_config->def          = SPIS_DEFAULT_DEF;
    p_config->bit_order    = NRF_DRV_SPIS_BIT_ORDER_MSB_FIRST;
    p_config->csn_pullup   = NRF_DRV_SPIS_DEFAULT_CSN_PULLUP;
    p_config->miso_drive   = NRF_DRV_SPIS_DEFAULT_MISO_DRIVE;
}

void spi_init(spi_t *obj,
              PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    int i;

    // This block is only a workaround that allows to create SPI object several
    // times, what would be otherwise impossible in the current implementation
    // of mbed driver that does not call spi_free() from SPI destructor.
    // Once this mbed's imperfection is corrected, this block should be removed.
    for (i = 0; i < SPI_COUNT; ++i) {
        spi_info_t *p_spi_info = &m_spi_info[i];
        if (p_spi_info->initialized &&
            p_spi_info->mosi_pin == (uint8_t)mosi &&
            p_spi_info->miso_pin == (uint8_t)miso &&
            p_spi_info->sck_pin  == (uint8_t)sclk &&
            p_spi_info->ss_pin   == (uint8_t)ssel) {
            // Reuse the already allocated SPI instance (instead of allocating
            // a new one), if it appears to be initialized with exactly the same
            // pin assignments.
            SPI_IDX(obj) = i;
            return;
        }
    }

    for (i = SPI_COUNT - 1; i >= 0; i--) {
        spi_info_t *p_spi_info = &m_spi_info[i];

        if (!p_spi_info->initialized) {

            p_spi_info->sck_pin   = (uint8_t)sclk;
            p_spi_info->mosi_pin  = (mosi != NC) ?
                (uint8_t)mosi : NRF_DRV_SPI_PIN_NOT_USED;
            p_spi_info->miso_pin  = (miso != NC) ?
                (uint8_t)miso : NRF_DRV_SPI_PIN_NOT_USED;
            p_spi_info->ss_pin    = (ssel != NC) ?
                (uint8_t)ssel : NRF_DRV_SPI_PIN_NOT_USED;
            p_spi_info->spi_mode  = (uint8_t)NRF_DRV_SPI_MODE_0;
            p_spi_info->frequency = NRF_DRV_SPI_FREQ_1M;

            // By default each SPI instance is initialized to work as a master.
            // Should the slave mode be used, the instance will be reconfigured
            // appropriately in 'spi_format'.
            nrf_drv_spi_config_t config;
            prepare_master_config(&config, p_spi_info);

            nrf_drv_spi_t const *p_spi = &m_instances[i].master;
            ret_code_t ret_code = nrf_drv_spi_init(p_spi,
                &config, m_master_event_handlers[i]);
            if (ret_code == NRF_SUCCESS) {
                p_spi_info->initialized = true;
                p_spi_info->master      = true;
                p_spi_info->flag.busy   = false;
#if DEVICE_SPI_ASYNCH
                p_spi_info->handler     = 0;
#endif
                SPI_IDX(obj) = i;
                NVIC_SetVector(spi_handler_desc[i].IRQn, spi_handler_desc[i].vector);
                return;
            }
        }
    }

    // No available peripheral
    error("No available SPI peripheral\r\n");
}

void spi_free(spi_t *obj)
{
    spi_info_t *p_spi_info = SPI_INFO(obj);
    if (p_spi_info->master) {
        nrf_drv_spi_uninit(MASTER_INST(obj));
    }
    else {
        nrf_drv_spis_uninit(SLAVE_INST(obj));
    }
    p_spi_info->initialized = false;
}

int spi_busy(spi_t *obj)
{
    return (int)(SPI_INFO(obj)->flag.busy);
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    if (bits != 8) {
        error("Only 8-bits SPI is supported\r\n");
    }
    if (mode > 3) {
        error("SPI format error\r\n");
    }

    spi_info_t *p_spi_info = SPI_INFO(obj);

    if (slave)
    {
        nrf_drv_spis_mode_t spi_modes[4] = {
            NRF_DRV_SPIS_MODE_0,
            NRF_DRV_SPIS_MODE_1,
            NRF_DRV_SPIS_MODE_2,
            NRF_DRV_SPIS_MODE_3,
        };
        nrf_drv_spis_mode_t new_mode = spi_modes[mode];

        // If the peripheral is currently working as a master, the SDK driver
        // it uses needs to be switched from SPI to SPIS.
        if (p_spi_info->master) {
            nrf_drv_spi_uninit(MASTER_INST(obj));
        }
        // I the SPI mode has to be changed, the SDK's SPIS driver needs to be
        // re-initialized (there is no other way to change its configuration).
        else if (p_spi_info->spi_mode != (uint8_t)new_mode) {
            nrf_drv_spis_uninit(SLAVE_INST(obj));
        }
        else {
            return;
        }

        p_spi_info->spi_mode = (uint8_t)new_mode;
        p_spi_info->master = false;
        p_spi_info->flag.readable = false;

        // Initialize SDK's SPIS driver with the new configuration.
        nrf_drv_spis_config_t config;
        prepare_slave_config(&config, p_spi_info);
        (void)nrf_drv_spis_init(SLAVE_INST(obj), &config,
            m_slave_event_handlers[SPI_IDX(obj)]);

        // Prepare the slave for transfer.
        p_spi_info->tx_buf = SPIS_DEFAULT_ORC;
        nrf_drv_spis_buffers_set(SLAVE_INST(obj),
            (uint8_t const *)&p_spi_info->tx_buf, 1,
            (uint8_t *)&p_spi_info->rx_buf, 1);
    }
    else // master
    {
        nrf_drv_spi_mode_t spi_modes[4] = {
            NRF_DRV_SPI_MODE_0,
            NRF_DRV_SPI_MODE_1,
            NRF_DRV_SPI_MODE_2,
            NRF_DRV_SPI_MODE_3,
        };
        nrf_drv_spi_mode_t new_mode = spi_modes[mode];

        // If the peripheral is currently working as a slave, the SDK driver
        // it uses needs to be switched from SPIS to SPI.
        if (!p_spi_info->master) {
            nrf_drv_spis_uninit(SLAVE_INST(obj));
        }
        // I the SPI mode has to be changed, the SDK's SPI driver needs to be
        // re-initialized (there is no other way to change its configuration).
        else if (p_spi_info->spi_mode != (uint8_t)new_mode) {
            nrf_drv_spi_uninit(MASTER_INST(obj));
        }
        else {
            return;
        }

        p_spi_info->spi_mode = (uint8_t)new_mode;
        p_spi_info->master = true;
        p_spi_info->flag.busy = false;

        // Initialize SDK's SPI driver with the new configuration.
        nrf_drv_spi_config_t config;
        prepare_master_config(&config, p_spi_info);
        (void)nrf_drv_spi_init(MASTER_INST(obj), &config,
            m_master_event_handlers[SPI_IDX(obj)]);
    }
}

static nrf_drv_spi_frequency_t freq_translate(int hz)
{
    nrf_drv_spi_frequency_t frequency;
    if (hz<250000) { //125Kbps
        frequency = NRF_DRV_SPI_FREQ_125K;
    } else if (hz<500000) { //250Kbps
        frequency = NRF_DRV_SPI_FREQ_250K;
    } else if (hz<1000000) { //500Kbps
        frequency = NRF_DRV_SPI_FREQ_500K;
    } else if (hz<2000000) { //1Mbps
        frequency = NRF_DRV_SPI_FREQ_1M;
    } else if (hz<4000000) { //2Mbps
        frequency = NRF_DRV_SPI_FREQ_2M;
    } else if (hz<8000000) { //4Mbps
        frequency = NRF_DRV_SPI_FREQ_4M;
    } else { //8Mbps
        frequency = NRF_DRV_SPI_FREQ_8M;
    }
    return frequency;
}

void spi_frequency(spi_t *obj, int hz)
{
    spi_info_t *p_spi_info = SPI_INFO(obj);
    nrf_drv_spi_frequency_t new_frequency = freq_translate(hz);

    if (p_spi_info->master)
    {
        if (p_spi_info->frequency != new_frequency) {
            p_spi_info->frequency = new_frequency;

            nrf_drv_spi_config_t config;
            prepare_master_config(&config, p_spi_info);

            nrf_drv_spi_t const *p_spi = MASTER_INST(obj);
            nrf_drv_spi_uninit(p_spi);
            (void)nrf_drv_spi_init(p_spi, &config,
                m_master_event_handlers[SPI_IDX(obj)]);
        }
    }
    // There is no need to set anything in slaves when it comes to frequency,
    // since slaves just synchronize with the clock provided by a master.
}

int spi_master_write(spi_t *obj, int value)
{
    spi_info_t *p_spi_info = SPI_INFO(obj);

#if DEVICE_SPI_ASYNCH
    while (p_spi_info->flag.busy) {
    }
#endif

    p_spi_info->tx_buf = value;
    p_spi_info->flag.busy = true;
    (void)nrf_drv_spi_transfer(MASTER_INST(obj),
        (uint8_t const *)&p_spi_info->tx_buf, 1,
        (uint8_t *)&p_spi_info->rx_buf, 1);
    while (p_spi_info->flag.busy) {
    }

    return p_spi_info->rx_buf;
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length, char write_fill) {
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

int spi_slave_receive(spi_t *obj)
{
    spi_info_t *p_spi_info = SPI_INFO(obj);
    MBED_ASSERT(!p_spi_info->master);
    return p_spi_info->flag.readable;
;
}

int spi_slave_read(spi_t *obj)
{
    spi_info_t *p_spi_info = SPI_INFO(obj);
    MBED_ASSERT(!p_spi_info->master);
    while (!p_spi_info->flag.readable) {
    }
    p_spi_info->flag.readable = false;
    return p_spi_info->rx_buf;
}

void spi_slave_write(spi_t *obj, int value)
{
    spi_info_t *p_spi_info = SPI_INFO(obj);
    MBED_ASSERT(!p_spi_info->master);

    p_spi_info->tx_buf = (uint8_t)value;
}

const PinMap *spi_master_mosi_pinmap()
{
    return PinMap_SPI_testing;
}

const PinMap *spi_master_miso_pinmap()
{
    return PinMap_SPI_testing;
}

const PinMap *spi_master_clk_pinmap()
{
    return PinMap_SPI_testing;
}

const PinMap *spi_master_cs_pinmap()
{
    return PinMap_SPI_testing;
}

const PinMap *spi_slave_mosi_pinmap()
{
    return PinMap_SPI_testing;
}

const PinMap *spi_slave_miso_pinmap()
{
    return PinMap_SPI_testing;
}

const PinMap *spi_slave_clk_pinmap()
{
    return PinMap_SPI_testing;
}

const PinMap *spi_slave_cs_pinmap()
{
    return PinMap_SPI_testing;
}

#if DEVICE_SPI_ASYNCH

void spi_master_transfer(spi_t *obj,
                         const void *tx, size_t tx_length,
                         void *rx, size_t rx_length, uint8_t bit_width,
                         uint32_t handler, uint32_t event, DMAUsage hint)
{
    spi_info_t *p_spi_info = SPI_INFO(obj);
    MBED_ASSERT(p_spi_info->master);
    (void)hint;
    (void)bit_width;

    p_spi_info->handler = handler;
    p_spi_info->event   = event;

    p_spi_info->flag.busy = true;
    (void)nrf_drv_spi_transfer(MASTER_INST(obj),
        (uint8_t const *)tx, tx_length,
        (uint8_t *)rx, rx_length);
}

uint32_t spi_irq_handler_asynch(spi_t *obj)
{
    spi_info_t *p_spi_info = SPI_INFO(obj);
    MBED_ASSERT(p_spi_info->master);
    return p_spi_info->event & SPI_EVENT_COMPLETE;
}

uint8_t spi_active(spi_t *obj)
{
    spi_info_t *p_spi_info = SPI_INFO(obj);
    MBED_ASSERT(p_spi_info->master);
    return p_spi_info->flag.busy;
}

void spi_abort_asynch(spi_t *obj)
{
    MBED_ASSERT(SPI_INFO(obj)->master);
    nrf_drv_spi_abort(MASTER_INST(obj));
}

#endif // DEVICE_SPI_ASYNCH

#endif // DEVICE_SPI
