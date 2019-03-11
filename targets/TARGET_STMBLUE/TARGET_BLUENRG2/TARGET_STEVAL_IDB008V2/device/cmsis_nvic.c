/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2019, STMicroelectronics
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 *
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 *
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 *
 * either express or implied.
 *
 * See the License for the specific language governing permissions and limitations under the License.
 */

#include "cmsis_nvic.h"
#include "BlueNRG1_flash.h"

inline void NVIC_SetVector(IRQn_Type IRQn, uint32_t vector) {

    // Copy from flash and switch to dynamic vectors if first time called
    if(FLASH->CONFIG == FLASH_PREMAP_MAIN){
    	volatile uint32_t *old_vectors = (uint32_t *)NVIC_FLASH_VECTOR_ADDRESS;
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
