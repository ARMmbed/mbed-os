/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MBED_PERIPHERALPINMAPS_H
#define MBED_PERIPHERALPINMAPS_H

#include <mstd_cstddef>

typedef enum {
    NRF_SAADC_CHANNEL_0 = 0,
    NRF_SAADC_CHANNEL_1 = 1,
    NRF_SAADC_CHANNEL_2 = 2,
    NRF_SAADC_CHANNEL_3 = 3,
    NRF_SAADC_CHANNEL_4 = 4,
    NRF_SAADC_CHANNEL_5 = 5,
    NRF_SAADC_CHANNEL_6 = 6,
    NRF_SAADC_CHANNEL_7 = 7,
} nrf_saadc_channel_t;

/************ADC***************/
/* The third "function" value is used to select the correct ADC channel */
#if DEVICE_ANALOGIN
MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_ADC[] = {
    { p2,  ADC0_0, NRF_SAADC_CHANNEL_0 },
    { p3,  ADC0_0, NRF_SAADC_CHANNEL_1 },
    { p4,  ADC0_0, NRF_SAADC_CHANNEL_2 },
    { p5,  ADC0_0, NRF_SAADC_CHANNEL_3 },
    { p28, ADC0_0, NRF_SAADC_CHANNEL_4 },
    { p29, ADC0_0, NRF_SAADC_CHANNEL_5 },
    { p30, ADC0_0, NRF_SAADC_CHANNEL_6 },
    { p31, ADC0_0, NRF_SAADC_CHANNEL_7 },
    { NC,  NC,     NC                  }
};
#endif

// Pinmap used for testing only
MSTD_CONSTEXPR_OBJ_11 PinMap All_pins[] = {
    {P0_0,  0, 0},
    {P0_1,  0, 0},
    {P0_2,  0, 0},
    {P0_3,  0, 0},
    {P0_4,  0, 0},
    {P0_5,  0, 0},
    {P0_6,  0, 0},
    {P0_7,  0, 0},
    {P0_8,  0, 0},
    {P0_9,  0, 0},
    {P0_10,  0, 0},
    {P0_11,  0, 0},
    {P0_12,  0, 0},
    {P0_13,  0, 0},
    {P0_14,  0, 0},
    {P0_15,  0, 0},
    {P0_16,  0, 0},
    {P0_17,  0, 0},
    {P0_18,  0, 0},
    {P0_19,  0, 0},
    {P0_20,  0, 0},
    {P0_21,  0, 0},
    {P0_22,  0, 0},
    {P0_23,  0, 0},
    {P0_24,  0, 0},
    {P0_25,  0, 0},
    {P0_28,  0, 0},
    {P0_29,  0, 0},
    {P0_30,  0, 0},
    {P0_31,  0, 0},

    {P1_0,  0, 0},
    {P1_1,  0, 0},
    {P1_2,  0, 0},
    {P1_3,  0, 0},
    {P1_4,  0, 0},
    {P1_5,  0, 0},
    {P1_6,  0, 0},
    {P1_7,  0, 0},
    {P1_8,  0, 0},
    {P1_9,  0, 0},
    {P1_10,  0, 0},
    {P1_11,  0, 0},
    {P1_12,  0, 0},
    {P1_13,  0, 0},
    {P1_14,  0, 0},
    {P1_15,  0, 0},

    {NC, NC, 0}
};

#define PINMAP_ANALOGIN PinMap_ADC
#define PINMAP_ANALOGOUT All_pins
#define PINMAP_I2C_SDA All_pins
#define PINMAP_I2C_SCL All_pins
#define PINMAP_UART_TX All_pins
#define PINMAP_UART_RX All_pins
#define PINMAP_UART_CTS All_pins
#define PINMAP_UART_RTS All_pins
#define PINMAP_SPI_SCLK All_pins
#define PINMAP_SPI_MOSI All_pins
#define PINMAP_SPI_MISO All_pins
#define PINMAP_SPI_SSEL All_pins
#define PINMAP_PWM All_pins

#endif
