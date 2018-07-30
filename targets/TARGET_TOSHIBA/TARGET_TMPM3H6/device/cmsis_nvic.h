/* mbed Microcontroller Library - cmsis_nvic for TMPM3H6
 * Copyright (c) 2011 ARM Limited. All rights reserved.
 *
 * CMSIS-style functionality to support dynamic vectors
 */

#ifndef MBED_CMSIS_NVIC_H
#define MBED_CMSIS_NVIC_H

#define NVIC_FLASH_VECTOR_ADDRESS 0x00000000
#define NVIC_USER_IRQ_NUMBER 117
#define NVIC_NUM_VECTORS            (16 + 117)   // CORE + MCU Peripherals
#define NVIC_RAM_VECTOR_ADDRESS     0x20000000    // Location of vectors in RAM

#endif
