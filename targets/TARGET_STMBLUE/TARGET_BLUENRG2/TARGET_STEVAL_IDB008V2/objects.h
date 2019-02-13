/* mbed Microcontroller Library
 *******************************************************************************
 */
#ifndef MBED_OBJECTS_H
#define MBED_OBJECTS_H

#include "BlueNRG1_spi.h"
#include "BlueNRG1_uart.h"
#include "BlueNRG1_gpio.h"
#include "cmsis.h"
#include "PeripheralNames.h"
#include "PinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

#define gpio_t GPIO_InitType

struct gpio_irq_s{
	GPIO_EXTIConfigType exti;
	uint32_t id;
};

struct serial_s{
    UARTName uart;
    uint32_t index_irq; // Used by irq
    UART_InitType init;  //bluenrg struct
    PinName pin_tx;
    PinName pin_rx;
};

/*
struct gpio_irq_s {
    IRQn_Type irq_n;
    uint32_t irq_index;
    uint32_t event;
    PinName pin;
};
*/

struct spi_s{
	SPI_InitType init;
    PinName pin_miso;
    PinName pin_mosi;
    PinName pin_sclk;
    PinName pin_ssel;
    char dummy_char;
};

#ifdef __cplusplus
}
#endif

///* STM32F0 HAL doesn't provide this API called in rtc_api.c */
//#define __HAL_RCC_RTC_CLKPRESCALER(__RTCCLKSource__)
//#define RTC_WKUP_IRQn RTC_IRQn


#endif
