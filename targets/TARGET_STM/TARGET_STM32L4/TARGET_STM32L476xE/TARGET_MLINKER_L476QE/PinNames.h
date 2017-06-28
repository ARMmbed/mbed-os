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
    PA_0  = 0x00, // On P1/P2 connectors
    PA_1  = 0x01, // On P1/P2 connectors
    PA_2  = 0x02, // On P1/P2 connectors
    PA_3  = 0x03, // On P1/P2 connectors
    PA_4  = 0x04,
    PA_5  = 0x05, // On P1/P2 connectors
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
    PB_2  = 0x12, // On P1/P2 connectors
    PB_3  = 0x13, // On P1/P2 connectors
    PB_4  = 0x14,
    PB_5  = 0x15,
    PB_6  = 0x16,
    PB_7  = 0x17,
    PB_8  = 0x18, // On P1/P2 connectors
    PB_9  = 0x19, // On P1/P2 connectors
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
    PC_14 = 0x2E, // On P1/P2 connectors
    PC_15 = 0x2F, // On P1/P2 connectors

    PD_0  = 0x30, // On P1/P2 connectors
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
    PE_8  = 0x48, // On P1/P2 connectors
    PE_9  = 0x49,
    PE_10 = 0x4A, // On P1/P2 connectors
    PE_11 = 0x4B, // On P1/P2 connectors
    PE_12 = 0x4C, // On P1/P2 connectors
    PE_13 = 0x4D, // On P1/P2 connectors
    PE_14 = 0x4E, // On P1/P2 connectors
    PE_15 = 0x4F, // On P1/P2 connectors
    
    PF_0  = 0x50,
    PF_1  = 0x51,
    PF_2  = 0x52,
    PF_3  = 0x53,
    PF_4  = 0x54,
    PF_5  = 0x55,
    PF_6  = 0x56,
    PF_7  = 0x57,
    PF_8  = 0x58,
    PF_9  = 0x59,
    PF_10 = 0x5A,
    PF_11 = 0x5B,
    PF_12 = 0x5C,
    PF_13 = 0x5D,
    PF_14 = 0x5E,
    PF_15 = 0x5F,
    
    PG_0  = 0x60,
    PG_1  = 0x61,
    PG_2  = 0x62, 
    PG_3  = 0x63, 
    PG_4  = 0x64, 
    PG_5  = 0x65, 
    PG_6  = 0x66, 
    PG_7  = 0x67, 
    PG_8  = 0x68, 
    PG_9  = 0x69, 
    PG_10  = 0x6A, 
    PG_11  = 0x6B, 
    PG_12  = 0x6C, 
    PG_13  = 0x6D, 
    PG_14  = 0x6E,   
    PG_15  = 0x6F, 
    
    
    PH_0  = 0x70, // On P1/P2 connectors
    PH_1  = 0x71, // On P1/P2 connectors

    // ADC internal channels
    ADC_TEMP = 0xF0,
    ADC_VREF = 0xF1,
    ADC_VBAT = 0xF2,

    
    // Generic signals namings
    LED1 = PE_8, // LD5 Green
    LED2 = PB_2, // LD4 Red
    LED3 = LED1,
    LED4 = LED2,
    JOYSTICK_CENTER = PA_0,
    JOYSTICK_LEFT = PA_1,
    JOYSTICK_RIGHT = PA_2,
    JOYSTICK_UP = PA_3,
    JOYSTICK_DOWN = PA_5,
    USER_BUTTON = JOYSTICK_CENTER,
    SERIAL_TX = PD_5,
    SERIAL_RX = PD_6,
    USBTX = PD_5,
    USBRX = PD_6,

    I2C_SCL     = PB_8,
    I2C_SDA     = PB_9,
    SPI_MOSI    = PA_7,
    SPI_MISO    = PA_6,
    SPI_SCK     = PA_5,
    SPI_CS      = PA_4,
    PWM_OUT     = PB_3,
    A2=	PE_1,
    A3=	PB_8,
    A5=	PD_7,
    A6=	PD_5,
    A7=	PB_4,
    A8=	PB_3,
    A9=	PA_15,
    A10=	PA_14,
    A11=	PA_13,
    A12=	PA_12,
    B4=	PB_7,
    B5=	PB_6,
    B7=	PD_4,
    B8=	PD_3,
    B9=	PD_1,
    B10=	PC_12,
    B11=	PC_10,
    B12=	PA_11,
    C3=	PE_0,
    C5=	PB_5,
    C7=	PG_13,
    C9=	PD_0,
    C10=	PC_11,
    D1=	PC_14,
    D4=	PF_2,
    D5=	PF_1,
    D6=	PF_0,
    D7=	PG_12,
    E1=	PC_15,
    E4=	PF_3,
    E9=	PG_5,
    F1=	PH_0,
    F3=	PF_4,
    F4=	PF_5,
    F9=	PG_3,
    F10=PG_4,
    G1=	PH_1,
    G3=	PG_11,
    G9=	PG_1,
    G10=PG_2,
    H1=	PC_0,
    H9=	PG_0,
    H10=	PD_15,
    H11=	PD_14,
    J2=	PC_1,
    J4=	PA_4,
    J7=	PF_12,
    J8=	PF_14,
    J9=	PF_15,
    J10=	PD_12,
    J11=	PD_11,
    J12=	PD_10,
    K4=	PA_5,
    K7=	PF_13,
    K8=	PD_9,
    K9=	PD_8,
    K10=	PB_15,
    K11=	PB_14,
    K12=	PB_13,
    L4=	PA_6,
    L7=	PE_8,
    L8=	PE_10,
    L9=	PE_12,
    L10=	PB_10,
    M2=	PA_1,
    M7=	PE_7,
    M8=	PE_9,
    M9=	PE_11,
    M10=	PE_13,
    M11=	PE_14,
    M12=	PE_15,
    OSC32_IN=PC_14,
    OSC32_OUT=PC_15,
    OSC_IN=PH_0,
    OSC_OUT=PH_1,
    
    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

#ifdef __cplusplus
}
#endif

#endif
