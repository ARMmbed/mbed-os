/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2014, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */

#ifndef MBED_PERIPHERALPINS_H
#define MBED_PERIPHERALPINS_H

#include "pinmap.h"
#include "PeripheralNames.h"

#if TARGET_STM32F1
#define AFIO_NONE                 0
#define AFIO_REMAP_SPI1_ENABLE    1
#define AFIO_REMAP_I2C1_ENABLE    2
#define AFIO_REMAP_USART1_ENABLE  3
#define AFIO_REMAP_USART2_ENABLE  4
#define AFIO_REMAP_USART3_PARTIAL 5
#define AFIO_REMAP_TIM1_PARTIAL   6
#define AFIO_REMAP_TIM3_PARTIAL   7
#define AFIO_REMAP_TIM2_ENABLE    8
#define AFIO_REMAP_TIM3_ENABLE    9
#define AFIO_REMAP_CAN1_2         10
#define AFIO_REMAP_TIM1_ENABLE    11
#define AFIO_REMAP_USART3_ENABLE  12
#define AFIO_REMAP_CAN1_3         13
#define AFIO_REMAP_TIM2_PARTIAL_1 14
#define AFIO_REMAP_TIM2_PARTIAL_2 15
#define AFIO_REMAP_TIM4_ENABLE    16
#endif


//*** GPIO ***
#if GPIO_PINMAP_READY
/* If this macro is defined, then PinMap_GPIO is present in PeripheralPins.c */
extern const PinMap PinMap_GPIO[];
#endif

//*** ADC ***
#if DEVICE_ANALOGIN
extern const PinMap PinMap_ADC[];
extern const PinMap PinMap_ADC_Internal[];
#endif

//*** DAC ***
#if DEVICE_ANALOGOUT
extern const PinMap PinMap_DAC[];
#endif

//*** I2C ***
#if DEVICE_I2C
extern const PinMap PinMap_I2C_SDA[];
extern const PinMap PinMap_I2C_SCL[];
#endif

//*** PWM ***
#if DEVICE_PWMOUT
extern const PinMap PinMap_PWM[];
#endif

//*** SERIAL ***
#if DEVICE_SERIAL
extern const PinMap PinMap_UART_TX[];
extern const PinMap PinMap_UART_RX[];
#if DEVICE_SERIAL_FC
extern const PinMap PinMap_UART_RTS[];
extern const PinMap PinMap_UART_CTS[];
#endif
#endif

//*** SPI ***
#if DEVICE_SPI
extern const PinMap PinMap_SPI_MOSI[];
extern const PinMap PinMap_SPI_MISO[];
extern const PinMap PinMap_SPI_SCLK[];
extern const PinMap PinMap_SPI_SSEL[];
#endif

//*** CAN ***
#if DEVICE_CAN
extern const PinMap PinMap_CAN_RD[];
extern const PinMap PinMap_CAN_TD[];
#endif

//*** QSPI ***
#if DEVICE_QSPI
extern const PinMap PinMap_QSPI_DATA0[];
extern const PinMap PinMap_QSPI_DATA1[];
extern const PinMap PinMap_QSPI_DATA2[];
extern const PinMap PinMap_QSPI_DATA3[];
extern const PinMap PinMap_QSPI_SCLK[];
extern const PinMap PinMap_QSPI_SSEL[];
#endif

//*** OSPI ***
#if DEVICE_OSPI
extern const PinMap PinMap_OSPI_DATA0[];
extern const PinMap PinMap_OSPI_DATA1[];
extern const PinMap PinMap_OSPI_DATA2[];
extern const PinMap PinMap_OSPI_DATA3[];
extern const PinMap PinMap_OSPI_DATA4[];
extern const PinMap PinMap_OSPI_DATA5[];
extern const PinMap PinMap_OSPI_DATA6[];
extern const PinMap PinMap_OSPI_DATA7[];
extern const PinMap PinMap_OSPI_DQS[];
extern const PinMap PinMap_OSPI_SCLK[];
extern const PinMap PinMap_OSPI_SSEL[];
#endif

//*** USB ***
#define USE_USB_NO_OTG   0
#define USE_USB_OTG_FS   1
#define USE_USB_OTG_HS   2
#define USE_USB_HS_IN_FS 3

#if DEVICE_USBDEVICE
extern const PinMap PinMap_USB_HS[];
extern const PinMap PinMap_USB_FS[];
#endif /* DEVICE_USBDEVICE */

#endif
