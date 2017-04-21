/* mbed Microcontroller Library - cmsis_nvic
 * Copyright (c) 2009-2011 ARM Limited. All rights reserved.
 *
 * CMSIS-style functionality to support dynamic vectors
 */ 

#ifndef MBED_CMSIS_NVIC_H
#define MBED_CMSIS_NVIC_H

#include "cmsis.h"

#define NVIC_NUM_VECTORS      (16 + EXT_IRQ_COUNT)   // CORE + MCU Peripherals
#define NVIC_USER_IRQ_OFFSET  16

#if __CORTEX_M == 0
#define __NVIC_SetVector NVIC_SetVector
#define __NVIC_GetVector NVIC_GetVector
#endif

#ifdef __cplusplus
extern "C" {
#endif

void __NVIC_SetVector(IRQn_Type IRQn, uint32_t vector);
uint32_t __NVIC_GetVector(IRQn_Type IRQn);

#ifdef __cplusplus
}
#endif

#endif
