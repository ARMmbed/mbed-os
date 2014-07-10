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

#include "PinNames.h"
#include "PeripheralNames.h"
#include "PortNames.h"

#ifdef __cplusplus
extern "C" {
#endif

#if DEVICE_ANALOGIN
struct analogin_s {
    ADC_TypeDef *adc;
    uint32_t channel;
};
#endif

#if DEVICE_ANALOGOUT
struct dac_s {
    DACName dac;
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
    //The period of the pulse in clock cycles
    uint32_t period_cycles;
    //The width of the pulse in clock cycles
    uint32_t width_cycles;
    //Channel on TIMER
    uint32_t channel;
};
#endif

#if DEVICE_INTERRUPTIN
struct gpio_irq_s {
    uint32_t port;
    PinName pin;
    uint32_t risingEdge;
    uint32_t fallingEdge;
};
#endif

#if DEVICE_SERIAL
struct serial_s {
    USART_TypeDef *uart;
    int index;
    uint32_t clock;
    PinName rx_pin;
    PinName tx_pin;
    IRQn_Type rx_irq;
    IRQn_Type tx_irq;
    uint32_t rx_vector;
    uint32_t tx_vector;
    USART_Enable_TypeDef enable;
    uint32_t baudrate;
    USART_OVS_TypeDef oversampling;
    USART_Databits_TypeDef databits;
    USART_Parity_TypeDef parity;
    USART_Stopbits_TypeDef stopbits;
};
#endif

#if DEVICE_SPI
struct spi_s {
    USART_TypeDef *spi;
    int index;
    int location;
    uint32_t clock;
    uint8_t bits;
    PinName mosi;
    PinName miso;
    PinName clk;
    PinName cs;
    USART_Enable_TypeDef enable;
    uint32_t baudrate;
    USART_Databits_TypeDef databits;
    bool master;
    USART_ClockMode_TypeDef clockMode;
};
#endif

#include "gpio_object.h"

#ifdef __cplusplus
}
#endif

#endif
