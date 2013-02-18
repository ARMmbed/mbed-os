/* mbed Microcontroller Library - cmsis_nvic for LPC2368
 * Copyright (c) 2009 ARM Limited. All rights reserved.
 *
 * CMSIS-style functionality to support dynamic vectors
 */ 

#include "cmsis_nvic.h"

void NVIC_SetVector(IRQn_Type IRQn, uint32_t vector) {
    LPC_VIC->VectAddr[(int)IRQn] = vector;
}

uint32_t NVIC_GetVector(IRQn_Type IRQn) {
    return LPC_VIC->VectAddr[(int)IRQn];
}

