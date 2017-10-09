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

typedef enum {
    ALT0  = 0x100,
    ALT1  = 0x200,
    ALT2  = 0x300,
    ALT3  = 0x400
} ALTx;

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  PA_0  = 0x00,
  PA_1  = 0x01,
  PA_2  = 0x02,
  PA_2_ALT0 = PA_2|ALT0,
  PA_3  = 0x03,
  PA_3_ALT0 = PA_3|ALT0,
  PA_4  = 0x04,
  PA_4_ALT0 = PA_4|ALT0,
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
  PB_3_ALT0 = PB_3|ALT0,
  PB_4  = 0x14,
  PB_4_ALT0 = PB_4|ALT0,
  PB_5  = 0x15,
  PB_5_ALT0 = PB_5|ALT0,
  PB_6  = 0x16,
  PB_7  = 0x17,

  PC_14 = 0x2E,
  PC_15 = 0x2F,

  // ADC internal channels
  ADC_TEMP = 0xF0,
  ADC_VREF = 0xF1,
  ADC_VBAT = 0xF2,

  // Arduino connector namings
  A0          = PA_0,
  A1          = PA_1,
  A2          = PA_3,
  A3          = PA_4,
  A4          = PA_5,
  A5          = PA_6,
  A6          = PA_7,
  A7          = PA_2,
  D0          = PA_10,
  D1          = PA_9,
  D2          = PA_12,
  D3          = PB_0,
  D4          = PB_7,
  D5          = PB_6,
  D6          = PB_1,
  D7          = PC_14,
  D8          = PC_15,
  D9          = PA_8,
  D10         = PA_11,
  D11         = PB_5,
  D12         = PB_4,
  D13         = PB_3,

  // Generic signals namings
  LED1        = PB_3,
  LED2        = PB_3,
  LED3        = PB_3,
  LED4        = PB_3,
  SERIAL_TX   = PA_2,
  SERIAL_RX   = PA_15,
  USBTX       = PA_2,
  USBRX       = PA_15,
  I2C_SCL     = PB_6,
  I2C_SDA     = PB_7,
  SPI_MOSI    = PB_5,
  SPI_MISO    = PB_4,
  SPI_SCK     = PB_3,
  SPI_CS      = PA_11,
  PWM_OUT     = PB_0,

    //USB pins
    USB_DM = PA_11,
    USB_DP = PA_12,
    USB_NOE = PA_13,

  // Not connected
  NC = (int)0xFFFFFFFF
} PinName;

#ifdef __cplusplus
}
#endif

#endif
