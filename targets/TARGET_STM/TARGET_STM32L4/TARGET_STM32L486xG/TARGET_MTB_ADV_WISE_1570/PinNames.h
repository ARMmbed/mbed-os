/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2016, STMicroelectronics
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
#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"
#include "PinNamesTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PA_0            = 0x00,
    PA_1            = 0x01,
    PA_2            = 0x02,
    PA_3            = 0x03,
    PA_4            = 0x04,
    PA_5            = 0x05,
    PA_6            = 0x06,
    PA_7            = 0x07,
    PA_8            = 0x08,
    PA_9            = 0x09,
    PA_10           = 0x0A,
    PA_11           = 0x0B,
    PA_12           = 0x0C,
    PA_13           = 0x0D,
    PA_14           = 0x0E,
    PA_15           = 0x0F,

    PB_0            = 0x10,
    PB_1            = 0x11,
    PB_2            = 0x12,
    PB_3            = 0x13,
    PB_4            = 0x14,
    PB_5            = 0x15,
    PB_6            = 0x16,
    PB_7            = 0x17,
    PB_8            = 0x18,
    PB_9            = 0x19,
    PB_10           = 0x1A,
    PB_11           = 0x1B,
    PB_12           = 0x1C,
    PB_13           = 0x1D,
    PB_14           = 0x1E,
    PB_15           = 0x1F,

    PC_0            = 0x20,
    PC_1            = 0x21,
    PC_2            = 0x22,
    PC_3            = 0x23,
    PC_4            = 0x24,
    PC_5            = 0x25,
    PC_6            = 0x26,
    PC_7            = 0x27,
    PC_8            = 0x28,
    PC_9            = 0x29,
    PC_10           = 0x2A,
    PC_11           = 0x2B,
    PC_12           = 0x2C,
    PC_13           = 0x2D,
    PC_14           = 0x2E,
    PC_15           = 0x2F,

    PD_2            = 0x32,

    PH_0            = 0x70,
    PH_1            = 0x71,

    // Not connected
    NC              = (int)0xFFFFFFFF,


    // MCU pins
    CB_RESET_OUT    = PA_8,
    CB_PWR_ON       = PA_11,

    UART3_TX        = PC_10,
    UART3_RX        = PC_11,
    UART3_RTS       = PB_13,
    UART3_CTS       = PB_14,

    GPIO0           = PB_9,
    GPIO1           = PC_7,
    GPIO2           = PB_15,
    GPIO3           = PB_12,
    GPIO4           = PC_8,
    GPIO5           = PC_13,

    W_DISABLE       = PB_2,

    I2C0_SDA        = PB_7,
    I2C0_SCL        = PB_6,

    SPI_MOSI        = PA_7,
    SPI_MISO        = PA_6,
    SPI_SCK         = PA_5,
    SPI_CS0         = PA_4,
    SPI_CS          = SPI_CS0,

    I2C1_SDA        = PB_11,
    I2C1_SCL        = PB_10,

    PWM0_OUT        = PB_0,
    PWM1_OUT        = PB_1,

    ADC00           = PA_1,
    ADC01           = PC_2,
    ADC02           = PC_4,

    BACKUP          = PA_12,
    RESET_IN        = PC_9,
    WAKE            = PA_0,

    // UARTs
    UART1_TX        = PA_9,
    UART1_RX        = PA_10,

    LPUART1_TX      = PC_1,
    LPUART1_RX      = PC_0,

    UART2_TX        = PA_2,
    UART2_RX        = PA_3,

    // Aliases
    SERIAL_TX       = UART3_TX,
    SERIAL_RX       = UART3_RX,
    SERIAL_CTS      = UART3_CTS,
    SERIAL_RTS      = UART3_RTS,

    UART_TXD        = UART3_TX,
    UART_RXD        = UART3_RX,
    UART_CTS        = UART3_CTS,
    UART_RTS        = UART3_RTS,

    LED1            = GPIO2,    // Green
    LED2            = PWM1_OUT, // Blue
    LED3            = PWM0_OUT, // Red

    ADC_BC95        = PC_5,
    MDMTXD          = LPUART1_TX,
    MDMRXD          = LPUART1_RX,

    USBTX           = UART3_TX,
    USBRX           = UART3_RX,

    USER_BUTTON     = GPIO5,

    // STDIO for console print
#ifdef MBED_CONF_TARGET_STDIO_UART_TX
    STDIO_UART_TX   = MBED_CONF_TARGET_STDIO_UART_TX,
#else
    STDIO_UART_TX   = UART3_TX,
#endif
#ifdef MBED_CONF_TARGET_STDIO_UART_RX
    STDIO_UART_RX   = MBED_CONF_TARGET_STDIO_UART_RX,
#else
    STDIO_UART_RX   = UART3_RX,
#endif

    // Module external pins
    P_1             = NC,
    P_2             = NC,
    P_3             = NC,
    P_4             = NC,
    P_5             = NC,
    P_6             = NC,
    P_7             = NC,
    P_8             = NC,
    P_9             = NC,
    P_10            = NC,
    P_11            = NC,
    P_12            = NC,
    P_13            = NC,
    P_14            = NC,
    P_15            = NC,
    P_16            = CB_RESET_OUT,
    P_17            = NC,
    P_18            = NC,
    P_19            = NC,
    P_20            = CB_PWR_ON,
    P_21            = NC,
    P_22            = UART3_TX,
    P_23            = NC,
    P_24            = NC,
    P_25            = NC,
    P_26            = NC,
    P_27            = NC,
    P_28            = NC,
    P_29            = NC,
    P_30            = NC,
    P_31            = NC,
    P_32            = UART3_RX,
    P_33            = NC,
    P_34            = UART3_RTS,
    P_35            = I2C1_SDA,
    P_36            = UART3_CTS,
    P_37            = I2C1_SCL,
    P_38            = GPIO0,
    P_39            = NC,
    P_40            = GPIO1,
    P_41            = PWM0_OUT,
    P_42            = GPIO2,
    P_43            = PWM1_OUT,
    P_44            = GPIO3,
    P_45            = NC,
    P_46            = GPIO4,
    P_47            = ADC00,
    P_48            = GPIO5,
    P_49            = ADC01,
    P_50            = NC,
    P_51            = NC,
    P_52            = NC,
    P_53            = ADC02,
    P_54            = NC,
    P_55            = NC,
    P_56            = W_DISABLE,
    P_57            = NC,
    P_58            = I2C0_SDA,
    P_59            = NC,
    P_60            = I2C0_SCL,
    P_61            = NC,
    P_62            = SPI_MOSI,
    P_63            = NC,
    P_64            = SPI_MISO,
    P_65            = NC,
    P_66            = SPI_SCK,
    P_67            = BACKUP,
    P_68            = SPI_CS0,
    P_69            = NC,
    P_70            = NC,
    P_71            = RESET_IN,
    P_72            = NC,
    P_73            = WAKE,
    P_74            = NC,
    P_75            = NC,

} PinName;

#ifdef __cplusplus
}
#endif

#endif
