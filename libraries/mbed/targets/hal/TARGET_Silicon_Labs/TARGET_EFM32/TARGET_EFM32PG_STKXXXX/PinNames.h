/***************************************************************************//**
 * @file PinNames.h
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/
#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"
#include "em_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT = 0,
    PIN_OUTPUT = 1
} PinDirection;

typedef enum {
    /* EFM32 Pin Names
     * First 4 bits represent pin number, the remaining
     * bits represent port number (A = 0, B = 1, ...)
     */
    PA0 = 0 << 4, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
    PB0 = 1 << 4, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15,
    PC0 = 2 << 4, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12, PC13, PC14, PC15,
    PD0 = 3 << 4, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12, PD13, PD14, PD15,
    PE0 = 4 << 4, PE1, PE2, PE3, PE4, PE5, PE6, PE7, PE8, PE9, PE10, PE11, PE12, PE13, PE14, PE15,
    PF0 = 5 << 4, PF1, PF2, PF3, PF4, PF5, PF6, PF7, PF8, PF9, PF10, PF11, PF12, PF13, PF14, PF15,

    /* Starter Kit says LED0 and LED1, but mbed expects 1 and 2. This way using 1 and 2 or 0 and 1 will work. */
    LED0 = PF4,
    LED1 = PF5,
    LED2 = LED0,
    LED3 = LED0,
    LED4 = LED1,

    /* Push Buttons */
    SW0 = PF6,
    SW1 = PF7,
    BTN0 = SW0,
    BTN1 = SW1,

    /* Serial (just some usable pins) */
    SERIAL_TX   = PD10,
    SERIAL_RX   = PD11,

    /* Board Controller UART (USB) + enable pin */
    USBTX       = PA0,
    USBRX       = PA1,
    EFM_BC_EN   = PA5,

    /* Not connected */
    NC = (int) 0xFFFFFFFF
} PinName;


/* 0x10 represents setting the DOUT register (see efm32 modes in ref. manual) */
typedef enum {
    /* EFM32 pin modes */
    Disabled             = gpioModeDisabled,
    DisabledPullUp       = gpioModeDisabled | 0x10,
    Input                = gpioModeInput,
    InputFilter          = gpioModeInput | 0x10,
    InputPullDown        = gpioModeInputPull,
    InputPullUp          = gpioModeInputPull | 0x10,
    InputPullFilterDown  = gpioModeInputPullFilter,
    InputPullFilterUp    = gpioModeInputPullFilter | 0x10,
    PushPull             = gpioModePushPull,
    WiredOr              = gpioModeWiredOr,
    WiredOrPullDown      = gpioModeWiredOrPullDown,
    WiredAnd             = gpioModeWiredAnd,
    WiredAndFilter       = gpioModeWiredAndFilter,
    WiredAndPullUp       = gpioModeWiredAndPullUp,
    WiredAndPullUpFilter = gpioModeWiredAndPullUpFilter,

    /* mbed modes:
     * PullUp, PullDown, PullNone, OpenDrain
     *
     * mbed default digital input mode:
     * PullDefault
     *
     * mbed default digital output mode:
     * PullNone
     */
    PullUp = InputPullUp,
    PullDown = InputPullDown,
    OpenDrain = WiredAnd,
    PullNone = PushPull,
    PullDefault = PushPull
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
