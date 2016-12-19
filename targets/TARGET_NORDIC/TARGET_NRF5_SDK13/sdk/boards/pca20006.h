/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
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
#ifndef PCA20006_H
#define PCA20006_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_gpio.h"

// LEDs and buttons definition for PCA20006 board (beacon)
#define LEDS_NUMBER    3

#define LED_RGB_RED    16
#define LED_RGB_GREEN  12
#define LED_RGB_BLUE   15

#define LEDS_ACTIVE_STATE 0

#define BSP_LED_0 LED_RGB_RED
#define BSP_LED_1 LED_RGB_GREEN
#define BSP_LED_2 LED_RGB_BLUE

#define LEDS_LIST { LED_RGB_RED, LED_RGB_GREEN, LED_RGB_BLUE}

#define LEDS_INV_MASK  LEDS_MASK

#define BUTTON_0       8
#define BUTTON_1       18
#define BUTTON_PULL    NRF_GPIO_PIN_PULLUP

#define BUTTONS_ACTIVE_STATE 0

#define BSP_BUTTON_0   BUTTON_0
#define BSP_BUTTON_1   BUTTON_1

#define BUTTONS_NUMBER 2

#define BUTTONS_LIST { BUTTON_0, BUTTON_1 }

#define RX_PIN_NUMBER  24
#define TX_PIN_NUMBER  9
#define CTS_PIN_NUMBER 21
#define RTS_PIN_NUMBER 11
#define HWFC           true

// Low frequency clock source to be used by the SoftDevice
#ifdef S210
#define NRF_CLOCK_LFCLKSRC      NRF_CLOCK_LFCLKSRC_XTAL_20_PPM
#else
#define NRF_CLOCK_LFCLKSRC      {.source = NRF_CLOCK_LF_SRC_XTAL, .rc_ctiv = 0, .rc_temp_ctiv = 0, .xtal_accuracy=NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM}
#endif




#ifdef __cplusplus
}
#endif

#endif
