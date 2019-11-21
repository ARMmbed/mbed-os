/*
 * Copyright (c) 2019 ARM Limited. All rights reserved.
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

#ifndef STORAGE_UTILS_H
#define STORAGE_UTILS_H

#include <stdint.h>

/** \defgroup storage-internal-api Storage-internal
 * @{
 * \ingroup mbed-os-internal
 */

/** Align an address to next border of given block size.
 * @param val   unaligned value.
 * @param size  block size to align for.
 * @return Aligned value
 */
uint32_t align_up(uint64_t val, uint64_t size);

/** Align an address to previous border of given block size.
 * @param val   unaligned value.
 * @param size  block size to align for.
 * @return Aligned value
 */
uint32_t align_down(uint64_t val, uint64_t size);

/** Calculate CRC32 of given buffer.
 * @param init_crc  Initial CRC or zero.
 * @param data_size Size of given buffer
 * @param data_buf  Pointer to buffer
 * @return value of calculated CRC32
 */
uint32_t calc_crc(uint32_t init_crc, uint32_t data_size, const void *data_buf);

/** @}*/

#endif
