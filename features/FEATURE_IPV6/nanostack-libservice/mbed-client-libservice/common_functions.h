/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef COMMON_FUNCTIONS_H_
#define COMMON_FUNCTIONS_H_

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Common write 64-bit variable to 8-bit pointer.
 *
 * Write 64 bits in big-endian (network) byte order.
 *
 * \param value 64-bit variable
 * \param ptr pointer where data to be written
 *
 * \return updated pointer
 */
NS_INLINE uint8_t *common_write_64_bit(uint64_t value, uint8_t ptr[__static 8]);

/*
 * Common read 64-bit variable from 8-bit pointer.
 *
 * Read 64 bits in big-endian (network) byte order.
 *
 * \param data_buf pointer where data to be read
 *
 * \return 64-bit variable
 */
NS_INLINE uint64_t common_read_64_bit(const uint8_t data_buf[__static 8]);

/*
 * Common write 32-bit variable to 8-bit pointer.
 *
 * Write 32 bits in big-endian (network) byte order.
 *
 * \param value 32-bit variable
 * \param ptr pointer where data to be written
 *
 * \return updated pointer
 */
NS_INLINE uint8_t *common_write_32_bit(uint32_t value, uint8_t ptr[__static 4]);

/*
 * Common read 32-bit variable from 8-bit pointer.
 *
 * Read 32 bits in big-endian (network) byte order.
 *
 * \param data_buf pointer where data to be read
 *
 * \return 32-bit variable
 */
NS_INLINE uint32_t common_read_32_bit(const uint8_t data_buf[__static 4]);

/*
 * Common write 32-bit variable to 8-bit pointer.
 *
 * Write 32 bits in little-endian byte order.
 *
 * \param value 32-bit variable
 * \param ptr pointer where data to be written
 *
 * \return updated pointer
 */
NS_INLINE uint8_t *common_write_32_bit_inverse(uint32_t value, uint8_t ptr[__static 4]);

/*
 * Common read 32-bit variable from 8-bit pointer.
 *
 * Read 32 bits in little-endian byte order.
 *
 * \param data_buf pointer where data to be read
 *
 * \return 32-bit variable
 */
NS_INLINE uint32_t common_read_32_bit_inverse(const uint8_t data_buf[__static 4]);

/*
 * Common write 24-bit variable to 8-bit pointer.
 *
 * Write 24 bits in big-endian (network) byte order.
 *
 * \param value 24-bit variable
 * \param ptr pointer where data to be written
 *
 * \return updated pointer
 */
NS_INLINE uint8_t *common_write_24_bit(uint_fast24_t value, uint8_t ptr[__static 3]);

/*
 * Common read 24-bit variable from 8-bit pointer.
 *
 * Read 24 bits in big-endian (network) byte order.
 *
 * \param data_buf pointer where data to be read
 *
 * \return 24-bit variable
 */
NS_INLINE uint_fast24_t common_read_24_bit(const uint8_t data_buf[__static 3]);

/*
 * Common write 16-bit variable to 8-bit pointer.
 *
 * Write 16 bits in big-endian (network) byte order.
 *
 * \param value 16-bit variable
 * \param ptr pointer where data to be written
 *
 * \return updated pointer
 */
NS_INLINE uint8_t *common_write_16_bit(uint16_t value, uint8_t ptr[__static 2]);

/*
 * Common read 16-bit variable from 8-bit pointer.
 *
 * Read 16 bits in big-endian (network) byte order.
 *
 * \param data_buf pointer where data to be read
 *
 * \return 16-bit variable
 */
NS_INLINE uint16_t common_read_16_bit(const uint8_t data_buf[__static 2]);

/*
 * Common write 16-bit variable to 8-bit pointer.
 *
 * Write 16 bits in little-endian byte order.
 *
 * \param value 16-bit variable
 * \param ptr pointer where data to be written
 *
 * \return updated pointer
 */
NS_INLINE uint8_t *common_write_16_bit_inverse(uint16_t value, uint8_t ptr[__static 2]);

/*
 * Common read 16-bit variable from 8-bit pointer.
 *
 * Read 16 bits in little-endian byte order.
 *
 * \param data_buf pointer where data to be read
 *
 * \return 16-bit variable
 */
NS_INLINE uint16_t common_read_16_bit_inverse(const uint8_t data_buf[__static 2]);

/*
 * Count bits in a byte
 *
 * \param byte byte to inspect
 *
 * \return number of 1-bits in byte
 */
NS_INLINE uint_fast8_t common_count_bits(uint8_t byte);

/*
 * Count leading zeros in a byte
 *
 * \param byte byte to inspect
 *
 * \return number of leading zeros in byte (0-8)
 */
NS_INLINE uint_fast8_t common_count_leading_zeros(uint8_t byte);

/*
 * Compare 8-bit serial numbers
 *
 * Compare two 8-bit serial numbers, according to RFC 1982 Serial Number
 * Arithmetic.
 *
 * \param s1 first serial number
 * \param s2 second serial number
 *
 * \return true if s1 > s2
 * \return false if s1 <= s2, or the comparison is undefined
 */
NS_INLINE bool common_serial_number_greater_8(uint8_t s1, uint8_t s2);

/*
 * Compare 16-bit serial numbers
 *
 * Compare two 16-bit serial numbers, according to RFC 1982 Serial Number
 * Arithmetic.
 *
 * \param s1 first serial number
 * \param s2 second serial number
 *
 * \return true if s1 > s2
 * \return false if s1 <= s2, or the comparison is undefined
 */
NS_INLINE bool common_serial_number_greater_16(uint16_t s1, uint16_t s2);

/*
 * Compare 32-bit serial numbers
 *
 * Compare two 32-bit serial numbers, according to RFC 1982 Serial Number
 * Arithmetic.
 *
 * \param s1 first serial number
 * \param s2 second serial number
 *
 * \return true if s1 > s2
 * \return false if s1 <= s2, or the comparison is undefined
 */
NS_INLINE bool common_serial_number_greater_32(uint32_t s1, uint32_t s2);

/*
 * Test a bit in an bit array.
 *
 * Check whether a particular bit is set in a bit string. The bit array
 * is in big-endian (network) bit order.
 *
 * \param bitset pointer to bit array
 * \param bit index of bit - 0 is the most significant bit of the first byte
 *
 * \return true if the bit is set
 */
NS_INLINE bool bit_test(const uint8_t *bitset, uint_fast8_t bit);

/*
 * Set a bit in an bit array.
 *
 * Set a bit in a bit array. The array is in big-endian (network) bit order.
 *
 * \param bitset pointer to bit array
 * \param bit index of bit - 0 is the most significant bit of the first byte
 */
NS_INLINE void bit_set(uint8_t *bitset, uint_fast8_t bit);

/*
 * Clear a bit in an bit array.
 *
 * Clear a bit in a bit array. The bit array is in big-endian (network) bit order.
 *
 * \param bitset pointer to bit array
 * \param bit index of bit - 0 is the most significant bit of the first byte
 */
NS_INLINE void bit_clear(uint8_t *bitset, uint_fast8_t bit);

/*
 * Compare two bitstrings.
 *
 * Compare two bitstrings of specified length. The bit strings are in
 * big-endian (network) bit order.
 *
 * \param a pointer to first string
 * \param b pointer to second string
 * \param bits number of bits to compare
 *
 * \return true if the strings compare equal
 */
bool bitsequal(const uint8_t *a, const uint8_t *b, uint_fast8_t bits);

/*
 * Copy a bitstring
 *
 * Copy a bitstring of specified length. The bit string is in big-endian
 * (network) bit order. Bits beyond the bitlength at the destination are not
 * modified.
 *
 * For example, copying 4 bits sets the first 4 bits of dst[0] from src[0],
 * the lower 4 bits of dst[0] are unmodified.
 *
 * \param dst destination pointer
 * \param src source pointer
 * \param bits number of bits to copy
 *
 * \return the value of dst
 */
uint8_t *bitcopy(uint8_t *restrict dst, const uint8_t *restrict src, uint_fast8_t bits);

/*
 * Copy a bitstring and pad last byte with zeros
 *
 * Copy a bitstring of specified length. The bit string is in big-endian
 * (network) bit order. Bits beyond the bitlength in the last destination byte are
 * zeroed.
 *
 * For example, copying 4 bits sets the first 4 bits of dst[0] from src[0], and
 * the lower 4 bits of dst[0] are set to 0.
 *
 * \param dst destination pointer
 * \param src source pointer
 * \param bits number of bits to copy
 *
 * \return the value of dst
 */
uint8_t *bitcopy0(uint8_t *restrict dst, const uint8_t *restrict src, uint_fast8_t bits);

/* Provide definitions, either for inlining, or for common_functions.c */
#if defined NS_ALLOW_INLINING || defined COMMON_FUNCTIONS_FN
#ifndef COMMON_FUNCTIONS_FN
#define COMMON_FUNCTIONS_FN NS_INLINE
#endif

COMMON_FUNCTIONS_FN uint8_t *common_write_64_bit(uint64_t value, uint8_t ptr[__static 8])
{
    *ptr++ = value >> 56;
    *ptr++ = value >> 48;
    *ptr++ = value >> 40;
    *ptr++ = value >> 32;
    *ptr++ = value >> 24;
    *ptr++ = value >> 16;
    *ptr++ = value >> 8;
    *ptr++ = value;
    return ptr;
}

COMMON_FUNCTIONS_FN uint64_t common_read_64_bit(const uint8_t data_buf[__static 8])
{
    uint64_t temp_64;
    temp_64 = (uint64_t)(*data_buf++) << 56;
    temp_64 += (uint64_t)(*data_buf++) << 48;
    temp_64 += (uint64_t)(*data_buf++) << 40;
    temp_64 += (uint64_t)(*data_buf++) << 32;
    temp_64 += (uint64_t)(*data_buf++) << 24;
    temp_64 += (uint64_t)(*data_buf++) << 16;
    temp_64 += (uint64_t)(*data_buf++) << 8;
    temp_64 += *data_buf++;
    return temp_64;
}

COMMON_FUNCTIONS_FN uint8_t *common_write_32_bit(uint32_t value, uint8_t ptr[__static 4])
{
    *ptr++ = value >> 24;
    *ptr++ = value >> 16;
    *ptr++ = value >> 8;
    *ptr++ = value;
    return ptr;
}

COMMON_FUNCTIONS_FN uint32_t common_read_32_bit(const uint8_t data_buf[__static 4])
{
    uint32_t temp_32;
    temp_32 = (uint32_t)(*data_buf++) << 24;
    temp_32 += (uint32_t)(*data_buf++) << 16;
    temp_32 += (uint32_t)(*data_buf++) << 8;
    temp_32 += *data_buf++;
    return temp_32;
}

COMMON_FUNCTIONS_FN uint8_t *common_write_32_bit_inverse(uint32_t value, uint8_t ptr[__static 4])
{
    *ptr++ = value;
    *ptr++ = value >> 8;
    *ptr++ = value >> 16;
    *ptr++ = value >> 24;
    return ptr;
}

COMMON_FUNCTIONS_FN uint32_t common_read_32_bit_inverse(const uint8_t data_buf[__static 4])
{
    uint32_t temp_32;
    temp_32 =  *data_buf++;
    temp_32 += (uint32_t)(*data_buf++) << 8;
    temp_32 += (uint32_t)(*data_buf++) << 16;
    temp_32 += (uint32_t)(*data_buf++) << 24;
    return temp_32;
}

COMMON_FUNCTIONS_FN uint8_t *common_write_24_bit(uint_fast24_t value, uint8_t ptr[__static 3])
{
    *ptr++ = value >> 16;
    *ptr++ = value >> 8;
    *ptr++ = value;
    return ptr;
}

COMMON_FUNCTIONS_FN uint_fast24_t common_read_24_bit(const uint8_t data_buf[__static 3])
{
    uint_fast24_t temp_24;
    temp_24 = (uint_fast24_t)(*data_buf++) << 16;
    temp_24 += (uint_fast24_t)(*data_buf++) << 8;
    temp_24 += *data_buf++;
    return temp_24;
}

COMMON_FUNCTIONS_FN uint8_t *common_write_16_bit(uint16_t value, uint8_t ptr[__static 2])
{
    *ptr++ = value >> 8;
    *ptr++ = value;
    return ptr;
}

COMMON_FUNCTIONS_FN uint16_t common_read_16_bit(const uint8_t data_buf[__static 2])
{
    uint16_t temp_16;
    temp_16 = (uint16_t)(*data_buf++) << 8;
    temp_16 += *data_buf++;
    return temp_16;
}

COMMON_FUNCTIONS_FN uint8_t *common_write_16_bit_inverse(uint16_t value, uint8_t ptr[__static 2])
{
    *ptr++ = value;
    *ptr++ = value >> 8;
    return ptr;
}

COMMON_FUNCTIONS_FN uint16_t common_read_16_bit_inverse(const uint8_t data_buf[__static 2])
{
    uint16_t temp_16;
    temp_16 = *data_buf++;
    temp_16 += (uint16_t)(*data_buf++) << 8;
    return temp_16;
}

COMMON_FUNCTIONS_FN uint_fast8_t common_count_bits(uint8_t byte)
{
    /* First step sets each bit pair to be count of bits (00,01,10) */
    /* [00-00 = 00, 01-00 = 01, 10-01 = 01, 11-01 = 10] */
    uint_fast8_t count = byte - ((byte >> 1) & 0x55);
    /* Add bit pairs to make each nibble contain count of bits (0-4) */
    count = (count & 0x33) + ((count >> 2) & 0x33);
    /* Final result is sum of nibbles (0-8) */
    count = (count >> 4) + (count & 0x0F);
    return count;
}

COMMON_FUNCTIONS_FN uint_fast8_t common_count_leading_zeros(uint8_t byte)
{
#ifdef  __CC_ARM
    return byte ? __clz((unsigned int) byte << 24) : 8;
#elif defined __GNUC__
    return byte ? __builtin_clz((unsigned int) byte << 24) : 8;
#else
    uint_fast8_t cnt = 0;
    if (byte == 0) {
        return 8;
    }
    if ((byte & 0xF0) == 0) {
        byte <<= 4;
        cnt += 4;
    }
    if ((byte & 0xC0) == 0) {
        byte <<= 2;
        cnt += 2;
    }
    if ((byte & 0x80) == 0) {
        cnt += 1;
    }

    return cnt;
#endif
}

COMMON_FUNCTIONS_FN bool common_serial_number_greater_8(uint8_t s1, uint8_t s2)
{
    return (s1 > s2 && s1 - s2 < UINT8_C(0x80)) || (s1 < s2 && s2 - s1 > UINT8_C(0x80));
}

COMMON_FUNCTIONS_FN bool common_serial_number_greater_16(uint16_t s1, uint16_t s2)
{
    return (s1 > s2 && s1 - s2 < UINT16_C(0x8000)) || (s1 < s2 && s2 - s1 > UINT16_C(0x8000));
}

COMMON_FUNCTIONS_FN bool common_serial_number_greater_32(uint32_t s1, uint32_t s2)
{
    return (s1 > s2 && s1 - s2 < UINT32_C(0x80000000)) || (s1 < s2 && s2 - s1 > UINT32_C(0x80000000));
}

COMMON_FUNCTIONS_FN bool bit_test(const uint8_t *bitset, uint_fast8_t bit)
{
    return bitset[bit >> 3] & (0x80 >> (bit & 7));
}

COMMON_FUNCTIONS_FN void bit_set(uint8_t *bitset, uint_fast8_t bit)
{
    bitset[bit >> 3] |= (0x80 >> (bit & 7));
}

COMMON_FUNCTIONS_FN void bit_clear(uint8_t *bitset, uint_fast8_t bit)
{
    bitset[bit >> 3] &= ~(0x80 >> (bit & 7));
}

#endif /* defined NS_ALLOW_INLINING || defined COMMON_FUNCTIONS_FN */

#ifdef __cplusplus
}
#endif
#endif /*__COMMON_FUNCTIONS_H_*/
