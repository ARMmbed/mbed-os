/* mbed Microcontroller Library - cmsis_nvic
 * Copyright (c) 2009-2011 ARM Limited. All rights reserved.
 *
 * CMSIS-style functionality to support dynamic vectors
 */ 

#ifndef MBED_CMSIS_NVIC_H
#define MBED_CMSIS_NVIC_H

// CORE: 16 vectors (= 64 bytes from 0x00 to 0x3F)
// MCU Peripherals: 43 vectors (= 172 bytes from 0x40 to 0xEB)
// Total:  236 bytes to be reserved in RAM (see scatter file)
#define NVIC_NUM_VECTORS      (16 + 43)
#define NVIC_USER_IRQ_OFFSET  16

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

void NVIC_SetVector(IRQn_Type IRQn, uint32_t vector);
uint32_t NVIC_GetVector(IRQn_Type IRQn);

#ifdef __cplusplus
}
#endif

#endif
