/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2020 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#include "cmsis.h"
#include "nvic_addr.h"

#define NVIC_USER_IRQ_OFFSET  16

void NVIC_SetVector(IRQn_Type IRQn, uint32_t vector)
{
    int i;

    // Copy and switch to dynamic vectors if first time called
    if ((SYSCFG->CFGR1 & SYSCFG_CFGR1_MEM_MODE) != SYSCFG_CFGR1_MEM_MODE) {
        uint32_t *old_vectors = (uint32_t *)NVIC_FLASH_VECTOR_ADDRESS;
        for (i = 0; i < NVIC_NUM_VECTORS; i++) {
            *((uint32_t *)(NVIC_RAM_VECTOR_ADDRESS + (i * 4))) = old_vectors[i];
        }
        SYSCFG->CFGR1 |= SYSCFG_CFGR1_MEM_MODE; // Embedded SRAM mapped at 0x00000000
    }

    // Set the vector
    *((uint32_t *)(NVIC_RAM_VECTOR_ADDRESS + (IRQn * 4) + (NVIC_USER_IRQ_OFFSET * 4))) = vector;
}

uint32_t NVIC_GetVector(IRQn_Type IRQn)
{
    uint32_t *vectors = (uint32_t *)NVIC_RAM_VECTOR_ADDRESS;
    // Return the vector
    return vectors[IRQn + 16];
}
