/***************************************************************************//**
 * @file PeripheralNames.h
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
#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H

#include "em_adc.h"
#include "em_usart.h"
#include "em_i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ADC_0 = ADC0_BASE
} ADCName;

typedef enum {
    I2C_0 = I2C0_BASE,
} I2CName;

typedef enum {
    PWM_CH0 = 0,
    PWM_CH1 = 1,
    PWM_CH2 = 2,
    PWM_CH3 = 3
} PWMName;

typedef enum {
    USART_0 = USART0_BASE,
    USART_1 = USART1_BASE,
    LEUART_0 = LEUART0_BASE,
} UARTName;

#define STDIO_UART_TX     USBTX
#define STDIO_UART_RX     USBRX
#define STDIO_UART        USART0

typedef enum {
    SPI_0 = USART0_BASE,
    SPI_1 = USART1_BASE,
} SPIName;

#ifdef __cplusplus
}
#endif

#endif
