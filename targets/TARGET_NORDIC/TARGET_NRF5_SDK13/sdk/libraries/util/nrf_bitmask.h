/*
 * Copyright (c) 2006 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor. The use,
 * copying, transfer or disclosure of such information is prohibited except by express written
 * agreement with Nordic Semiconductor.
 *
 */


#ifndef NRF_BITMASK_H
#define NRF_BITMASK_H

#include "compiler_abstraction.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BITMASK_BYTE_GET(abs_bit) ((abs_bit)/8)
#define BITMASK_RELBIT_GET(abs_bit) ((abs_bit) & 0x00000007)

/**
 * Function for checking if bit in the multi-byte bit mask is set.
 *
 * @param bit    Bit index.
 * @param p_mask A pointer to mask with bit fields.
 *
 * @return 0 if bit is not set, positive value otherwise.
 */
__STATIC_INLINE uint32_t nrf_bitmask_bit_is_set(uint32_t bit, void const * p_mask)
{
    uint8_t const * p_mask8 = (uint8_t const *)p_mask;
    uint32_t byte_idx = BITMASK_BYTE_GET(bit);
    bit = BITMASK_RELBIT_GET(bit);
    return (1 << bit) & p_mask8[byte_idx];
}

/**
 * Function for setting a bit in the multi-byte bit mask.
 *
 * @param bit    Bit index.
 * @param p_mask A pointer to mask with bit fields.
 */
__STATIC_INLINE void nrf_bitmask_bit_set(uint32_t bit, void * p_mask)
{
    uint8_t * p_mask8 = (uint8_t *)p_mask;
    uint32_t byte_idx = BITMASK_BYTE_GET(bit);
    bit = BITMASK_RELBIT_GET(bit);
    p_mask8[byte_idx] |= (1 << bit);
}

/**
 * Function for clearing a bit in the multi-byte bit mask.
 *
 * @param bit    Bit index.
 * @param p_mask A pointer to mask with bit fields.
 */
__STATIC_INLINE void nrf_bitmask_bit_clear(uint32_t bit, void * p_mask)
{
    uint8_t * p_mask8 = (uint8_t *)p_mask;
    uint32_t byte_idx = BITMASK_BYTE_GET(bit);
    bit = BITMASK_RELBIT_GET(bit);
    p_mask8[byte_idx] &= ~(1 << bit);
}

/**
 * Function for performing bitwise OR operation on two multi-byte bit masks.
 *
 * @param p_mask1    A pointer to the first bit mask.
 * @param p_mask2    A pointer to the second bit mask.
 * @param p_mask_out A pointer to the output bit mask.
 * @param length     Length of output mask in bytes.
 */
__STATIC_INLINE void nrf_bitmask_masks_or(void const *   p_mask1,
                                          void const *   p_mask2,
                                          void *         p_out_mask,
                                          uint32_t       length)
{
    uint8_t const * p_mask8_1 = (uint8_t const *)p_mask1;
    uint8_t const * p_mask8_2 = (uint8_t const *)p_mask2;
    uint8_t * p_mask8_out = (uint8_t *)p_out_mask;
    uint32_t i;
    for (i = 0; i < length; i++)
    {
        p_mask8_out[i] = p_mask8_1[i] | p_mask8_2[i];
    }
}

/**
 * Function for performing bitwise AND operation on two multi-byte bit masks.
 *
 * @param p_mask1    A pointer to the first bit mask.
 * @param p_mask2    A pointer to the second bit mask.
 * @param p_mask_out A pointer to the output bit mask.
 * @param length     Length of output mask in bytes.
 */
__STATIC_INLINE void nrf_bitmask_masks_and(void const *   p_mask1,
                                           void const *   p_mask2,
                                           void *         p_out_mask,
                                           uint32_t       length)
{
    uint8_t const * p_mask8_1 = (uint8_t const *)p_mask1;
    uint8_t const * p_mask8_2 = (uint8_t const *)p_mask2;
    uint8_t * p_mask8_out = (uint8_t *)p_out_mask;
    uint32_t i;
    for (i = 0; i < length; i++)
    {
        p_mask8_out[i] = p_mask8_1[i] & p_mask8_2[i];
    }
}

#ifdef __cplusplus
}
#endif

#endif //NRF_BITMASK_H
