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

#if (defined(DEVICE_I2C) && defined(DEVICE_LPTICKER))
/* I2C
 *
 * This HAL implementation uses the nrf_drv_twi.h API primarily but switches to TWI for the
 * low-level HAL functions. These calls can't be implemented with the TWIM due to the
 * different API.
 *
 * Known limitations:
 *  * The TWI/TWIM only supports 7-bit addresses.
 *  * The TWI API doesn't allow reading 1 byte. At least 2 bytes will be read.
 */

#include "i2c_api.h"
#include "lp_ticker_api.h"

#include "object_owners.h"
#include "pinmap_ex.h"

#include "nrf_drv_twi.h"
#include "nrf_drv_common.h"
#include "app_util_platform.h"

#if 0
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif

#define DEFAULT_TIMEOUT_US (1000)   // timeout for waiting for address NACK
#define MAXIMUM_TIMEOUT_US (10000)  // timeout for waiting for RX
#define I2C_READ_BIT 0x01           // read bit

/* Keep track of what mode the peripheral is in. On NRF52, Driver mode can use TWIM. */
typedef enum {
    NORDIC_I2C_MODE_NONE,
    NORDIC_I2C_MODE_TWI,
    NORDIC_I2C_MODE_DRIVER
} nordic_nrf5_mode_t;

/* In simple mode, the Start signal is sent on the first write call due to hardware limitations. */
typedef enum {
    NORDIC_TWI_STATE_IDLE,
    NORDIC_TWI_STATE_START,
    NORDIC_TWI_STATE_BUSY
} nordic_nrf5_twi_state_t;

/* Forward declaration. These functions are implemented in the driver but not
 * set up in the NVIC due to it being relocated.
 */
void SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler(void);
void SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler(void);

/** Initialize the I2C peripheral. It sets the default parameters for I2C
 *  peripheral, and configures its specifieds pins.
 *
 *  @param obj  The I2C object
 *  @param sda  The sda pin
 *  @param scl  The scl pin
 */
void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    DEBUG_PRINTF("i2c_init: %p %d %d\r\n", obj, sda, scl);

#if DEVICE_I2C_ASYNCH
    struct i2c_s *config = &obj->i2c;
#else
    struct i2c_s *config = obj;
#endif

    /* Get instance from pin configuration. */
    int instance = pin_instance_i2c(sda, scl);
    MBED_ASSERT(instance < ENABLED_TWI_COUNT);

    /* Initialize i2c_t object */
    config->instance = instance;
    config->sda = sda;
    config->scl = scl;
    config->frequency = NRF_TWI_FREQ_100K;
    config->state = NORDIC_TWI_STATE_IDLE;
    config->mode = NORDIC_I2C_MODE_NONE;

#if DEVICE_I2C_ASYNCH
    config->handler = 0;
    config->event = 0;
    config->mask = 0;
#endif

    /* Force reconfiguration */
    config->update = true;

    static bool first_init = true;

    if (first_init) {
        first_init = false;

        /* Register interrupt handlers in driver with the NVIC. */
        NVIC_SetVector(SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn, (uint32_t) SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler);
        NVIC_SetVector(SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn, (uint32_t) SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler);
    }
}

/** Configure the I2C frequency
 *
 *  @param obj The I2C object
 *  @param hz  Frequency in Hz
 */
void i2c_frequency(i2c_t *obj, int hz)
{
    DEBUG_PRINTF("i2c_frequency: %p %d\r\n", obj, hz);

#if DEVICE_I2C_ASYNCH
    struct i2c_s *config = &obj->i2c;
#else
    struct i2c_s *config = obj;
#endif

    /* Round down to nearest valid frequency. */
    nrf_twi_frequency_t new_frequency;

    if (hz < 250000) {
        new_frequency = NRF_TWI_FREQ_100K;
    } else if (hz < 400000) {
        new_frequency = NRF_TWI_FREQ_250K;
    } else {
        new_frequency = NRF_TWI_FREQ_400K;
    }

    /* Only store frequency in object. Configuration happens at the beginning of each transaction. */
    config->frequency = new_frequency;
    config->update = true;
}


/***
 *       _____ _                 _        _________          _______
 *      / ____(_)               | |      |__   __\ \        / /_   _|
 *     | (___  _ _ __ ___  _ __ | | ___     | |   \ \  /\  / /  | |
 *      \___ \| | '_ ` _ \| '_ \| |/ _ \    | |    \ \/  \/ /   | |
 *      ____) | | | | | | | |_) | |  __/    | |     \  /\  /   _| |_
 *     |_____/|_|_| |_| |_| .__/|_|\___|    |_|      \/  \/   |_____|
 *                        | |
 *                        |_|
 */

/*****************************************************************************/
/* Simple API implementation using TWI                                       */
/*****************************************************************************/

/* Global array for easy register selection for each instance. */
static NRF_TWI_Type * nordic_nrf5_twi_register[2] = { NRF_TWI0, NRF_TWI1 };

/**
 * @brief      Reconfigure TWI register.
 *
 *             If the peripheral is enabled, it will be disabled first. All
 *             registers are cleared to their default values unless replaced
 *             by new configuration.
 *
 *             If the object is the owner, the mode hasn't changed, and the
 *             force change flag is false, all settings are kept unchanged.
 *
 * @param      obj           The object
 */
void i2c_configure_twi_instance(i2c_t *obj)
{
#if DEVICE_I2C_ASYNCH
    struct i2c_s *config = &obj->i2c;
#else
    struct i2c_s *config = obj;
#endif

    int instance = config->instance;

    /* Get pointer to object of the current owner of the peripheral. */
    void *current_owner = object_owner_spi2c_get(instance);

    /* Check if reconfiguration is actually necessary. */
    if ((obj != current_owner) || (config->mode != NORDIC_I2C_MODE_TWI) || config->update) {

        DEBUG_PRINTF("i2c_configure_twi_instance: %p %p\r\n", obj, current_owner);

        /* Claim ownership of peripheral. */
        object_owner_spi2c_set(instance, obj);

        /* Set current mode. */
        config->mode = NORDIC_I2C_MODE_TWI;

        /* Disable peripheral if it is currently enabled. */
        if (nordic_nrf5_twi_register[instance]->ENABLE) {
            nrf_twi_disable(nordic_nrf5_twi_register[instance]);
        }

        /* Force resource release. This is necessary because mbed drivers don't
         * deinitialize on object destruction.
         */
        nrf_drv_common_irq_disable(nrf_drv_get_IRQn((void *) nordic_nrf5_twi_register[instance]));
        nrf_drv_common_per_res_release(nordic_nrf5_twi_register[instance]);

        /* Reset shorts register. */
        nrf_twi_shorts_set(nordic_nrf5_twi_register[instance], 0);

        /* Disable all TWI interrupts. */
        nrf_twi_int_disable(nordic_nrf5_twi_register[instance],
                            NRF_TWI_INT_STOPPED_MASK  |
                            NRF_TWI_INT_RXDREADY_MASK |
                            NRF_TWI_INT_TXDSENT_MASK  |
                            NRF_TWI_INT_ERROR_MASK    |
                            NRF_TWI_INT_BB_MASK       |
                            NRF_TWI_INT_SUSPENDED_MASK);

        /* Clear error register. */
        nrf_twi_errorsrc_get_and_clear(nordic_nrf5_twi_register[instance]);

        /* Clear all previous events. */
        nrf_twi_event_clear(nordic_nrf5_twi_register[instance],
                            NRF_TWI_EVENT_STOPPED  |
                            NRF_TWI_EVENT_RXDREADY |
                            NRF_TWI_EVENT_TXDSENT  |
                            NRF_TWI_EVENT_ERROR    |
                            NRF_TWI_EVENT_BB       |
                            NRF_TWI_EVENT_SUSPENDED);

        /* Configure SDA and SCL pins. */
        nrf_twi_pins_set(nordic_nrf5_twi_register[instance],
                         config->scl,
                         config->sda);

        /* Set frequency. */
        nrf_twi_frequency_set(nordic_nrf5_twi_register[instance],
                              config->frequency);

        /* Enable TWI peripheral with new settings. */
        nrf_twi_enable(nordic_nrf5_twi_register[instance]);
    }
}

/** Send START command
 *
 *  @param obj The I2C object
 */
int i2c_start(i2c_t *obj)
{
    DEBUG_PRINTF("i2c_start: %p\r\n", obj);

#if DEVICE_I2C_ASYNCH
    struct i2c_s *config = &obj->i2c;
#else
    struct i2c_s *config = obj;
#endif

    /* Change state but defer actual signaling until the first byte (the address)
       is transmitted. This is due to hardware limitations.
    */
    config->state = NORDIC_TWI_STATE_START;

    return 0;
}

/** Write one byte
 *
 *  @param obj The I2C object
 *  @param data Byte to be written
 *  @return 0 if NAK was received, 1 if ACK was received, 2 for timeout.
 */
int i2c_byte_write(i2c_t *obj, int data)
{
    DEBUG_PRINTF("i2c_byte_write: %p %d\r\n", obj, data);

#if DEVICE_I2C_ASYNCH
    struct i2c_s *config = &obj->i2c;
#else
    struct i2c_s *config = obj;
#endif

    int instance = config->instance;
    int result = 1; // default to ACK

    /* Check if this is the first byte to be transferred. If it is, then send start signal and address. */
    if (config->state == NORDIC_TWI_STATE_START) {
        config->state = NORDIC_TWI_STATE_BUSY;

        /* Beginning of new transaction, configure peripheral if necessary. */
        i2c_configure_twi_instance(obj);

        /* Set I2C device address. NOTE: due to hardware limitations only 7-bit addresses are supported. */
        nrf_twi_address_set(nordic_nrf5_twi_register[instance], data >> 1);

        /* If read bit is set, trigger read task otherwise trigger write task. */
        if (data & I2C_READ_BIT) {
            /* For timing reasons, reading bytes requires shorts to suspend peripheral after each byte. */
            nrf_twi_shorts_set(nordic_nrf5_twi_register[instance], NRF_TWI_SHORT_BB_SUSPEND_MASK);
            nrf_twi_task_trigger(nordic_nrf5_twi_register[instance], NRF_TWI_TASK_STARTRX);
        } else {
            /* Reset shorts register. */
            nrf_twi_shorts_set(nordic_nrf5_twi_register[instance], 0);
            nrf_twi_task_trigger(nordic_nrf5_twi_register[instance], NRF_TWI_TASK_STARTTX);
        }

        /* Setup stop watch for timeout. */
        uint32_t start_us = lp_ticker_read();
        uint32_t now_us = start_us;

        /* Block until timeout or an address error has been detected. */
        while (((now_us - start_us) < DEFAULT_TIMEOUT_US) &&
               !(nrf_twi_event_check(nordic_nrf5_twi_register[instance], NRF_TWI_EVENT_ERROR))) {
            now_us = lp_ticker_read();
        }

        /* Check error register and update return value if an address NACK was detected. */
        uint32_t error = nrf_twi_errorsrc_get_and_clear(nordic_nrf5_twi_register[instance]);

        if (error & NRF_TWI_ERROR_ADDRESS_NACK) {
            result = 0; // set NACK
        }
    } else {

        /* Normal write. Send next byte after clearing event flag. */
        nrf_twi_event_clear(nordic_nrf5_twi_register[instance], NRF_TWI_EVENT_TXDSENT);
        nrf_twi_txd_set(nordic_nrf5_twi_register[instance], data);

        /* Setup stop watch for timeout. */
        uint32_t start_us = lp_ticker_read();
        uint32_t now_us = start_us;

        /* Block until timeout or the byte has been sent. */
        while (((now_us - start_us) < MAXIMUM_TIMEOUT_US) &&
               !(nrf_twi_event_check(nordic_nrf5_twi_register[instance], NRF_TWI_EVENT_TXDSENT))) {
            now_us = lp_ticker_read();
        }

        /* Check the error code to see if the byte was acknowledged. */
        uint32_t error = nrf_twi_errorsrc_get_and_clear(nordic_nrf5_twi_register[instance]);

        if (error & NRF_TWI_ERROR_DATA_NACK) {
            result = 0; // set NACK
        } else if (now_us - start_us >= MAXIMUM_TIMEOUT_US) {
            result = 2; // set timeout
        }
    }

    return result;
}

/** Read one byte
 *
 *  @param obj The I2C object
 *  @param last Acknoledge
 *  @return The read byte
 */
int i2c_byte_read(i2c_t *obj, int last)
{
    DEBUG_PRINTF("i2c_byte_read: %p %d\r\n", obj, last);

#if DEVICE_I2C_ASYNCH
    struct i2c_s *config = &obj->i2c;
#else
    struct i2c_s *config = obj;
#endif

    int instance = config->instance;
    int retval = I2C_ERROR_NO_SLAVE;

    uint32_t start_us = 0;
    uint32_t now_us = 0;

    /* Due to hardware limitations, the stop condition must triggered through a short before
     * reading the last byte.
     */
    if (last) {
        nrf_twi_shorts_set(nordic_nrf5_twi_register[instance], NRF_TWI_SHORT_BB_STOP_MASK);

        /* Transaction will be complete after this call, reset state. */
        config->state = NORDIC_TWI_STATE_IDLE;
    }

    /* Due to the way events are generated, if a byte is available it should be read directly
     * but without resuming reading. Otherwise the TWI will read one byte too many.
     */
    if (nrf_twi_event_check(nordic_nrf5_twi_register[instance], NRF_TWI_EVENT_RXDREADY)) {
        retval = nrf_twi_rxd_get(nordic_nrf5_twi_register[instance]);
        nrf_twi_event_clear(nordic_nrf5_twi_register[instance], NRF_TWI_EVENT_RXDREADY);
    } else {

        /* No data available, resume reception. */
        nrf_twi_task_trigger(nordic_nrf5_twi_register[instance], NRF_TWI_TASK_RESUME);

        /* Setup timeout */
        start_us = lp_ticker_read();
        now_us = start_us;

        /* Block until timeout or data ready event has been signaled. */
        while (((now_us - start_us) < MAXIMUM_TIMEOUT_US) &&
               !(nrf_twi_event_check(nordic_nrf5_twi_register[instance], NRF_TWI_EVENT_RXDREADY))) {
            now_us = lp_ticker_read();
        }

        /* Retrieve data from buffer. */
        if ((now_us - start_us) < MAXIMUM_TIMEOUT_US) {
            retval = nrf_twi_rxd_get(nordic_nrf5_twi_register[instance]);
            nrf_twi_event_clear(nordic_nrf5_twi_register[instance], NRF_TWI_EVENT_RXDREADY);
        }
    }

    return retval;
}

/** Send STOP command
 *
 *  @param obj The I2C object
 */
int i2c_stop(i2c_t *obj)
{
    DEBUG_PRINTF("i2c_stop: %p\r\n", obj);

#if DEVICE_I2C_ASYNCH
    struct i2c_s *config = &obj->i2c;
#else
    struct i2c_s *config = obj;
#endif

    int instance = config->instance;

    /* Set explicit stop signal. */
    nrf_twi_event_clear(nordic_nrf5_twi_register[instance], NRF_TWI_EVENT_STOPPED);
    nrf_twi_task_trigger(nordic_nrf5_twi_register[instance], NRF_TWI_TASK_STOP);

    /* Block until stop signal has been generated. */
    uint32_t start_us = lp_ticker_read();
    uint32_t now_us = start_us;

    while (((now_us - start_us) < MAXIMUM_TIMEOUT_US) &&
           !(nrf_twi_event_check(nordic_nrf5_twi_register[instance], NRF_TWI_EVENT_STOPPED))) {
        now_us = lp_ticker_read();
    }

    /* Reset state. */
    config->state = NORDIC_TWI_STATE_IDLE;

    return 0;
}

/** Reset I2C peripheral. TODO: The action here. Most of the implementation sends stop()
 *
 *  @param obj The I2C object
 */
void i2c_reset(i2c_t *obj)
{
    DEBUG_PRINTF("i2c_reset: %p\r\n", obj);

#if DEVICE_I2C_ASYNCH
    struct i2c_s *config = &obj->i2c;
#else
    struct i2c_s *config = obj;
#endif

    /* Force reconfiguration to reset peripheral completely. */
    config->update = true;
    i2c_configure_twi_instance(obj);
}



/***
 *      _____       _                  _________          _______
 *     |  __ \     (_)                |__   __\ \        / /_   _|
 *     | |  | |_ __ ___   _____ _ __     | |   \ \  /\  / /  | |
 *     | |  | | '__| \ \ / / _ \ '__|    | |    \ \/  \/ /   | |
 *     | |__| | |  | |\ V /  __/ |       | |     \  /\  /   _| |_
 *     |_____/|_|  |_| \_/ \___|_|       |_|      \/  \/   |_____|
 *
 *
 */

/* Global array holding driver configuration for easy access. */
static const nrf_drv_twi_t nordic_nrf5_instance[2] = { NRF_DRV_TWI_INSTANCE(0), NRF_DRV_TWI_INSTANCE(1) };

/* Forward declare interrupt handler. */
#if DEVICE_I2C_ASYNCH
static void nordic_nrf5_twi_event_handler(nrf_drv_twi_evt_t const *p_event, void *p_context);
#endif

/**
 * @brief      Reconfigure driver.
 *
 *             If the peripheral is enabled, it will be disabled first. All
 *             registers are cleared to their default values unless replaced
 *             by new configuration.
 *
 *             If the object is the owner, the mode hasn't changed, and the
 *             force change flag is false, all settings are kept unchanged.
 *
 * @param      obj           The object
 * @param[in]  force_change  Set to true to force a reconfiguration.
 */
static void i2c_configure_driver_instance(i2c_t *obj)
{
#if DEVICE_I2C_ASYNCH
    struct i2c_s *config = &obj->i2c;
#else
    struct i2c_s *config = obj;
#endif

    int instance = config->instance;

    /* Get pointer to object of the current owner of the peripheral. */
    void *current_owner = object_owner_spi2c_get(instance);

    /* Check if reconfiguration is actually necessary. */
    if ((obj != current_owner) || (config->mode != NORDIC_I2C_MODE_DRIVER) || config->update) {

        DEBUG_PRINTF("i2c_configure_driver_instance: %p %p\r\n", obj, current_owner);

        /* Claim ownership of peripheral. */
        object_owner_spi2c_set(instance, obj);

        /* Set current mode. */
        config->mode = NORDIC_I2C_MODE_DRIVER;

        /* If the peripheral is already running, then disable it and use the driver API to uninitialize it.*/
        if (nordic_nrf5_instance[instance].reg.p_twi->ENABLE) {
            nrf_drv_twi_disable(&nordic_nrf5_instance[instance]);
            nrf_drv_twi_uninit(&nordic_nrf5_instance[instance]);
        }

        /* Force resource release. This is necessary because mbed drivers don't
         * deinitialize on object destruction.
         */
        nrf_drv_common_irq_disable(nrf_drv_get_IRQn((void *) nordic_nrf5_twi_register[instance]));
        nrf_drv_common_per_res_release(nordic_nrf5_twi_register[instance]);

        /* Configure driver with new settings. */
        nrf_drv_twi_config_t twi_config = {
            .scl = config->scl,
            .sda = config->sda,
            .frequency = config->frequency,
            .interrupt_priority = APP_IRQ_PRIORITY_LOWEST,
            .clear_bus_init = false,
            .hold_bus_uninit = false
        };

#if DEVICE_I2C_ASYNCH
        /* Set callback handler in asynchronous mode. */
        if (config->handler) {

            /* Initialze driver in non-blocking mode. */
            nrf_drv_twi_init(&nordic_nrf5_instance[instance],
                             &twi_config,
                             nordic_nrf5_twi_event_handler,
                             obj);
        } else {

            /* Initialze driver in blocking mode. */
            nrf_drv_twi_init(&nordic_nrf5_instance[instance],
                             &twi_config,
                             NULL,
                             NULL);
        }
#else
        /* Initialze driver in blocking mode. */
        nrf_drv_twi_init(&nordic_nrf5_instance[instance],
                         &twi_config,
                         NULL,
                         NULL);
#endif

        /* Enable peripheral. */
        nrf_drv_twi_enable(&nordic_nrf5_instance[instance]);
    }
}

/** Blocking reading data
 *
 *  @param obj     The I2C object
 *  @param address 7-bit address (last bit is 1)
 *  @param data    The buffer for receiving
 *  @param length  Number of bytes to read
 *  @param stop    Stop to be generated after the transfer is done
 *  @return Number of read bytes
 */
int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    DEBUG_PRINTF("i2c_read: %p %d %p %d %d\r\n", obj, address, data, length, stop);

#if DEVICE_I2C_ASYNCH
    struct i2c_s *config = &obj->i2c;
#else
    struct i2c_s *config = obj;
#endif

    int instance = config->instance;
    int result = I2C_ERROR_NO_SLAVE;

    /* Force peripheral configuration to avoid timing errors. */
    config->update = true;
    i2c_configure_driver_instance(obj);

    /* Initialize transaction. */
    ret_code_t retval = nrf_drv_twi_rx(&nordic_nrf5_instance[instance],
                                       address >> 1,
                                       (uint8_t *) data,
                                       length);

    /* Set return value on success. */
    if (retval == NRF_SUCCESS) {
        result = length;
    }

    DEBUG_PRINTF("result: %lu %d\r\n", retval, result);

    return result;
}

/** Blocking sending data
 *
 *  @param obj     The I2C object
 *  @param address 7-bit address (last bit is 0)
 *  @param data    The buffer for sending
 *  @param length  Number of bytes to write
 *  @param stop    Stop to be generated after the transfer is done
 *  @return
 *      zero or non-zero - Number of written bytes
 *      negative - I2C_ERROR_XXX status
 */
int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    DEBUG_PRINTF("i2c_write: %p %d %p %d %d\r\n", obj, address, data, length, stop);

#if DEVICE_I2C_ASYNCH
    struct i2c_s *config = &obj->i2c;
#else
    struct i2c_s *config = obj;
#endif

    int instance = config->instance;
    int result = I2C_ERROR_NO_SLAVE;

    /* Force peripheral configuration to avoid timing errors. */
    config->update = true;
    i2c_configure_driver_instance(obj);

    /* Initialize transaction. */
    ret_code_t retval = nrf_drv_twi_tx(&nordic_nrf5_instance[instance],
                                       address >> 1,
                                       (const uint8_t *) data,
                                       length,
                                       !stop);

    /* Set return value on success. */
    if (retval == NRF_SUCCESS) {
        result = length;
    }

    DEBUG_PRINTF("result: %lu %d\r\n", retval, result);

    return result;
}


#if DEVICE_I2C_ASYNCH

/***
 *                                               _____ _____
 *         /\                              /\   |  __ \_   _|
 *        /  \   ___ _   _ _ __   ___     /  \  | |__) || |
 *       / /\ \ / __| | | | '_ \ / __|   / /\ \ |  ___/ | |
 *      / ____ \\__ \ |_| | | | | (__   / ____ \| |    _| |_
 *     /_/    \_\___/\__, |_| |_|\___| /_/    \_\_|   |_____|
 *                    __/ |
 *                   |___/
 */

/* Callback function for driver calls. This is called from ISR context. */
static void nordic_nrf5_twi_event_handler(nrf_drv_twi_evt_t const *p_event, void *p_context)
{
    // Only safe to use with mbed-printf.
    //DEBUG_PRINTF("nordic_nrf5_twi_event_handler: %d %p\r\n", p_event->type, p_context);

    i2c_t *obj = (i2c_t *) p_context;
    struct i2c_s *config = &obj->i2c;

    /* Translate event type from NRF driver values to mbed HAL values. */
    switch (p_event->type)
    {
        /* Transfer completed event. */
        case NRF_DRV_TWI_EVT_DONE:
            config->event = I2C_EVENT_TRANSFER_COMPLETE;
            break;

        /* Error event: NACK received after sending the address. */
        case NRF_DRV_TWI_EVT_ADDRESS_NACK:
            config->event = I2C_EVENT_ERROR_NO_SLAVE;
            break;

        /* Error event: NACK received after sending a data byte. */
        case NRF_DRV_TWI_EVT_DATA_NACK:
            config->event = I2C_EVENT_TRANSFER_EARLY_NACK;
            break;

        default:
            config->event = I2C_EVENT_ERROR;
            break;
    }

    /* If event matches event mask and event handler is set, signal event. */
    if ((config->event & config->mask) && config->handler) {

        /* Cast handler to function pointer. */
        void (*callback)(void) = (void (*)(void)) config->handler;

        /* Reset handler and force reconfiguration. */
        config->handler = 0;
        config->update = true;

        /* Signal callback handler. */
        callback();
    }
}

/** Start I2C asynchronous transfer
 *
 *  @param obj       The I2C object
 *  @param tx        The transmit buffer
 *  @param tx_length The number of bytes to transmit
 *  @param rx        The receive buffer
 *  @param rx_length The number of bytes to receive
 *  @param address   The address to be set - 7bit or 9bit
 *  @param stop      If true, stop will be generated after the transfer is done
 *  @param handler   The I2C IRQ handler to be set
 *  @param event     Event mask for the transfer. See \ref hal_I2CEvents
 *  @param hint      DMA hint usage
 */
void i2c_transfer_asynch(i2c_t *obj,
                         const void *tx,
                         size_t tx_length,
                         void *rx,
                         size_t rx_length,
                         uint32_t address,
                         uint32_t stop,
                         uint32_t handler,
                         uint32_t mask,
                         DMAUsage hint)
{
    DEBUG_PRINTF("i2c_transfer_asynch\r\n");

    /* TWI only supports 7 bit addresses. */
    MBED_ASSERT(address < 0xFF);

    struct i2c_s *config = &obj->i2c;
    int instance = config->instance;

    /* Save event handler and event mask in global variables so they can be called from interrupt handler. */
    config->handler = handler;
    config->mask = mask;

    /* Clear event flag. */
    config->event = 0;

    /* Configure peripheral. */
    config->update = true;
    i2c_configure_driver_instance(obj);

    /* Configure TWI transfer. */
    const nrf_drv_twi_xfer_desc_t twi_config = NRF_DRV_TWI_XFER_DESC_TXRX(address >> 1,
                                                                          (uint8_t*) tx,
                                                                          tx_length,
                                                                          rx,
                                                                          rx_length);

    uint32_t flags = (stop) ? 0 : NRF_DRV_TWI_FLAG_TX_NO_STOP;

    /* Initiate TWI transfer using NRF driver. */
    ret_code_t result = nrf_drv_twi_xfer(&nordic_nrf5_instance[instance],
                                         &twi_config,
                                         flags);

    /* Signal error if event mask matches and event handler is set. */
    if ((result != NRF_SUCCESS) && (mask & I2C_EVENT_ERROR) && handler) {

        /* Store event value so it can be read back. */
        config->event = I2C_EVENT_ERROR;

        /* Cast handler to function pointer. */
        void (*callback)(void) = (void (*)(void)) handler;

        /* Reset handler and force reconfiguration. */
        config->handler = 0;
        config->update = true;

        /* Signal callback handler. */
        callback();
    }
}

/** The asynchronous IRQ handler
 *
 *  @param obj The I2C object which holds the transfer information
 *  @return Event flags if a transfer termination condition was met, otherwise return 0.
 */
uint32_t i2c_irq_handler_asynch(i2c_t *obj)
{
    DEBUG_PRINTF("i2c_irq_handler_asynch\r\n");

    /* Return latest event. */
    return obj->i2c.event;
}

/** Attempts to determine if the I2C peripheral is already in use
 *
 *  @param obj The I2C object
 *  @return Non-zero if the I2C module is active or zero if it is not
 */
uint8_t i2c_active(i2c_t *obj)
{
    DEBUG_PRINTF("i2c_active\r\n");

    /* Query NRF driver if transaction is in progress. */
    return nrf_drv_twi_is_busy(&nordic_nrf5_instance[obj->i2c.instance]);
}

/** Abort asynchronous transfer
 *
 *  This function does not perform any check - that should happen in upper layers.
 *  @param obj The I2C object
 */
void i2c_abort_asynch(i2c_t *obj)
{
    DEBUG_PRINTF("i2c_abort_asynch\r\n");

    /* Reconfiguration will disable and enable the TWI module. */
    obj->i2c.update = true;
    i2c_configure_driver_instance(obj);
}

#endif // DEVICE_I2C_ASYNCH

#if DEVICE_I2CSLAVE
#warning DEVICE_I2CSLAVE

/***
 *      _____ ___   _____    _____ _
 *     |_   _|__ \ / ____|  / ____| |
 *       | |    ) | |      | (___ | | __ ___   _____
 *       | |   / /| |       \___ \| |/ _` \ \ / / _ \
 *      _| |_ / /_| |____   ____) | | (_| |\ V /  __/
 *     |_____|____|\_____| |_____/|_|\__,_| \_/ \___|
 *
 *
 */

/** Configure I2C as slave or master.
 *  @param obj The I2C object
 *  @param enable_slave Enable i2c hardware so you can receive events with ::i2c_slave_receive
 *  @return non-zero if a value is available
 */
void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
    DEBUG_PRINTF("i2c_slave_mode\r\n");
}

/** Check to see if the I2C slave has been addressed.
 *  @param obj The I2C object
 *  @return The status - 1 - read addresses, 2 - write to all slaves,
 *         3 write addressed, 0 - the slave has not been addressed
 */
int i2c_slave_receive(i2c_t *obj)
{
    DEBUG_PRINTF("i2c_slave_receive\r\n");

    return 0;
}

/** Configure I2C as slave or master.
 *  @param obj The I2C object
 *  @param data    The buffer for receiving
 *  @param length  Number of bytes to read
 *  @return non-zero if a value is available
 */
int i2c_slave_read(i2c_t *obj, char *data, int length)
{
    DEBUG_PRINTF("i2c_slave_read\r\n");

    return 0;
}

/** Configure I2C as slave or master.
 *  @param obj The I2C object
 *  @param data    The buffer for sending
 *  @param length  Number of bytes to write
 *  @return non-zero if a value is available
 */
int i2c_slave_write(i2c_t *obj, const char *data, int length)
{
    DEBUG_PRINTF("i2c_slave_write\r\n");

    return 0;
}

/** Configure I2C address.
 *  @param obj     The I2C object
 *  @param idx     Currently not used
 *  @param address The address to be set
 *  @param mask    Currently not used
 */
void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{
    DEBUG_PRINTF("i2c_slave_address\r\n");
}

#endif // DEVICE_I2CSLAVE

#endif // DEVICE_I2C
