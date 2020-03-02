/*
 * Copyright 2018-2020 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @file
 *  Provides declarations for buffer management functionality
 */
#ifndef _SCL_INTERNAL_BUFFER_API_H_
#define _SCL_INTERNAL_BUFFER_API_H_

#include "scl_types.h"
#include "scl_common.h"
#include <stdlib.h>
#include "cy_utils.h"
#include "memp.h"
#include "pbuf.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
*                  Constants
******************************************************/
/**
 * Size of the SDIO block
 */
#define  SDIO_BLOCK_SIZE (64U)

/******************************************************
*                      Macros
******************************************************/

/******************************************************
*             Structures and Enumerations
******************************************************/
/**
 * Indicates the transmit/receive direction that the buffer has
 * been used for. This is needed if transmit/receive pools are separate.
 */
typedef enum {
    SCL_NETWORK_TX = 0, /**< Transmit direction */
    SCL_NETWORK_RX = 1 /**< Receive direction */
} scl_buffer_dir_t;

/******************************************************
*             Function Prototypes
******************************************************/
/** Allocates the SCL buffer.
 *
 *  Attempts to allocate a buffer of the requested size. A buffer
 *  is either allocated from a static pool of memory or allocated dynamically.
 *
 *  @param   buffer    A pointer which receives the allocated buffer.
 *  @param   direction Indicates transmit/receive direction that the buffer is
 *                      used for. This may be needed if transmit/receive pools are separate.
 *  @param   size      The number of bytes to allocate.
 *  @param   wait      Time to wait for a buffer to be available in milli-seconds.
 *
 *  @return  SCL_SUCCESS or Error code
 *
 */
scl_result_t scl_host_buffer_get(scl_buffer_t *buffer, scl_buffer_dir_t direction,
                                 uint16_t size, uint32_t wait);

/** Releases the SCL buffer.
 *
 *  This function is used by SCL to indicate that it no longer requires
 *  the buffer. The buffer can then be released back into a pool for
 *  reuse or the dynamically allocated memory can be freed.
 *
 *  @param   buffer    The buffer to be released.
 *  @param   direction Indicates the transmit/receive direction that the buffer has
 *                     been used for. This might be needed if transmit/receive pools are separate.
 *
 */
void scl_buffer_release(scl_buffer_t buffer, scl_buffer_dir_t direction);

/** Retrieves the pointer to the payload of the buffer.
 *
 *  @param   buffer   The buffer whose payload pointer is to be retrieved.
 *
 *  @return  Pointer to the payload.
 */
uint8_t *scl_buffer_get_current_piece_data_pointer(scl_buffer_t buffer);

/** Retrieves the size of the buffer.
 *
 *  @param   buffer   The buffer whose size is to be retrieved.
 *
 *  @return  The size of the buffer.
 */
uint16_t scl_buffer_get_current_piece_size(scl_buffer_t buffer);

#ifdef __cplusplus
} /*extern "C" */
#endif

#endif /* ifndef _SCL_INTERNAL_BUFFER_API_H_ */
