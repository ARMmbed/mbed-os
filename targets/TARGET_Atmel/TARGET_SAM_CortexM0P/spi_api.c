/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

#include "cmsis.h"
#include "pinmap.h"
#include "sercom.h"

#include "pinmap_function.h"

#define 	SERCOM_SPI_STATUS_SYNCBUSY_Pos   15
#define 	SERCOM_SPI_STATUS_SYNCBUSY   (0x1u << SERCOM_SPI_STATUS_SYNCBUSY_Pos)

#define SPI_MOSI_INDEX	0
#define SPI_MISO_INDEX	1
#define SPI_SCLK_INDEX	2
#define SPI_SSEL_INDEX	3

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
#define SPI_DEFAULT_BAUD	100000


/** SPI timeout value. */
#  define SPI_TIMEOUT 10000

extern uint8_t g_sys_init;
uint16_t dummy_fill_word = 0xFFFF;


static inline bool spi_is_syncing(spi_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

#  ifdef FEATURE_SPI_SYNC_SCHEME_VERSION_2
    /* Return synchronization status */
    return (_SPI(obj).SYNCBUSY.reg);
#  else
    /* Return synchronization status */
    return (_SPI(obj).STATUS.reg & SERCOM_SPI_STATUS_SYNCBUSY);
#  endif
}

static inline void spi_enable(spi_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Wait until the synchronization is complete */
    while (spi_is_syncing(obj));

    /* Enable SPI */
    _SPI(obj).CTRLA.reg |= SERCOM_SPI_CTRLA_ENABLE;
}

static inline void spi_disable(spi_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

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
        return false;
    }

    /* Write the character to the DATA register */
    _SPI(obj).DATA.reg = tx_data & SERCOM_SPI_DATA_MASK;

    return true;
}

static inline bool spi_read(spi_t *obj, uint16_t *rx_data)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Check if data is ready to be read */
    if (!spi_is_ready_to_read(obj)) {
        /* No data has been received, return */
        return false;
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

    return true;
}

static uint32_t spi_find_mux_settings(spi_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    uint8_t i_dipo;
    uint8_t i_dopo;
    uint32_t dipo = 0;
    uint32_t dopo = 0;
    uint32_t mux_pad;

    uint32_t mux_settings = 0;

    uint32_t sercom_index = _sercom_get_sercom_inst_index(pSPI_SERCOM(obj));

    if (pSPI_S(obj)->mode == SPI_MODE_MASTER) {
        i_dipo = SPI_MISO_INDEX;
        i_dopo = SPI_MOSI_INDEX;
    } else {
        i_dipo = SPI_MOSI_INDEX;
        i_dopo = SPI_MISO_INDEX;
    }

    /* Find MUX setting */
    if (pSPI_S(obj)->pins[i_dipo] != NC) {
        /* Set Data input MUX padding for master */
        mux_pad = pinmap_pad_sercom(pSPI_S(obj)->pins[i_dipo], sercom_index);
        if (mux_pad != (uint32_t)NC) {
            /* MUX pad value is same as DIPO value */
            dipo = mux_pad;
            mux_settings |= ((dipo << SERCOM_SPI_CTRLA_DIPO_Pos) & SERCOM_SPI_CTRLA_DIPO_Msk);
        }
    }

    if (pSPI_S(obj)->pins[i_dopo] != NC) {
        /* Set Data output MUX padding for master */
        mux_pad = pinmap_pad_sercom(pSPI_S(obj)->pins[i_dopo], sercom_index);
        if (mux_pad != (uint32_t)NC) {
            if (mux_pad != 0) {
                dopo = mux_pad - 1;
            } else {
                if (3 == pinmap_pad_sercom(pSPI_S(obj)->pins[SPI_SCLK_INDEX], sercom_index)) {
                    dopo = 3;
                } else {
                    dopo = 0;
                }
            }
            mux_settings |= ((dopo << SERCOM_SPI_CTRLA_DOPO_Pos) & SERCOM_SPI_CTRLA_DOPO_Msk);
        }
    }

    return mux_settings;
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
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(sclk != NC);

    uint16_t baud = 0;
    uint32_t ctrla = 0;
    uint32_t ctrlb = 0;
    enum status_code error_code;

    if (g_sys_init == 0) {
        system_init();
        g_sys_init = 1;
    }

    /* Calculate SERCOM instance from pins */
    uint32_t sercom_index = pinmap_find_sercom(mosi, miso, sclk, ssel);
    pSPI_SERCOM(obj) = (Sercom*)pinmap_peripheral_sercom(NC, sercom_index);

    /* Disable SPI */
    spi_disable(obj);

    /* Check if reset is in progress. */
    if (_SPI(obj).CTRLA.reg & SERCOM_SPI_CTRLA_SWRST) {
        return;
    }

    uint32_t pm_index, gclk_index;
#if (SAML21)
    if (sercom_index == 5) {
#  ifdef ID_SERCOM5
        pm_index     = MCLK_APBDMASK_SERCOM5_Pos;
        gclk_index   =  SERCOM5_GCLK_ID_CORE;
#  else
        return STATUS_ERR_INVALID_ARG;
#  endif
    } else {
        pm_index     = sercom_index + MCLK_APBCMASK_SERCOM0_Pos;
        gclk_index   = sercom_index + SERCOM0_GCLK_ID_CORE;
    }
#elif (SAMC21)
    if (sercom_index == 5) {
#  ifdef ID_SERCOM5
        pm_index     = MCLK_APBCMASK_SERCOM5_Pos;
        gclk_index   =  SERCOM5_GCLK_ID_CORE;
#  else
        return STATUS_ERR_INVALID_ARG;
#  endif
    } else {
        pm_index     = sercom_index + MCLK_APBCMASK_SERCOM0_Pos;
        gclk_index   = sercom_index + SERCOM0_GCLK_ID_CORE;
    }
#elif (SAMC20)
    pm_index     = sercom_index + MCLK_APBCMASK_SERCOM0_Pos;
    gclk_index   = sercom_index + SERCOM0_GCLK_ID_CORE;
#else
    pm_index     = sercom_index + PM_APBCMASK_SERCOM0_Pos;
    gclk_index   = sercom_index + SERCOM0_GCLK_ID_CORE;
#endif

    /* Turn on module in PM */
#if (SAML21)
    if (sercom_index == 5) {
#  ifdef ID_SERCOM5
        system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBD, 1 << pm_index);
#  else
        return STATUS_ERR_INVALID_ARG;
#  endif
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

    /* Set the SERCOM in SPI master mode */
    _SPI(obj).CTRLA.reg |= SERCOM_SPI_CTRLA_MODE(0x3);
    pSPI_S(obj)->mode = SPI_MODE_MASTER;

    /* TODO: Do pin muxing here */
    struct system_pinmux_config pin_conf;
    system_pinmux_get_config_defaults(&pin_conf);
    pin_conf.direction = SYSTEM_PINMUX_PIN_DIR_INPUT;

    pSPI_S(obj)->pins[SPI_MOSI_INDEX] = mosi;
    pSPI_S(obj)->pins[SPI_MISO_INDEX] = miso;
    pSPI_S(obj)->pins[SPI_SCLK_INDEX] = sclk;
    pSPI_S(obj)->pins[SPI_SSEL_INDEX] = ssel;
    /* Configure the SERCOM pins according to the user configuration */
    for (uint8_t pad = 0; pad < 4; pad++) {
        uint32_t current_pin = pSPI_S(obj)->pins[pad];
        if (current_pin != (uint32_t)NC) {
            pin_conf.mux_position = pinmap_function_sercom((PinName)current_pin, sercom_index);
            if ((uint8_t)NC != pin_conf.mux_position) {
                system_pinmux_pin_set_config(current_pin, &pin_conf);
            }
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

    /* TODO: Find MUX settings */
    ctrla |= spi_find_mux_settings(obj);

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
    PinMode pull_mode;
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Disable SPI */
    spi_disable(obj);


    if (slave) {
        /* Set the SERCOM in SPI mode */
        _SPI(obj).CTRLA.bit.MODE = 0x2;
        pSPI_S(obj)->mode = SPI_MODE_SLAVE;
        pull_mode = PullNone;
        /* Enable PLOADEN to avoid sending dummy character by slave */
        _SPI(obj).CTRLB.bit.PLOADEN = 1;
    } else {
        /* Set the SERCOM in SPI mode */
        _SPI(obj).CTRLA.bit.MODE = 0x3;
        pSPI_S(obj)->mode = SPI_MODE_MASTER;
        pull_mode = PullUp;
    }

    /* Change pull mode of pins */
    for (uint8_t pad = 0; pad < 4; pad++) {
        if (pSPI_S(obj)->pins[pad] != NC) {
            pin_mode(pSPI_S(obj)->pins[pad], pull_mode);
        }
    }

    /* Change MUX settings */
    uint32_t ctrla = _SPI(obj).CTRLA.reg;
    ctrla &= ~(SERCOM_SPI_CTRLA_DIPO_Msk | SERCOM_SPI_CTRLA_DOPO_Msk);
    ctrla |= spi_find_mux_settings(obj);
    _SPI(obj).CTRLA.reg = ctrla;

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
    uint32_t gclk_index = 0;
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Disable SPI */
    spi_disable(obj);

    /* Find frequency of the internal SERCOMi_GCLK_ID_CORE */
    uint32_t sercom_index = _sercom_get_sercom_inst_index(pSPI_SERCOM(obj));
#if (SAML21)
    if (sercom_index == 5) {
#  ifdef ID_SERCOM5
        gclk_index   =  SERCOM5_GCLK_ID_CORE;
#  else
        return STATUS_ERR_INVALID_ARG;
#  endif
    } else {
        gclk_index   = sercom_index + SERCOM0_GCLK_ID_CORE;
    }
#elif (SAMC21)
    if (sercom_index == 5) {
#  ifdef ID_SERCOM5
        gclk_index   =  SERCOM5_GCLK_ID_CORE;
#  else
        return STATUS_ERR_INVALID_ARG;
#  endif
    } else {
        gclk_index   = sercom_index + SERCOM0_GCLK_ID_CORE;
    }
#elif (SAMC20)
    gclk_index   = sercom_index + SERCOM0_GCLK_ID_CORE;
#else
    gclk_index   = sercom_index + SERCOM0_GCLK_ID_CORE;
#endif
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
        if (tx_buffer) {
            data_to_send = tx_buffer[obj->tx_buff.pos];
        } else {
            data_to_send = dummy_fill_word;
        }
        /* Increment 8-bit index */
        obj->tx_buff.pos++;

        if (_SPI(obj).CTRLB.bit.CHSIZE == 1) {
            if (tx_buffer)
                data_to_send |= (tx_buffer[obj->tx_buff.pos] << 8);
            /* Increment 8-bit index */
            obj->tx_buff.pos++;
        }
    } else {
        /* Write a dummy packet */
        /* TODO: Current implementation do not enter this condition, remove if not needed */
        data_to_send = dummy_fill_word;
    }

    /* Write the data to send*/
    _SPI(obj).DATA.reg = data_to_send & SERCOM_SPI_DATA_MASK;

    /* Check for error */
    if ((_SPI(obj).INTFLAG.reg & SERCOM_SPI_INTFLAG_ERROR) && (obj->spi.mask & SPI_EVENT_ERROR)) {
        obj->spi.event |= SPI_EVENT_ERROR;
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
            obj->spi.event |= SPI_EVENT_RX_OVERFLOW;
            return;
        }
    }

    /* Read data, either valid, or dummy */
    uint16_t received_data = (_SPI(obj).DATA.reg & SERCOM_SPI_DATA_MASK);

    /* Do nothing if we are at the end of buffer */
    if ((obj->rx_buff.pos >= obj->rx_buff.length) && rx_buffer) {
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
        obj->spi.event |= SPI_EVENT_ERROR;
    }
}

/**
 * \internal
 * Clears all interrupt flags of SPI
 *
 * \param[in,out]  module  Pointer to SPI software instance struct
 */
static void _spi_clear_interrupts(spi_t *obj)
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
    NVIC_DisableIRQ((IRQn_Type)((uint8_t)SERCOM0_IRQn + sercom_index));
    NVIC_SetVector((IRQn_Type)((uint8_t)SERCOM0_IRQn + sercom_index), (uint32_t)NULL);
}

/**
 * \internal
 * Starts transceive of buffers with a given length
 *
 * \param[in,out]  obj   Pointer to SPI software instance struct
 *
 */
static enum status_code _spi_transceive_buffer(spi_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    uint16_t interrupt_status = _SPI(obj).INTFLAG.reg;
    interrupt_status &= _SPI(obj).INTENSET.reg;

    if (interrupt_status & SERCOM_SPI_INTFLAG_DRE) {
        /* Clear DRE interrupt */
        _SPI(obj).INTENCLR.reg = SERCOM_SPI_INTFLAG_DRE;
        /* Write data */
        _spi_write_async(obj);
        /* Set TXC interrupt */
        _SPI(obj).INTENSET.reg |= SERCOM_SPI_INTFLAG_TXC;
    }
    if (interrupt_status & SERCOM_SPI_INTFLAG_TXC) {
        /* Clear TXC interrupt */
        _SPI(obj).INTENCLR.reg = SERCOM_SPI_INTFLAG_TXC;
        if ((obj->rx_buff.buffer) && (obj->rx_buff.pos < obj->rx_buff.length)) {
            while (!spi_is_ready_to_read(obj));
            _spi_read_async(obj);
            if ((obj->tx_buff.pos >= obj->tx_buff.length) && (obj->tx_buff.length < obj->rx_buff.length)) {
                obj->tx_buff.length = obj->rx_buff.length;
                obj->tx_buff.buffer = 0;
            }
        }
        if (obj->tx_buff.pos < obj->tx_buff.length) {
            /* Set DRE interrupt */
            _SPI(obj).INTENSET.reg |= SERCOM_SPI_INTFLAG_DRE;
        }
    }

    if (obj->spi.event & (SPI_EVENT_ERROR | SPI_EVENT_RX_OVERFLOW) || (interrupt_status & SERCOM_SPI_INTFLAG_ERROR)) {
        /* Clear all interrupts */
        _spi_clear_interrupts(obj);

        if (interrupt_status & SERCOM_SPI_INTFLAG_ERROR) {
            obj->spi.event = STATUS_ERR_BAD_DATA;
        }

        /* Transfer interrupted, invoke the callback function */
        if (obj->spi.event & SPI_EVENT_RX_OVERFLOW) {
            obj->spi.status = STATUS_ERR_OVERFLOW;
        } else {
            obj->spi.status = STATUS_ERR_BAD_DATA;
        }
        return (enum status_code)obj->spi.status;
    }

    if ((obj->tx_buff.pos >= obj->tx_buff.length) && (obj->rx_buff.pos >= obj->rx_buff.length) && (interrupt_status & SERCOM_SPI_INTFLAG_TXC)) {
        /* Clear all interrupts */
        _spi_clear_interrupts(obj);

        /* Transfer complete, invoke the callback function */
        obj->spi.event = SPI_EVENT_INTERNAL_TRANSFER_COMPLETE;
        obj->spi.status = STATUS_OK;
    }

    return (enum status_code)(obj->spi.status);
}

/** Begin the SPI transfer. Buffer pointers and lengths are specified in tx_buff and rx_buff
 *
 * @param[in] obj       The SPI object which holds the transfer information
 * @param[in] tx        The buffer to send
 * @param[in] tx_length The number of words to transmit
 * @param[out]rx        The buffer to receive
 * @param[in] rx_length The number of words to receive
 * @param[in] bit_width The bit width of buffer words
 * @param[in] event     The logical OR of events to be registered
 * @param[in] handler   SPI interrupt handler
 * @param[in] hint      A suggestion for how to use DMA with this transfer **< DMA currently not implemented >**
 */
void spi_master_transfer(spi_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint8_t bit_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
    uint16_t dummy_read;
    (void) dummy_read;
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    uint8_t sercom_index = _sercom_get_sercom_inst_index(obj->spi.spi);

    obj->spi.tx_buffer = (void *)tx;
    obj->tx_buff.buffer =(void *)tx;
    obj->tx_buff.pos = 0;
    if (tx) {
        /* Only two bit rates supported now */
        obj->tx_buff.length = tx_length * ((bit_width > 8)? 2 : 1);
    } else {
        if (rx) {
            obj->tx_buff.length = rx_length * ((bit_width > 8)? 2 : 1);
        } else {
            /* Nothing to transfer */
            return;
        }
    }

    obj->spi.rx_buffer = rx;
    obj->rx_buff.buffer = rx;
    obj->rx_buff.pos = 0;
    if (rx) {
        /* Only two bit rates supported now */
        obj->rx_buff.length = rx_length * ((bit_width > 8)? 2 : 1);
    } else {
        /* Disable RXEN */
        spi_disable(obj);
        _SPI(obj).CTRLB.bit.RXEN = 0;
        spi_enable(obj);
        obj->rx_buff.length = 0;
    }

    /* Clear data buffer if there is anything pending to read */
    while (spi_is_ready_to_read(obj)) {
        dummy_read = _SPI(obj).DATA.reg;
    }

    obj->spi.mask = event;

    obj->spi.dma_usage = hint;

    /*if (hint == DMA_USAGE_NEVER) {** TEMP: Commented as DMA is not implemented now */
    /* Use irq method */
    uint16_t irq_mask = 0;
    obj->spi.status = STATUS_BUSY;

    /* Enable interrupt */
    NVIC_SetVector((IRQn_Type)((uint8_t)SERCOM0_IRQn + sercom_index), handler);
    NVIC_EnableIRQ((IRQn_Type)((uint8_t)SERCOM0_IRQn + sercom_index));

    /* Clear all interrupts */
    _SPI(obj).INTENCLR.reg = SERCOM_SPI_INTFLAG_TXC | SERCOM_SPI_INTFLAG_RXC | SERCOM_SPI_INTFLAG_ERROR;
    _SPI(obj).INTFLAG.reg =  SERCOM_SPI_INTFLAG_TXC | SERCOM_SPI_INTFLAG_ERROR;
    _SPI(obj).STATUS.reg |=  SERCOM_SPI_STATUS_BUFOVF;

    /* Set SPI interrupts */
    /* Set DRE flag to kick start transmission */
    irq_mask |= SERCOM_SPI_INTFLAG_DRE;

    if (event & SPI_EVENT_ERROR) {
        irq_mask |= SERCOM_SPI_INTFLAG_ERROR;
    }
    _SPI(obj).INTENSET.reg = irq_mask;
    /*} ** TEMP: Commented as DMA is not implemented now */
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
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    enum status_code tmp_status;

    uint32_t transfer_event = 0;

    /*if (obj->spi.dma_usage == DMA_USAGE_NEVER) {** TEMP: Commented as DMA is not implemented now */
    /* IRQ method */
    tmp_status = _spi_transceive_buffer(obj);
    if (STATUS_BUSY != tmp_status) {
        if ((obj->spi.event & SPI_EVENT_INTERNAL_TRANSFER_COMPLETE) && (tmp_status == STATUS_OK)) {
            obj->spi.event |= SPI_EVENT_COMPLETE;
        }
        transfer_event = obj->spi.event & (obj->spi.mask | SPI_EVENT_INTERNAL_TRANSFER_COMPLETE);
    }
    /*}** TEMP: Commented as DMA is not implemented now */
    return transfer_event;
}

/** Attempts to determine if the SPI peripheral is already in use.
 * @param[in] obj The SPI object to check for activity
 * @return non-zero if the SPI port is active or zero if it is not.
 */
uint8_t spi_active(spi_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Check if the SPI module is busy with a job */
    return (obj->spi.status == STATUS_BUSY);
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
    NVIC_DisableIRQ((IRQn_Type)((uint8_t)SERCOM0_IRQn + sercom_index));
    NVIC_SetVector((IRQn_Type)((uint8_t)SERCOM0_IRQn + sercom_index), (uint32_t)NULL);

    obj->spi.status = STATUS_ABORTED;
}

#endif /* DEVICE_SPI_ASYNCH */
