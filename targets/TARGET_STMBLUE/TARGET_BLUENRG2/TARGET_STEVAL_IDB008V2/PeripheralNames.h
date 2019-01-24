/* mbed Microcontroller Library
 *******************************************************************************
 * Author: Antonio O.  31/10/2018
 *******************************************************************************
 */
#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ADC_1 = (int)ADC_BASE
} ADCName;

typedef enum {
    UART_1 = (int)UART_BASE,
} UARTName;
/*
#define STDIO_UART_TX  IO_8
#define STDIO_UART_RX  IO_11
#define STDIO_UART     UART_1
*/
typedef enum {
    SPI_1 = (int)SPI_BASE,
} SPIName;

typedef enum {
    I2C_1 = (int)I2C1_BASE,
    I2C_2 = (int)I2C2_BASE
} I2CName;

//typedef enum {
//    PWM_1  = (int)TIM1_BASE,
//    PWM_3  = (int)TIM3_BASE,
//    PWM_14 = (int)TIM14_BASE,
//    PWM_15 = (int)TIM15_BASE,
//    PWM_16 = (int)TIM16_BASE,
//    PWM_17 = (int)TIM17_BASE
//} PWMName;


#ifdef __cplusplus
}
#endif

#endif
