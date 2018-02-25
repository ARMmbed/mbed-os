/* mbed Microcontroller Library
 * Copyright (c) 2013 Nordic Semiconductor
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
#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

#define PORT_SHIFT  3

typedef enum {
    p0  = 0,
    p1  = 1,
    p2  = 2,
    p3  = 3,
    p4  = 4,
    p5  = 5,
    p6  = 6,
    p7  = 7,
    p8  = 8,
    p9  = 9,
    p10 = 10,
    p11 = 11,
    p12 = 12,
    p13 = 13,
    p14 = 14,
    p15 = 15,
    p16 = 16,
    p17 = 17,
    p18 = 18,
    p19 = 19,
    p20 = 20,
    p21 = 21,
    p22 = 22,
    p23 = 23,
    p24 = 24,
    p25 = 25,
    p26 = 26,
    p27 = 27,
    p28 = 28,
    p29 = 29,
    p30 = 30,
    p31 = 31,


    P0_0  = p0,
    P0_1  = p1,
    P0_2  = p2,
    P0_3  = p3,
    P0_4  = p4,
    P0_5  = p5,
    P0_6  = p6,
    P0_7  = p7,
    
    P0_8  = p8,
    P0_9  = p9,
    P0_10 = p10,
    P0_11 = p11,
    P0_12 = p12,
    P0_13 = p13,
    P0_14 = p14,
    P0_15 = p15,
    
    P0_16 = p16,
    P0_17 = p17,
    P0_18 = p18,
    P0_19 = p19,
    P0_20 = p20,
    P0_21 = p21,
    P0_22 = p22,
    P0_23 = p23,
    
    P0_24 = p24,
    P0_25 = p25,
    P0_26 = p26,
    P0_27 = p27,
    P0_28 = p28,
    P0_29 = p29,
    P0_30 = p30,

   // Module pins. Refer datasheet for pin numbers.
    SIO_1 = P0_1,
    SIO_2 = P0_2,
    SIO_3 = P0_3,
    SIO_4 = P0_4,
    SIO_5 = P0_5,
    SIO_6 = P0_6,
    SIO_7 = P0_7,
    SIO_8 = P0_8,
    SIO_9 = P0_9,
    SIO_10 = P0_10,
    SIO_11 = P0_11,
    SIO_12 = P0_12,
    SIO_13 = P0_13,
    SIO_14 = P0_14,
    SIO_15 = P0_15,
    SIO_16 = P0_16,
    SIO_17 = P0_17,
    SIO_18 = P0_18,
    SIO_19 = P0_19,
    SIO_20 = P0_20,
    SIO_21 = P0_21,
    SIO_22 = P0_22,
    SIO_23 = P0_23,
    SIO_24 = P0_24,
    SIO_25 = P0_25,
    SIO_26 = P0_26,
    SIO_27 = P0_27,
    SIO_28 = P0_28,
    SIO_29 = P0_29,
    SIO_30 = P0_30,
    SIO_0 = P0_0,

    // Not connected
    NC = (int)0xFFFFFFFF,

    //Mbed MTB pin defines.
    P_1 = NC,
    P_2 = SIO_1,
    P_3 = NC,
    P_4 = SIO_2,
    P_5 = SIO_3,
    P_6 = SIO_4,
    P_7 = SIO_5,
    P_8 = SIO_6,
    P_9 = SIO_7,
    P_10 = NC,
    P_11 = NC,
    P_12 = SIO_8,
    P_13 = SIO_9,
    P_14 = SIO_10,
    P_15 = SIO_11,
    P_16 = SIO_12,
    P_17 = NC,
    P_18 = SIO_13,
    P_19 = SIO_14,
    P_20 = SIO_15,
    P_21 = SIO_16,
    P_22 = NC,
    P_23 = NC,
    P_24 = SIO_17,
    P_25 = SIO_18,
    P_26 = SIO_19,
    P_27 = SIO_20,
    P_28 = NC,
    P_29 = NC,
    P_30 = NC,
    P_31 = NC,
    P_32 = SIO_21,
    P_33 = SIO_22,
    P_34 = SIO_23,
    P_35 = SIO_24,
    P_36 = SIO_25,
    P_37 = NC,
    P_38 = SIO_26,
    P_39 = SIO_27,
    P_40 = SIO_28,
    P_41 = SIO_29,
    P_42 = SIO_30,
    P_43 = NC,
    P_44 = SIO_0,

    //MTB aliases
    GP0    =  SIO_30,
    GP1    = SIO_0,
    AIN0  = SIO_1,
    AIN1  = SIO_2,
    AIN2  = SIO_3,
    GP2    = SIO_7,
    GP3    = SIO_13,
    UART1_TX = SIO_17,
    UART1_RX = SIO_18,
    UART2_TX = SIO_25,
    UART2_RX = SIO_26,
    GP4     = SIO_14,
    GP5     = SIO_15, //A0 for LCD on MTB
    GP6     = SIO_16, //RESET for LCD on MTB
    GP7     = SIO_19,
    GP8     = SIO_20,

    //LEDs
    LED1 = SIO_4,
    LED2 = SIO_5,
    LED3 = SIO_6,
    
    //Standardized button name
    BUTTON1 = SIO_30,
     
    //Nordic SDK pin names 
    RX_PIN_NUMBER = p22,
    TX_PIN_NUMBER = p21,
    CTS_PIN_NUMBER = p24,
    RTS_PIN_NUMBER = p23,

    // mBed interface Pins
    USBTX = MBED_CONF_TARGET_USB_TX,
    USBRX = MBED_CONF_TARGET_USB_RX,

    SPI_MOSI0 = SIO_10,
    SPI_MISO0 = SIO_11,
    SPI_SS0 = SIO_19, //CS for LCD on MTB
    SPI_SCK0 = SIO_12,

    SPI_MOSI1 = SIO_27,
    SPI_MISO1 = SIO_28,
    SPI_SS1 = SIO_7, //CS for SD card on MTB
    SPI_SCK1 = SIO_29,

    //Default SPI
    SPIS_PSELMOSI = SPI_MOSI0,
    SPIS_PSELMISO = SPI_MISO0,
    SPIS_PSELSS = SPI_SS0,    
    SPIS_PSELSCK = SPI_SCK0,

    SPI_MOSI = SPI_MOSI0,
    SPI_MISO = SPI_MISO0,
    SPI_SS = SPI_SS0,    
    SPI_SCK = SPI_SCK0,

    I2C_SDA0 = SIO_8,
    I2C_SCL0 = SIO_9,

} PinName;

typedef enum {
    PullNone = 0,
    PullDown = 1,
    PullUp = 3,
    PullDefault = PullUp
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
