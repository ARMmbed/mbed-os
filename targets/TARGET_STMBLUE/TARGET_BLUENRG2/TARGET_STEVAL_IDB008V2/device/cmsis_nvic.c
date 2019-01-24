/*
 * Author: Antonio O. 06/11/2018
 */

#include "cmsis_nvic.h"
#include "BlueNRG1_flash.h"

void NVIC_SetVector(IRQn_Type IRQn, uint32_t vector) {

	uint32_t *old_vectors = (uint32_t *)NVIC_FLASH_VECTOR_ADDRESS;
    // Copy from flash and switch to dynamic vectors if first time called
    if(FLASH->CONFIG == FLASH_PREMAP_MAIN){
    	for (int i = 0; i < NVIC_NUM_VECTORS; i++) {
    		*((uint32_t *)(NVIC_RAM_VECTOR_ADDRESS + (i*4))) = old_vectors[i];
    	}
    	FLASH->CONFIG = FLASH_REMAP_RAM;
    }
    // Set the vector
    *(uint32_t *)(NVIC_RAM_VECTOR_ADDRESS + (IRQn*4) + (NVIC_USER_IRQ_OFFSET*4)) = vector;
}

uint32_t NVIC_GetVector(IRQn_Type IRQn) {
    uint32_t *vectors = (uint32_t*)NVIC_RAM_VECTOR_ADDRESS;
    // Return the vector
    return vectors[(uint32_t)IRQn + NVIC_USER_IRQ_OFFSET];
}
