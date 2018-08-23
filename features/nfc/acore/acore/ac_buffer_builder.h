/*
 * Copyright (c) 2017, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * \file ac_buffer_builder.h
 * \copyright Copyright (c) ARM Ltd 2017
 * \author Donatien Garnier
 */

/** \addtogroup ACore
 *  @{
 *  \name Buffer Builder
 *  @{
 */

#ifndef ACORE_BUFFER_BUILDER_H_
#define ACORE_BUFFER_BUILDER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

#include "acore/ac_buffer.h"

typedef struct __ac_buffer_builder {
    ac_buffer_t ac_buffer;
    uint8_t *data;
    size_t size;
} ac_buffer_builder_t;

/** Write data to big endian ac_buffer (on a LE architecture, byte order will be swapped)
 * \param pBuilder ac_buffer builder to use
 * \param buf pointer to data
 * \param size the data size
 */
void ac_buffer_builder_write_be(ac_buffer_builder_t *pBuilder, const uint8_t *buf, size_t size);

/** Write data to little endian ac_buffer (on a LE architecture, byte order will be preserved)
 * \param pBuilder ac_buffer builder to use
 * \param buf pointer to data
 * \param size the data size
 */
void ac_buffer_builder_write_le(ac_buffer_builder_t *pBuilder, const uint8_t *buf, size_t size);

/** Write data to big endian ac_buffer at specific position (on a LE architecture, byte order will be swapped)
 * \param pBuilder ac_buffer builder to use
 * \param pos position in ac_buffer to write from
 * \param buf pointer to data
 * \param size the data size
 */
void ac_buffer_builder_write_be_at(ac_buffer_builder_t *pBuilder, size_t pos, const uint8_t *buf, size_t size);

/** Write data to little endian ac_buffer at specific position (on a LE architecture, byte order will be preserved)
 * \param pBuilder ac_buffer builder to use
 * \param pos position in ac_buffer to write from
 * \param buf pointer to data
 * \param size the data size
 */
void ac_buffer_builder_write_le_at(ac_buffer_builder_t *pBuilder, size_t pos, const uint8_t *buf, size_t size);

/** Initialize ac_buffer builder
 * \param pBuilder ac_buffer builder to init
 * \param data pointer to byte array to use
 * \param size of byte array
 */
void ac_buffer_builder_init(ac_buffer_builder_t *pBuilder, uint8_t *data, size_t size);

/** Initialize ac_buffer builder from underlying ac_buffer
 * \param pBuilder ac_buffer builder to init
 */
void ac_buffer_builder_from_buffer(ac_buffer_builder_t *pBuilder);

/** Reset ac_buffer builder
 * \param pBuilder ac_buffer builder to reset
 */
void ac_buffer_builder_reset(ac_buffer_builder_t *pBuilder);

/** Set ac_buffer builder's ac_buffer to full size
 * \param pBuilder ac_buffer builder to set to full size
 */
void ac_buffer_builder_set_full(ac_buffer_builder_t *pBuilder);

/** Get ac_buffer builder's length
 * \param pBuilder ac_buffer builder to get length of
 * \return number of valid bytes in ac_buffer
 */
static inline size_t ac_buffer_builder_length(ac_buffer_builder_t *pBuilder)
{
    return ac_buffer_size(&pBuilder->ac_buffer);
}

/** Set ac_buffer builder's length
 * \param pBuilder ac_buffer builder to set length of
 * \param length number of valid bytes in ac_buffer
 */
static inline void ac_buffer_builder_set_length(ac_buffer_builder_t *pBuilder, size_t length)
{
    if (ac_buffer_data(&pBuilder->ac_buffer) + length > pBuilder->data + pBuilder->size) {
        return;
    }
    pBuilder->ac_buffer.size = length;
}

/** Get ac_buffer builder's pointer to write position
 * \param pBuilder ac_buffer builder
 * \return pointer to write position
 */
static inline uint8_t *ac_buffer_builder_write_position(ac_buffer_builder_t *pBuilder)
{
    return (uint8_t *)ac_buffer_data(&pBuilder->ac_buffer) + ac_buffer_size(&pBuilder->ac_buffer);
}

/** Get ac_buffer builder's write offset
 * \param pBuilder ac_buffer builder
 * \return write offset
 */
static inline size_t ac_buffer_builder_write_offset(ac_buffer_builder_t *pBuilder)
{
    return ac_buffer_data(&pBuilder->ac_buffer) + ac_buffer_size(&pBuilder->ac_buffer) - pBuilder->data;
}

/** Set ac_buffer builder's write offset
 * \param pBuilder ac_buffer builder
 * \param off new write offset
 */
static inline void ac_buffer_builder_set_write_offset(ac_buffer_builder_t *pBuilder, size_t off)
{
    if (off > pBuilder->size) {
        return;
    }
    if (pBuilder->data + off > pBuilder->ac_buffer.data) {
        pBuilder->ac_buffer.size = off - (pBuilder->ac_buffer.data - pBuilder->data);
    } else {
        pBuilder->ac_buffer.size = 0;
    }
}

/** Get ac_buffer builder's read offset
 * \param pBuilder ac_buffer builder
 * \return read offset
 */
static inline size_t ac_buffer_builder_read_offset(ac_buffer_builder_t *pBuilder)
{
    return ac_buffer_data(&pBuilder->ac_buffer) - pBuilder->data;
}

/** Set ac_buffer builder's read offset
 * \param pBuilder ac_buffer builder
 * \param off new read offset
 */
static inline void ac_buffer_builder_set_read_offset(ac_buffer_builder_t *pBuilder, size_t off)
{
    if (off > pBuilder->size) {
        return;
    }
    if (pBuilder->data + off < pBuilder->ac_buffer.data + pBuilder->ac_buffer.size) {
        pBuilder->ac_buffer.size = pBuilder->ac_buffer.data - (pBuilder->data + off);
    } else {
        pBuilder->ac_buffer.size = 0;
    }
    pBuilder->ac_buffer.data = pBuilder->data + off;
}

/** Get ac_buffer builder's underlying ac_buffer
 * \param pBuilder ac_buffer builder
 * \return ac_buffer
 */
static inline ac_buffer_t *ac_buffer_builder_buffer(ac_buffer_builder_t *pBuilder)
{
    return &pBuilder->ac_buffer;
}

/** Get space in ac_buffer builder
 * \param pBuilder ac_buffer builder
 * \return number of free bytes in ac_buffer builder
 */
static inline size_t ac_buffer_builder_space(ac_buffer_builder_t *pBuilder)
{
    return pBuilder->size - (ac_buffer_data(&pBuilder->ac_buffer) - pBuilder->data + ac_buffer_size(&pBuilder->ac_buffer));
}

/** Is ac_buffer builder empty
 * \param pBuilder ac_buffer builder
 * \return true if ac_buffer builder is empty
 */
static inline bool ac_buffer_builder_empty(ac_buffer_builder_t *pBuilder)
{
    return (ac_buffer_builder_length(pBuilder) == 0);
}

/** Is ac_buffer builder full
 * \param pBuilder ac_buffer builder
 * \return true if ac_buffer builder is full
 */
static inline bool ac_buffer_full(ac_buffer_builder_t *pBuilder)
{
    return (ac_buffer_builder_space(pBuilder) == 0);
}

/** Write 8-bit value in ac_buffer builder
 * \param pBuilder ac_buffer builder
 * \param hu8 8-bit value to write
 */
static inline void ac_buffer_builder_write_nu8(ac_buffer_builder_t *pBuilder, uint8_t hu8)
{
    ac_buffer_builder_write_be(pBuilder, &hu8, 1);
}

/** Write 16-bit value in ac_buffer builder
 * \param pBuilder ac_buffer builder
 * \param hu16 16-bit value to write in big-endian format
 */
static inline void ac_buffer_builder_write_nu16(ac_buffer_builder_t *pBuilder, uint16_t hu16)
{
    ac_buffer_builder_write_be(pBuilder, (uint8_t *)&hu16, 2);
}

/** Write 24-bit value in ac_buffer builder
 * \param pBuilder ac_buffer builder
 * \param hu24 24-bit value to write in big-endian format
 */
static inline void ac_buffer_builder_write_nu24(ac_buffer_builder_t *pBuilder, uint32_t hu24)
{
    ac_buffer_builder_write_be(pBuilder, (uint8_t *)&hu24, 3);
}

/** Write 32-bit value in ac_buffer builder
 * \param pBuilder ac_buffer builder
 * \param hu32 32-bit value to write in big-endian format
 */
static inline void ac_buffer_builder_write_nu32(ac_buffer_builder_t *pBuilder, uint32_t hu32)
{
    ac_buffer_builder_write_be(pBuilder, (uint8_t *)&hu32, 4);
}

/** Write 64-bit value in ac_buffer builder
 * \param pBuilder ac_buffer builder
 * \param hu64 64-bit value to write in big-endian format
 */
static inline void ac_buffer_builder_write_nu64(ac_buffer_builder_t *pBuilder, uint64_t hu64)
{
    ac_buffer_builder_write_be(pBuilder, (uint8_t *)&hu64, 8);
}

/** Write n-bytes value in ac_buffer builder
 * \param pBuilder ac_buffer builder
 * \param data data to write
 * \param size data length
 */
static inline void ac_buffer_builder_write_n_bytes(ac_buffer_builder_t *pBuilder, const uint8_t *data, size_t size)
{
    ac_buffer_builder_write_le(pBuilder, data, size);
}

/** Write 8-bit value in ac_buffer builder at specified position
 * \param pBuilder ac_buffer builder
 * \param off offset at which to write
 * \param hu8 8-bit value to write
 */
static inline void ac_buffer_builder_write_nu8_at(ac_buffer_builder_t *pBuilder, size_t off, uint8_t hu8)
{
    ac_buffer_builder_write_be_at(pBuilder, off, &hu8, 1);
}

/** Write 16-bit value in ac_buffer builder at specified position
 * \param pBuilder ac_buffer builder
 * \param off offset at which to write
 * \param hu16 16-bit value to write
 */
static inline void ac_buffer_builder_write_nu16_at(ac_buffer_builder_t *pBuilder, size_t off, uint16_t hu16)
{
    ac_buffer_builder_write_be_at(pBuilder, off, (uint8_t *)&hu16, 2);
}

/** Write 24-bit value in ac_buffer builder at specified position
 * \param pBuilder ac_buffer builder
 * \param off offset at which to write
 * \param hu24 24-bit value to write
 */
static inline void ac_buffer_builder_write_nu24_at(ac_buffer_builder_t *pBuilder, size_t off, uint32_t hu24)
{
    ac_buffer_builder_write_be_at(pBuilder, off, (uint8_t *)&hu24, 3);
}

/** Write 32-bit value in ac_buffer builder at specified position
 * \param pBuilder ac_buffer builder
 * \param off offset at which to write
 * \param hu32 32-bit value to write
 */
static inline void ac_buffer_builder_write_nu32_at(ac_buffer_builder_t *pBuilder, size_t off, uint32_t hu32)
{
    ac_buffer_builder_write_be_at(pBuilder, off, (uint8_t *)&hu32, 4);
}

/** Write 64-bit value in ac_buffer builder at specified position
 * \param pBuilder ac_buffer builder
 * \param off offset at which to write
 * \param hu64 64-bit value to write
 */
static inline void ac_buffer_builder_write_nu64_at(ac_buffer_builder_t *pBuilder, size_t off, uint64_t hu64)
{
    ac_buffer_builder_write_be_at(pBuilder, off, (uint8_t *)&hu64, 8);
}

/** Write n-bytes value in ac_buffer builder at specified position
 * \param pBuilder ac_buffer builder
 * \param off offset at which to write
 * \param data data to write
 * \param size data length
 */
static inline void ac_buffer_builder_write_n_bytes_at(ac_buffer_builder_t *pBuilder, size_t off, const uint8_t *data, size_t size)
{
    ac_buffer_builder_write_be_at(pBuilder, off, data, size);
}

/** Skip n-bytes in ac_buffer builder
 * \param pBuilder ac_buffer builder
 * \param size number of bytes to skip
 */
void ac_buffer_builder_write_n_skip(ac_buffer_builder_t *pBuilder, size_t size);

/** Copy n bytes from buffer to builder
 * \param pBuilderOut ac_buffer builder
 * \param pBufIn the input buffer
 * \param size number of bytes to copy
 */
void ac_buffer_builder_copy_n_bytes(ac_buffer_builder_t *pBuilderOut, ac_buffer_t *pBufIn, size_t size);

/** Compact builder
 * Will move underlying buffer's byte to start of allocated buffer
 * \param pBuilder ac_buffer builder
 */
void ac_buffer_builder_compact(ac_buffer_builder_t *pBuilder);

/** Get number of writable bytes in ac_buffer builder
 * \param pBuilder ac_buffer builder
 * \return number of free bytes in ac_buffer builder
 */
static inline size_t ac_buffer_builder_writable(ac_buffer_builder_t *pBuilder)
{
    return ac_buffer_builder_space(pBuilder);
}

#ifdef __cplusplus
}
#endif

#endif /* ACORE_BUFFER_BUILDER_H_ */

/**
 * @}
 * @}
 * */
