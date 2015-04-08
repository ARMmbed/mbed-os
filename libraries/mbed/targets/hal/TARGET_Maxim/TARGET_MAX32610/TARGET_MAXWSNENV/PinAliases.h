/*******************************************************************************
 * Copyright (C) 2015 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */

#ifndef _PIN_ALIASES_H
#define _PIN_ALIASES_H

#define LED_RED         P1_7
#define LED_GREEN       P1_6
#define LED_BLUE        P1_4

// mbed original LED naming
#define LED1            LED_RED
#define LED2            LED_GREEN
#define LED3            LED_BLUE
#define LED4            LED_BLUE

#define LED_OFF         1
#define LED_ON          !LED_OFF

// Push button
#define SW1             P1_5

// USB Pins
#define USBTX           P1_3
#define USBRX           P1_2

#define I2C_SCL         P0_5
#define I2C_SDA         P0_4

#define HCI_SCK         P2_0
#define HCI_MOSI        P2_1
#define HCI_MISO        P2_2
#define HCI_CSN         P2_3
#define HCI_IRQ         P2_4
#define HCI_RST         P2_5

#define TP1             P1_1
#define TP2             P1_0
#define TP3             P0_1
#define TP4             P0_0
#define TP5             P0_5
#define TP6             P0_4

#endif /* _PIN_ALIASES_H */
