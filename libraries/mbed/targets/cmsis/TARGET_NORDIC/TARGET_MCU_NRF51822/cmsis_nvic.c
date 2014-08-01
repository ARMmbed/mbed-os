/* mbed Microcontroller Library - cmsis_nvic for LCP407x_8x
 * Copyright (c) 2009-2011 ARM Limited. All rights reserved.
 *
 * CMSIS-style functionality to support dynamic vectors
 */ 
#include "cmsis_nvic.h"

/* In the M0, there is no VTOR. In the LPC range such as the LPC11U,
 * whilst the vector table may only be something like 48 entries (192 bytes, 0xC0), 
 * the SYSMEMREMAP register actually remaps the memory from 0x10000000-0x100001FF 
 * to adress 0x0-0x1FF. In this case, RAM can be addressed at both 0x10000000 and 0x0
 * 
 * If we just copy the vectors to RAM and switch the SYSMEMMAP, any accesses to FLASH
 * above the vector table before 0x200 will actually go to RAM. So we need to provide 
 * a solution where the compiler gets the right results based on the memory map
 *
 * Option 1 - We allocate and copy 0x200 of RAM rather than just the table
 *  - const data and instructions before 0x200 will be copied to and fetched/exec from RAM
 *  - RAM overhead: 0x200 - 0xC0 = 320 bytes, FLASH overhead: 0
 * 
 * Option 2 - We pad the flash to 0x200 to ensure the compiler doesn't allocate anything there  
 *  - No flash accesses will go to ram, as there will be nothing there
 *  - RAM only needs to be allocated for the vectors, as all other ram addresses are normal
 *  - RAM overhead: 0, FLASH overhead: 320 bytes
 *
 * Option 2 is the one to go for, as RAM is the most valuable resource
 */


#define NVIC_RAM_VECTOR_ADDRESS   (0x10000000)  // Location of vectors in RAM
#define NVIC_FLASH_VECTOR_ADDRESS (0x0)       // Initial vector position in flash
/*
void NVIC_SetVector(IRQn_Type IRQn, uint32_t vector) {
    uint32_t *vectors = (uint32_t*)SCB->VTOR;
    uint32_t i;

    // Copy and switch to dynamic vectors if the first time called
    if (SCB->VTOR == NVIC_FLASH_VECTOR_ADDRESS) {
        uint32_t *old_vectors = vectors;
        vectors = (uint32_t*)NVIC_RAM_VECTOR_ADDRESS;
        for (i=0; i<NVIC_NUM_VECTORS; i++) {
            vectors[i] = old_vectors[i];
        }
        SCB->VTOR = (uint32_t)NVIC_RAM_VECTOR_ADDRESS;
    }
    vectors[IRQn + 16] = vector;
}

uint32_t NVIC_GetVector(IRQn_Type IRQn) {
    uint32_t *vectors = (uint32_t*)SCB->VTOR;
    return vectors[IRQn + 16];
}*/

void NVIC_SetVector(IRQn_Type IRQn, uint32_t vector) {
   // int i;
    // Space for dynamic vectors, initialised to allocate in R/W
    static volatile uint32_t* vectors = (uint32_t*)NVIC_RAM_VECTOR_ADDRESS;
    /*
    // Copy and switch to dynamic vectors if first time called
    if((LPC_SYSCON->SYSMEMREMAP & 0x3) != 0x1) {     
      uint32_t *old_vectors = (uint32_t *)0;         // FLASH vectors are at 0x0
      for(i = 0; i < NVIC_NUM_VECTORS; i++) {    
            vectors[i] = old_vectors[i];
        }
        LPC_SYSCON->SYSMEMREMAP = 0x1; // Remaps 0x0-0x1FF FLASH block to RAM block
    }*/

    // Set the vector 
    vectors[IRQn + 16] = vector; 
}

uint32_t NVIC_GetVector(IRQn_Type IRQn) {
    // We can always read vectors at 0x0, as the addresses are remapped
    uint32_t *vectors = (uint32_t*)0; 

    // Return the vector
    return vectors[IRQn + 16];
}
