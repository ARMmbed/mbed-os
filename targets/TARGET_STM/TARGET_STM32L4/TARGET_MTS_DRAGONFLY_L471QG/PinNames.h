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

    PA_0 = 0x00,
    PA_1 = 0x01,
    PA_2 = 0x02,
    PA_3 = 0x03,
    PA_4 = 0x04,
    PA_5 = 0x05,
    PA_6 = 0x06,
    PA_7 = 0x07,
    PA_8 = 0x08,
    PA_9 = 0x09,
    PA_10 = 0x0A,
    PA_11 = 0x0B,
    PA_12 = 0x0C,
    PA_13 = 0x0D,
    PA_14 = 0x0E,
    PA_15 = 0x0F,

    PB_0 = 0x10,
    PB_1 = 0x11,
    PB_2 = 0x12,
    PB_3 = 0x13,
    PB_4 = 0x14,
    PB_5 = 0x15,
    PB_6 = 0x16,
    PB_7 = 0x17,
    PB_8 = 0x18,
    PB_9 = 0x19,
    PB_10 = 0x1A,
    PB_11 = 0x1B,
    PB_12 = 0x1C,
    PB_13 = 0x1D,
    PB_14 = 0x1E,
    PB_15 = 0x1F,

    PC_0 = 0x20,
    PC_1 = 0x21,
    PC_2 = 0x22,
    PC_3 = 0x23,
    PC_4 = 0x24,
    PC_5 = 0x25,
    PC_6 = 0x26,
    PC_7 = 0x27,
    PC_8 = 0x28,
    PC_9 = 0x29,
    PC_10 = 0x2A,
    PC_11 = 0x2B,
    PC_12 = 0x2C,
    PC_13 = 0x2D,
    PC_14 = 0x2E,
    PC_15 = 0x2F,

    PD_0 = 0x30,
    PD_1 = 0x31,
    PD_2 = 0x32,
    PD_3 = 0x33,
    PD_4 = 0x34,
    PD_5 = 0x35,
    PD_6 = 0x36,
    PD_7 = 0x37,
    PD_8 = 0x38,
    PD_9 = 0x39,
    PD_10 = 0x3A,
    PD_11 = 0x3B,
    PD_12 = 0x3C,
    PD_13 = 0x3D,
    PD_14 = 0x3E,
    PD_15 = 0x3F,

    PE_0 = 0x40,
    PE_1 = 0x41,
    PE_2 = 0x42,
    PE_3 = 0x43,
    PE_4 = 0x44,
    PE_5 = 0x45,
    PE_6 = 0x46,
    PE_7 = 0x47,
    PE_8 = 0x48,
    PE_9 = 0x49,
    PE_10 = 0x4A,
    PE_11 = 0x4B,
    PE_12 = 0x4C,
    PE_13 = 0x4D,
    PE_14 = 0x4E,
    PE_15 = 0x4F,

    PF_0 = 0x50,
    PF_1 = 0x51,
    PF_2 = 0x52,
    PF_3 = 0x53,
    PF_4 = 0x54,
    PF_5 = 0x55,
    PF_6 = 0x56,
    PF_7 = 0x57,
    PF_8 = 0x58,
    PF_9 = 0x59,
    PF_10 = 0x5A,
    PF_11 = 0x5B,
    PF_12 = 0x5C,
    PF_13 = 0x5D,
    PF_14 = 0x5E,
    PF_15 = 0x5F,

    PG_0 = 0x60,
    PG_1 = 0x61,
    PG_2 = 0x62,
    PG_3 = 0x63,
    PG_4 = 0x64,
    PG_5 = 0x65,
    PG_6 = 0x66,
    PG_7 = 0x67,
    PG_8 = 0x68,
    PG_9 = 0x69,
    PG_10 = 0x6A,
    PG_11 = 0x6B,
    PG_12 = 0x6C,
    PG_13 = 0x6D,
    PG_14 = 0x6E,
    PG_15 = 0x6F,

    PH_0 = 0x70,
    PH_1 = 0x71,

    // ADC internal channels
    ADC_TEMP = 0xF0,
    ADC_VREF = 0xF1,
    ADC_VBAT = 0xF2,

    // Arduino connector namings
    A0  = PC_2,
    A1  = PC_13,     //rev b PC_0; rev c PC_13
    A2  = PC_4,
    A3  = PE_6,     //reb b PB_1; rev c PE_6
    A4  = PA_6,
    A5  = PG_8,
    D0  = PA_3,
    D1  = PA_2,
    D2  = PB_15,
    D3  = PA_0,
    D4  = PA_7,
    D5  = PA_9,
    D6  = PA_1,
    D7  = PG_7,
    D8  = PB_0,
    D9  = PB_10,
    D10 = PC_8,
    D11 = PB_5,
    D12 = PG_3,
    D13 = PG_2,
    D14 = PB_9,
    D15 = PB_8,

    // 40 pin
    IO_00 = D1,
    IO_01 = D4,
    IO_02 = D8,
    IO_03 = D6,
    IO_04 = D11,
    IO_05 = D13,
    IO_06 = D15,
    IO_07 = D2,
    IO_08 = A0,
    IO_09 = A3,
    IO_10 = A1,
    IO_11 = A4,
    IO_12 = A2,
    IO_13 = D9,
    IO_14 = A5,
    IO_15 = D14,
    IO_16 = D12,
    IO_17 = D10,
    IO_18 = D3,
    IO_19 = D5,
    IO_20 = D7,
    IO_21 = D0,

    // 40 pin USB debug port
    USBTX = PB_6,
    USBRX = PB_7,

    //UARTS
    UART3_TX = PD_2,
    UART3_RX = PD_9,
    UART3_CTS = PD_11,
    UART3_RTS = PD_12,

    UART2_TX = PA_2,
    UART2_RX = PA_3,
    UART2_RTS = PA_1,
    UART2_CTS = PA_0,
    UART2_DSR = PA_9,
    UART2_DTR = PG_7,
    UART2_DCD = PA_7,


    UART1_TX = PB_6,
    UART1_RX = PB_7,
    UART1_RTS = PA_12,
    UART1_CTS = PA_11,

    // 40 pin JTAG/SWD
    J_TCK = PA_14,	// a.k.a. SWCLK
    J_TDI = PA_15,
    J_RST = PB_4,
    J_TDO = PB_3,	// a.k.a. SWO
    J_TMS = PA_13,	// a.k.a. SWDIO

    // Generic signals namings
    LED1 = D3,
    LED2 = D3,
    LED3 = D3,
    LED4 = D3,

    // SERIAL PORT
    SERIAL_TX  = D1,
    SERIAL_RX  = D0,
    SERIAL_RTS = D6,
    SERIAL_CTS = D3,
    SERIAL_DCD = D4,
    SERIAL_DSR = D5,
    SERIAL_DTR = D7,
    SERIAL_RI  = D8,

    // SPI1 and SPI2 are available on Arduino pins
    SPI1_MOSI = D11,
    SPI1_MISO = D12,
    SPI1_SCK  = D13,
    SPI2_MOSI = D2,
    SPI2_MISO = A0,
    SPI2_SCK  = D9,

    // SPI3 connects to flash part
    SPI3_MOSI = PC_12,
    SPI3_MISO = PC_11,
    SPI3_SCK  = PC_10,
    SPI3_SSEL = PG_12,

    // I2C1 and I2C3 are available on Arduino pins
    I2C1_SCL = D15,
    I2C1_SDA = D14,
    I2C3_SCL = D7,
    I2C3_SDA = A5,

    // Modem
    RADIO_PWR   = PC_3,
    RADIO_RESET = PF_3,
    RADIO_TX    = PD_8,
    RADIO_RX    = PD_9,
    RADIO_RI    = PD_10,
    RADIO_CTS   = PD_11,
    RADIO_RTS   = PD_12,
    RADIO_DTR   = PD_13,
    RADIO_DCD   = PD_14,
    RADIO_DSR   = PD_15,
    RADIO_ONOFF = PE_4,     //rev b PC_13; rev c PC_0 PE_4
    MON_1V8     = PC_5,
    VUSB_EN		= PE_3,
    // Power control for level shifter and SPI flash. Low powers on.
    BUF_EN	= PC_6,
    VMEM_EN	= PE_1,
    VUSB_DET = PE_3,

    MDMPWRON = RADIO_ONOFF, // 3G_ONOFF DragonFly Design Guide, Page No. 16
    MDMTXD   = RADIO_TX, // Transmit Data
    MDMRXD   = RADIO_RX, // Receive Data
    MDMRTS   = RADIO_RTS, // Request to Send
    MDMCTS   = RADIO_CTS, // Clear to Send
    MDMDCD   = RADIO_DCD, // Data Carrier Detect
    MDMDSR   = RADIO_DSR, // Data Set Ready
    MDMDTR   = RADIO_DTR, // Data Terminal Ready
    MDMRI    = RADIO_RI, // Ring Indicator

    CELL_GPIO5 = PE_11,
    CELL_GPIO2 = PE_13,
    CELL_GPIO3 = PE_14,
    CELL_GPIO4 = PE_15,

    // Voltage measuring
    VCC_IN_MEAS_EN = PB_12,
    VCC_IN_MEAS = PC_0,

    // added to support ppp
    MDM_PIN_POLARITY = 0,
    MDMRST = RADIO_RESET,
    MDMCURRENTSENSE = (int) 0xFFFFFFFF,

    // GNSS
    GNSSEN  = (int) 0xFFFFFFFF,     // VCC_IO to GNSS, should be set to push-pull, no pull-up, output
    GNSSTXD =(int) 0xFFFFFFFF,
    GNSSRXD = (int) 0xFFFFFFFF,
    PWR3V3 = (int) 0xFFFFFFFF,

    WAKEUP   = D3,

    // TIMERS
    TIM2_1 = PA_15,
    TIM2_2 = PB_3,
    TIM2_3 = PA_2,
    TIM3_1 = PB_4,
    TIM3_2 = PA_7,
    TIM3_3 = PB_0,
    TIM4_3 = PB_8,
    TIM5_1 = PA_0,
    TIM5_2 = PA_1,
    TIM5_3 = PA_2,
    TIM7_1 = PA_7,
    TIM8_3 = PC_8,

    // CAN
    CAN1_TX = PB_9,
    CAN1_RX = PB_8,

    // ADC
    ADC1_IN7 = PA_2,
    ADC2_IN7 = PA_2,

    ADC1_IN12 = PA_7,
    ADC2_IN12 = PA_7,

    ADC1_IN15 = PB_0,
    ADC2_IN15 = PB_0,

    ADC1_IN6 = PA_1,
    ADC2_IN6 = PA_1,

    ADC1_IN3 = PC_2,
    ADC2_IN3 = PC_2,
    ADC3_IN3 = PC_2,

    ADC1_IN16 = PB_1,
    ADC2_IN16 = PB_1,

    ADC1_IN1 = PC_2,
    ADC2_IN1 = PC_2,
    ADC3_IN1 = PC_2,

    ADC1_IN11 = PA_6,
    ADC2_IN11 = PA_6,

    ADC1_IN13 = PC_4,
    ADC2_IN13 = PC_4,

    //ADC1_IN13 = PA_0,
    //ADC2_IN13 = PA_0,

    ADC1_IN8 = PA_3,
    ADC2_IN8 = PA_3,

    // Not connected
    NC = (int) 0xFFFFFFFF
} PinName;


#ifdef __cplusplus
}
#endif

#endif
