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

#define PORT_SHIFT  5

typedef enum {
    // MPS2 EXP Pin Names
        EXP0 = 0 ,
        EXP1 = 4 ,
        EXP2 = 2 ,
        EXP3 = 3 ,
        EXP4 = 1 ,
        EXP5 = 15,
        EXP6 = 5 ,
        EXP7 = 6 ,
        EXP8 = 7 ,
        EXP9 = 8 ,
        EXP10 =9 ,
        EXP11 =13,
        EXP12 =10,
        EXP13 =11,
        EXP14 =12,
        EXP15 =14,
        EXP16 =18,
        EXP17 =19,
        EXP18 =20,
        EXP19 =21,
        EXP20 =52,
        EXP21 =53,
        EXP22 =54,
        EXP23 =55,
        EXP24 =56,
        EXP25 =57,
        
        EXP26 =16,
        EXP27 =25,
        EXP28 =24,
        EXP29 =31,
        EXP30 =17,
        EXP31 =23,
        EXP32 =27,
        EXP33 =30,
        EXP34 =26,
        EXP35 =28,
        EXP36 =29,
        EXP37 =58,
        EXP38 =48,
        EXP39 =49,
        EXP40 =50,
        EXP41 =22,
        EXP42 =59,
        EXP43 =60,
        EXP44 =51,
        EXP45 =61,
        EXP46 =62,
        EXP47 =63,
        EXP48 =64,
        EXP49 =65,
        EXP50 =66,
        EXP51 =67,
        
// Other mbed Pin Names

        //LEDs on mps2
        //user leds
        USERLED1 = 100,
        USERLED2 = 101,
        //user switches
        USERSW1  = 110,
        USERSW2  = 111,
        
        //mcc leds
        LED1 = 200,
        LED2 = 201,
        LED3 = 202,
        LED4 = 203,
        LED5 = 204,
        LED6 = 205,
        LED7 = 206,
        LED8 = 207,
        
        //MCC Switches
        SW1 = 210,
        SW2 = 211,
        SW3 = 212,
        SW4 = 213,
        SW5 = 214,
        SW6 = 215,
        SW7 = 216,
        SW8 = 217,
        
        //MPS2 SPI header pins j21
        MOSI_SPI = 300,
        MISO_SPI = 301,
        SCLK_SPI = 302,
        SSEL_SPI = 303,
        
        //MPS2 CLCD SPI
        CLCD_MOSI = 304,
        CLCD_MISO = 305,
        CLCD_SCLK = 306,
        CLCD_SSEL = 307,
        CLCD_RESET = 308,
        CLCD_RS = 309,
        CLCD_RD = 310,
        CLCD_BL_CTRL = 311,
        
        //MPS2 shield 0 SPI
        SHIELD_0_SPI_SCK = 320, 
        SHIELD_0_SPI_MOSI = 321,
        SHIELD_0_SPI_MISO = 322,
        SHIELD_0_SPI_nCS = 323, 

        //MPS2 shield 1 SPI
        SHIELD_1_SPI_SCK = 331, 
        SHIELD_1_SPI_MOSI = 332,
        SHIELD_1_SPI_MISO = 333,
        SHIELD_1_SPI_nCS = 334, 
        
        //MPS2 shield ADC SPI
        ADC_MOSI = 650,
        ADC_MISO = 651,
        ADC_SCLK = 652,
        ADC_SSEL = 653,
        
        //MPS2 Uart
        USBTX  = 400,
        USBRX  = 401,
        XB_TX = 402,
        XB_RX = 403,
        SH0_TX = 404,
        SH0_RX = 405,
        SH1_TX = 406,
        SH1_RX = 407,
        
        //MPS2 I2C touchscreen and audio
        TSC_SDA = 500,
        TSC_SCL = 501,
        AUD_SDA = 502,
        AUD_SCL = 503,
    
        //MPS2 I2C for shield
        SHIELD_0_SDA = 504, 
        SHIELD_0_SCL = 505, 
        SHIELD_1_SDA = 506,
        SHIELD_1_SCL = 507,
        
        //MPS2 shield Analog pins
        A0_0 = 600,
        A0_1 = 601,
        A0_2 = 602,
        A0_3 = 603,
        A0_4 = 604,
        A0_5 = 605,
        A1_0 = 606,
        A1_1 = 607,
        A1_2 = 608,
        A1_3 = 609,
        A1_4 = 610,
        A1_5 = 611,
        //MPS2 Shield Digital pins
        D0_0  = EXP0,
        D0_1  = EXP4,
        D0_2  = EXP2,
        D0_3  = EXP3,
        D0_4  = EXP1,
        D0_5  = EXP6,
        D0_6  = EXP7,
        D0_7  = EXP8,
        D0_8  = EXP9,
        D0_9  = EXP10,
        D0_10 = EXP12,
        D0_11 = EXP13,
        D0_12 = EXP14,
        D0_13 = EXP11,
        D0_14 = EXP15,
        D0_15 = EXP5,
    
        D1_0  = EXP26,
        D1_1  = EXP30,
        D1_2  = EXP28,
        D1_3  = EXP29,
        D1_4  = EXP27,
        D1_5  = EXP32,
        D1_6  = EXP33,
        D1_7  = EXP34,
        D1_8  = EXP35,
        D1_9  = EXP36,
        D1_10 = EXP38,
        D1_11 = EXP39,
        D1_12 = EXP40,
        D1_13 = EXP44,
        D1_14 = EXP41,
        D1_15 = EXP31,
        
        // Not connected
        NC = (int)0xFFFFFFFF,
} PinName;


typedef enum {
    PullUp = 2,
    PullDown = 1,
    PullNone = 0,
    Repeater = 3,
    OpenDrain = 4,
    PullDefault = PullDown
} PinMode;

#ifdef __cplusplus
}
#endif

#endif

