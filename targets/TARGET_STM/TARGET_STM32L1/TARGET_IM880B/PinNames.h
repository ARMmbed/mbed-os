/* mbed Microcontroller Library
 *
 * SPDX-License-Identifier: BSD-3-Clause
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

    // Not connected
    NC = (int)0xFFFFFFFF,

    //Module pins. Refer iM880B_Datasheet (https://wireless-solutions.de/products/radiomodules/im880b-l.html)
    P_1 = NC,		// GND
    P_2 = PA_14,	// Digital IO / JTCK / SWCLK 
    P_3 = PA_13,	// Digital IO / JTMS / SWDIO
    P_4 = PB_3,		// Digital IO / JTDO
    P_5 = PA_15,	// Digital IO / JTDI 
    P_6 = NC,		// GND
    P_7 = NC,		// NRST
    P_8 = PA_11,	// Digital IO / USART1-CTS 
    P_9 = PA_12,	// Digital IO / USART1-RTS
    P_10 = NC,		
    P_11 = NC,		// GND	
    P_12 = PB_14,	// Digital IO / SPI2_MISO / ADC_IN20
    P_13 = PB_15,	// Digital IO / SPI2_MOSI / ADC_IN21
    P_14 = PB_13,	// Digital IO / SPI2_CLK / ADC_IN19
    P_15 = PB_12,	// Digital IO / SPI2_NSS / ADC_IN18
    P_16 = NC,		// GND
    P_17 = NC,		// VDD
    P_18 = PA_10,	// Digital IO / USART1-RX 
    P_19 = PA_9,	// Digital IO / USART1-TX 
    P_20 = PA_8,	// Digital IO
    P_21 = PB_8,	// Digital IO / I2C1-SCL
    P_22 = NC,		// GND
    P_23 = PB_9,	// Digital IO / I2C1-SDA
    P_24 = PA_1,	// Digital IO / ADC_IN1
    P_25 = PA_0,	// Digital IO / WKUP1 / ADC_IN0 
    P_26 = NC,		// BOOT0
    P_27 = NC,		// GND
    P_28 = NC,		
    P_29 = PA_3,	// Digital IO / ADC_IN3
    P_30 = NC,		// GND
    P_31 = NC,		// RF_50R
    P_32 = NC,		// GND

    // LoRa
    SPI_RF_RESET = PA_2,
    SPI_RF_MOSI  = PA_7,	// SPI1_MOSI
    SPI_RF_MISO  = PA_6,	// SPI1_MISO
    SPI_RF_SCK   = PA_5,	// SPI1_CLK
    SPI_RF_NSS   = PB_0,
	
    DIO0 = PB_1,		
    DIO1 = PB_10,
    DIO2 = PB_11,
    DIO3 = PB_7,			
    DIO4 = PB_5,		
    DIO5 = PB_4,		
    
    // Antenna tx
    ANT_CTX_PA = PA_4,
    // Antenna rx
    ANT_CRX_RX = PC_13,    

    // ADC internal channels
    ADC_TEMP = 0xF0,
    ADC_VREF = 0xF1,

    // LED
    LED1 = PA_3,
    LED2 = PA_0,
    LED3 = PA_1,   
    LED4 = PA_8,

    //Standardized button name
    SW1     = PB_15,
    BUTTON1 = SW1,

    // Wake Pin
    WAKE = SW1,

    // UART 1
    UART1_TX  = PA_9,
    UART1_RX  = PA_10,
    UART1_CTS = NC,
    UART1_RTS = NC,

    UART_TX  = UART1_TX,
    UART_RX  = UART1_RX,
    UART_CTS = UART1_CTS,
    UART_RTS = UART1_RTS,
    
    STDIO_UART_TX = UART_TX,
    STDIO_UART_RX = UART_RX,

    // DAPLink
    USBTX = UART_TX,
    USBRX = UART_RX,
    SWDIO = NC,
    SWCLK = NC,
	
    // SPI
    SPI2_MOSI = PB_15,
    SPI2_MISO = PB_14,
    SPI2_SCK  = PB_13,
    SPI2_NSS  = PB_12,

    SPI_MOSI  = SPI2_MOSI,
    SPI_MISO  = SPI2_MISO,
    SPI_SCK   = SPI2_SCK,
    SPI_NSS   = SPI2_NSS,

    // I2C
    I2C1_SCL = PB_8,
    I2C1_SDA = PB_9,

    I2C_SCL  = I2C1_SCL,
    I2C_SDA  = I2C1_SDA
} PinName;

#ifdef __cplusplus
}
#endif

#endif
