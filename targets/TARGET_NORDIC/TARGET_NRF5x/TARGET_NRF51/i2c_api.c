/* 
 * Copyright (c) 2017 Nordic Semiconductor ASA
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


#include "i2c_api.h"

#if (defined(DEVICE_I2C) && defined(DEVICE_LPTICKER))

#include "mbed_assert.h"
#include "mbed_error.h"
#include "nrf_twi.h"
#include "nrf_drv_common.h"
#include "sdk_config.h"
#include "app_util_platform.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"

#include "us_ticker_api.h"

// An arbitrary value used as the timeout in loops waiting for given event
// (e.g. STOPPED), needed to avoid infinite loops.
// This value might be defined externally.
#ifndef I2C_TIMEOUT_VALUE_US
    #define I2C_TIMEOUT_VALUE_US 1000000
#endif

#if DEVICE_I2C_ASYNCH
    #define TWI_IDX(obj)    ((obj)->i2c.twi_idx)
#else
    #define TWI_IDX(obj)    ((obj)->twi_idx)
#endif
#define TWI_INFO(obj)   (&m_twi_info[TWI_IDX(obj)])

#ifdef TARGET_SDK13
    #define TWI0_INSTANCE_INDEX 0
    #define TWI1_INSTANCE_INDEX TWI0_INSTANCE_INDEX+TWI0_ENABLED
#endif

typedef struct {
    bool                initialized;
    uint32_t            pselsda;
    uint32_t            pselscl;
    nrf_twi_frequency_t frequency;
    bool                start_twi;

#if DEVICE_I2C_ASYNCH
    volatile bool   active;
    uint8_t const  *tx;
    size_t          tx_length;
    uint8_t        *rx;
    size_t          rx_length;
    bool            stop;

    volatile uint32_t   events;
    void              (*handler)(void);
    uint32_t            evt_mask;
#endif // DEVICE_I2C_ASYNCH
} twi_info_t;
static twi_info_t m_twi_info[TWI_COUNT];

static NRF_TWI_Type * const m_twi_instances[TWI_COUNT] = {
#if TWI0_ENABLED
    NRF_TWI0,
#endif
#if TWI1_ENABLED
    NRF_TWI1,
#endif
};

void SPI0_TWI0_IRQHandler(void);
void SPI1_TWI1_IRQHandler(void);

static const peripheral_handler_desc_t twi_handlers[TWI_COUNT] =
{
#if TWI0_ENABLED
    {
        SPI0_TWI0_IRQn,
        (uint32_t) SPI0_TWI0_IRQHandler
    },
#endif
#if TWI1_ENABLED
    {
        SPI1_TWI1_IRQn,
        (uint32_t) SPI1_TWI1_IRQHandler
    }
#endif
};
#ifdef NRF51
    #define TWI_IRQ_PRIORITY  APP_IRQ_PRIORITY_LOW
#elif defined(NRF52) || defined(NRF52840_XXAA)
    #define TWI_IRQ_PRIORITY  APP_IRQ_PRIORITY_LOWEST
#endif


#if DEVICE_I2C_ASYNCH
static void start_asynch_rx(twi_info_t *twi_info, NRF_TWI_Type *twi)
{
    if (twi_info->rx_length == 1 && twi_info->stop) {
        nrf_twi_shorts_set(twi, NRF_TWI_SHORT_BB_STOP_MASK);
    } else {
        nrf_twi_shorts_set(twi, NRF_TWI_SHORT_BB_SUSPEND_MASK);
    }
    nrf_twi_task_trigger(twi, NRF_TWI_TASK_STARTRX);
}

static void twi_irq_handler(uint8_t instance_idx)
{
    twi_info_t *twi_info = &m_twi_info[instance_idx];

    NRF_TWI_Type *twi = m_twi_instances[instance_idx];
    if (nrf_twi_event_check(twi, NRF_TWI_EVENT_ERROR)) {
        nrf_twi_event_clear(twi, NRF_TWI_EVENT_ERROR);

        // In case of an error, force STOP.
        // The current transfer may be suspended (if it is RX), so it must be
        // resumed before the STOP task is triggered.
        nrf_twi_task_trigger(twi, NRF_TWI_TASK_RESUME);
        nrf_twi_task_trigger(twi, NRF_TWI_TASK_STOP);

        uint32_t errorsrc = nrf_twi_errorsrc_get_and_clear(twi);
        twi_info->events |= I2C_EVENT_ERROR;
        if (errorsrc & NRF_TWI_ERROR_ADDRESS_NACK) {
            twi_info->events |= I2C_EVENT_ERROR_NO_SLAVE;
        }
        if (errorsrc & NRF_TWI_ERROR_DATA_NACK) {
            twi_info->events |= I2C_EVENT_TRANSFER_EARLY_NACK;
        }
    }

    bool finished = false;

    if (nrf_twi_event_check(twi, NRF_TWI_EVENT_TXDSENT)) {
        nrf_twi_event_clear(twi, NRF_TWI_EVENT_TXDSENT);

        MBED_ASSERT(twi_info->tx_length > 0);
        --(twi_info->tx_length);
        // Send next byte if there is still something to be sent.
        if (twi_info->tx_length > 0) {
            nrf_twi_txd_set(twi, *(twi_info->tx));
            ++(twi_info->tx);
        // It TX is done, start RX if requested.
        } else if (twi_info->rx_length > 0) {
            start_asynch_rx(twi_info, twi);
        // If there is nothing more to do, finalize the transfer.
        } else {
            if (twi_info->stop) {
                nrf_twi_task_trigger(twi, NRF_TWI_TASK_STOP);
            } else {
                nrf_twi_task_trigger(twi, NRF_TWI_TASK_SUSPEND);
                finished = true;
            }
            twi_info->events |= I2C_EVENT_TRANSFER_COMPLETE;
        }
    }

    if (nrf_twi_event_check(twi, NRF_TWI_EVENT_RXDREADY)) {
        nrf_twi_event_clear(twi, NRF_TWI_EVENT_RXDREADY);

        MBED_ASSERT(twi_info->rx_length > 0);
        *(twi_info->rx) = nrf_twi_rxd_get(twi);
        ++(twi_info->rx);
        --(twi_info->rx_length);

        if (twi_info->rx_length > 0) {
            // If more bytes should be received, resume the transfer
            // (in case the stop condition should be generated after the next
            // byte, change the shortcuts configuration first).
            if (twi_info->rx_length == 1 && twi_info->stop) {
                nrf_twi_shorts_set(twi, NRF_TWI_SHORT_BB_STOP_MASK);
            }
            nrf_twi_task_trigger(twi, NRF_TWI_TASK_RESUME);
        } else {
            // If all requested bytes were received, finalize the transfer.
            finished = true;
            twi_info->events |= I2C_EVENT_TRANSFER_COMPLETE;
        }
    }

    if (finished ||
        nrf_twi_event_check(twi, NRF_TWI_EVENT_STOPPED) ||
        (nrf_twi_int_enable_check(twi, NRF_TWI_INT_SUSPENDED_MASK) &&
         nrf_twi_event_check(twi, NRF_TWI_EVENT_SUSPENDED))) {
        // There is no need to clear the STOPPED and SUSPENDED events here,
        // they will no longer generate the interrupt - see below.

        nrf_twi_shorts_set(twi, 0);
        // Disable all interrupt sources.
        nrf_twi_int_disable(twi, UINT32_MAX);
        twi_info->active = false;

        if (twi_info->handler) {
            twi_info->handler();
        }
    }
}

#if TWI0_ENABLED
static void irq_handler_twi0(void)
{
    twi_irq_handler(TWI0_INSTANCE_INDEX);
}
#endif
#if TWI1_ENABLED
static void irq_handler_twi1(void)
{
    twi_irq_handler(TWI1_INSTANCE_INDEX);
}
#endif
static nrf_drv_irq_handler_t const m_twi_irq_handlers[TWI_COUNT] =
{
#if TWI0_ENABLED
    irq_handler_twi0,
#endif
#if TWI1_ENABLED
    irq_handler_twi1,
#endif
};
#endif // DEVICE_I2C_ASYNCH


static void configure_twi_pin(uint32_t pin, nrf_gpio_pin_dir_t dir)
{
    nrf_gpio_cfg(pin,
        dir,
        NRF_GPIO_PIN_INPUT_CONNECT,
        NRF_GPIO_PIN_PULLUP,
        NRF_GPIO_PIN_S0D1,
        NRF_GPIO_PIN_NOSENSE);
}

static void twi_clear_bus(twi_info_t *twi_info)
{
    // Try to set SDA high, and check if no slave tries to drive it low.
    nrf_gpio_pin_set(twi_info->pselsda);
    configure_twi_pin(twi_info->pselsda, NRF_GPIO_PIN_DIR_OUTPUT);
    // In case SDA is low, make up to 9 cycles on SCL line to help the slave
    // that pulls SDA low release it.
    if (!nrf_gpio_pin_read(twi_info->pselsda)) {
        nrf_gpio_pin_set(twi_info->pselscl);
        configure_twi_pin(twi_info->pselscl, NRF_GPIO_PIN_DIR_OUTPUT);
        nrf_delay_us(4);

        for (int i = 0; i < 9; i++) {
            if (nrf_gpio_pin_read(twi_info->pselsda)) {
                break;
            }
            nrf_gpio_pin_clear(twi_info->pselscl);
            nrf_delay_us(4);
            nrf_gpio_pin_set(twi_info->pselscl);
            nrf_delay_us(4);
        }

        // Finally, generate STOP condition to put the bus into initial state.
        nrf_gpio_pin_clear(twi_info->pselsda);
        nrf_delay_us(4);
        nrf_gpio_pin_set(twi_info->pselsda);
    }
}

void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    ret_code_t ret;
    int i;

    for (i = 0; i < TWI_COUNT; ++i) {
        if (m_twi_info[i].initialized &&
            m_twi_info[i].pselsda == (uint32_t)sda &&
            m_twi_info[i].pselscl == (uint32_t)scl) {
            TWI_IDX(obj) = i;
            TWI_INFO(obj)->frequency = NRF_TWI_FREQ_100K;
            i2c_reset(obj);
            return;
        }
    }

    for (i = 0; i < TWI_COUNT; ++i) {
        if (!m_twi_info[i].initialized) {
            ret = nrf_drv_common_per_res_acquire(m_twi_instances[i],
                    m_twi_irq_handlers[i]);

            if (ret != NRF_SUCCESS) {
                continue; /* the hw resource is busy - test another one */
            }

            TWI_IDX(obj) = i;

            twi_info_t *twi_info = TWI_INFO(obj);
            twi_info->initialized = true;
            twi_info->pselsda     = (uint32_t)sda;
            twi_info->pselscl     = (uint32_t)scl;
            twi_info->frequency   = NRF_TWI_FREQ_100K;
            twi_info->start_twi   = false;
#if DEVICE_I2C_ASYNCH
            twi_info->active      = false;
#endif

            twi_clear_bus(twi_info);

            configure_twi_pin(twi_info->pselsda, NRF_GPIO_PIN_DIR_INPUT);
            configure_twi_pin(twi_info->pselscl, NRF_GPIO_PIN_DIR_INPUT);

            i2c_reset(obj);

#if DEVICE_I2C_ASYNCH
            NVIC_SetVector(twi_handlers[i].IRQn, twi_handlers[i].vector);
            nrf_drv_common_irq_enable(twi_handlers[i].IRQn, TWI_IRQ_PRIORITY);
#endif

            return;
        }
    }

    error("No available I2C peripheral\r\n");
}

void i2c_reset(i2c_t *obj)
{
    twi_info_t *twi_info = TWI_INFO(obj);
    NRF_TWI_Type *twi = m_twi_instances[TWI_IDX(obj)];

    nrf_twi_disable(twi);
    nrf_twi_pins_set(twi, twi_info->pselscl, twi_info->pselsda);
    nrf_twi_frequency_set(twi, twi_info->frequency);
    nrf_twi_enable(twi);
}

int i2c_start(i2c_t *obj)
{
    twi_info_t *twi_info = TWI_INFO(obj);
#if DEVICE_I2C_ASYNCH
    if (twi_info->active) {
        return I2C_ERROR_BUS_BUSY;
    }
#endif
    twi_info->start_twi = true;

    return 0;
}

int i2c_stop(i2c_t *obj)
{
    NRF_TWI_Type *twi = m_twi_instances[TWI_IDX(obj)];
    uint32_t t0;

    // The current transfer may be suspended (if it is RX), so it must be
    // resumed before the STOP task is triggered.
    nrf_twi_task_trigger(twi, NRF_TWI_TASK_RESUME);
    nrf_twi_task_trigger(twi, NRF_TWI_TASK_STOP);

    t0 = ticker_read(get_us_ticker_data());

    do {
        if (nrf_twi_event_check(twi, NRF_TWI_EVENT_STOPPED)) {
            return 0;
        }
    } while (((uint32_t)ticker_read(get_us_ticker_data()) - t0) < I2C_TIMEOUT_VALUE_US);

    return 1;
}

void i2c_frequency(i2c_t *obj, int hz)
{
    twi_info_t *twi_info = TWI_INFO(obj);
    NRF_TWI_Type *twi = m_twi_instances[TWI_IDX(obj)];

    if (hz < 250000) {
        twi_info->frequency = NRF_TWI_FREQ_100K;
    } else if (hz < 400000) {
        twi_info->frequency = NRF_TWI_FREQ_250K;
    } else {
        twi_info->frequency = NRF_TWI_FREQ_400K;
    }
    nrf_twi_frequency_set(twi, twi_info->frequency);
}

static uint8_t twi_address(int i2c_address)
{
    // The TWI peripheral requires 7-bit slave address (without R/W bit).
    return (i2c_address >> 1);
}

static void start_twi_read(NRF_TWI_Type *twi, int address)
{
    nrf_twi_event_clear(twi, NRF_TWI_EVENT_STOPPED);
    nrf_twi_event_clear(twi, NRF_TWI_EVENT_RXDREADY);
    nrf_twi_event_clear(twi, NRF_TWI_EVENT_ERROR);
    (void)nrf_twi_errorsrc_get_and_clear(twi);

    nrf_twi_shorts_set(twi, NRF_TWI_SHORT_BB_SUSPEND_MASK);

    nrf_twi_address_set(twi, twi_address(address));
    nrf_twi_task_trigger(twi, NRF_TWI_TASK_RESUME);
    nrf_twi_task_trigger(twi, NRF_TWI_TASK_STARTRX);
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    // Zero-length RX transfers are not supported. Such transfers cannot
    // be easily achieved with TWI peripheral (some dirty tricks would be
    // required for this), and they are actually useless (TX can be used
    // to check if the address is acknowledged by a slave).
    MBED_ASSERT(length > 0);

    twi_info_t *twi_info = TWI_INFO(obj);
#if DEVICE_I2C_ASYNCH
    if (twi_info->active) {
        return I2C_ERROR_BUS_BUSY;
    }
#endif
    twi_info->start_twi = false;

    NRF_TWI_Type *twi = m_twi_instances[TWI_IDX(obj)];
    start_twi_read(twi, address);

    int result = length;
    while (length > 0) {
        int byte_read_result = i2c_byte_read(obj, (stop && length == 1));
        if (byte_read_result < 0) {
            // When an error occurs, return the number of bytes that have been
            // received successfully.
            result -= length;
            // Force STOP condition.
            stop = 1;
            break;
        }
        *data++ = (uint8_t)byte_read_result;
        --length;
    }

    if (stop) {
        (void)i2c_stop(obj);
    }

    return result;
}

static uint8_t twi_byte_write(NRF_TWI_Type *twi, uint8_t data)
{
    uint32_t t0;

    nrf_twi_event_clear(twi, NRF_TWI_EVENT_TXDSENT);
    nrf_twi_event_clear(twi, NRF_TWI_EVENT_ERROR);

    nrf_twi_txd_set(twi, data);

    t0 = ticker_read(get_us_ticker_data());

    do {
        if (nrf_twi_event_check(twi, NRF_TWI_EVENT_TXDSENT)) {
            nrf_twi_event_clear(twi, NRF_TWI_EVENT_TXDSENT);
            return 1; // ACK received
        }
        if (nrf_twi_event_check(twi, NRF_TWI_EVENT_ERROR)) {
            nrf_twi_event_clear(twi, NRF_TWI_EVENT_ERROR);
            return 0; // some error occurred
        }
    } while (((uint32_t)ticker_read(get_us_ticker_data()) - t0) < I2C_TIMEOUT_VALUE_US);

    return 2; // timeout;
}

static void start_twi_write(NRF_TWI_Type *twi, int address)
{
    nrf_twi_event_clear(twi, NRF_TWI_EVENT_STOPPED);
    nrf_twi_event_clear(twi, NRF_TWI_EVENT_TXDSENT);
    nrf_twi_event_clear(twi, NRF_TWI_EVENT_ERROR);
    (void)nrf_twi_errorsrc_get_and_clear(twi);

    nrf_twi_shorts_set(twi, 0);

    nrf_twi_address_set(twi, twi_address(address));
    nrf_twi_task_trigger(twi, NRF_TWI_TASK_RESUME);
    nrf_twi_task_trigger(twi, NRF_TWI_TASK_STARTTX);
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    twi_info_t *twi_info = TWI_INFO(obj);
    bool timeout = false;
    uint32_t t0, t1;

#if DEVICE_I2C_ASYNCH
    if (twi_info->active) {
        return I2C_ERROR_BUS_BUSY;
    }
#endif
    twi_info->start_twi = false;

    NRF_TWI_Type *twi = m_twi_instances[TWI_IDX(obj)];
    start_twi_write(twi, address);

    // Special case - transaction with no data.
    // It can be used to check if a slave acknowledges the address.
    if (length == 0) {
        nrf_twi_event_t event;
        if (stop) {
            event = NRF_TWI_EVENT_STOPPED;
            nrf_twi_task_trigger(twi, NRF_TWI_TASK_STOP);
        } else {
            event = NRF_TWI_EVENT_SUSPENDED;
            nrf_twi_event_clear(twi, event);
            nrf_twi_task_trigger(twi, NRF_TWI_TASK_SUSPEND);
        }

        t0 = ticker_read(get_us_ticker_data());

        do {
            if (nrf_twi_event_check(twi, event)) {
                break;
            }
            t1 = ticker_read(get_us_ticker_data());
            timeout = (t1 - t0) >= I2C_TIMEOUT_VALUE_US;
        } while (!timeout);

        uint32_t errorsrc = nrf_twi_errorsrc_get_and_clear(twi);
        if (errorsrc & NRF_TWI_ERROR_ADDRESS_NACK) {
            if (!stop) {
                i2c_stop(obj);
            }
            return I2C_ERROR_NO_SLAVE;
        }

        return (timeout ? I2C_ERROR_BUS_BUSY : 0);
    }

    int result = length;
    do {
        uint8_t byte_write_result = twi_byte_write(twi, (uint8_t)*data++);
        if (byte_write_result != 1) {
            if (byte_write_result == 0) {
                // Check what kind of error has been signaled by TWI.
                uint32_t errorsrc = nrf_twi_errorsrc_get_and_clear(twi);
                if (errorsrc & NRF_TWI_ERROR_ADDRESS_NACK) {
                    result = I2C_ERROR_NO_SLAVE;
                } else {
                    // Some other error - return the number of bytes that
                    // have been sent successfully.
                    result -= length;
                }
            } else {
                result = I2C_ERROR_BUS_BUSY;
            }
            // Force STOP condition.
            stop = 1;
            break;
        }
        --length;
    } while (length > 0);

    if (stop) {
        (void)i2c_stop(obj);
    }

    return result;
}

int i2c_byte_read(i2c_t *obj, int last)
{
    NRF_TWI_Type *twi = m_twi_instances[TWI_IDX(obj)];
    uint32_t t0;

    if (last) {
        nrf_twi_shorts_set(twi, NRF_TWI_SHORT_BB_STOP_MASK);
    }
    nrf_twi_task_trigger(twi, NRF_TWI_TASK_RESUME);

    t0 = ticker_read(get_us_ticker_data());

    do {
        if (nrf_twi_event_check(twi, NRF_TWI_EVENT_RXDREADY)) {
            nrf_twi_event_clear(twi, NRF_TWI_EVENT_RXDREADY);
            return nrf_twi_rxd_get(twi);
        }
        if (nrf_twi_event_check(twi, NRF_TWI_EVENT_ERROR)) {
            nrf_twi_event_clear(twi, NRF_TWI_EVENT_ERROR);
            return I2C_ERROR_NO_SLAVE;
        }
    } while (((uint32_t)ticker_read(get_us_ticker_data()) - t0) < I2C_TIMEOUT_VALUE_US);

    return I2C_ERROR_BUS_BUSY;
}

int i2c_byte_write(i2c_t *obj, int data)
{
    NRF_TWI_Type *twi = m_twi_instances[TWI_IDX(obj)];
    twi_info_t *twi_info = TWI_INFO(obj);
    if (twi_info->start_twi) {
        twi_info->start_twi = false;

        if (data & 1) {
            start_twi_read(twi, data);
        } else {
            start_twi_write(twi, data);
        }
        return 1;
    } else {
        nrf_twi_task_trigger(twi, NRF_TWI_TASK_RESUME);
        // 0 - TWI signaled error (NAK is the only possibility here)
        // 1 - ACK received
        // 2 - timeout (clock stretched for too long?)
        return twi_byte_write(twi, (uint8_t)data);
    }
}


#if DEVICE_I2C_ASYNCH
void i2c_transfer_asynch(i2c_t *obj, const void *tx, size_t tx_length,
                         void *rx, size_t rx_length, uint32_t address,
                         uint32_t stop, uint32_t handler,
                         uint32_t event, DMAUsage hint)
{
    (void)hint;

    twi_info_t *twi_info = TWI_INFO(obj);
    if (twi_info->active) {
        return;
    }
    twi_info->active    = true;
    twi_info->events    = 0;
    twi_info->handler   = (void (*)(void))handler;
    twi_info->evt_mask  = event;
    twi_info->tx_length = tx_length;
    twi_info->tx        = tx;
    twi_info->rx_length = rx_length;
    twi_info->rx        = rx;
    twi_info->stop      = stop;

    NRF_TWI_Type *twi = m_twi_instances[TWI_IDX(obj)];

    nrf_twi_event_clear(twi, NRF_TWI_EVENT_TXDSENT);
    nrf_twi_event_clear(twi, NRF_TWI_EVENT_RXDREADY);
    nrf_twi_event_clear(twi, NRF_TWI_EVENT_STOPPED);
    nrf_twi_event_clear(twi, NRF_TWI_EVENT_SUSPENDED);
    nrf_twi_event_clear(twi, NRF_TWI_EVENT_ERROR);
    (void)nrf_twi_errorsrc_get_and_clear(twi);

    nrf_twi_address_set(twi, twi_address(address));
    nrf_twi_task_trigger(twi, NRF_TWI_TASK_RESUME);
    // TX only, or TX + RX (after a repeated start).
    if (tx_length > 0) {
        nrf_twi_task_trigger(twi, NRF_TWI_TASK_STARTTX);
        nrf_twi_txd_set(twi, *(twi_info->tx));
        ++(twi_info->tx);
    // RX only.
    } else if (rx_length > 0) {
        start_asynch_rx(twi_info, twi);
    // Both 'tx_length' and 'rx_length' are 0 - this case may be used
    // to test if the slave is presentand ready for transfer (by just
    // sending the address and checking if it is acknowledged).
    } else {
        nrf_twi_task_trigger(twi, NRF_TWI_TASK_STARTTX);
        if (stop) {
            nrf_twi_task_trigger(twi, NRF_TWI_TASK_STOP);
        } else {
            nrf_twi_task_trigger(twi, NRF_TWI_TASK_SUSPEND);
            nrf_twi_int_enable(twi, NRF_TWI_INT_SUSPENDED_MASK);
        }
        twi_info->events |= I2C_EVENT_TRANSFER_COMPLETE;
    }

    nrf_twi_int_enable(twi, NRF_TWI_INT_TXDSENT_MASK |
                            NRF_TWI_INT_RXDREADY_MASK |
                            NRF_TWI_INT_STOPPED_MASK |
                            NRF_TWI_INT_ERROR_MASK);
}

uint32_t i2c_irq_handler_asynch(i2c_t *obj)
{
    twi_info_t *twi_info = TWI_INFO(obj);
    return (twi_info->events & twi_info->evt_mask);
}

uint8_t i2c_active(i2c_t *obj)
{
    twi_info_t *twi_info = TWI_INFO(obj);
    return twi_info->active;
}

void i2c_abort_asynch(i2c_t *obj)
{
    i2c_reset(obj);
}
#endif // DEVICE_I2C_ASYNCH

#endif // DEVICE_I2C
