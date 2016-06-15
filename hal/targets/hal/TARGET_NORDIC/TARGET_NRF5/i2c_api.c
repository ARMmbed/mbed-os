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
#include "mbed_assert.h"
#include "mbed_error.h"
#include "i2c_api.h"
#include "nrf_drv_twi.h"
#include "app_util_platform.h"

#if DEVICE_I2C

#if DEVICE_I2C_ASYNCH
    #define TWI_IDX(obj)    ((obj)->i2c.twi_idx)
#else
    #define TWI_IDX(obj)    ((obj)->twi_idx)
#endif
#define TWI_INFO(obj)   (&m_twi_info[TWI_IDX(obj)])

typedef struct {
    bool initialized;
    nrf_drv_twi_config_t config;
    volatile bool transfer_finished;

    #if DEVICE_I2C_ASYNCH
    volatile uint32_t events;
    void (*handler)(void);
    uint32_t event_mask;
    #endif
} twi_info_t;
static twi_info_t m_twi_info[TWI_COUNT];

static nrf_drv_twi_t const m_twi_instances[TWI_COUNT] = {
#if TWI0_ENABLED
    NRF_DRV_TWI_INSTANCE(0),
#endif
#if TWI1_ENABLED
    NRF_DRV_TWI_INSTANCE(1),
#endif
};

static void twi_event_handler(nrf_drv_twi_evt_t const *event, void *context)
{
    twi_info_t * twi_info = TWI_INFO((i2c_t *)context);
    twi_info->transfer_finished = true;

#if DEVICE_I2C_ASYNCH
    switch (event->type) {
    case NRF_DRV_TWI_EVT_DONE:
        twi_info->events |= I2C_EVENT_TRANSFER_COMPLETE;
        break;

    case NRF_DRV_TWI_EVT_ADDRESS_NACK:
        twi_info->events |= I2C_EVENT_ERROR_NO_SLAVE;
        break;

    case NRF_DRV_TWI_EVT_DATA_NACK:
        twi_info->events |= I2C_EVENT_ERROR;
        break;
    }

    if (twi_info->handler) {
        twi_info->handler();
    }
#endif // DEVICE_I2C_ASYNCH
}

static uint8_t twi_address(int i2c_address)
{
    // The TWI driver requires 7-bit slave address (without R/W bit).
    return (i2c_address >> 1);
}

void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    int i;
    for (i = 0; i < TWI_COUNT; ++i) {
        if (m_twi_info[i].initialized &&
            m_twi_info[i].config.sda == (uint32_t)sda &&
            m_twi_info[i].config.scl == (uint32_t)scl) {
            TWI_IDX(obj) = i;
            TWI_INFO(obj)->config.frequency = NRF_TWI_FREQ_100K;
            i2c_reset(obj);
            return;
        }
    }

    nrf_drv_twi_config_t const config = {
        .scl                = scl,
        .sda                = sda,
        .frequency          = NRF_TWI_FREQ_100K,
        .interrupt_priority = APP_IRQ_PRIORITY_LOW,
    };

    for (i = 0; i < TWI_COUNT; ++i) {
        if (!m_twi_info[i].initialized) {
            nrf_drv_twi_t const *twi = &m_twi_instances[i];
            ret_code_t ret_code =
                nrf_drv_twi_init(twi, &config, twi_event_handler, obj);
            if (ret_code == NRF_SUCCESS) {
                TWI_IDX(obj) = i;
                TWI_INFO(obj)->initialized = true;
                TWI_INFO(obj)->config = config;

                nrf_drv_twi_enable(twi);
                return;
            }
        }
    }

    // No available peripheral
    error("No available I2C");
}

void i2c_reset(i2c_t *obj)
{
    twi_info_t *twi_info = TWI_INFO(obj);
    nrf_drv_twi_t const *twi = &m_twi_instances[TWI_IDX(obj)];

    nrf_drv_twi_uninit(twi);
    nrf_drv_twi_init(twi, &twi_info->config, twi_event_handler, obj);
    nrf_drv_twi_enable(twi);
}

int i2c_start(i2c_t *obj)
{
    (void)obj;

    return -1; // Not implemented.
}

int i2c_stop(i2c_t *obj)
{
    (void)obj;

    return -1; // Not implemented.
}

void i2c_frequency(i2c_t *obj, int hz)
{
    twi_info_t *twi_info = TWI_INFO(obj);
    nrf_drv_twi_t const *twi = &m_twi_instances[TWI_IDX(obj)];

    if (hz < 250000) {
        twi_info->config.frequency = NRF_TWI_FREQ_100K;
    } else if (hz < 400000) {
        twi_info->config.frequency = NRF_TWI_FREQ_250K;
    } else {
        twi_info->config.frequency = NRF_TWI_FREQ_400K;
    }
    nrf_twi_frequency_set(twi->reg.p_twi, twi_info->config.frequency);
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    (void)stop;

    twi_info_t *twi_info = TWI_INFO(obj);
    nrf_drv_twi_t const *twi = &m_twi_instances[TWI_IDX(obj)];

    twi_info->transfer_finished = false;
    ret_code_t ret_code = nrf_drv_twi_rx(twi, twi_address(address),
        (uint8_t *)data, length);
    if (ret_code != NRF_SUCCESS) {
        return 0;
    }
    while (!twi_info->transfer_finished) {}
    return nrf_drv_twi_data_count_get(twi);
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    twi_info_t *twi_info = TWI_INFO(obj);
    nrf_drv_twi_t const *twi = &m_twi_instances[TWI_IDX(obj)];

    twi_info->transfer_finished = false;
    ret_code_t ret_code = nrf_drv_twi_tx(twi, twi_address(address),
        (uint8_t const *)data, length, (stop == 0));
    if (ret_code != NRF_SUCCESS) {
        return 0;
    }
    while (!twi_info->transfer_finished) {}
    return nrf_drv_twi_data_count_get(twi);
}

int i2c_byte_read(i2c_t *obj, int last)
{
    (void)obj;
    (void)last;

    return -1; // Not implemented.
}

int i2c_byte_write(i2c_t *obj, int data)
{
    (void)obj;
    (void)data;

    return -1; // Not implemented.
}


#if DEVICE_I2C_ASYNCH

void i2c_transfer_asynch(i2c_t *obj, void *tx, size_t tx_length,
                         void *rx, size_t rx_length, uint32_t address,
                         uint32_t stop, uint32_t handler,
                         uint32_t event, DMAUsage hint)
{
    (void)stop;
    (void)hint;

    if (i2c_active(obj)) {
        return;
    }
    if ((tx_length == 0) && (rx_length == 0)) {
        return;
    }

    twi_info_t *twi_info = TWI_INFO(obj);
    twi_info->events     = 0;
    twi_info->handler    = (void (*)(void))handler;
    twi_info->event_mask = event;

    uint8_t twi_addr = twi_address(address);
    nrf_drv_twi_t const *twi = &m_twi_instances[TWI_IDX(obj)];

    if ((tx_length > 0) && (rx_length == 0)) {
        nrf_drv_twi_xfer_desc_t const xfer =
            NRF_DRV_TWI_XFER_DESC_TX(twi_addr, (uint8_t *)tx, tx_length);
        nrf_drv_twi_xfer(twi, &xfer,
            stop ? 0 : NRF_DRV_TWI_FLAG_TX_NO_STOP);
    }
    else if ((tx_length == 0) && (rx_length > 0)) {
        nrf_drv_twi_xfer_desc_t const xfer =
            NRF_DRV_TWI_XFER_DESC_RX(twi_addr, rx, rx_length);
        nrf_drv_twi_xfer(twi, &xfer, 0);
    }
    else if ((tx_length > 0) && (rx_length > 0)) {
        nrf_drv_twi_xfer_desc_t const xfer =
            NRF_DRV_TWI_XFER_DESC_TXRX(twi_addr,
                (uint8_t *)tx, tx_length, rx, rx_length);
        nrf_drv_twi_xfer(twi, &xfer, 0);
    }
}

uint32_t i2c_irq_handler_asynch(i2c_t *obj)
{
    twi_info_t *twi_info = TWI_INFO(obj);
    return (twi_info->events & twi_info->event_mask);
}

uint8_t i2c_active(i2c_t *obj)
{
    nrf_drv_twi_t const *twi = &m_twi_instances[TWI_IDX(obj)];
    return nrf_drv_twi_is_busy(twi);
}

void i2c_abort_asynch(i2c_t *obj)
{
    i2c_reset(obj);
}

#endif // DEVICE_I2C_ASYNCH

#endif // DEVICE_I2C
