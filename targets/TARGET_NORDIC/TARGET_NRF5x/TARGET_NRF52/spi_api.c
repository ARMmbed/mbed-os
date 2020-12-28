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

#if DEVICE_SPI

#include "hal/spi_api.h"

#include "object_owners.h"
#include "pinmap_ex.h"
#include "PeripheralPins.h"

#if NRFX_CHECK(NRFX_SPIM_ENABLED)
#include "nrfx_spim.h"
#elif NRFX_CHECK(NRFX_SPI_ENABLED)
#include "nrfx_spi.h"
#endif

#if 0
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif

#if NRFX_CHECK(NRFX_SPIM_ENABLED)
/* Pre-allocate instances and share them globally. */
static const nrfx_spim_t nordic_nrf5_spim_instance[4] = {
    NRFX_SPIM_INSTANCE(0),
    NRFX_SPIM_INSTANCE(1),
    NRFX_SPIM_INSTANCE(2),
    NRFX_SPIM_INSTANCE(3)
};

/* Keep track of which instance has been initialized. */
static bool nordic_nrf5_spi_initialized[4] = { false, false, false, false };

/* Forware declare interrupt handler. */
#if DEVICE_SPI_ASYNCH
static void nordic_nrf5_spi_event_handler(nrfx_spim_evt_t const *p_event, void *p_context);
#endif

#elif NRFX_CHECK(NRFX_SPI_ENABLED)

/* Pre-allocate instances and share them globally. */
static const nrfx_spi_t nordic_nrf5_spi_instance[3] = {
    NRFX_SPI_INSTANCE(0),
    NRFX_SPI_INSTANCE(1),
    NRFX_SPI_INSTANCE(2)
};

/* Keep track of which instance has been initialized. */
static bool nordic_nrf5_spi_initialized[3] = { false, false, false };

/* Forware declare interrupt handler. */
#if DEVICE_SPI_ASYNCH
static void nordic_nrf5_spi_event_handler(nrfx_spi_evt_t const *p_event, void *p_context);
#endif
#endif //NRFX_SPI_ENABLED

/* Forward declaration. These functions are implemented in the driver but not
 * set up in the NVIC due to it being relocated.
 */
void SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler(void);
void SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler(void);
void SPIM2_SPIS2_SPI2_IRQHandler(void);

#if NRFX_CHECK(NRFX_SPIM_ENABLED)

/* Forward declaration. These functions are implemented in the driver but not
 * set up in the NVIC due to it being relocated.
 */
void SPIM3_IRQHandler(void);

#endif // NRFX_SPIM_ENABLED

/**
 * Brief       Reconfigure peripheral.
 *
 *             If the peripheral has changed ownership clear old configuration and
 *             re-initialize the peripheral with the new settings.
 *
 * Parameter   obj           The object
 * Parameter   handler       Optional callback handler.
 * Parameter   force_change  Force change regardless of ownership.
 */
static void spi_configure_driver_instance(spi_t *obj)
{
#if DEVICE_SPI_ASYNCH
    struct spi_s *spi_inst = &obj->spi;
#else
    struct spi_s *spi_inst = obj;
#endif

    int instance = spi_inst->instance;

    /* Get pointer to object of the current owner of the peripheral. */
    void *current_owner = object_owner_spi2c_get(instance);

    /* Check if reconfiguration is actually necessary. */
    if ((obj != current_owner) || spi_inst->update) {

        /* Update applied, reset flag. */
        spi_inst->update = false;

        /* Clean up and uninitialize peripheral if already initialized. */
        if (nordic_nrf5_spi_initialized[instance]) {
#if NRFX_CHECK(NRFX_SPIM_ENABLED)
            nrfx_spim_uninit(&nordic_nrf5_spim_instance[instance]);
#elif NRFX_CHECK(NRFX_SPI_ENABLED)
            nrfx_spi_uninit(&nordic_nrf5_spi_instance[instance]);
#endif
        }

#if DEVICE_SPI_ASYNCH
        /* Set callback handler in asynchronous mode. */
        if (spi_inst->handler) {
#if NRFX_CHECK(NRFX_SPIM_ENABLED)
            nrfx_spim_init(&nordic_nrf5_spim_instance[instance], &(spi_inst->config), nordic_nrf5_spi_event_handler, obj);
#elif NRFX_CHECK(NRFX_SPI_ENABLED)
            nrfx_spi_init(&nordic_nrf5_spi_instance[instance], &(spi_inst->config), nordic_nrf5_spi_event_handler, obj);
#endif
        } else {
#if NRFX_CHECK(NRFX_SPIM_ENABLED)
            nrfx_spim_init(&nordic_nrf5_spim_instance[instance], &(spi_inst->config), NULL, NULL);
#elif NRFX_CHECK(NRFX_SPI_ENABLED)
            nrfx_spi_init(&nordic_nrf5_spi_instance[instance], &(spi_inst->config), NULL, NULL);
#endif
        }
#else
        /* Set callback handler to NULL in synchronous mode. */
#if NRFX_CHECK(NRFX_SPIM_ENABLED)
        nrfx_spim_init(&nordic_nrf5_spim_instance[instance], &(spi_inst->config), NULL, NULL);
#elif NRFX_CHECK(NRFX_SPI_ENABLED)
        nrfx_spi_init(&nordic_nrf5_spim_instance[instance], &(spi_inst->config), NULL, NULL);
#endif

#endif

        /* Mark instance as initialized. */
        nordic_nrf5_spi_initialized[instance] = true;

        /* Claim ownership of peripheral. */
        object_owner_spi2c_set(instance, obj);
    }
}

void spi_get_capabilities(PinName ssel, bool slave, spi_capabilities_t *cap)
{
    if (slave) {
        cap->minimum_frequency = 200000;            // 200 kHz
        cap->maximum_frequency = 2000000;           // 2 MHz
        cap->word_length = 0x00000080;              // 8 bit symbols
        cap->support_slave_mode = false;            // to be determined later based on ssel
        cap->hw_cs_handle = false;                  // irrelevant in slave mode
        cap->slave_delay_between_symbols_ns = 2500; // 2.5 us
        cap->clk_modes = 0x0f;                      // all clock modes
        cap->tx_rx_buffers_equal_length = false;    // rx/tx buffers can have different sizes
#if DEVICE_SPI_ASYNCH
        cap->async_mode = true;
#else
        cap->async_mode = false;
#endif
    } else {
        cap->minimum_frequency = 200000;          // 200 kHz
        cap->maximum_frequency = 2000000;         // 2 MHz
        cap->word_length = 0x00000080;            // 8 bit symbols
        cap->support_slave_mode = false;          // to be determined later based on ssel
        cap->hw_cs_handle = false;                // to be determined later based on ssel
        cap->slave_delay_between_symbols_ns = 0;  // irrelevant in master mode
        cap->clk_modes = 0x0f;                    // all clock modes
        cap->tx_rx_buffers_equal_length = false;  // rx/tx buffers can have different sizes
#if DEVICE_SPI_ASYNCH
        cap->async_mode = true;
#else
        cap->async_mode = false;
#endif
    }

    // check if given ssel pin is in the cs pinmap
    const PinMap *cs_pins = spi_master_cs_pinmap();
    PinName pin = NC;
    while (cs_pins->pin != NC) {
        if (cs_pins->pin == ssel) {
#if DEVICE_SPISLAVE
            cap->support_slave_mode = true;
#endif
            cap->hw_cs_handle = true;
            break;
        }
        cs_pins++;
    }
}

/** Initialize the SPI peripheral
 *
 * Configures the pins used by SPI, sets a default format and frequency, and enables the peripheral
 * Parameter   obj  The SPI object to initialize
 * Parameter   mosi The pin to use for MOSI
 * Parameter   miso The pin to use for MISO
 * Parameter   sclk The pin to use for SCLK
 * Parameter   ssel The pin to use for SSEL
 */
void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
#if DEVICE_SPI_ASYNCH
    struct spi_s *spi_inst = &obj->spi;
#else
    struct spi_s *spi_inst = obj;
#endif

    /* Get instance based on requested pins. */
    spi_inst->instance = pin_instance_spi(mosi, miso, sclk);
#if NRFX_CHECK(NRFX_SPIM_ENABLED)
    MBED_ASSERT(spi_inst->instance < NRFX_SPIM_ENABLED_COUNT);
#elif NRFX_CHECK(NRFX_SPI_ENABLED)
    MBED_ASSERT(spi_inst->instance < NRFX_SPI_ENABLED_COUNT);
#endif

    /* Store chip select separately for manual enabling. */
    spi_inst->cs = ssel;

    /* Store pins except chip select. */
    spi_inst->config.sck_pin        = sclk;
    spi_inst->config.mosi_pin       = mosi;
    spi_inst->config.miso_pin       = miso;
#if NRFX_CHECK(NRFX_SPIM_ENABLED)
    spi_inst->config.ss_pin         = NRFX_SPIM_PIN_NOT_USED;
#elif NRFX_CHECK(NRFX_SPI_ENABLED)
    spi_inst->config.ss_pin         = NRFX_SPI_PIN_NOT_USED;
#endif
    /* Use the default config. */
    spi_inst->config.irq_priority   = SPI_DEFAULT_CONFIG_IRQ_PRIORITY;
    spi_inst->config.orc            = SPI_FILL_CHAR;
#if NRFX_CHECK(NRFX_SPIM_ENABLED)
    spi_inst->config.frequency      = NRF_SPIM_FREQ_4M;
    spi_inst->config.mode           = NRF_SPIM_MODE_0;
    spi_inst->config.bit_order      = NRF_SPIM_BIT_ORDER_MSB_FIRST;
#elif NRFX_CHECK(NRFX_SPI_ENABLED)
    spi_inst->config.frequency      = NRF_SPI_FREQ_4M;
    spi_inst->config.mode           = NRF_SPI_MODE_0;
    spi_inst->config.bit_order      = NRF_SPI_BIT_ORDER_MSB_FIRST;
#endif


#if DEVICE_SPI_ASYNCH
    /* Set default values for asynchronous variables. */
    spi_inst->handler = 0;
    spi_inst->mask = 0;
    spi_inst->event = 0;
#endif

    /* Configure peripheral. This is called on each init to ensure all pins are set correctly
     * according to the SPI mode before calling CS for the first time.
     */
    spi_configure_driver_instance(obj);

    /* Configure GPIO pin if chip select has been set. */
    if (ssel != NC) {
        nrf_gpio_pin_set(ssel);
        nrf_gpio_cfg_output(ssel);
    }

    static bool first_init = true;

    if (first_init) {
        first_init = false;

        /* Register interrupt handlers in driver with the NVIC. */
        NVIC_SetVector(SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn, (uint32_t) SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler);
        NVIC_SetVector(SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn, (uint32_t) SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler);
        NVIC_SetVector(SPIM2_SPIS2_SPI2_IRQn, (uint32_t) SPIM2_SPIS2_SPI2_IRQHandler);
#if NRFX_CHECK(NRFX_SPIM_ENABLED)
        NVIC_SetVector(SPIM3_IRQn, (uint32_t) SPIM3_IRQHandler);
#endif
    }
}

/** Release a SPI object
 *
 * TODO: spi_free is currently unimplemented
 * This will require reference counting at the C++ level to be safe
 *
 * Return the pins owned by the SPI object to their reset state
 * Disable the SPI peripheral
 * Disable the SPI clock
 * Parameter  obj The SPI object to deinitialize
 */
void spi_free(spi_t *obj)
{
#if DEVICE_SPI_ASYNCH
    struct spi_s *spi_inst = &obj->spi;
#else
    struct spi_s *spi_inst = obj;
#endif

    int instance = spi_inst->instance;

    /* Use driver uninit to free instance. */
#if NRFX_CHECK(NRFX_SPIM_ENABLED)
    nrfx_spim_uninit(&nordic_nrf5_spim_instance[instance]);
#elif NRFX_CHECK(NRFX_SPI_ENABLED)
    nrfx_spi_uninit(&nordic_nrf5_spi_instance[instance]);
#endif

    /* Mark instance as uninitialized. */
    nordic_nrf5_spi_initialized[instance] = false;
}

/** Configure the SPI format
 *
 * Set the number of bits per frame, configure clock polarity and phase, shift order and master/slave mode.
 * The default bit order is MSB.
 * Parameter      obj   The SPI object to configure
 * Parameter      bits  The number of bits per frame
 * Parameter      mode  The SPI mode (clock polarity, phase, and shift direction)
 * Parameter      slave Zero for master mode or non-zero for slave mode
 */
void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    /* SPI module only supports 8 bit transfers. */
    MBED_ASSERT(bits == 8);
    /* SPI module doesn't support Mbed HAL Slave API. */
    MBED_ASSERT(slave == 0);

#if DEVICE_SPI_ASYNCH
    struct spi_s *spi_inst = &obj->spi;
#else
    struct spi_s *spi_inst = obj;
#endif

#if NRFX_CHECK(NRFX_SPIM_ENABLED)
    nrf_spim_mode_t new_mode = NRF_SPIM_MODE_0;

    /* Convert Mbed HAL mode to Nordic mode. */
    if (mode == 0) {
        new_mode = NRF_SPIM_MODE_0;
    } else if (mode == 1) {
        new_mode = NRF_SPIM_MODE_1;
    } else if (mode == 2) {
        new_mode = NRF_SPIM_MODE_2;
    } else if (mode == 3) {
        new_mode = NRF_SPIM_MODE_3;
    }
#elif NRFX_CHECK(NRFX_SPI_ENABLED)
    nrf_spi_mode_t new_mode = NRF_SPI_MODE_0;

    /* Convert Mbed HAL mode to Nordic mode. */
    if (mode == 0) {
        new_mode = NRF_SPI_MODE_0;
    } else if (mode == 1) {
        new_mode = NRF_SPI_MODE_1;
    } else if (mode == 2) {
        new_mode = NRF_SPI_MODE_2;
    } else if (mode == 3) {
        new_mode = NRF_SPI_MODE_3;
    }
#endif

    /* Check if configuration has changed. */
    if (spi_inst->config.mode != new_mode) {
        spi_inst->config.mode = new_mode;

        /* Set flag to force update. */
        spi_inst->update = true;
    }

    /* Configure peripheral if necessary. Must be called on each format to ensure the pins are set
     * correctly according to the SPI mode.
     */
    spi_configure_driver_instance(obj);
}

/** Set the SPI baud rate
 *
 * Actual frequency may differ from the desired frequency due to available dividers and bus clock
 * Configures the SPI peripheral's baud rate
 * Parameter      obj The SPI object to configure
 * Parameter      hz  The baud rate in Hz
 */
void spi_frequency(spi_t *obj, int hz)
{
#if DEVICE_SPI_ASYNCH
    struct spi_s *spi_inst = &obj->spi;
#else
    struct spi_s *spi_inst = obj;
#endif

#if NRFX_CHECK(NRFX_SPIM_ENABLED)
    nrf_spim_frequency_t new_frequency = NRF_SPIM_FREQ_1M;

    /* Convert frequency to Nordic enum type. */
    if (hz < 250000) {
        new_frequency = NRF_SPIM_FREQ_125K;
    } else if (hz < 500000) {
        new_frequency = NRF_SPIM_FREQ_250K;
    } else if (hz < 1000000) {
        new_frequency = NRF_SPIM_FREQ_500K;
    } else if (hz < 2000000) {
        new_frequency = NRF_SPIM_FREQ_1M;
    } else if (hz < 4000000) {
        new_frequency = NRF_SPIM_FREQ_2M;
    } else if (hz < 8000000) {
        new_frequency = NRF_SPIM_FREQ_4M;
    } else if (hz < 16000000) {
        new_frequency = NRF_SPIM_FREQ_8M;
    } else if (hz < 32000000) {
        new_frequency = NRF_SPIM_FREQ_16M;
    } else {
        new_frequency = NRF_SPIM_FREQ_32M;
    }
#elif NRFX_CHECK(NRFX_SPI_ENABLED)
    nrf_spi_frequency_t new_frequency = NRF_SPI_FREQ_1M;

    /* Convert frequency to Nordic enum type. */
    if (hz < 250000) {
        new_frequency = NRF_SPI_FREQ_125K;
    } else if (hz < 500000) {
        new_frequency = NRF_SPI_FREQ_250K;
    } else if (hz < 1000000) {
        new_frequency = NRF_SPI_FREQ_500K;
    } else if (hz < 2000000) {
        new_frequency = NRF_SPI_FREQ_1M;
    } else if (hz < 4000000) {
        new_frequency = NRF_SPI_FREQ_2M;
    } else if (hz < 8000000) {
        new_frequency = NRF_SPI_FREQ_4M;
    } else {
        new_frequency = NRF_SPI_FREQ_8M;
    }
#endif
    /* Check if configuration has changed. */
    if (spi_inst->config.frequency != new_frequency) {
        spi_inst->config.frequency = new_frequency;

        /* Set flag to force update. */
        spi_inst->update = true;
    }
}

/** Write a byte out in master mode and receive a value
 *
 * Parameter  obj   The SPI peripheral to use for sending
 * Parameter  value The value to send
 * Return     Returns the value received during send
 */
int spi_master_write(spi_t *obj, int value)
{
    nrfx_err_t ret;

#if DEVICE_SPI_ASYNCH
    struct spi_s *spi_inst = &obj->spi;
#else
    struct spi_s *spi_inst = obj;
#endif

    int instance = spi_inst->instance;

    /* Local variables used in transfer. */
    const uint8_t tx_buff = (uint8_t) value;
    uint8_t rx_buff;

    /* Configure peripheral if necessary. */
    spi_configure_driver_instance(obj);

    /* Manually clear chip select pin if defined. */
    if (spi_inst->cs != NC) {
        nrf_gpio_pin_clear(spi_inst->cs);
    }

    /* Transfer 1 byte. */
#if NRFX_CHECK(NRFX_SPIM_ENABLED)
   nrfx_spim_xfer_desc_t desc = NRFX_SPIM_XFER_TRX(&tx_buff, 1, &rx_buff, 1);
#elif NRFX_CHECK(NRFX_SPI_ENABLED)
   nrfx_spi_xfer_desc_t desc = NRFX_SPI_XFER_TRX(&tx_buff, 1, &rx_buff, 1);
#endif

#if NRFX_CHECK(NRFX_SPIM_ENABLED)
    ret = nrfx_spim_xfer(&nordic_nrf5_spim_instance[instance], &desc, 0);
#elif NRFX_CHECK(NRFX_SPI_ENABLED)
    ret = nrfx_spi_xfer(&nordic_nrf5_spi_instance[instance], &desc, 0);
#endif

    if (ret != NRFX_SUCCESS) {
        DEBUG_PRINTF("%d error returned from nrf_spi_xfer\n\r", ret);
    }

    /* Manually set chip select pin if defined. */
    if (spi_inst->cs != NC) {
        nrf_gpio_pin_set(spi_inst->cs);
    }

    return rx_buff;
}

/** Write a block out in master mode and receive a value
 *
 *  The total number of bytes sent and recieved will be the maximum of
 *  tx_length and rx_length. The bytes written will be padded with the
 *  value 0xff.
 *
 * Parameter  obj        The SPI peripheral to use for sending
 * Parameter  tx_buffer  Pointer to the byte-array of data to write to the device
 * Parameter  tx_length  Number of bytes to write, may be zero
 * Parameter  rx_buffer  Pointer to the byte-array of data to read from the device
 * Parameter  rx_length  Number of bytes to read, may be zero
 * Parameter  write_fill Default data transmitted while performing a read
 * @returns
 *      The number of bytes written and read from the device. This is
 *      maximum of tx_length and rx_length.
 */
int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length, char write_fill)
{
#if DEVICE_SPI_ASYNCH
    struct spi_s *spi_inst = &obj->spi;
#else
    struct spi_s *spi_inst = obj;
#endif

    int instance = spi_inst->instance;

    /* Check if overflow character has changed. */
    if (spi_inst->config.orc != write_fill) {

        /* Store new overflow character and force reconfiguration. */
        spi_inst->update = true;
        spi_inst->config.orc = write_fill;
    }

    /* Configure peripheral if necessary. */
    spi_configure_driver_instance(obj);

    /* Manually clear chip select pin if defined. */
    if (spi_inst->cs != NC) {
        nrf_gpio_pin_clear(spi_inst->cs);
    }

    /* The Nordic SPI driver is only able to transfer 255 bytes at a time.
     * The following code will write/read the data 255 bytes at a time and
     * ensure that asymmetrical transfers are handled properly.
     */
    int tx_offset = 0;
    int rx_offset = 0;

    ret_code_t result = NRFX_SUCCESS;

#if NRFX_CHECK(NRFX_SPIM_ENABLED)
    nrfx_spim_xfer_desc_t desc = NRFX_SPIM_XFER_TRX(tx_buffer, tx_length, rx_buffer, rx_length);
    result = nrfx_spim_xfer(&nordic_nrf5_spim_instance[instance], &desc, 0);
#elif NRFX_CHECK(NRFX_SPI_ENABLED)
    /* Loop until all data is sent and received. */
    while (((tx_length > 0) || (rx_length > 0)) && (result == NRFX_SUCCESS)) {

        /* Check if tx_length is larger than 255 and if so, limit to 255. */
        int tx_actual_length = (tx_length > 255) ? 255 : tx_length;

        /* Set tx buffer pointer. Set to NULL if no data is going to be transmitted. */
        const uint8_t *tx_actual_buffer = (tx_actual_length > 0) ?
                                          (const uint8_t *)(tx_buffer + tx_offset) :
                                          NULL;

        /* Check if rx_length is larger than 255 and if so, limit to 255. */
        int rx_actual_length = (rx_length > 255) ? 255 : rx_length;

        /* Set rx buffer pointer. Set to NULL if no data is going to be received. */
        uint8_t *rx_actual_buffer = (rx_actual_length > 0) ?
                                    (uint8_t *)(rx_buffer + rx_offset) :
                                    NULL;

        /* Blocking transfer. */
        nrfx_spi_xfer_desc_t desc = NRFX_SPI_XFER_TRX(tx_actual_buffer, tx_actual_length, rx_actual_buffer, rx_actual_length);
        result = nrfx_spi_xfer(&nordic_nrf5_spi_instance[instance],
                               &desc, 0);
        /* Update loop variables. */
        tx_length -= tx_actual_length;
        tx_offset += tx_actual_length;

        rx_length -= rx_actual_length;
        rx_offset += rx_actual_length;
    }
#endif

    /* Manually set chip select pin if defined. */
    if (spi_inst->cs != NC) {
        nrf_gpio_pin_set(spi_inst->cs);
    }

    return (rx_offset < tx_offset) ? tx_offset : rx_offset;
}

/** Checks if the specified SPI peripheral is in use
 *
 * Parameter  obj The SPI peripheral to check
 * Return     non-zero if the peripheral is currently transmitting
 */
int spi_busy(spi_t *obj)
{
    /* Legacy API call. Always return zero. */
    return 0;
}

/** Get the module number
 *
 * Parameter  obj The SPI peripheral to check
 * Return     The module number
 */
uint8_t spi_get_module(spi_t *obj)
{
#if DEVICE_SPI_ASYNCH
    struct spi_s *spi_inst = &obj->spi;
#else
    struct spi_s *spi_inst = obj;
#endif

    return spi_inst->instance;
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

#if DEVICE_SPISLAVE

/** Check if a value is available to read
 *
 * Parameter  obj The SPI peripheral to check
 * Return     non-zero if a value is available
 */
int spi_slave_receive(spi_t *obj)
{
    return 0;
}

/** Get a received value out of the SPI receive buffer in slave mode
 *
 * Blocks until a value is available
 * Parameter  obj The SPI peripheral to read
 * Return     The value received
 */
int spi_slave_read(spi_t *obj)
{
    return 0;
}

/** Write a value to the SPI peripheral in slave mode
 *
 * Blocks until the SPI peripheral can be written to
 * Parameter  obj   The SPI peripheral to write
 * Parameter  value The value to write
 */
void spi_slave_write(spi_t *obj, int value)
{
    return;
}

#endif

#if DEVICE_SPI_ASYNCH

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

static ret_code_t spi_master_transfer_async_continue(spi_t *obj)
{
#if NRFX_CHECK(NRFX_SPIM_ENABLED)
    nrfx_spim_xfer_desc_t desc;
#elif NRFX_CHECK(NRFX_SPI_ENABLED)
    nrfx_spi_xfer_desc_t desc;
#endif
    /* Remaining data to be transferred. */
    size_t tx_length = obj->tx_buff.length - obj->tx_buff.pos;
    size_t rx_length = obj->rx_buff.length - obj->rx_buff.pos;

    /* Cap TX length to 255 bytes. */
    if (tx_length > 255) {
        tx_length = 255;
    }

    /* Cap RX length to 255 bytes. */
    if (rx_length > 255) {
        rx_length = 255;
    }

    desc.p_tx_buffer = ((const uint8_t *)(obj->tx_buff.buffer) + obj->tx_buff.pos);
    desc.p_rx_buffer = ((uint8_t *)(obj->rx_buff.buffer) + obj->rx_buff.pos);
    desc.tx_length = tx_length;
    desc.rx_length = rx_length;

#if NRFX_CHECK(NRFX_SPIM_ENABLED)
    ret_code_t result = nrfx_spim_xfer(&nordic_nrf5_spim_instance[obj->spi.instance], &desc, 0);
#elif NRFX_CHECK(NRFX_SPI_ENABLED)
    ret_code_t result = nrfx_spi_xfer(&nordic_nrf5_spi_instance[obj->spi.instance], &desc, 0);
#endif
    return result;
}

/* Callback function for driver calls. This is called from ISR context. */
#if NRFX_CHECK(NRFX_SPIM_ENABLED)
static void nordic_nrf5_spi_event_handler(nrfx_spim_evt_t const *p_event, void *p_context)
#elif NRFX_CHECK(NRFX_SPI_ENABLED)
static void nordic_nrf5_spi_event_handler(nrfx_spi_evt_t const *p_event, void *p_context)
#endif
{
    // Only safe to use with mbed-printf.
    //DEBUG_PRINTF("nordic_nrf5_twi_event_handler: %d %p\r\n", p_event->type, p_context);

    bool signal_complete = false;
    bool signal_error = false;

    spi_t *obj = (spi_t *) p_context;
    struct spi_s *spi_inst = &obj->spi;

#if NRFX_CHECK(NRFX_SPIM_ENABLED)
    if (p_event->type == NRFX_SPIM_EVENT_DONE) {
#elif NRFX_CHECK(NRFX_SPI_ENABLED)
    if (p_event->type == NRFX_SPI_EVENT_DONE) {
#endif

        /* Update buffers with new positions. */
        obj->tx_buff.pos += p_event->xfer_desc.tx_length;
        obj->rx_buff.pos += p_event->xfer_desc.rx_length;

        /* Setup a new transfer if more data is pending. */
        if ((obj->tx_buff.pos < obj->tx_buff.length) || (obj->rx_buff.pos < obj->tx_buff.length)) {

            /* Initiate SPI transfer. */
            ret_code_t result = spi_master_transfer_async_continue(obj);

            /* Abort if transfer wasn't accepted. */
            if (result != NRFX_SUCCESS) {

                /* Signal callback handler that transfer failed. */
                signal_error = true;
            }

        } else {

            /* Signal callback handler that transfer is complete. */
            signal_complete = true;
        }
    } else {

        /* Unexpected event, signal callback handler that transfer failed. */
        signal_error = true;
    }

    /* Transfer complete, signal success if mask is set.*/
    if (signal_complete) {

        /* Signal success if event mask matches and event handler is set. */
        if ((spi_inst->mask & SPI_EVENT_COMPLETE) && spi_inst->handler) {

            /* Cast handler to callback function pointer. */
            void (*callback)(void) = (void (*)(void)) spi_inst->handler;

            /* Reset object. */
            spi_inst->handler = 0;
            spi_inst->update = true;

            /* Store event value so it can be read back. */
            spi_inst->event = SPI_EVENT_COMPLETE;

            /* Signal callback handler. */
            callback();
        }

        /* Transfer failed, signal error if mask is set. */
    } else if (signal_error) {

        /* Signal error if event mask matches and event handler is set. */
        if ((spi_inst->mask & SPI_EVENT_ERROR) && spi_inst->handler) {

            /* Cast handler to callback function pointer. */
            void (*callback)(void) = (void (*)(void)) spi_inst->handler;

            /* Reset object. */
            spi_inst->handler = 0;
            spi_inst->update = true;

            /* Store event value so it can be read back. */
            spi_inst->event = SPI_EVENT_ERROR;

            /* Signal callback handler. */
            callback();
        }
    }

    /* Transfer completed one way or another. Set chip select manually if defined. */
    if (signal_complete || signal_error) {

        if (spi_inst->cs != NC) {
            nrf_gpio_pin_set(spi_inst->cs);
        }
    }
}

/** Begin the SPI transfer. Buffer pointers and lengths are specified in tx_buff and rx_buff
 *
 * Parameter  obj       The SPI object that holds the transfer information
 * Parameter  tx        The transmit buffer
 * Parameter  tx_length The number of bytes to transmit
 * Parameter  rx        The receive buffer
 * Parameter  rx_length The number of bytes to receive
 * Parameter  bit_width The bit width of buffer words
 * Parameter  event     The logical OR of events to be registered
 * Parameter  handler   SPI interrupt handler
 * Parameter  hint      A suggestion for how to use DMA with this transfer
 */
void spi_master_transfer(spi_t *obj,
                         const void *tx,
                         size_t tx_length,
                         void *rx,
                         size_t rx_length,
                         uint8_t bit_width,
                         uint32_t handler,
                         uint32_t mask,
                         DMAUsage hint)
{
    /* SPI peripheral only supports 8 bit transfers. */
    MBED_ASSERT(bit_width == 8);

    /* Setup buffers for transfer. */
    struct buffer_s *buffer_pointer;

    buffer_pointer = &obj->tx_buff;
    buffer_pointer->buffer = (void *) tx;
    buffer_pointer->length = tx_length;
    buffer_pointer->pos    = 0;
    buffer_pointer->width  = 8;

    buffer_pointer = &obj->rx_buff;
    buffer_pointer->buffer = rx;
    buffer_pointer->length = rx_length;
    buffer_pointer->pos    = 0;
    buffer_pointer->width  = 8;

    /* Save event handler and event mask so they can be called from interrupt handler. */
    struct spi_s *spi_inst = &obj->spi;
    spi_inst->handler = handler;
    spi_inst->mask = mask;

    /* Clear event flag. */
    spi_inst->event = 0;

    /* Force reconfiguration. */
    spi_inst->update = true;

    /* Configure peripheral if necessary. */
    spi_configure_driver_instance(obj);

    /* Manually clear chip select pin if defined. */
    if (spi_inst->cs != NC) {
        nrf_gpio_pin_clear(spi_inst->cs);
    }

    /* Initiate SPI transfer. */
    ret_code_t result = spi_master_transfer_async_continue(obj);

    /* Signal error if event mask matches and event handler is set. */
    if ((result != NRFX_SUCCESS) && (mask & SPI_EVENT_ERROR) && handler) {

        /* Cast handler to callback function pointer. */
        void (*callback)(void) = (void (*)(void)) handler;

        /* Reset object. */
        spi_inst->handler = 0;
        spi_inst->update = true;

        /* Store event value so it can be read back. */
        spi_inst->event = SPI_EVENT_ERROR;

        /* Signal callback handler. */
        callback();
    }
}

/** The asynchronous IRQ handler
 *
 * Reads the received values out of the RX FIFO, writes values into the TX FIFO and checks for transfer termination
 * conditions, such as buffer overflows or transfer complete.
 * Parameter  obj     The SPI object that holds the transfer information
 * Return     Event flags if a transfer termination condition was met; otherwise 0.
 */
uint32_t spi_irq_handler_asynch(spi_t *obj)
{
    /* Return latest event. */
    return obj->spi.event;
}

/** Attempts to determine if the SPI peripheral is already in use
 *
 * If a temporary DMA channel has been allocated, peripheral is in use.
 * If a permanent DMA channel has been allocated, check if the DMA channel is in use.  If not, proceed as though no DMA
 * channel were allocated.
 * If no DMA channel is allocated, check whether tx and rx buffers have been assigned.  For each assigned buffer, check
 * if the corresponding buffer position is less than the buffer length.  If buffers do not indicate activity, check if
 * there are any bytes in the FIFOs.
 * Parameter  obj The SPI object to check for activity
 * Return     Non-zero if the SPI port is active or zero if it is not.
 */
uint8_t spi_active(spi_t *obj)
{
    /* Callback handler is non-zero when a transfer is in progress. */
    return (obj->spi.handler != 0);
}

/** Abort an SPI transfer
 *
 * Parameter  obj The SPI peripheral to stop
 */
void spi_abort_asynch(spi_t *obj)
{
    int instance = obj->spi.instance;

    /* Abort transfer. */
#if NRFX_CHECK(NRFX_SPIM_ENABLED)
    nrfx_spim_abort(&nordic_nrf5_spim_instance[instance]);
#elif NRFX_CHECK(NRFX_SPI_ENABLED)
    nrfx_spi_abort(&nordic_nrf5_spi_instance[instance]);
#endif
    /* Force reconfiguration. */
    object_owner_spi2c_set(instance, NULL);
}

#endif // DEVICE_SPI_ASYNCH
#endif // DEVICE_SPI
