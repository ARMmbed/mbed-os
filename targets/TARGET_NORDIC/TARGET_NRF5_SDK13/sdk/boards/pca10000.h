/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */
#ifndef PCA10000_H
#define PCA10000_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_gpio.h"

// Definitions for PCA10000 v2.0.0 or higher
#if 1

#define LEDS_NUMBER    3

// there is RGB LED on this board
#define LED_RGB_RED    21
#define LED_RGB_GREEN  22
#define LED_RGB_BLUE   23

#define LED_START  LED_RGB_RED
#define BSP_LED_0  LED_RGB_RED
#define BSP_LED_1  LED_RGB_GREEN
#define BSP_LED_2  LED_RGB_BLUE
#define LED_STOP   LED_RGB_BLUE

#define LEDS_ACTIVE_STATE 0

#define BUTTONS_LIST {}
#define LEDS_LIST { LED_RGB_RED, LED_RGB_GREEN, LED_RGB_BLUE }

#define LEDS_INV_MASK  LEDS_MASK

// there are no buttons on this board
#define BUTTONS_NUMBER 0

// UART pins connected to J-Link
#define RX_PIN_NUMBER  11
#define TX_PIN_NUMBER  9
#define CTS_PIN_NUMBER 10
#define RTS_PIN_NUMBER 8
#define HWFC           true

// Definitions for PCA10000 v1.0
#else

#define RX_PIN_NUMBER  3
#define TX_PIN_NUMBER  1
#define CTS_PIN_NUMBER 2
#define RTS_PIN_NUMBER 0
#define HWFC           true

#endif

// Low frequency clock source to be used by the SoftDevice
#define NRF_CLOCK_LFCLKSRC      {.source        = NRF_CLOCK_LF_SRC_XTAL,            \
                                 .rc_ctiv       = 0,                                \
                                 .rc_temp_ctiv  = 0,                                \
                                 .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM}

#ifdef __cplusplus
}
#endif

#endif
