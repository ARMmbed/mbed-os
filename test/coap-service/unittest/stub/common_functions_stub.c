/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 */
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "ns_types.h"
//#include "common_functions.h"
#define COMMON_FUNCTIONS_FN

#include <string.h>

bool bitsequal(const uint8_t *a, const uint8_t *b, uint_fast8_t bits)
{
    return true;
}

uint8_t *bitcopy(uint8_t *restrict dst, const uint8_t *restrict src, uint_fast8_t bits)
{
    return NULL;
}

COMMON_FUNCTIONS_FN uint8_t *common_write_64_bit(uint64_t value, uint8_t ptr[__static 8])
{
    return NULL;
}

COMMON_FUNCTIONS_FN uint64_t common_read_64_bit(const uint8_t data_buf[__static 8])
{
    return 0;
}

COMMON_FUNCTIONS_FN uint8_t *common_write_32_bit(uint32_t value, uint8_t ptr[__static 4])
{
    return NULL;
}

COMMON_FUNCTIONS_FN uint32_t common_read_32_bit(const uint8_t data_buf[__static 4])
{
    return 0;
}

COMMON_FUNCTIONS_FN uint8_t *common_write_32_bit_inverse(uint32_t value, uint8_t ptr[__static 4])
{
    return NULL;
}

COMMON_FUNCTIONS_FN uint32_t common_read_32_bit_inverse(const uint8_t data_buf[__static 4])
{
    return 0;
}

COMMON_FUNCTIONS_FN uint8_t *common_write_24_bit(uint_fast24_t value, uint8_t ptr[__static 3])
{
    return NULL;
}

COMMON_FUNCTIONS_FN uint_fast24_t common_read_24_bit(const uint8_t data_buf[__static 3])
{
    return 0;
}

COMMON_FUNCTIONS_FN uint8_t *common_write_16_bit(uint16_t value, uint8_t ptr[__static 2])
{
    return NULL;
}

COMMON_FUNCTIONS_FN uint16_t common_read_16_bit(const uint8_t data_buf[__static 2])
{
    return 0;
}

COMMON_FUNCTIONS_FN uint8_t *common_write_16_bit_inverse(uint16_t value, uint8_t ptr[__static 2])
{
    return NULL;
}

COMMON_FUNCTIONS_FN uint_fast8_t common_count_bits(uint8_t byte)
{
    return 0;
}

COMMON_FUNCTIONS_FN uint_fast8_t common_count_leading_zeros(uint8_t byte)
{
    return 0;
}

COMMON_FUNCTIONS_FN bool common_serial_number_greater_8(uint8_t s1, uint8_t s2)
{
    return  false;
}

COMMON_FUNCTIONS_FN bool common_serial_number_greater_16(uint16_t s1, uint16_t s2)
{
    return  false;
}

COMMON_FUNCTIONS_FN bool common_serial_number_greater_32(uint32_t s1, uint32_t s2)
{
    return  false;
}

COMMON_FUNCTIONS_FN bool bit_test(const uint8_t *bitset, uint_fast8_t bit)
{
    return false;
}

COMMON_FUNCTIONS_FN void bit_set(uint8_t *bitset, uint_fast8_t bit)
{
}

COMMON_FUNCTIONS_FN void bit_clear(uint8_t *bitset, uint_fast8_t bit)
{

}
