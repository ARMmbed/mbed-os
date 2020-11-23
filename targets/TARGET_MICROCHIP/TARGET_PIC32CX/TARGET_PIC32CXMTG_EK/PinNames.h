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

#ifdef __cplusplus
extern "C" {
#endif

#define PORT_SHIFT  5

typedef enum {
	PIO_PA0_IDX = (0 << PORT_SHIFT | 0),
	PIO_PA1_IDX = (0 << PORT_SHIFT | 1),
	PIO_PA4_IDX = (0 << PORT_SHIFT | 4), 
	PIO_PA5_IDX = (0 << PORT_SHIFT | 5),  
	PIO_PA6_IDX = (0 << PORT_SHIFT | 6), 
	PIO_PA7_IDX = (0 << PORT_SHIFT | 7),
	PIO_PA8_IDX = (0 << PORT_SHIFT | 8),
	PIO_PA9_IDX = (0 << PORT_SHIFT | 8), 
	PIO_PA12_IDX = (0 << PORT_SHIFT | 12), 
	PIO_PA13_IDX = (0 << PORT_SHIFT | 13), 
	PIO_PA14_IDX = (0 << PORT_SHIFT | 14), 
	PIO_PA15_IDX = (0 << PORT_SHIFT | 15), 
	PIO_PA21_IDX = (0 << PORT_SHIFT | 21), 
	PIO_PA22_IDX = (0 << PORT_SHIFT | 22), 
	PIO_PA23_IDX = (0 << PORT_SHIFT | 23),  
	PIO_PA24_IDX = (0 << PORT_SHIFT | 24), 
	PIO_PA25_IDX = (0 << PORT_SHIFT | 25), 
	PIO_PA26_IDX = (0 << PORT_SHIFT | 26), 
	PIO_PA27_IDX = (0 << PORT_SHIFT | 27),
	PIO_PA28_IDX = (0 << PORT_SHIFT | 28), 
	PIO_PA29_IDX = (0 << PORT_SHIFT | 29),
	PIO_PA30_IDX = (0 << PORT_SHIFT | 30),  
	PIO_PA31_IDX = (0 << PORT_SHIFT | 31), 
	PIO_PB0_IDX = (1 << PORT_SHIFT | 0),   
	PIO_PB1_IDX = (1 << PORT_SHIFT | 1),     
	PIO_PB2_IDX = (1 << PORT_SHIFT | 2),
	PIO_PB3_IDX = (1 << PORT_SHIFT | 3),
	PIO_PB4_IDX = (1 << PORT_SHIFT | 4), 
	PIO_PB5_IDX = (1 << PORT_SHIFT | 5),
	PIO_PB6_IDX = (1 << PORT_SHIFT | 6),
	PIO_PB7_IDX = (1 << PORT_SHIFT | 7), 
	PIO_PB8_IDX = (1 << PORT_SHIFT | 8),
	PIO_PB9_IDX = (1 << PORT_SHIFT | 9), 
	PIO_PB10_IDX = (1 << PORT_SHIFT | 10),
	PIO_PB11_IDX = (1 << PORT_SHIFT | 11),
	PIO_PB12_IDX = (1 << PORT_SHIFT | 12),
	PIO_PB18_IDX = (1 << PORT_SHIFT | 18), 
	PIO_PB19_IDX = (1 << PORT_SHIFT | 19),
	PIO_PB20_IDX = (1 << PORT_SHIFT | 20),
	PIO_PB21_IDX = (1 << PORT_SHIFT | 21), 
	PIO_PB22_IDX = (1 << PORT_SHIFT | 22), 
	PIO_PB23_IDX = (1 << PORT_SHIFT | 23), 
	PIO_PB24_IDX = (1 << PORT_SHIFT | 24), 
	PIO_PB25_IDX = (1 << PORT_SHIFT | 25),
	PIO_PB26_IDX = (1 << PORT_SHIFT | 26), 
	PIO_PC0_IDX = (2 << PORT_SHIFT | 0),
	PIO_PC2_IDX = (2 << PORT_SHIFT | 2),   
	PIO_PC3_IDX = (2 << PORT_SHIFT | 3),  
	PIO_PC4_IDX = (2 << PORT_SHIFT | 4), 
	PIO_PC5_IDX = (2 << PORT_SHIFT | 5),
	PIO_PC6_IDX = (2 << PORT_SHIFT | 6), 
	PIO_PC7_IDX = (2 << PORT_SHIFT | 7), 
	PIO_PC10_IDX = (2 << PORT_SHIFT | 10), 
	PIO_PC11_IDX = (2 << PORT_SHIFT | 11), 
	PIO_PC12_IDX = (2 << PORT_SHIFT | 12),
	PIO_PC13_IDX = (2 << PORT_SHIFT | 13),  
	PIO_PC14_IDX = (2 << PORT_SHIFT | 14),   
	PIO_PC15_IDX = (2 << PORT_SHIFT | 15), 
	PIO_PC22_IDX = (2 << PORT_SHIFT | 22), 
	PIO_PD0_IDX = (3 << PORT_SHIFT | 0),  
	PIO_PD1_IDX = (3 << PORT_SHIFT | 1),
	PIO_PD2_IDX = (3 << PORT_SHIFT | 2),  
	PIO_PD4_IDX = (3 << PORT_SHIFT | 4), 
	PIO_PD5_IDX = (3 << PORT_SHIFT | 5), 
	PIO_PD6_IDX = (3 << PORT_SHIFT | 6), 
	PIO_PD7_IDX = (3 << PORT_SHIFT | 7), 
	PIO_PD8_IDX = (3 << PORT_SHIFT | 8),  
	PIO_PD9_IDX = (3 << PORT_SHIFT | 9),
	PIO_PD10_IDX = (3 << PORT_SHIFT | 10), 
	PIO_PD11_IDX = (3 << PORT_SHIFT | 11),  
	PIO_PD14_IDX = (3 << PORT_SHIFT | 14), 
	PIO_PD15_IDX = (3 << PORT_SHIFT | 15), 
	PIO_PD16_IDX = (3 << PORT_SHIFT | 16), 
	PIO_PD17_IDX = (3 << PORT_SHIFT | 17), 
	PIO_PD18_IDX = (3 << PORT_SHIFT | 18),   
	PIO_PD19_IDX = (3 << PORT_SHIFT | 19),

	/* Board leds */
    LED_GREEN = PIO_PD16_IDX,
	LED_BLUE  = PIO_PD19_IDX,

    /* mbed original LED naming */
    LED1 = LED_GREEN,
    LED2 = LED_BLUE,

    /* Push buttons */
    SW2 = PIO_PA7_IDX,
    /* Standardized button names */
    BUTTON1 = SW2,

    /* Serial */
    SERIAL_TX = PIO_PA4_IDX,
    SERIAL_RX = PIO_PA5_IDX,

    /* Not connected */
    NC = (int)0xFFFFFFFF
} PinName;

#ifdef __cplusplus
}
#endif

#endif
