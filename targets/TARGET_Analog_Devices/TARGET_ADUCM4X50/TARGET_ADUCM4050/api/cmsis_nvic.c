#include "cmsis_nvic.h"

void NVIC_SetVector(IRQn_Type IRQn, uint32_t vector)
{
    static volatile uint32_t *vectors = (uint32_t *)NVIC_RAM_VECTOR_ADDRESS;

    vectors[IRQn + 16] = vector;
}

uint32_t NVIC_GetVector(IRQn_Type IRQn)
{
    uint32_t *vectors = (uint32_t*)NVIC_RAM_VECTOR_ADDRESS;
    // Return the vector
    return vectors[IRQn + 16];
}
