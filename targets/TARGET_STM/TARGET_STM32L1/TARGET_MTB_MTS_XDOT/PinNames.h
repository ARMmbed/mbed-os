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
    PA_0  = 0x00,
    PA_1  = 0x01,
    PA_2  = 0x02,
    PA_3  = 0x03,
    PA_4  = 0x04,
    PA_5  = 0x05,
    PA_6  = 0x06,
    PA_7  = 0x07,
    PA_8  = 0x08,
    PA_9  = 0x09,
    PA_10 = 0x0A,
    PA_11 = 0x0B,
    PA_12 = 0x0C,
    PA_13 = 0x0D,
    PA_14 = 0x0E,
    PA_15 = 0x0F,

    PB_0  = 0x10,
    PB_1  = 0x11,
    PB_2  = 0x12,
    PB_3  = 0x13,
    PB_4  = 0x14,
    PB_5  = 0x15,
    PB_6  = 0x16,
    PB_7  = 0x17,
    PB_8  = 0x18,
    PB_9  = 0x19,
    PB_10 = 0x1A,
    PB_11 = 0x1B,
    PB_12 = 0x1C,
    PB_13 = 0x1D,
    PB_14 = 0x1E,
    PB_15 = 0x1F,

    PC_0  = 0x20,
    PC_1  = 0x21,
    PC_2  = 0x22,
    PC_3  = 0x23,
    PC_4  = 0x24,
    PC_5  = 0x25,
    PC_6  = 0x26,
    PC_7  = 0x27,
    PC_8  = 0x28,
    PC_9  = 0x29,
    PC_10 = 0x2A,
    PC_11 = 0x2B,
    PC_12 = 0x2C,
    PC_13 = 0x2D,
    PC_14 = 0x2E,
    PC_15 = 0x2F,

    PD_2  = 0x32,

    PH_0  = 0x70,
    PH_1  = 0x71,

    // Not connected
    NC = (int)0xFFFFFFFF,

    //Module pins. Refer device "S000645" guide, version 2.1
    P_1 = NC,
    P_2 = NC,
    P_3 = NC,
    P_4 = NC,
    P_5 = NC,
    P_6 = NC,
    P_7 = NC,
    P_8 = NC,
    P_9 = PB_12,
    P_10 = PB_13,
    P_11 = PB_15,
    P_12 = PB_14,
    P_13 = PA_9, //UART_TX
    P_14 = PA_10, //UART_RX
    P_15 = PA_2,
    P_16 = PA_3,
    P_17 = NC,
    P_18 = NC,
    P_19 = NC,
    P_20 = NC,
    P_21 = NC,
    P_22 = NC,
    P_23 = PB_2,
    P_24 = PB_0,
    P_25 = PA_5,
    P_26 = PA_4,
    P_27 = PB_8,
    P_28 = PB_9,
    P_29 = PA_14,
    P_30 = PA_13,
    P_31 = PA_12, //UART1_RTS
    P_32 = PA_11, //UART1_CTS
    P_33 = NC, //NRST
    P_34 = PA_0,  //WAKE
    P_35 = NC,
    P_36 = NC,
    P_37 = NC,
    P_38 = NC,
    P_39 = NC,
    P_40 = NC,
    P_41 = NC,
    P_42 = NC,
    P_43 = NC,
    P_44 = NC,
    P_45 = NC,
    P_46 = NC,
    P_47 = NC,
    P_48 = NC,
    P_49 = NC,
    P_50 = NC,
    P_51 = NC,
    P_52 = NC,
    P_53 = NC,
    P_54 = NC,
    P_55 = NC,
    P_56 = NC,
    P_57 = NC,
    P_58 = NC,

    // LoRa
    LORA_RESET      = PA_1,
    LORA_MOSI       = PB_5,
    LORA_MISO       = PB_4,
    LORA_SCK        = PB_3,
    LORA_NSS        = PA_15,
    LORA_DIO0       = PA_6,
    LORA_DIO1       = PA_7,
    LORA_DIO2       = PA_8,
    LORA_DIO3       = PB_1,
    LORA_DIO4       = PC_13,

    // Secure Element
    SE_RESET        = PB_7,
    SE_CTRL         = PB_6,
    SE_IO           = PB_10,
    SE_CLK          = PB_11,

    // ADC internal channels
    ADC_TEMP = 0xF0,
    ADC_VREF = 0xF1,

    // GPIOs
    GPIO0           = PA_4,     // analog out capable, analog in capable. Also used as A0 for LCD on MTB
    GPIO1           = PA_5,     // analog out capable, analog in capable. Also used as RESET for LCD on MTB
    GPIO2           = PB_0,     // analog in capable, pwm capable // CS for LCD on MTB
    GPIO3           = PB_2,

    // LED
    LED1            = GPIO3,
    LED2            = GPIO3,

    //Standardized button name
    SW1 = PA_0,
    BUTTON1 = SW1,

    // Wake Pin
    WAKE            = SW1,

    // UART 1
    UART1_TX        = PA_9,
    UART1_RX        = PA_10,
    UART1_CTS       = PA_11,
    UART1_RTS       = PA_12,

    UART_TX         = UART1_TX,
    UART_RX         = UART1_RX,
    UART_CTS        = UART1_CTS,
    UART_RTS        = UART1_RTS,

    //UART 2
    UART2_TX        = PA_2,
    UART2_RX        = PA_3,

    // DAPLink
    USBTX           = MBED_CONF_TARGET_STDIO_UART_TX,
    USBRX           = MBED_CONF_TARGET_STDIO_UART_RX,
    SWDIO           = PA_13,
    SWCLK           = PA_14,

    // SPI
    SPI1_MOSI       = PB_15,
    SPI1_MISO       = PB_14,
    SPI1_SCK        = PB_13,
    SPI1_NSS        = PB_12,

    SPI_MOSI        = SPI1_MOSI,
    SPI_MISO        = SPI1_MISO,
    SPI_SCK         = SPI1_SCK,
    SPI_NSS         = SPI1_NSS,

    // I2C
    I2C0_SCL        = PB_8,
    I2C0_SDA        = PB_9,

    I2C_SCL         = I2C0_SCL,
    I2C_SDA         = I2C0_SDA,

} PinName;

#ifdef __cplusplus
}
#endif

#endif
