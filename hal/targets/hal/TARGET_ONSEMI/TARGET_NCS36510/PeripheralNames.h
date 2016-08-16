/**
 ******************************************************************************
 * @file peripheralnames.h
 * @brief Implements an assertion.
 * @internal
 * @author ON Semiconductor
 * $Rev: 0.1 $
 * $Date: 2015-11-07 $
 ******************************************************************************
 * @copyright (c) 2012 ON Semiconductor. All rights reserved.
 * ON Semiconductor is supplying this software for use with ON Semiconductor
 * processor based microcontrollers only.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @ingroup debug
 */

#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H

#include "cmsis.h"

#include "memory_map.h"  //I think this is needed because enums are using base adresses
#include "PinNames.h"   //this needed?
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ADC_0 = (int)ADCREG_BASE
} ADCName;

typedef enum {
    UART_0 = (int)UART1REG_BASE,
    UART_1 = (int)UART2REG_BASE,
} UARTName;

#define STDIO_UART_TX     USBTX
#define STDIO_UART_RX     USBRX
#define STDIO_UART        UART_0

typedef enum {
    SPI_0 = (int)SPI1REG_BASE,
    SPI_1 = (int)SPI2REG_BASE
} SPIName;

typedef enum {
    I2C_0 = (int)I2C1REG_BASE,
    I2C_1 = (int)I2C2REG_BASE
} I2CName;

typedef enum {
	PWM_0 = (int)PWMREG_BASE,
}PWMName;

#ifdef __cplusplus
}
#endif

#endif
