/* mbed Microcontroller Library
 * Copyright (c) 2018-2019 ARM Limited
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
#ifndef MBED_OBJECTS_H
#define MBED_OBJECTS_H

#include "stdbool.h"
#include "cmsis.h"
#include "PortNames.h"
#include "PeripheralNames.h"
#include "PinNames.h"
#include "serial_object.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    unsigned long   baseAddr;
    PinName         pin;
    PinMode         mode;
    PinDirection    dir;
    unsigned long pin_mask;
} gpio_t;

struct gpio_irq_s {
    unsigned long   baseAddr;
    uint32_t        port;
    PinName         pin;
    uint32_t        ch;
    unsigned long   pin_mask;
    unsigned long   irq_offset;
};

struct port_s {
    unsigned long   baseAddr;
    unsigned long   peripheralId;
    PortName        port;
    uint32_t        mask;
};

struct pwmout_s {
    uint32_t        period_us;
    float           duty_percent;
    void           *handle;
    PWMName         pwm;
};

struct serial_s {
    CC3220SF_UART_TypeDef *uart;
    int             index;
    uint32_t        baudRate;        /*!< Baud rate for UART */
    unsigned int    baseAddr;        /*! UART Peripheral's base address */
    unsigned int    intNum;          /*! UART Peripheral's interrupt vector */
    unsigned int    powerMgrId;      /* Determined from base address */
    unsigned long   peripheralId;    /* Value that can be passed to PRCM functions */
    UART_LEN        dataLength;      /* Data length for UART */
    UART_STOP       stopBits;        /* Stop bits for UART */
    UART_PAR        parityType;      /* Parity bit type for UART */
};

struct analogin_s {
    PinName pin;
    unsigned long adc_ch;
};

struct trng_s {
    uint32_t placeholder;
};

struct flash_s {
    uint32_t placeholder;
};

typedef struct spi_clock_config_s {

    //! \param ulSPIClk is the rate of clock supplied to the SPI module.
    uint32_t ulSPIClk;

    //! \param ulBitRate is the desired bit rate.(master mode)
    uint32_t ulBitRate;

    //!
    //! The SPI module can operate in either master or slave mode. The parameter
    //! \e ulMode can be one of the following
    //! -\b SPI_MODE_MASTER
    //! -\b SPI_MODE_SLAVE
    uint32_t ulMode;

    //!
    //! The SPI module supports 4 sub modes based on SPI clock polarity and phase.
    //!
    //! <pre>
    //! Polarity Phase  Sub-Mode
    //!   0       0        0
    //!   0       1        1
    //!   1       0        2
    //!   1       1        3
    //! </pre>

    //! Required sub mode can be select by setting \e ulSubMode parameter to one
    //! of the following
    //! - \b SPI_SUB_MODE_0
    //! - \b SPI_SUB_MODE_1
    //! - \b SPI_SUB_MODE_2
    //! - \b SPI_SUB_MODE_3
    uint32_t ulSubMode;

    //! The parameter \e ulConfig is logical OR of five values: the word length,
    //! active level for chip select, software or hardware controlled chip select,
    //! 3 or 4 pin mode and turbo mode.
    //! mode.
    //!
    //! SPI support 8, 16 and 32 bit word lengths defined by:-
    //! - \b SPI_WL_8
    //! - \b SPI_WL_16
    //! - \b SPI_WL_32
    //!
    //! Active state of Chip Select can be defined by:-
    //! - \b SPI_CS_ACTIVELOW
    //! - \b SPI_CS_ACTIVEHIGH
    //!
    //! SPI chip select can be configured to be controlled either by hardware or
    //! software:-
    //! - \b SPI_SW_CS
    //! - \b SPI_HW_CS
    //!
    //! The module can work in 3 or 4 pin mode defined by:-
    //! - \b SPI_3PIN_MODE
    //! - \b SPI_4PIN_MODE
    //!
    //! Turbo mode can be set on or turned off using:-
    //! - \b SPI_TURBO_MODE_ON
    //! - \b SPI_TURBO_MODE_OFF
    uint32_t ulConfig;
} spi_clock_config_t;

struct spi_s {
    /*! SPI module number */
    uint32_t   instance;

    /*! SPICC32XXDMA Peripheral's base address */
    uint32_t   baseAddr;

    /*! SPI Word lengh */
    uint32_t   word_length;

    /*! SPI clock configuration */
    spi_clock_config_t clock_config;

    /*! Is clock update needed */
    bool clock_update;

    /*! Is CS controlled by GPIO */
    bool cs_control_gpio;

#if DEVICE_SPI_ASYNCH
    uint32_t handler;
    uint32_t mask;
    uint32_t event;
#endif
};
#ifdef __cplusplus
}
#endif

#endif
