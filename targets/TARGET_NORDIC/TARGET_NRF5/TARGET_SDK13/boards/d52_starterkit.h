/*
This software is subject to the license described in the License.txt file
included with this software distribution. You may not use this file except in compliance
with this license.

Copyright (c) Dynastream Innovations Inc. 2016
All rights reserved.
*/

#ifndef D52STARTERKIT_H
#define D52STARTERKIT_H

#include "nrf_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

// LEDs definitions for D52DK1
#define LEDS_NUMBER    4

// IO board active low leds
// D52DK1 does not define LED_START or LED_STOP since the LEDS are not on sequential pins
#define LED_A        24 //LED A on D52 Starter Kit IO Board
#define LED_B        31 //LED B on D52 Starter Kit IO Board
#define LED_C        17 //LED C on D52 Starter Kit IO Board
#define LED_D        20 //LED D on D52 Starter Kit IO Board

#define LEDS_ACTIVE_STATE 0

#define LEDS_LIST { LED_A, LED_B, LED_C, LED_D }

#define BSP_LED_0      LED_A
#define BSP_LED_1      LED_B
#define BSP_LED_2      LED_C
#define BSP_LED_3      LED_D

#define LEDS_INV_MASK  LEDS_MASK

#define BUTTONS_NUMBER 4

// IO board pull-up buttons
#define BUTTON_A     6  //BUTTON A on D52 Starter Kit IO Board
#define BUTTON_B     7  //BUTTON B on D52 Starter Kit IO Board
#define BUTTON_C     16 //BUTTON C on D52 Starter Kit IO Board
#define BUTTON_D     19 //BUTTON D on D52 Starter Kit IO Board
#define BUTTON_PULL  NRF_GPIO_PIN_PULLUP

#define BUTTONS_ACTIVE_STATE 0

#define BSP_BUTTON_0   BUTTON_A
#define BSP_BUTTON_1   BUTTON_B
#define BSP_BUTTON_2   BUTTON_C
#define BSP_BUTTON_3   BUTTON_D

#define BUTTONS_LIST { BUTTON_A, BUTTON_B, BUTTON_C, BUTTON_D }

// Battery board pull-up switches
#define SWITCH_1     12 // Switch 1 on D52 Starter Kit Battery Board
#define SWITCH_2     8  // Switch 2 on D52 Starter Kit Battery Board
#define SWITCH_3     15 // Switch 3 on D52 Starter Kit Battery Board
#define SWITCH_4     11 // Switch 4 on D52 Starter Kit Battery Board
#define SWITCH_5     14 // Switch 5 on D52 Starter Kit Battery Board
#define SWITCH_PULL  NRF_GPIO_PIN_PULLUP

#define SWITCHES_NUMBER 5

#define BSP_SWITCH_0 SWITCH_1
#define BSP_SWITCH_1 SWITCH_2
#define BSP_SWITCH_2 SWITCH_3
#define BSP_SWITCH_3 SWITCH_4
#define BSP_SWITCH_4 SWITCH_5

#define BSP_SWITCH_0_MASK   (1<<BSP_SWITCH_0)
#define BSP_SWITCH_1_MASK   (1<<BSP_SWITCH_1)
#define BSP_SWITCH_2_MASK   (1<<BSP_SWITCH_2)
#define BSP_SWITCH_3_MASK   (1<<BSP_SWITCH_3)
#define BSP_SWITCH_4_MASK   (1<<BSP_SWITCH_4)

#define SWITCHES_MASK (BSP_SWITCH_0_MASK | BSP_SWITCH_1_MASK | BSP_SWITCH_2_MASK | BSP_SWITCH_3_MASK | BSP_SWITCH_4_MASK)

// D52DK1 does not have UART peripheral. Dummy defines for compilation.
#define RX_PIN_NUMBER  UART_PIN_DISCONNECTED
#define TX_PIN_NUMBER  UART_PIN_DISCONNECTED
#define CTS_PIN_NUMBER UART_PIN_DISCONNECTED
#define RTS_PIN_NUMBER UART_PIN_DISCONNECTED

// Low frequency clock source to be used by the SoftDevice
#define NRF_CLOCK_LFCLKSRC      {.source        = NRF_CLOCK_LF_SRC_XTAL,            \
                                 .rc_ctiv       = 0,                                \
                                 .rc_temp_ctiv  = 0,                                \
                                 .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM}

#ifdef __cplusplus
}
#endif

#endif
