/* mbed Microcontroller Library - cmsis_nvic for EFM32
 * Copyright (c) 2011 ARM Limited. All rights reserved.
 *
 * CMSIS-style functionality to support dynamic vectors
 */
#include "cmsis_nvic.h"

#if (defined (__GNUC__) && (!defined(__CC_ARM)))
extern uint32_t __start_vector_table__;		  // Dynamic vector positioning in GCC
#endif

// Vectors positioned at start of RAM
#define NVIC_RAM_VECTOR_ADDRESS   (0x20000000)
// Initial vector position in flash (after bootloader if it exists)
#if defined(__ICCARM__)
    #pragma section=".intvec"
    #define NVIC_FLASH_VECTOR_ADDRESS   ((uint32_t)__section_begin(".intvec"))
#elif defined(__CC_ARM)
    extern uint32_t Load$$LR$$LR_IROM1$$Base[];
    #define NVIC_FLASH_VECTOR_ADDRESS   ((uint32_t)Load$$LR$$LR_IROM1$$Base)
#elif defined(__GNUC__)
    extern uint32_t vectors[];
    #define NVIC_FLASH_VECTOR_ADDRESS   ((uint32_t)vectors)
#else
    #error "Flash vector address not set for this toolchain"
#endif

void __NVIC_SetVector(IRQn_Type IRQn, uint32_t vector) {
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

uint32_t __NVIC_GetVector(IRQn_Type IRQn) {
    uint32_t *vectors = (uint32_t*)SCB->VTOR;
    return vectors[IRQn + 16];
}
