/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "spi_api.h"

#include <math.h>

#include "cmsis.h"
#include "pinmap.h"
#include "sercom.h"

/** Temporary definitions START */
#define EXT1_SPI_MODULE              SERCOM5
#define EXT1_SPI_SERCOM_MUX_SETTING  ((0x1 << SERCOM_SPI_CTRLA_DOPO_Pos) | (0x0 << SERCOM_SPI_CTRLA_DIPO_Pos))
#define EXT1_SPI_SERCOM_PINMUX_PAD0  PINMUX_PB02D_SERCOM5_PAD0
#define EXT1_SPI_SERCOM_PINMUX_PAD1  PINMUX_PB03D_SERCOM5_PAD1
#define EXT1_SPI_SERCOM_PINMUX_PAD2  PINMUX_PB22D_SERCOM5_PAD2
#define EXT1_SPI_SERCOM_PINMUX_PAD3  PINMUX_PB23D_SERCOM5_PAD3
#define EXT1_SPI_SERCOM_DMAC_ID_TX   SERCOM5_DMAC_ID_TX
#define EXT1_SPI_SERCOM_DMAC_ID_RX   SERCOM5_DMAC_ID_RX

/** Default pinmux. */
#  define PINMUX_DEFAULT 0

/** Unused pinmux. */
#  define PINMUX_UNUSED 0xFFFFFFFF
/** Temporary definitions END */

/**
 * \brief SPI modes enum
 *
 * SPI mode selection.
 */
enum spi_mode {
    /** Master mode. */
    SPI_MODE_MASTER         = 1,
    /** Slave mode. */
    SPI_MODE_SLAVE          = 0,
};

#if DEVICE_SPI_ASYNCH
#define pSPI_S(obj)			(&obj->spi)
#define pSPI_SERCOM(obj)	obj->spi.spi
#else
#define pSPI_S(obj)			(obj)
#define pSPI_SERCOM(obj)	(obj->spi)
#endif
#define _SPI(obj)			pSPI_SERCOM(obj)->SPI

/** SPI default baud rate. */
#define SPI_DEFAULT_BAUD	50000//100000


/** SPI timeout value. */
#  define SPI_TIMEOUT 10000

extern uint8_t g_sys_init;

#if DEVICE_SPI_ASYNCH
/* Global variables */
extern void *_sercom_instances[SERCOM_INST_NUM];

static void _spi_transceive_buffer(spi_t *obj);

/** \internal
 * Generates a SERCOM interrupt handler function for a given SERCOM index.
 */
#define _SERCOM_SPI_INTERRUPT_HANDLER(n, unused) \
void SERCOM##n##_SPIHandler(void) \
{ \
	_spi_transceive_buffer((spi_t *)_sercom_instances[n]); \
}
#define _SERCOM_SPI_INTERRUPT_HANDLER_DECLR(n, unused) \
			(uint32_t)SERCOM##n##_SPIHandler,

/** Auto-generate a set of interrupt handlers for each SERCOM SPI in the device */
MREPEAT(SERCOM_INST_NUM, _SERCOM_SPI_INTERRUPT_HANDLER, ~)

const uint32_t _sercom_handlers[SERCOM_INST_NUM] = {
    MREPEAT(SERCOM_INST_NUM, _SERCOM_SPI_INTERRUPT_HANDLER_DECLR, ~)
};
uint32_t _sercom_callbacks[SERCOM_INST_NUM] = {0};
#endif

static inline bool spi_is_syncing(spi_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Return synchronization status */
    return (_SPI(obj).SYNCBUSY.reg);
}

static inline void spi_enable(spi_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

#if DEVICE_SPI_ASYNCH
    /* Enable interrupt */
    NVIC_EnableIRQ(SERCOM0_IRQn + _sercom_get_sercom_inst_index(pSPI_SERCOM(obj)));
#endif

    /* Wait until the synchronization is complete */
    while (spi_is_syncing(obj));

    /* Enable SPI */
    _SPI(obj).CTRLA.reg |= SERCOM_SPI_CTRLA_ENABLE;
}

static inline void spi_disable(spi_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

#if DEVICE_SPI_ASYNCH
    /* Disable interrupt */
    NVIC_DisableIRQ(SERCOM0_IRQn + _sercom_get_sercom_inst_index(pSPI_SERCOM(obj)));
#endif
    /* Wait until the synchronization is complete */
    while (spi_is_syncing(obj));

    /* Disable SPI */
    _SPI(obj).CTRLA.reg &= ~SERCOM_SPI_CTRLA_ENABLE;
}

static inline bool spi_is_write_complete(spi_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Check interrupt flag */
    return (_SPI(obj).INTFLAG.reg & SERCOM_SPI_INTFLAG_TXC);
}

static inline bool spi_is_ready_to_write(spi_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Check interrupt flag */
    return (_SPI(obj).INTFLAG.reg & SERCOM_SPI_INTFLAG_DRE);
}

static inline bool spi_is_ready_to_read(spi_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Check interrupt flag */
    return (_SPI(obj).INTFLAG.reg & SERCOM_SPI_INTFLAG_RXC);
}

static inline bool spi_write(spi_t *obj, uint16_t tx_data)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Check if the data register has been copied to the shift register */
    if (!spi_is_ready_to_write(obj)) {
        /* Data register has not been copied to the shift register, return */
        return 0;
    }

    /* Write the character to the DATA register */
    _SPI(obj).DATA.reg = tx_data & SERCOM_SPI_DATA_MASK;

    return 1;
}

static inline bool spi_read(spi_t *obj, uint16_t *rx_data)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Check if data is ready to be read */
    if (!spi_is_ready_to_read(obj)) {
        /* No data has been received, return */
        return 0;
    }

    /* Check if data is overflown */
    if (_SPI(obj).STATUS.reg & SERCOM_SPI_STATUS_BUFOVF) {
        /* Clear overflow flag */
        _SPI(obj).STATUS.reg |= SERCOM_SPI_STATUS_BUFOVF;
    }

    /* Read the character from the DATA register */
    if (_SPI(obj).CTRLB.bit.CHSIZE == 1) {
        *rx_data = (_SPI(obj).DATA.reg & SERCOM_SPI_DATA_MASK);
    } else {
        *rx_data = (uint8_t)_SPI(obj).DATA.reg;
    }

    return 1;
}

/**
 * \defgroup GeneralSPI SPI Configuration Functions
 * @{
 */

/** Initialize the SPI peripheral
 *
 * Configures the pins used by SPI, sets a default format and frequency, and enables the peripheral
 * @param[out] obj  The SPI object to initialize
 * @param[in]  mosi The pin to use for MOSI
 * @param[in]  miso The pin to use for MISO
 * @param[in]  sclk The pin to use for SCLK
 * @param[in]  ssel The pin to use for SSEL
 */
void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    uint16_t baud = 0;
    uint32_t ctrla = 0;
    uint32_t ctrlb = 0;
    enum status_code error_code;

    if (g_sys_init == 0) {
        system_init();
        g_sys_init = 1;
    }

    // TODO: Calculate SERCOM instance from pins
    // TEMP: Giving our own value for testing
    pSPI_SERCOM(obj) = EXT1_SPI_MODULE;

    /* Disable SPI */
    spi_disable(obj);

    /* Check if reset is in progress. */
    if (_SPI(obj).CTRLA.reg & SERCOM_SPI_CTRLA_SWRST) {
        return;
    }
    uint32_t sercom_index = _sercom_get_sercom_inst_index(pSPI_SERCOM(obj));
    uint32_t pm_index, gclk_index;
#if (SAML21)
    if (sercom_index == 5) {
        pm_index     = MCLK_APBDMASK_SERCOM5_Pos;
        gclk_index   =  SERCOM5_GCLK_ID_CORE;
    } else {
        pm_index     = sercom_index + MCLK_APBCMASK_SERCOM0_Pos;
        gclk_index   = sercom_index + SERCOM0_GCLK_ID_CORE;
    }
#else
    pm_index     = sercom_index + PM_APBCMASK_SERCOM0_Pos;
    gclk_index   = sercom_index + SERCOM0_GCLK_ID_CORE;
#endif

    /* Turn on module in PM */
#if (SAML21)
    if (sercom_index == 5) {
        system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBD, 1 << pm_index);
    } else {
        system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBC, 1 << pm_index);
    }
#else
    system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBC, 1 << pm_index);
#endif

    /* Set up the GCLK for the module */
    struct system_gclk_chan_config gclk_chan_conf;
    system_gclk_chan_get_config_defaults(&gclk_chan_conf);
    gclk_chan_conf.source_generator = GCLK_GENERATOR_0;
    system_gclk_chan_set_config(gclk_index, &gclk_chan_conf);
    system_gclk_chan_enable(gclk_index);
    sercom_set_gclk_generator(GCLK_GENERATOR_0, false);

#if DEVICE_SPI_ASYNCH
    /* Save the object */
    _sercom_instances[sercom_index] = obj;

    /* Configure interrupt handler */
    NVIC_SetVector((SERCOM0_IRQn + sercom_index), (uint32_t)_sercom_handlers[sercom_index]);
#endif

    /* Set the SERCOM in SPI master mode */
    _SPI(obj).CTRLA.reg |= SERCOM_SPI_CTRLA_MODE(0x3);
    pSPI_S(obj)->mode = SPI_MODE_MASTER;

    // TODO: Do pin muxing here
    struct system_pinmux_config pin_conf;
    system_pinmux_get_config_defaults(&pin_conf);
    pin_conf.direction = SYSTEM_PINMUX_PIN_DIR_INPUT;
    //if(config->mode == SPI_MODE_SLAVE) {
    //pin_conf.input_pull = SYSTEM_PINMUX_PIN_PULL_NONE;
    //}

    uint32_t pad_pinmuxes[] = {
        EXT1_SPI_SERCOM_PINMUX_PAD0, EXT1_SPI_SERCOM_PINMUX_PAD1,
        EXT1_SPI_SERCOM_PINMUX_PAD2, EXT1_SPI_SERCOM_PINMUX_PAD3
    };

    /* Configure the SERCOM pins according to the user configuration */
    for (uint8_t pad = 0; pad < 4; pad++) {
        uint32_t current_pinmux = pad_pinmuxes[pad];
        if (current_pinmux != PINMUX_UNUSED) {
            pin_conf.mux_position = current_pinmux & 0xFFFF;
            system_pinmux_pin_set_config(current_pinmux >> 16, &pin_conf);
        }
    }

    /* Get baud value, based on baudrate and the internal clock frequency */
    uint32_t internal_clock = system_gclk_chan_get_hz(gclk_index);
    //internal_clock = 8000000;
    error_code = _sercom_get_sync_baud_val(SPI_DEFAULT_BAUD, internal_clock, &baud);
    if (error_code != STATUS_OK) {
        /* Baud rate calculation error */
        return;
    }
    _SPI(obj).BAUD.reg = (uint8_t)baud;

    /* Set MUX setting */
    ctrla |= EXT1_SPI_SERCOM_MUX_SETTING; // TODO: Change this to appropriate Settings

    /* Set SPI character size */
    ctrlb |= SERCOM_SPI_CTRLB_CHSIZE(0);

    /* Enable receiver */
    ctrlb |= SERCOM_SPI_CTRLB_RXEN;

    /* Write CTRLA register */
    _SPI(obj).CTRLA.reg |= ctrla;

    /* Write CTRLB register */
    _SPI(obj).CTRLB.reg |= ctrlb;

    /* Enable SPI */
    spi_enable(obj);
}

/** Release a SPI object
 *
 * TODO: spi_free is currently unimplemented
 * This will require reference counting at the C++ level to be safe
 *
 * Return the pins owned by the SPI object to their reset state
 * Disable the SPI peripheral
 * Disable the SPI clock
 * @param[in] obj The SPI object to deinitialize
 */
void spi_free(spi_t *obj)
{
    // [TODO]
}

/** Configure the SPI format
 *
 * Set the number of bits per frame, configure clock polarity and phase, shift order and master/slave mode
 * @param[in,out] obj   The SPI object to configure
 * @param[in]     bits  The number of bits per frame
 * @param[in]     mode  The SPI mode (clock polarity, phase, and shift direction)
 * @param[in]     slave Zero for master mode or non-zero for slave mode
 */
void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    /* Disable SPI */
    spi_disable(obj);

    if (slave) {
        /* Set the SERCOM in SPI mode */
        _SPI(obj).CTRLA.bit.MODE = 0x2;
        pSPI_S(obj)->mode = SPI_MODE_SLAVE;

        struct system_pinmux_config pin_conf;
        system_pinmux_get_config_defaults(&pin_conf);
        pin_conf.direction = SYSTEM_PINMUX_PIN_DIR_INPUT;
        pin_conf.input_pull = SYSTEM_PINMUX_PIN_PULL_NONE;

        uint32_t pad_pinmuxes[] = {
            EXT1_SPI_SERCOM_PINMUX_PAD0, EXT1_SPI_SERCOM_PINMUX_PAD1,
            EXT1_SPI_SERCOM_PINMUX_PAD2, EXT1_SPI_SERCOM_PINMUX_PAD3
        };

        /* Configure the SERCOM pins according to the user configuration */
        for (uint8_t pad = 0; pad < 4; pad++) {
            uint32_t current_pinmux = pad_pinmuxes[pad];
            if (current_pinmux != PINMUX_UNUSED) {
                pin_conf.mux_position = current_pinmux & 0xFFFF;
                system_pinmux_pin_set_config(current_pinmux >> 16, &pin_conf);
            }
        }
    } else {
        /* Already in SPI master mode */
    }

    /* Set SPI Frame size - only 8-bit and 9-bit supported now */
    _SPI(obj).CTRLB.bit.CHSIZE = (bits > 8)? 1 : 0;

    /* Set SPI Clock Phase */
    _SPI(obj).CTRLA.bit.CPHA = (mode & 0x01)? 1 : 0;

    /* Set SPI Clock Polarity */
    _SPI(obj).CTRLA.bit.CPOL = (mode & 0x02)? 1 : 0;

    /* Enable SPI */
    spi_enable(obj);
}

/** Set the SPI baud rate
 *
 * Actual frequency may differ from the desired frequency due to available dividers and bus clock
 * Configures the SPI peripheral's baud rate
 * @param[in,out] obj The SPI object to configure
 * @param[in]     hz  The baud rate in Hz
 */
void spi_frequency(spi_t *obj, int hz)
{
    uint16_t baud = 0;

    /* Disable SPI */
    spi_disable(obj);

    /* Find frequency of the internal SERCOMi_GCLK_ID_CORE */
    uint32_t sercom_index = _sercom_get_sercom_inst_index(pSPI_SERCOM(obj));
    uint32_t gclk_index   = sercom_index + SERCOM0_GCLK_ID_CORE;
    uint32_t internal_clock = system_gclk_chan_get_hz(gclk_index);

    /* Get baud value, based on baudrate and the internal clock frequency */
    enum status_code error_code = _sercom_get_sync_baud_val(hz, internal_clock, &baud);

    if (error_code != STATUS_OK) {
        /* Baud rate calculation error, return status code */
        /* Enable SPI */
        spi_enable(obj);
        return;
    }

    _SPI(obj).BAUD.reg = (uint8_t)baud;

    /* Enable SPI */
    spi_enable(obj);
}

/**@}*/
/**
 * \defgroup SynchSPI Synchronous SPI Hardware Abstraction Layer
 * @{
 */

/** Write a byte out in master mode and receive a value
 *
 * @param[in] obj   The SPI peripheral to use for sending
 * @param[in] value The value to send
 * @return Returns the value received during send
 */
int spi_master_write(spi_t *obj, int value)
{
    uint16_t rx_data = 0;

    /* Sanity check arguments */
    MBED_ASSERT(obj);

#if DEVICE_SPI_ASYNCH
    if (obj->spi.status == STATUS_BUSY) {
        /* Check if the SPI module is busy with a job */
        return 0;
    }
#endif

    /* Wait until the module is ready to write the character */
    while (!spi_is_ready_to_write(obj));

    /* Write data */
    spi_write(obj, value);

    if (!(_SPI(obj).CTRLB.bit.RXEN)) {
        return 0;
    }

    /* Wait until the module is ready to read the character */
    while (!spi_is_ready_to_read(obj));

    /* Read data */
    spi_read(obj, &rx_data);

    return rx_data;
}

/** Check if a value is available to read
 *
 * @param[in] obj The SPI peripheral to check
 * @return non-zero if a value is available
 */
int spi_slave_receive(spi_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    return spi_is_ready_to_read(obj);
}

/** Get a received value out of the SPI receive buffer in slave mode
 *
 * Blocks until a value is available
 * @param[in] obj The SPI peripheral to read
 * @return The value received
 */
int spi_slave_read(spi_t *obj)
{
    int i;
    uint16_t rx_data = 0;

    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Check for timeout period */
    for (i = 0; i < SPI_TIMEOUT; i++) {
        if (spi_is_ready_to_read(obj)) {
            break;
        }
    }
    if (i == SPI_TIMEOUT) {
        /* Not ready to read data within timeout period */
        return 0;
    }

    /* Read data */
    spi_read(obj, &rx_data);

    return rx_data;
}

/** Write a value to the SPI peripheral in slave mode
 *
 * Blocks until the SPI peripheral can be written to
 * @param[in] obj   The SPI peripheral to write
 * @param[in] value The value to write
 */
void spi_slave_write(spi_t *obj, int value)
{
    int i;

    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Check for timeout period */
    for (i = 0; i < SPI_TIMEOUT; i++) {
        if (spi_is_ready_to_write(obj)) {
            break;
        }
    }
    if (i == SPI_TIMEOUT) {
        /* Not ready to write data within timeout period */
        return;
    }

    /* Write data */
    spi_write(obj, value);
}

/** Checks if the specified SPI peripheral is in use
 *
 * @param[in] obj The SPI peripheral to check
 * @return non-zero if the peripheral is currently transmitting
 */
int spi_busy(spi_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    return spi_is_write_complete(obj);
}

/** Get the module number
 *
 * @param[in] obj The SPI peripheral to check
 * @return The module number
 */
uint8_t spi_get_module(spi_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    return _sercom_get_sercom_inst_index(pSPI_SERCOM(obj));
}


#if DEVICE_SPI_ASYNCH
/**
 * \defgroup AsynchSPI Asynchronous SPI Hardware Abstraction Layer
 * @{
 */


/**
 * \internal
 * Writes a character from the TX buffer to the Data register.
 *
 * \param[in,out]  module  Pointer to SPI software instance struct
 */
static void _spi_write_async(spi_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    uint16_t data_to_send;
    uint8_t *tx_buffer = obj->tx_buff.buffer;

    /* Do nothing if we are at the end of buffer */
    if (obj->tx_buff.pos < obj->tx_buff.length) {
        /* Write value will be at least 8-bits long */
        data_to_send = tx_buffer[obj->tx_buff.pos];
        /* Increment 8-bit index */
        obj->tx_buff.pos++;

        if (_SPI(obj).CTRLB.bit.CHSIZE == 1) {
            data_to_send |= (tx_buffer[obj->tx_buff.pos] << 8);
            /* Increment 8-bit index */
            obj->tx_buff.pos++;
        }
    } else {
        /* Write a dummy packet */
        data_to_send = ~0;
    }

    /* Write the data to send*/
    _SPI(obj).DATA.reg = data_to_send & SERCOM_SPI_DATA_MASK;

    /* Check for error */
    if ((_SPI(obj).INTFLAG.reg & SERCOM_SPI_INTFLAG_ERROR) && (obj->spi.mask & SPI_EVENT_ERROR)) {
        obj->spi.event != SPI_EVENT_ERROR;
    }
}

/**
 * \internal
 * Reads a character from the Data register to the RX buffer.
 *
 * \param[in,out]  module  Pointer to SPI software instance struct
 */
static void _spi_read_async(spi_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    uint8_t *rx_buffer = obj->rx_buff.buffer;

    /* Check if data is overflown */
    if (_SPI(obj).STATUS.reg & SERCOM_SPI_STATUS_BUFOVF) {
        /* Clear overflow flag */
        _SPI(obj).STATUS.reg |= SERCOM_SPI_STATUS_BUFOVF;
        if (obj->spi.mask & SPI_EVENT_RX_OVERFLOW) {
            /* Set overflow error */
            obj->spi.event != SPI_EVENT_RX_OVERFLOW;
            return;
        }
    }

    /* Read data, either valid, or dummy */
    uint16_t received_data = (_SPI(obj).DATA.reg & SERCOM_SPI_DATA_MASK);

    /* Do nothing if we are at the end of buffer */
    if (obj->rx_buff.pos >= obj->rx_buff.length) {
        return;
    }

    /* Read value will be at least 8-bits long */
    rx_buffer[obj->rx_buff.pos] = received_data;
    /* Increment 8-bit index */
    obj->rx_buff.pos++;

    if (_SPI(obj).CTRLB.bit.CHSIZE == 1) {
        /* 9-bit data, write next received byte to the buffer */
        rx_buffer[obj->rx_buff.pos] = (received_data >> 8);
        /* Increment 8-bit index */
        obj->rx_buff.pos++;
    }

    /* Check for error */
    if ((_SPI(obj).INTFLAG.reg & SERCOM_SPI_INTFLAG_ERROR) && (obj->spi.mask & SPI_EVENT_ERROR)) {
        obj->spi.event != SPI_EVENT_ERROR;
    }
}

/**
 * \internal
 * Starts transceive of buffers with a given length
 *
 * \param[in]  obj   Pointer to SPI software instance struct
 *
 */
static void _spi_transceive_buffer(spi_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    void (*callback_func)(void);


    uint8_t sercom_index = _sercom_get_sercom_inst_index(obj->spi.spi);

    uint16_t interrupt_status = _SPI(obj).INTFLAG.reg;
    interrupt_status &= _SPI(obj).INTENSET.reg;

    if (interrupt_status & SERCOM_SPI_INTFLAG_DRE) {
        _spi_write_async(obj);
    }

    if (interrupt_status & SERCOM_SPI_INTFLAG_RXC) {
        _spi_read_async(obj);
    }

    if (obj->spi.event & (SPI_EVENT_ERROR | SPI_EVENT_RX_OVERFLOW)) {
        /* Disable all interrupts */
        _SPI(obj).INTENCLR.reg =
            SERCOM_SPI_INTFLAG_DRE |
            SERCOM_SPI_INTFLAG_TXC |
            SERCOM_SPI_INTFLAG_RXC |
            SERCOM_SPI_INTFLAG_ERROR;
        NVIC_DisableIRQ(SERCOM0_IRQn + sercom_index);

        /* Transfer complete, invoke the callback function */
        if (obj->spi.event & SPI_EVENT_RX_OVERFLOW) {
            obj->spi.status = STATUS_ERR_OVERFLOW;
        } else {
            obj->spi.status = STATUS_ERR_BAD_DATA;
        }
        callback_func = _sercom_callbacks[sercom_index];
        if (callback_func && (obj->spi.mask & (SPI_EVENT_ERROR | SPI_EVENT_RX_OVERFLOW))) {
            callback_func();
        }
        obj->spi.status = STATUS_OK;
        return;
    }

    //if (interrupt_status & (SERCOM_SPI_INTFLAG_TXC | SERCOM_SPI_INTFLAG_RXC)) {
    if ((obj->tx_buff.pos >= obj->tx_buff.length) && (obj->rx_buff.pos >= obj->rx_buff.length)) {
        /* Clear all interrupts */
        _SPI(obj).INTENCLR.reg =
            SERCOM_SPI_INTFLAG_DRE |
            SERCOM_SPI_INTFLAG_TXC |
            SERCOM_SPI_INTFLAG_RXC |
            SERCOM_SPI_INTFLAG_ERROR;
        NVIC_DisableIRQ(SERCOM0_IRQn + sercom_index);
        NVIC_SetVector((SERCOM0_IRQn + sercom_index), (uint32_t)NULL);

        /* Transfer complete, invoke the callback function */
        obj->spi.event |= SPI_EVENT_COMPLETE;
        callback_func = _sercom_callbacks[sercom_index];
        if (callback_func && (obj->spi.mask & SPI_EVENT_COMPLETE)) {
            callback_func();
        }
        obj->spi.status = STATUS_OK;
        return;
    }
    //}
}

/** Begin the SPI transfer. Buffer pointers and lengths are specified in tx_buff and rx_buff
 *
 * @param[in] obj       The SPI object which holds the transfer information
 * @param[in] tx        The buffer to send
 * @param[in] tx_length The number of words to transmit
 * @param[in] rx        The buffer to receive
 * @param[in] rx_length The number of words to receive
 * @param[in] bit_width The bit width of buffer words
 * @param[in] event     The logical OR of events to be registered
 * @param[in] handler   SPI interrupt handler
 * @param[in] hint      A suggestion for how to use DMA with this transfer **< DMA currently not implemented >**
 */
void spi_master_transfer(spi_t *obj, void *tx, size_t tx_length, void *rx, size_t rx_length, uint8_t bit_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    uint8_t sercom_index = _sercom_get_sercom_inst_index(obj->spi.spi);

    obj->spi.tx_buffer = tx;
    obj->tx_buff.buffer = tx;
    obj->tx_buff.pos = 0;
    obj->tx_buff.length = tx_length * (bit_width>>3);

    obj->spi.rx_buffer = rx;
    obj->rx_buff.buffer = rx;
    obj->rx_buff.pos = 0;
    obj->rx_buff.length = rx_length * (bit_width>>3);

    _sercom_callbacks[sercom_index] = handler;
    obj->spi.mask = event;

    obj->spi.dma_usage = hint;

    if (hint == DMA_USAGE_NEVER) {
        /* Use irq method */
        uint16_t irq_mask = 0;
        obj->spi.status = STATUS_BUSY;

        /* Enable interrupt */
        NVIC_SetVector((SERCOM0_IRQn + sercom_index), _sercom_handlers[sercom_index]);
        NVIC_EnableIRQ(SERCOM0_IRQn + sercom_index);

        /* Clear all interrupts */
        _SPI(obj).INTENCLR.reg = SERCOM_SPI_INTFLAG_TXC | SERCOM_SPI_INTFLAG_RXC | SERCOM_SPI_INTFLAG_ERROR;
        _SPI(obj).INTFLAG.reg =  SERCOM_SPI_INTFLAG_TXC | SERCOM_SPI_INTFLAG_ERROR;
        _SPI(obj).STATUS.reg |=  SERCOM_SPI_STATUS_BUFOVF;

        /* Set SPI interrupts */
        if (tx) {
            irq_mask |= SERCOM_SPI_INTFLAG_DRE;
        }
        if (rx) {
            irq_mask |= SERCOM_SPI_INTFLAG_RXC;
        }
        if (event & SPI_EVENT_ERROR) {
            irq_mask |= SERCOM_SPI_INTFLAG_ERROR;
        }
        _SPI(obj).INTENSET.reg = irq_mask;
    }
}

/** The asynchronous IRQ handler
 *
 * Reads the received values out of the RX FIFO, writes values into the TX FIFO and checks for transfer termination
 * conditions, such as buffer overflows or transfer complete.
 * @param[in] obj     The SPI object which holds the transfer information
 * @return event flags if a transfer termination condition was met or 0 otherwise.
 */
uint32_t spi_irq_handler_asynch(spi_t *obj)
{
    uint32_t transfer_event = 0;
    uint32_t bytes_to_transfer = 0;

    uint8_t sercom_index = _sercom_get_sercom_inst_index(obj->spi.spi);

    if (obj->spi.dma_usage == DMA_USAGE_NEVER) {
        /* IRQ method */
        obj->spi.status = STATUS_BUSY;
        if ((obj->tx_buff.pos < obj->tx_buff.length) || (obj->rx_buff.pos < obj->rx_buff.length)) {
            bytes_to_transfer = (obj->tx_buff.length > obj->rx_buff.length)? obj->tx_buff.length : obj->rx_buff.length;
            while (bytes_to_transfer) {
                if (spi_is_ready_to_write(obj)) {
                    _spi_write_async(obj);
                }
                if (spi_is_ready_to_read(obj)) {
                    _spi_read_async(obj);
                }
                if (obj->spi.event & (SPI_EVENT_ERROR | SPI_EVENT_RX_OVERFLOW)) {
                    transfer_event = obj->spi.event;
                    obj->spi.status = (obj->spi.event & SPI_EVENT_RX_OVERFLOW)? STATUS_ERR_OVERFLOW : STATUS_ERR_BAD_DATA;
                    break;
                }
                bytes_to_transfer--;
            }
        }
        if (bytes_to_transfer == 0) {
            transfer_event |= SPI_EVENT_COMPLETE;
            /* Clear all interrupts */
            _SPI(obj).INTENCLR.reg =
                SERCOM_SPI_INTFLAG_DRE |
                SERCOM_SPI_INTFLAG_TXC |
                SERCOM_SPI_INTFLAG_RXC |
                SERCOM_SPI_INTFLAG_ERROR;
            NVIC_DisableIRQ(SERCOM0_IRQn + sercom_index);
            NVIC_SetVector((SERCOM0_IRQn + sercom_index), (uint32_t)NULL);
            obj->spi.status = STATUS_OK;
        }
        transfer_event &= obj->spi.mask;
    }

    return transfer_event;
}

/** Attempts to determine if the SPI peripheral is already in use.
 * @param[in] obj The SPI object to check for activity
 * @return non-zero if the SPI port is active or zero if it is not.
 */
uint8_t spi_active(spi_t *obj)
{
    if (obj->spi.status == STATUS_BUSY) {
        /* Check if the SPI module is busy with a job */
        return 1;
    } else {
        return 0;
    }
}

/** Abort an SPI transfer
 *
 * @param obj The SPI peripheral to stop
 */
void spi_abort_asynch(spi_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    uint8_t sercom_index = _sercom_get_sercom_inst_index(obj->spi.spi);

    /* Clear all interrupts */
    _SPI(obj).INTENCLR.reg =
        SERCOM_SPI_INTFLAG_DRE |
        SERCOM_SPI_INTFLAG_TXC |
        SERCOM_SPI_INTFLAG_RXC |
        SERCOM_SPI_INTFLAG_ERROR;

    // TODO: Disable and remove irq handler
    NVIC_DisableIRQ(SERCOM0_IRQn + sercom_index);
    NVIC_SetVector((SERCOM0_IRQn + sercom_index), (uint32_t)NULL);

    obj->spi.status = STATUS_ABORTED;
}

#endif
