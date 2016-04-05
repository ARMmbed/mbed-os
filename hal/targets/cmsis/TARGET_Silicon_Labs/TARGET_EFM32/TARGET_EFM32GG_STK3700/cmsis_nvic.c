/* mbed Microcontroller Library - cmsis_nvic for EFM32
 * Copyright (c) 2011 ARM Limited. All rights reserved.
 *
 * CMSIS-style functionality to support dynamic vectors
 */
#include "cmsis_nvic.h"

#if (defined (__GNUC__) && (!defined(__CC_ARM)))
extern uint32_t __start_vector_table__;		  // Dynamic vector positioning in GCC
#endif

#define NVIC_RAM_VECTOR_ADDRESS (0x20000000)  // Vectors positioned at start of RAM
#define NVIC_FLASH_VECTOR_ADDRESS (0x0)       // Initial vector position in flash

void NVIC_SetVector(IRQn_Type IRQn, uint32_t vector) {
    uint32_t *vectors = (uint32_t*)SCB->VTOR;
    uint32_t i;

    // Copy and switch to dynamic vectors if the first time called
    // For GCC, use dynamic vector table placement since otherwise we run into an alignment conflict
#if (defined (__GNUC__) && (!defined(__CC_ARM)))
    if (SCB->VTOR == NVIC_FLASH_VECTOR_ADDRESS) {
        uint32_t *old_vectors = vectors;
        vectors = (uint32_t*)(&__start_vector_table__);
        for (i=0; i<NVIC_NUM_VECTORS; i++) {
            vectors[i] = old_vectors[i];
        }
        SCB->VTOR = (uint32_t)(&__start_vector_table__);
    }
    // Other compilers don't matter as much...
#else
    if (SCB->VTOR == NVIC_FLASH_VECTOR_ADDRESS) {
		uint32_t *old_vectors = vectors;
		vectors = (uint32_t*)(NVIC_RAM_VECTOR_ADDRESS);
		for (i=0; i<NVIC_NUM_VECTORS; i++) {
			vectors[i] = old_vectors[i];
		}
		SCB->VTOR = (uint32_t)(NVIC_RAM_VECTOR_ADDRESS);
	}
#endif
    vectors[IRQn + 16] = vector;
}

uint32_t NVIC_GetVector(IRQn_Type IRQn) {
    uint32_t *vectors = (uint32_t*)SCB->VTOR;
    return vectors[IRQn + 16];
}
