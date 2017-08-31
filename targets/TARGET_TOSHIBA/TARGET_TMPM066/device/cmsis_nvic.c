/* mbed Microcontroller Library - cmsis_nvic for TMPM066
 * Copyright (c) 2011 ARM Limited. All rights reserved.
 *
 * CMSIS-style functionality to support dynamic vectors
 */
#include "cmsis_nvic.h"

#define NVIC_RAM_VECTOR_ADDRESS      (0x20000000)  // Location of vectors in RAM
#define NVIC_FLASH_VECTOR_ADDRESS    (0x00000000)  // Initial vector position in flash

void NVIC_SetVector(IRQn_Type IRQn, uint32_t vector)
{
    static int vector_copied = 0;
    uint32_t *vectors = (uint32_t *) NVIC_FLASH_VECTOR_ADDRESS;
    uint32_t i;

    // Copy and switch to dynamic vectors if the first time called
    if (vector_copied == 0) {
        uint32_t *old_vectors = vectors;
        vectors = (uint32_t *) NVIC_RAM_VECTOR_ADDRESS;
        for (i = 0; i < NVIC_NUM_VECTORS; i++) {
            vectors[i] = old_vectors[i];
        }
        vector_copied = 1;
    }
    vectors[IRQn + NVIC_USER_IRQ_OFFSET] = vector;
}

uint32_t NVIC_GetVector(IRQn_Type IRQn)
{
    uint32_t *vectors = (uint32_t *) NVIC_RAM_VECTOR_ADDRESS;
    return vectors[IRQn + NVIC_USER_IRQ_OFFSET];
}
