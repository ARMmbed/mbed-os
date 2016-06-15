/* mbed Microcontroller Library
 * Copyright (c) 2013 Nordic Semiconductor
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
//#include <math.h>

#include "spi_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "mbed_assert.h"
#include "mbed_error.h"
#include "nrf_drv_spi.h"
#include "app_util_platform.h"

#if DEVICE_SPI

#define SPI_MESSAGE_SIZE 1
volatile uint8_t m_tx_buf[SPI_MESSAGE_SIZE] = {0};
volatile uint8_t m_rx_buf[SPI_MESSAGE_SIZE] = {0};

static nrf_drv_spi_config_t m_config = NRF_DRV_SPI_DEFAULT_CONFIG(1);
static nrf_drv_spi_t spi1 = NRF_DRV_SPI_INSTANCE(1); 

typedef void (*user_handler_t)(void);
static user_handler_t m_user_handler;
static uint32_t m_event;
static spi_t * m_spi_struct;

#if DEVICE_SPI_ASYNCH
    #define SPI_S(obj)      (&obj->spi)
    #define SPI_DRV(obj)    (obj->spi.p_spi)
#else
    #define SPI_S(obj)      (obj)
    #define SPI_DRV(obj)    (obj->p_spi)
#endif

static void master_event_handler(nrf_drv_spi_evt_t const * event)
{
    if (event->type == NRF_DRV_SPI_EVENT_DONE) {
        SPI_S(m_spi_struct)->busy = false;
        if (SPI_S(m_spi_struct)->async_mode) {
            m_user_handler();
        }
    }
}

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk)
{
    m_config.sck_pin = sclk;
    m_config.mosi_pin = mosi;
    m_config.miso_pin = miso;
    
    SPI_S(obj)->busy = false;
    m_spi_struct = obj;
    
    SPI_DRV(obj) = &spi1;
    (void)nrf_drv_spi_init(&spi1, &m_config, master_event_handler);
}

void spi_free(spi_t *obj)
{
    nrf_drv_spi_uninit(&spi1);
}

int spi_busy(spi_t *obj)
{
    return (int)(SPI_S(obj)->busy);
}

static nrf_drv_spi_mode_t mode_translate(int mode)
{
    nrf_drv_spi_mode_t config_mode = NRF_DRV_SPI_MODE_0;
    switch (mode) {
        case 0:
            config_mode = NRF_DRV_SPI_MODE_0;
            break;
        case 1:
            config_mode = NRF_DRV_SPI_MODE_1;
            break;
        case 2:
            config_mode = NRF_DRV_SPI_MODE_2;
            break;
        case 3:
            config_mode = NRF_DRV_SPI_MODE_3;
            break;
        default:
            error("SPI format error");
            break;
    }
    return config_mode;
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

void spi_format(spi_t *obj, int bits, int mode, spi_bitorder_t order)
{
    if (bits != 8) {
        error("Only 8bits SPI supported");
    }
    
    m_config.bit_order = ((order == SPI_MSB) ? NRF_DRV_SPI_BIT_ORDER_MSB_FIRST : NRF_DRV_SPI_BIT_ORDER_LSB_FIRST);
    nrf_drv_spi_mode_t config_mode = mode_translate(mode);

    if (m_config.mode != config_mode) {
        nrf_drv_spi_uninit(&spi1);
        m_config.mode = config_mode;
        (void)nrf_drv_spi_init(&spi1, &m_config, master_event_handler);
    }

}

void spi_frequency(spi_t *obj, int hz)
{
    nrf_drv_spi_frequency_t frequency = freq_translate(hz);

    if (frequency != m_config.frequency) {
        nrf_drv_spi_uninit(&spi1);
        m_config.frequency = frequency;
        (void)nrf_drv_spi_init(&spi1, &m_config, master_event_handler);
    }
}

int spi_master_write(spi_t *obj, int value)
{
    while (SPI_S(obj)->busy);
    m_tx_buf[0] = value;
    SPI_S(obj)->async_mode = false;
    SPI_S(obj)->busy = true;
    
    (void)nrf_drv_spi_transfer(SPI_DRV(obj), (uint8_t const *) m_tx_buf, 1,
                                             (uint8_t *) m_rx_buf, 1);
    
    while (SPI_S(obj)->busy);
    return m_rx_buf[0];
}

int spi_slave_receive(spi_t *obj)
{
    return 0;
}

int spi_slave_read(spi_t *obj)
{
    return 0;
}

void spi_slave_write(spi_t *obj, int value)
{
    (void) obj;
    (void) value;
}

#if DEVICE_SPI_ASYNCH


void spi_master_transfer(spi_t *obj,
                         void *tx, size_t tx_length,
                         void *rx, size_t rx_length,
                         uint32_t handler,
                         uint32_t event,
                         DMAUsage hint)
{
    m_user_handler = (user_handler_t)handler;
    m_event = event;
    
    SPI_S(obj)->async_mode = true;
    SPI_S(obj)->busy = true;
    (void)nrf_drv_spi_transfer(SPI_DRV(obj), (uint8_t const *) tx, tx_length,
                                             (uint8_t *) rx, rx_length);
}

uint32_t spi_irq_handler_asynch(spi_t *obj)
{
    return m_event & SPI_EVENT_COMPLETE;
}

uint8_t spi_active(spi_t *obj)
{
    return SPI_S(obj)->busy;
}

void spi_abort_asynch(spi_t *obj)
{
    nrf_drv_spi_abort(SPI_DRV(obj));
}
#endif

#endif // DEVICE_SPI
