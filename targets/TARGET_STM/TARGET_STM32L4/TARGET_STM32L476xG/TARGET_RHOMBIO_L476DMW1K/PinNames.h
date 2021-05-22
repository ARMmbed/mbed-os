/* mbed Microcontroller Library
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *******************************************************************************
 * Copyright (c) 2018, STMicroelectronics
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

/* MBED TARGET LIST: RHOMBIO_L476DMW1K */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"
#include "PinNamesTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ALT0  = 0x100,
    ALT1  = 0x200,
    ALT2  = 0x300,
} ALTx;

typedef enum {
    PA_0  = 0x00, 
    PA_0_ALT0 = PA_0 | ALT0,
    PA_1  = 0x01, 
    PA_1_ALT0 = PA_1 | ALT0,
    PA_2  = 0x02, 
    PA_2_ALT0 = PA_2 | ALT0,
    PA_3  = 0x03, 
    PA_3_ALT0 = PA_3 | ALT0,
    PA_4  = 0x04,
    PA_4_ALT0 = PA_4 | ALT0,
    PA_5  = 0x05, 
    PA_5_ALT0 = PA_5 | ALT0,
    PA_6  = 0x06,
    PA_6_ALT0 = PA_6 | ALT0,
    PA_7  = 0x07,
    PA_7_ALT0 = PA_7 | ALT0,
    PA_7_ALT1 = PA_7 | ALT1,
    PA_7_ALT2 = PA_7 | ALT2,
    PA_8  = 0x08,
    PA_9  = 0x09,
    PA_10 = 0x0A,
    PA_11 = 0x0B,
    PA_12 = 0x0C,
    PA_13 = 0x0D,
    PA_14 = 0x0E,
    PA_15 = 0x0F,
    PA_15_ALT0 = PA_15 | ALT0,

    PB_0  = 0x10,
    PB_0_ALT0 = PB_0 | ALT0,
    PB_0_ALT1 = PB_0 | ALT1,
    PB_1  = 0x11,
    PB_1_ALT0 = PB_1 | ALT0,
    PB_1_ALT1 = PB_1 | ALT1,
    PB_2  = 0x12, 
    PB_3  = 0x13, 
    PB_3_ALT0 = PB_3 | ALT0,
    PB_4  = 0x14,
    PB_4_ALT0 = PB_4 | ALT0,
    PB_5  = 0x15,
    PB_5_ALT0 = PB_5 | ALT0,
    PB_6  = 0x16,
    PB_6_ALT0 = PB_6 | ALT0,
    PB_7  = 0x17,
    PB_7_ALT0 = PB_7 | ALT0,
    PB_8  = 0x18, 
    PB_8_ALT0 = PB_8 | ALT0,
    PB_9  = 0x19, 
    PB_9_ALT0 = PB_9 | ALT0,
    PB_10 = 0x1A,
    PB_11 = 0x1B,
    PB_12 = 0x1C,
    PB_13 = 0x1D,
    PB_13_ALT0 = PB_13 | ALT0,
    PB_14 = 0x1E,
    PB_14_ALT0 = PB_14 | ALT0,
    PB_14_ALT1 = PB_14 | ALT1,
    PB_15 = 0x1F,
    PB_15_ALT0 = PB_15 | ALT0,
    PB_15_ALT1 = PB_15 | ALT1,

    PC_0  = 0x20,
    PC_0_ALT0 = PC_0 | ALT0,
    PC_0_ALT1 = PC_0 | ALT1,
    PC_1  = 0x21,
    PC_1_ALT0 = PC_1 | ALT0,
    PC_1_ALT1 = PC_1 | ALT1,
    PC_2  = 0x22,
    PC_2_ALT0 = PC_2 | ALT0,
    PC_2_ALT1 = PC_2 | ALT1,
    PC_3  = 0x23,
    PC_3_ALT0 = PC_3 | ALT0,
    PC_3_ALT1 = PC_3 | ALT1,
    PC_4  = 0x24,
    PC_4_ALT0 = PC_4 | ALT0,
    PC_5  = 0x25,
    PC_5_ALT0 = PC_5 | ALT0,
    PC_6  = 0x26,
    PC_6_ALT0 = PC_6 | ALT0,
    PC_7  = 0x27,
    PC_7_ALT0 = PC_7 | ALT0,
    PC_8  = 0x28,
    PC_8_ALT0 = PC_8 | ALT0,
    PC_9  = 0x29,
    PC_9_ALT0 = PC_9 | ALT0,
    PC_10 = 0x2A,
    PC_10_ALT0 = PC_10 | ALT0,
    PC_11 = 0x2B,
    PC_11_ALT0 = PC_11 | ALT0,
    PC_12 = 0x2C,
    PC_13 = 0x2D,
    PC_14 = 0x2E, 
    PC_15 = 0x2F, 

    PD_0  = 0x30, 
    PD_1  = 0x31,
    PD_2  = 0x32,
    PD_3  = 0x33,
    PD_4  = 0x34,
    PD_5  = 0x35,
    PD_6  = 0x36,
    PD_7  = 0x37,
    PD_8  = 0x38,
    PD_9  = 0x39,
    PD_10 = 0x3A,
    PD_11 = 0x3B,
    PD_12 = 0x3C,
    PD_13 = 0x3D,
    PD_14 = 0x3E,
    PD_15 = 0x3F,

    PE_0  = 0x40,
    PE_1  = 0x41,
    PE_2  = 0x42,
    PE_3  = 0x43,
    PE_4  = 0x44,
    PE_5  = 0x45,
    PE_6  = 0x46,
    PE_7  = 0x47,
    PE_8  = 0x48, 
    PE_9  = 0x49,
    PE_10 = 0x4A, 
    PE_11 = 0x4B, 
    PE_12 = 0x4C, 
    PE_13 = 0x4D, 
    PE_14 = 0x4E, 
    PE_15 = 0x4F, 

    PH_0  = 0x70, 
    PH_1  = 0x71, 

    // ADC internal channels
    ADC_TEMP = 0xF0,
    ADC_VREF = 0xF1,
    ADC_VBAT = 0xF2,

    // STDIO for console print
    CONSOLE_TX = PC_4,
    CONSOLE_RX = PC_5,

    I2C_SCL     = PB_8,
    I2C_SDA     = PB_9,
    SPI_MOSI    = PA_7,
    SPI_MISO    = PA_6,
    SPI_SCK     = PA_5,
    SPI_CS      = PB_6,
    PWM_OUT     = PC_7,

    /**** USB pins ****/
    USB_OTG_FS_DM = PA_11,
    USB_OTG_FS_DP = PA_12,
    USB_OTG_FS_ID = PA_10,

    /**** OSCILLATOR pins ****/
    RCC_OSC32_IN = PC_14,
    RCC_OSC32_OUT = PC_15,
    RCC_OSC_IN = PH_0,
    RCC_OSC_OUT = PH_1,

    /**** DEBUG pins ****/
    SYS_JTCK_SWCLK = PA_14,
    SYS_JTDI = PA_15,
    SYS_JTDO_SWO = PB_3,
    SYS_JTMS_SWDIO = PA_13,
    SYS_JTRST = PB_4,

    /**** QSPI FLASH pins ****/
    QSPI_FLASH1_IO0 = PE_12,
    QSPI_FLASH1_IO1 = PE_13,
    QSPI_FLASH1_IO2 = PE_14,
    QSPI_FLASH1_IO3 = PE_15,
    QSPI_FLASH1_SCK = PE_10,
    QSPI_FLASH1_CSN = PE_11,

 // rhomb.io system namings //.........................
    NMI   = PB_1,
    INT0  = PB_3,
    INT1  = PB_7,
    INT2  = PA_15,

    IO0   = PB_5,
    IO1   = PB_4,
    IO2   = PB_10,
    IO3   = PA_8,
    IO4   = PA_9,
    IO5   = PE_8, 
    IO6   = PE_7,   
    IO7   = PB_15,
  
    IO8   = PC_13,
    IO9   = PD_7,   
    IO10  = PD_10,
    IO11  = PD_11,
    IO12  = PD_12,
    IO13  = PD_13,
    IO14  = PC_3,
    IO15  = PC_2,
 
    PWM0  = PC_7,
    PWM1  = PE_9,
    PWM2  = PD_14,
    PWM3  = PC_6,
    PWM4  = PB_2,
 
    UART_A_RXD  = PA_3,
    UART_A_TXD  = PA_2,
    UART_A_CTS  = PD_3,
    UART_A_RTS  = PD_4,

    UART_B_RXD  = PD_6,
    UART_B_TXD  = PD_5,

    UART_C_RXD  = PD_9,
    UART_C_TXD  = PD_8,  
    
    I2C_A_SDA   = PB_9,
    I2C_A_SCL   = PB_8,
    I2C_B_SDA   = PB_14, // R14 must be populated/closed (default is open)
    I2C_B_SCL   = PB_13, // R15 must be populated/closed (default is open)

    SPI_A_MOSI  = PA_7,  
    SPI_A_MISO  = PA_6,  
    SPI_A_CLK   = PA_5,
    SPI_A_CS0   = PB_6,
    SPI_A_CS1   = PB_14,
    SPI_A_CS2   = PB_13,
  
    QSPI_IO0    = PE_12, 
    QSPI_IO1    = PE_13,  
    QSPI_IO2    = PE_14, 
    QSPI_IO3    = PE_15,   
    QSPI_CLK    = PE_10, 
    QSPI_CS0    = PB_11,  
    QSPI_MEM_CS = PE_11,  

    _1WIRE    = PB_12,  
    CAN_A_RX  = PD_0,  
    CAN_A_TX  = PD_1, 
  
    SDIO_DATAO = PC_8,  
    SDIO_DATA1 = PC_9, 
    SDIO_DATA2 = PC_10,
    SDIO_DATA3 = PC_11,
    SDIO_CLK   = PC_12,   
    SDIO_CMD   = PD_2,  
    SDIO_CDN   = PD_15,  

    SAI_A_BCLK  = PE_5, 
    SAI_A_LRCLK = PE_4,  
    SAI_A_SDI   = PE_6,  
    SAI_A_SDO   = PE_3,  
    SAI_A_MCLK  = PE_2,  

    OTG_P  = PA_12,  
    OTG_N  = PA_11,    
    OTG_ID = PA_10,  
  
    AD0 = PA_0,
    AD1 = PA_1, 
    AD2 = PA_4, 
    AD3 = PB_0,
    AD4 = PC_1,
    AD5 = PC_0,
    // AD6 not used
    AD7 = PC_5,   
    AD8 = PC_4,

    COMP_A_P = PC_5,
    COMP_A_N = PC_4,   
  
    CAPT0  = PC_3,  // R18 must be populated/closed (default is open)
    CAPT1  = PC_2,  // R19 must be populated/closed (default is open)

    SWDIO  = PA_13,
    SWCLK  = PA_14,  
  
    RSTOUT = PE_0, 
 
    // Not connected
    NC = (int)0xFFFFFFFF

} PinName;

// Standardized LED and button names
#define LED1     PC_7   // LED Green
#define BUTTON1  PB_1

#ifdef __cplusplus
}
#endif

#endif