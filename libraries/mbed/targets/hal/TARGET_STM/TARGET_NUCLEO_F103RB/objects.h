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
#ifndef MBED_OBJECTS_H
#define MBED_OBJECTS_H

#include "cmsis.h"
#include "PortNames.h"
#include "PeripheralNames.h"
#include "PinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gpio_irq_s {
    uint32_t ch;
    IRQn_Type irq_n;
    uint32_t event; // 0=none, 1=rise, 2=fall, 3=rise+fall
};

struct port_s {
    PortName port;
    uint32_t mask;
    PinDirection direction;  
    __IO uint32_t *reg_in;
    __IO uint32_t *reg_out;
};

struct analogin_s {
    ADCName adc;
    PinName pin;
};

struct serial_s {
    UARTName uart;
    int index; // Used by irq
    uint32_t baudrate;
    uint32_t databits;
    uint32_t stopbits;
    uint32_t parity; 
};

struct spi_s {
    SPIName spi;
    uint32_t bits;
    uint32_t cpol;
    uint32_t cpha;
    uint32_t mode;
    uint32_t nss;
    uint32_t br_presc;
};

struct i2c_s {
    I2CName  i2c;
};

struct pwmout_s {
    PWMName pwm;
    PinName pin;
    uint32_t period;
    uint32_t pulse;
};

#include "gpio_object.h"

#ifdef __cplusplus
}
#endif

#endif
