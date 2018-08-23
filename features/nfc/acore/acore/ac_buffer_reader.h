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
 * \file ac_buffer_reader.h
 * \copyright Copyright (c) ARM Ltd 2015
 * \author Donatien Garnier
 */

/** \addtogroup ACore
 *  @{
 *  \name Buffer Reader
 *  @{
 */

#ifndef ACORE_BUFFER_READER_H_
#define ACORE_BUFFER_READER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

#include "acore/ac_buffer.h"

/** Read n-bytes in big-endian format from buffer reader and advance read posiion
 * \param pBuf the buffer to read from
 * \param buf the array to write to
 * \param size the number of bytes to read
 */
void ac_buffer_read_be(ac_buffer_t *pBuf, uint8_t *buf, size_t size);

/** Read n-bytes in little-endian format from buffer reader and advance read posiion
 * \param pBuf the buffer to read from
 * \param buf the array to write to
 * \param size the number of bytes to read
 */
void ac_buffer_read_le(ac_buffer_t *pBuf, uint8_t *buf, size_t size);

/** Read 8-bit value from buffer reader and advance read posiion
 * \param pBuf the buffer to read from
 * \return 8-bit value read
 */
static inline uint8_t ac_buffer_read_nu8(ac_buffer_t *pBuf)
{
    uint8_t hu8;
    ac_buffer_read_be(pBuf, &hu8, 1);
    return hu8;
}

/** Read BE 16-bit value from buffer reader and advance read posiion
 * \param pBuf the buffer to read from
 * \return 16-bit value read
 */
static inline uint16_t ac_buffer_read_nu16(ac_buffer_t *pBuf)
{
    uint16_t hu16;
    ac_buffer_read_be(pBuf, (uint8_t *)&hu16, 2);
    return hu16;
}

/** Read BE 24-bit value from buffer reader and advance read posiion
 * \param pBuf the buffer to read from
 * \return 24-bit value read
 */
static inline uint32_t ac_buffer_read_nu24(ac_buffer_t *pBuf)
{
    uint32_t hu24;
    ac_buffer_read_be(pBuf, (uint8_t *)&hu24, 3);
    return hu24;
}

/** Read BE 32-bit value from buffer reader and advance read posiion
 * \param pBuf the buffer to read from
 * \return 32-bit value read
 */
static inline uint32_t ac_buffer_read_nu32(ac_buffer_t *pBuf)
{
    uint32_t hu32;
    ac_buffer_read_be(pBuf, (uint8_t *)&hu32, 4);
    return hu32;
}

/** Read BE 64-bit value from buffer reader and advance read posiion
 * \param pBuf the buffer to read from
 * \return 64-bit value read
 */
static inline uint64_t ac_buffer_read_nu64(ac_buffer_t *pBuf)
{
    uint64_t hu64;
    ac_buffer_read_be(pBuf, (uint8_t *)&hu64, 8);
    return hu64;
}

/** Read n bytes from buffer reader and advance read posiion
 * \param pBuf the buffer to read from
 * \param data the array to write bytes to
 * \param size the number of bytes to read
 */
static inline void ac_buffer_read_n_bytes(ac_buffer_t *pBuf, uint8_t *data, size_t size)
{
    ac_buffer_read_le(pBuf, data, size);
}

/** Skip n bytes from buffer reader and advance read posiion
 * \param pBuf the buffer to read from
 * \param size the number of bytes to skip
 */
void ac_buffer_read_n_skip(ac_buffer_t *pBuf, size_t size);

/** Get number of bytes readable from buffer
 * \param pBuf the buffer to read from
 * \return The number of bytes which can be read
 */
size_t ac_buffer_reader_readable(const ac_buffer_t *pBuf);

/** Get a pointer to the current position within this buffer's current backing array
 * \param pBuf the buffer to read from
 * \return A pointer to the current position within the current backing array
 */
const uint8_t *ac_buffer_reader_current_buffer_pointer(ac_buffer_t *pBuf);

/** Get the number of bytes readable within the current backing array
 * \param pBuf the buffer to read from
 * \return The number of bytes readable within the current backing array
 */
size_t ac_buffer_reader_current_buffer_length(ac_buffer_t *pBuf);

/** Compare buffer with array (does not advance read position)
 * \param pBuf the buffer to compare from
 * \param bytes the array to compare with
 * \param length the array length
 * \return Whether the buffer is AT LEAST as long as the array AND the buffer and array have the same content
 */
bool ac_buffer_reader_cmp_bytes(const ac_buffer_t *pBuf, const uint8_t *bytes, size_t length);

/** Compare buffer with array (does not advance read position)
 * \param pBuf1 the buffer to compare from
 * \param pBuf2 the buffer to compare with
 * \return Whether the buffers have the same length and content
 */
bool ac_buffer_reader_cmp(const ac_buffer_t *pBuf1, const ac_buffer_t *pBuf2);

#ifdef __cplusplus
}
#endif

#endif /* CORE_ac_buffer_READER_H_ */

/**
 * @}
 * @}
 * */
