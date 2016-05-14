/***************************************************************************//**
 * @file objects.h
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/

#ifndef MBED_OBJECTS_H
#define MBED_OBJECTS_H

#include "PinNames.h"
#include "PeripheralNames.h"
#include "PortNames.h"
#include "em_i2c.h"
#include "em_dma.h"
#include "em_cmu.h"
#include "dma_api_HAL.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    PinName pin:8;
    PinMode mode:6;
    PinDirection dir:2;
} gpio_t;

#if DEVICE_ANALOGIN
struct analogin_s {
    ADC_TypeDef *adc;
    uint32_t channel;
};
#endif

#if DEVICE_ANALOGOUT
struct dac_s {
    DAC_TypeDef *dac;
    uint32_t channel;
};
#endif

#if DEVICE_I2C
struct i2c_s {
    I2C_TypeDef *i2c;
#if DEVICE_I2C_ASYNCH
    uint32_t events;
    I2C_TransferSeq_TypeDef xfer;
#endif
};
#endif

#if DEVICE_PORTOUT
struct port_s {
    PortName port;
    uint32_t mask;
    PinDirection dir;
};
#endif

#if DEVICE_PWMOUT
struct pwmout_s {
    //Channel on TIMER
    uint32_t channel;
    PinName pin;
};
#endif

#if DEVICE_INTERRUPTIN
struct gpio_irq_s {
    PinName pin:8; // Pin number 4 least significant bits, port number 4 most significant bits
    uint32_t risingEdge:1;
    uint32_t fallingEdge:1;
};
#endif

#if DEVICE_SERIAL
struct serial_s {
    union {
        USART_TypeDef *uart;
        LEUART_TypeDef *leuart;
    } periph;
#ifndef _SILICON_LABS_32B_PLATFORM_2
    uint32_t location;
#else
    uint32_t location_tx;
    uint32_t location_rx;
#endif
    PinName rx_pin;
    PinName tx_pin;
#if DEVICE_SERIAL_ASYNCH
    uint32_t events;
    DMA_OPTIONS_t dmaOptionsTX;
    DMA_OPTIONS_t dmaOptionsRX;
#endif
    uint32_t sleep_blocked;
};
#endif

#if DEVICE_SPI
struct spi_s {
    USART_TypeDef *spi;
    int location;
    uint8_t bits;
    uint8_t master;
#if DEVICE_SPI_ASYNCH
    uint32_t event;
    DMA_OPTIONS_t dmaOptionsTX;
    DMA_OPTIONS_t dmaOptionsRX;
#endif
};
#endif

#if DEVICE_RTC
struct lp_timer_s {
    uint32_t start;
    uint32_t stop;
};
#endif

#if DEVICE_SLEEP
#define NUM_SLEEP_MODES 5
typedef enum {
    EM0 = 0,
    EM1 = 1,
    EM2 = 2,
    EM3 = 3,
    EM4 = 4
} sleepstate_enum;
#endif


#ifdef __cplusplus
}
#endif

#endif
