/**
 ******************************************************************************
 * @file PinNames.h
 * @brief Implements common PIN names for peripherals.
 * @internal
 * @author ON Semiconductor
 * $Rev: 0.1 $
 * $Date: 2015-11-06 $
 ******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @ingroup debug
 */


#ifndef PINNAME_H_
#define PINNAME_H_

#include "types.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NC    = (int)0xFFFFFFFF,
    GPIO0 = 0,
    GPIO1,
    GPIO2,
    GPIO3,
    GPIO4,
    GPIO5,
    GPIO6,
    GPIO7,
    GPIO8,
    GPIO9,
    GPIO10,
    GPIO11,
    GPIO12,
    GPIO13,
    GPIO14,
    GPIO15,
    GPIO16,
    GPIO17,
    A0,
    A1,
    A2,
    A3,
    A4 = NC,
    A5 = NC,
    UART1_TX = GPIO0,
    UART1_RX = GPIO1,
    UART2_TX = GPIO8,
    UART2_RX = GPIO9,

    I2C1_SCLK_1    = GPIO2,
    I2C1_SDATA_1   = GPIO3,
    I2C1_SCLK_2    = GPIO5,
    I2C1_SDATA_2   = GPIO4,
    I2C1_SCLK      = I2C1_SCLK_1,   /*Default*/
    I2C1_SDATA     = I2C1_SDATA_1,  /*Default*/

    I2C2_SCLK_1    = GPIO14,
    I2C2_SDATA_1   = GPIO15,
    I2C2_SCLK_2    = GPIO17,
    I2C2_SDATA_2   = GPIO16,
    I2C2_SCLK      = I2C2_SCLK_2,  /*Default*/
    I2C2_SDATA     = I2C2_SDATA_2, /*Default*/
    I2C_SCL        = I2C1_SCLK_1,  /*Default*/
    I2C_SDA        = I2C1_SDATA_1, /*Default*/

    /* SPI 1 with 1st set of CROSS BAR */
    SPI1_SSNO0_1    = GPIO0,
    SPI1_SSNO1_1    = GPIO1,
    SPI1_SSNO2_1    = GPIO2,
    SPI1_SSNO3_1    = GPIO3,

    /* SPI 1 with 2st set of CROSS BAR */
    SPI1_SCLK_2      = GPIO4,
    SPI1_SDATAO_2    = GPIO5,
    SPI1_SDATAI_2    = GPIO6,
    SPI1_SSNI_2      = GPIO7,
    SPI1_SSNO0_2     = GPIO8,
    SPI1_SSNO1_2     = GPIO9,
    SPI1_SSNO2_2     = GPIO10,

    SPI1_SCLK    = SPI1_SCLK_2,   /*Default*/
    SPI1_SDATAO  = SPI1_SDATAO_2, /*Default*/
    SPI1_SDATAI  = SPI1_SDATAI_2, /*Default*/
    SPI1_SSNI    = SPI1_SSNI_2,   /*Default*/
    SPI1_SSNO0   = SPI1_SSNO0_2,  /*Default*/
    SPI1_SSNO1   = SPI1_SSNO1_2,  /*Default*/
    SPI1_SSNO2   = SPI1_SSNO2_2,  /*Default*/

    /* SPI 1 with 3rd set of CROSS BAR */
    SPI1_SCLK_3     = GPIO8,
    SPI1_SDATAO_3   = GPIO9,
    SPI1_SDATAI_3   = GPIO10,

    /* SPI 2 */
    SPI2_SCLK     = GPIO14,
    SPI2_SDATAO   = GPIO15,
    SPI2_SDATAI   = GPIO16,
    SPI2_SSNI     = GPIO17,
    SPI2_SSNO0    = GPIO17,

    // Generic signals namings
    LED1        = GPIO4,
    LED2        = GPIO6,
    LED3        = GPIO5,
    LED4        = (int)0xFFFFFFFF,
    LED5        = (int)0xFFFFFFFF,
    LED_GREEN   = GPIO4,
    LED_YELLOW  = GPIO6,
    LED_BLUE    = GPIO5,
    USER_BUTTON = GPIO7,  /*NEW connection on NCS36510-RF Rev 1.1 - Alias of SW1 */

    SW1 = GPIO7,  /*NEW connection on NCS36510-RF Rev 1.1 */
    SW2 = GPIO10,   /*NEW connection on NCS36510-RF Rev 1.1 */

    // Standardized button names
    BUTTON1 = SW1,
    BUTTON2 = SW2,

    SERIAL_TX   = GPIO0,
    SERIAL_RX   = GPIO1,
    USBTX       = GPIO0,
    USBRX       = GPIO1,
    D0          = GPIO9,
    D1          = GPIO8,
    D2          = GPIO4,
    D3          = GPIO6,
    D4          = GPIO5,
    D5          = GPIO7,
    D6          = GPIO10,
    D7          = (int)0xFFFFFFFF,
    D8          = (int)0xFFFFFFFF,
    D9          = (int)0xFFFFFFFF,
    D10         = GPIO17,
    D11         = GPIO15,
    D12         = GPIO16,
    D13         = GPIO14,
    D14         = GPIO3,
    D15         = GPIO2
} PinName;

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT,

} PinDirection;

typedef enum {
    PushPullPullDown  = 0,
    PushPullNoPull    = 1,
    PushPullPullUp    = 2,
    OpenDrainPullDown = 3,
    OpenDrainNoPull   = 4,
    OpenDrainPullUp   = 5,
    PullNone          = PushPullNoPull,
    PullUp            = PushPullPullUp,
    PullDown          = PushPullPullDown,
    OpenDrain         = OpenDrainPullUp,
    PullDefault       = PullNone

} PinMode;

#ifdef __cplusplus
}
#endif

#endif //PINNAME_H_
