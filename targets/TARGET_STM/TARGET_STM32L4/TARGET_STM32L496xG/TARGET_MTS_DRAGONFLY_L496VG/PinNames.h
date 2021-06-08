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

/* MBED TARGET LIST: MTS_DRAGONFLY_L471QG */

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
    PA_0       = 0x00,
    PA_0_ALT0  = PA_0  | ALT0, // same pin used for alternate HW
    PA_1       = 0x01,
    PA_1_ALT0  = PA_1  | ALT0, // same pin used for alternate HW
    PA_2       = 0x02,
    PA_2_ALT0  = PA_2  | ALT0, // same pin used for alternate HW
    PA_3       = 0x03,
    PA_3_ALT0  = PA_3  | ALT0, // same pin used for alternate HW
    PA_4       = 0x04,
    PA_4_ALT0  = PA_4  | ALT0, // same pin used for alternate HW
    PA_5       = 0x05,
    PA_5_ALT0  = PA_5  | ALT0, // same pin used for alternate HW
    PA_6       = 0x06,
    PA_6_ALT0  = PA_6  | ALT0, // same pin used for alternate HW
    PA_7       = 0x07,
    PA_7_ALT0  = PA_7  | ALT0, // same pin used for alternate HW
    PA_7_ALT1  = PA_7  | ALT1, // same pin used for alternate HW
    PA_7_ALT2  = PA_7  | ALT2, // same pin used for alternate HW
    PA_8       = 0x08,
    PA_9       = 0x09,
    PA_10      = 0x0A,
    PA_11      = 0x0B,
    PA_12      = 0x0C,
    PA_13      = 0x0D,
    PA_14      = 0x0E,
    PA_15      = 0x0F,
    PA_15_ALT0 = PA_15 | ALT0, // same pin used for alternate HW

    PB_0       = 0x10,
    PB_0_ALT0  = PB_0  | ALT0, // same pin used for alternate HW
    PB_0_ALT1  = PB_0  | ALT1, // same pin used for alternate HW
    PB_1       = 0x11,
    PB_1_ALT0  = PB_1  | ALT0, // same pin used for alternate HW
    PB_1_ALT1  = PB_1  | ALT1, // same pin used for alternate HW
    PB_2       = 0x12,
    PB_3       = 0x13,
    PB_3_ALT0  = PB_3  | ALT0, // same pin used for alternate HW
    PB_4       = 0x14,
    PB_4_ALT0  = PB_4  | ALT0, // same pin used for alternate HW
    PB_5       = 0x15,
    PB_5_ALT0  = PB_5  | ALT0, // same pin used for alternate HW
    PB_6       = 0x16,
    PB_6_ALT0  = PB_6  | ALT0, // same pin used for alternate HW
    PB_7       = 0x17,
    PB_7_ALT0  = PB_7  | ALT0, // same pin used for alternate HW
    PB_8       = 0x18,
    PB_8_ALT0  = PB_8  | ALT0, // same pin used for alternate HW
    PB_9       = 0x19,
    PB_9_ALT0  = PB_9  | ALT0, // same pin used for alternate HW
    PB_10      = 0x1A,
    PB_10_ALT0 = PB_10 | ALT0, // same pin used for alternate HW
    PB_11      = 0x1B,
    PB_11_ALT0 = PB_11 | ALT0, // same pin used for alternate HW
    PB_12      = 0x1C,
    PB_13      = 0x1D,
    PB_13_ALT0 = PB_13 | ALT0, // same pin used for alternate HW
    PB_14      = 0x1E,
    PB_14_ALT0 = PB_14 | ALT0, // same pin used for alternate HW
    PB_14_ALT1 = PB_14 | ALT1, // same pin used for alternate HW
    PB_15      = 0x1F,
    PB_15_ALT0 = PB_15 | ALT0, // same pin used for alternate HW
    PB_15_ALT1 = PB_15 | ALT1, // same pin used for alternate HW

    PC_0       = 0x20,
    PC_0_ALT0  = PC_0  | ALT0, // same pin used for alternate HW
    PC_0_ALT1  = PC_0  | ALT1, // same pin used for alternate HW
    PC_1       = 0x21,
    PC_1_ALT0  = PC_1  | ALT0, // same pin used for alternate HW
    PC_1_ALT1  = PC_1  | ALT1, // same pin used for alternate HW
    PC_2       = 0x22,
    PC_2_ALT0  = PC_2  | ALT0, // same pin used for alternate HW
    PC_2_ALT1  = PC_2  | ALT1, // same pin used for alternate HW
    PC_3       = 0x23,
    PC_3_ALT0  = PC_3  | ALT0, // same pin used for alternate HW
    PC_3_ALT1  = PC_3  | ALT1, // same pin used for alternate HW
    PC_4       = 0x24,
    PC_4_ALT0  = PC_4  | ALT0, // same pin used for alternate HW
    PC_5       = 0x25,
    PC_5_ALT0  = PC_5  | ALT0, // same pin used for alternate HW
    PC_6       = 0x26,
    PC_6_ALT0  = PC_6  | ALT0, // same pin used for alternate HW
    PC_7       = 0x27,
    PC_7_ALT0  = PC_7  | ALT0, // same pin used for alternate HW
    PC_8       = 0x28,
    PC_8_ALT0  = PC_8  | ALT0, // same pin used for alternate HW
    PC_9       = 0x29,
    PC_9_ALT0  = PC_9  | ALT0, // same pin used for alternate HW
    PC_10      = 0x2A,
    PC_10_ALT0 = PC_10 | ALT0, // same pin used for alternate HW
    PC_11      = 0x2B,
    PC_11_ALT0 = PC_11 | ALT0, // same pin used for alternate HW
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

    PH_0 = 0x70,
    PH_1 = 0x71,

    // ADC internal channels
    ADC_TEMP = 0xF0,
    ADC_VREF = 0xF1,
    ADC_VBAT = 0xF2,

#ifdef TARGET_FF_ARDUINO_UNO
    ARDUINO_UNO_A0  = PC_2,
    ARDUINO_UNO_A1  = PC_0,
    ARDUINO_UNO_A2  = PC_4,
    ARDUINO_UNO_A3  = PB_0,
    ARDUINO_UNO_A4  = PC_1,
    ARDUINO_UNO_A5  = PC_9,
    ARDUINO_UNO_D0  = PA_3,
    ARDUINO_UNO_D1  = PA_2,
    ARDUINO_UNO_D2  = PB_15,
    ARDUINO_UNO_D3  = PA_0,
    ARDUINO_UNO_D4  = PA_7,
    ARDUINO_UNO_D5  = PA_9,
    ARDUINO_UNO_D6  = PA_1,
    ARDUINO_UNO_D7  = PA_8,
    ARDUINO_UNO_D8  = PB_1,
    ARDUINO_UNO_D9  = PB_13,
    ARDUINO_UNO_D10 = PC_8,
    ARDUINO_UNO_D11 = PB_5,
    ARDUINO_UNO_D12 = PA_6,
    ARDUINO_UNO_D13 = PA_5,
    ARDUINO_UNO_D14 = PB_9,
    ARDUINO_UNO_D15 = PB_8,
#endif

    // 40 pin
    IO_00 = ARDUINO_UNO_D1,
    IO_01 = ARDUINO_UNO_D4,
    IO_02 = ARDUINO_UNO_D8,
    IO_03 = ARDUINO_UNO_D6,
    IO_04 = ARDUINO_UNO_D11,
    IO_05 = ARDUINO_UNO_D13,
    IO_06 = ARDUINO_UNO_D15,
    IO_07 = ARDUINO_UNO_D2,
    IO_08 = ARDUINO_UNO_A0,
    IO_09 = ARDUINO_UNO_A3,
    IO_10 = ARDUINO_UNO_A1,
    IO_11 = ARDUINO_UNO_A4,
    IO_12 = ARDUINO_UNO_A2,
    IO_13 = ARDUINO_UNO_D9,
    IO_14 = ARDUINO_UNO_A5,
    IO_15 = ARDUINO_UNO_D14,
    IO_16 = ARDUINO_UNO_D12,
    IO_17 = ARDUINO_UNO_D10,
    IO_18 = ARDUINO_UNO_D3,
    IO_19 = ARDUINO_UNO_D5,
    IO_20 = ARDUINO_UNO_D7,
    IO_21 = ARDUINO_UNO_D0,

    // 40 pin USB debug port
    CONSOLE_TX = PB_6,
    CONSOLE_RX = PB_7,

    // UARTS to external I/O
    UART2_TX = ARDUINO_UNO_D1,
    UART2_RX = ARDUINO_UNO_D0,
    UART2_RTS = ARDUINO_UNO_D6,
    UART2_CTS = ARDUINO_UNO_D3,
    UART2_DSR = ARDUINO_UNO_D5,
    UART2_DTR = ARDUINO_UNO_D7,
    UART2_DCD = ARDUINO_UNO_D4,
    UART2_RI = ARDUINO_UNO_D8,

    UART1_TX = PB_6,
    UART1_RX = PB_7,
    UART1_RTS = PB_3,
    UART1_CTS = PB_4,

    // 40 pin JTAG/SWD
    J_TCK = PA_14,	// a.k.a. SWCLK
    J_TDI = PA_15,
    J_RST = PB_4,
    J_TDO = PB_3,	// a.k.a. SWO
    J_TMS = PA_13,	// a.k.a. SWDIO

    // Exteranl serial port
    SERIAL_TX  = ARDUINO_UNO_D1,
    SERIAL_RX  = ARDUINO_UNO_D0,
    SERIAL_RTS = ARDUINO_UNO_D6,
    SERIAL_CTS = ARDUINO_UNO_D3,
    SERIAL_DCD = ARDUINO_UNO_D4,
    SERIAL_DSR = ARDUINO_UNO_D5,
    SERIAL_DTR = ARDUINO_UNO_D7,
    SERIAL_RI  = ARDUINO_UNO_D8,

    // SPI1 and SPI2 are available on Arduino pins
    SPI1_MOSI = ARDUINO_UNO_D11,
    SPI1_MISO = ARDUINO_UNO_D12,
    SPI1_SCK  = ARDUINO_UNO_D13,
    SPI2_MOSI = ARDUINO_UNO_D2,
    SPI2_MISO = ARDUINO_UNO_A0,
    SPI2_SCK  = ARDUINO_UNO_D9,

    // QuadSPI connects to flash part
    QSPI_FLASH1_SCK = PE_10,
    QSPI_FLASH1_CSN = PE_11,
    QSPI_FLASH1_IO0 = PE_12,
    QSPI_FLASH1_IO1 = PE_13,
    QSPI_FLASH1_IO2 = PE_14,
    QSPI_FLASH1_IO3 = PE_15,

    // I2C1 and I2C3 are available on Arduino pins
    I2C1_SCL = ARDUINO_UNO_D15,
    I2C1_SDA = ARDUINO_UNO_D14,
    I2C2_SCL = PB_10,
    I2C2_SDA = PB_14,
    I2C3_SCL = ARDUINO_UNO_A1,
    I2C3_SDA = ARDUINO_UNO_A4,

    // Cellular radio (note: no DSR signal)
    RADIO_PWR   = PE_0,
    RADIO_RESET = PE_1,
    RADIO_TX    = PD_8,
    RADIO_RX    = PD_9,
    RADIO_RI    = PD_10,
    RADIO_CTS   = PD_11,
    RADIO_RTS   = PD_12,
    RADIO_DTR   = PD_13,
    RADIO_DCD   = PD_14,
    RADIO_ONOFF = PE_2,
    RADIO_STATUS = PE_3,
    AP_READY    = PE_4,
    RADIO_WAKEUP = PE_5,
    NET_MODE    = PD_3,
    NET_STATUS  = PD_2,
    VRAD_ALERT  = PB_2,
    MON_1V8     = PC_5,
    RADIO_VDD_EXT = MON_1V8,

    MDMPWRON = RADIO_ONOFF, // 3G_ONOFF DragonFly Design Guide, Page No. 16
    MDMRST = RADIO_RESET,
    MDMTXD   = RADIO_TX, // Transmit Data
    MDMRXD   = RADIO_RX, // Receive Data
    MDMRTS   = RADIO_RTS, // Request to Send
    MDMCTS   = RADIO_CTS, // Clear to Send
    MDMDCD   = RADIO_DCD, // Data Carrier Detect
    MDMDTR   = RADIO_DTR, // Data Terminal Ready
    MDMRI    = RADIO_RI, // Ring Indicator

    // Power control for level shifter and SPI flash. Low powers on.
    BUF_EN	= PD_15,
    VMEM_EN	= PE_7,

    // Voltage measuring
    VCC_IN_MEAS_EN = PE_8,
    VCC_IN_MEAS = PC_3,

    INPUT_POWER = PC_7,

    // GNSS/GPS
    GPS_ANT_PWR_EN = PE_9,

    // Wake up from Standby/Shutdown options are PA0 & PA2.
    WAKEUP  = ARDUINO_UNO_D3,
    // Wake up build Options
    STM_WKUP = PC_13,

    // CAN
    CAN_TX = ARDUINO_UNO_D14,
    CAN_RX = ARDUINO_UNO_D15,
    // build option
    CAN1_TX = PD_1,
    CAN1_RX = PD_0,
    CAN2_RX = PB_12,
    CAN2_TX = ARDUINO_UNO_D9,

    LS_LED = PE_6,

    // Not connected
    NC = (int) 0xFFFFFFFF
} PinName;

// Standardized LED and button names
#define LED1  PA_0


#ifdef __cplusplus
}
#endif

#endif
