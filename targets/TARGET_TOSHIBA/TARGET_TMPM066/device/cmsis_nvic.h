/* mbed Microcontroller Library - cmsis_nvic for TMPM066
 * Copyright (c) 2011 ARM Limited. All rights reserved.
 *
 * CMSIS-style functionality to support dynamic vectors
 */

#ifndef MBED_CMSIS_NVIC_H
#define MBED_CMSIS_NVIC_H

#include "cmsis.h"

// CORE: 16 vectors = 64 bytes from 0x00 to 0x3F
// MCU Peripherals: 32 vectors = 128 bytes from 0x40 to 0xBF
// Total: 48 vectors = 192 bytes (0xC0) to be reserved in RAM
#define NVIC_NUM_VECTORS            (16 + 32)   // CORE + MCU Peripherals
#define NVIC_USER_IRQ_OFFSET        16

#ifdef __cplusplus
extern "C" {
#endif

/** Set the ISR for IRQn
 *
 * Sets an Interrupt Service Routine vector for IRQn; if the feature is available, the vector table is relocated to SRAM
 * the first time this function is called
 * @param[in] IRQn   The Interrupt Request number for which a vector will be registered
 * @param[in] vector The ISR vector to register for IRQn
 */
void NVIC_SetVector(IRQn_Type IRQn, uint32_t vector);

/** Get the ISR registered for IRQn
 *
 * Reads the Interrupt Service Routine currently registered for IRQn
 * @param[in] IRQn   The Interrupt Request number the vector of which will be read
 * @return           Returns the ISR registered for IRQn
 */
uint32_t NVIC_GetVector(IRQn_Type IRQn);

#ifdef __cplusplus
}
#endif

#endif
