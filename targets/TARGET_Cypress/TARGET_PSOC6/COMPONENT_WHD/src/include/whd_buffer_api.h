/*
 * Copyright 2019 Cypress Semiconductor Corporation
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
 *  Provides prototypes / declarations for common APSTA functionality
 */
#ifndef _WHD_INTERNAL_BUFFER_API_H_
#define _WHD_INTERNAL_BUFFER_API_H_

#include "whd.h"
#include "whd_int.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
*                      Macros
******************************************************/

/******************************************************
*             Structures
******************************************************/

/******************************************************
*             Function prototypes
******************************************************/
/** Allocates a packet buffer
 *
 *  Implemented in the port layer interface which is specific to the
 *  buffering scheme in use.
 *  Attempts to allocate a packet buffer of the size requested. It can do this
 *  by allocating a pre-existing packet from a pool, using a static buffer,
 *  or by dynamically allocating memory. The method of allocation does not
 *  concern WHD, however it must match the way the network stack expects packet
 *  buffers to be allocated.
 *
 *  @param buffer    : A pointer which receives the allocated packet buffer handle
 *  @param direction : Indicates transmit/receive direction that the packet buffer is
 *                    used for. This may be needed if tx/rx pools are separate.
 *  @param size      : The number of bytes to allocate.
 *  @param wait      : Whether to wait for a packet buffer to be available
 *
 *  @return          : WHD_SUCCESS or error code
 *
 */
whd_result_t whd_host_buffer_get(whd_driver_t whd_driver, whd_buffer_t *buffer, whd_buffer_dir_t direction,
                                 uint16_t size, uint32_t wait);

/** Releases a packet buffer
 *
 *  Implemented in the port layer interface, which will be specific to the
 *  buffering scheme in use.
 *  This function is used by WHD to indicate that it no longer requires
 *  a packet buffer. The buffer can then be released back into a pool for
 *  reuse, or the dynamically allocated memory can be freed, according to
 *  how the packet was allocated.
 *  Returns void since WHD cannot do anything about failures
 *
 *  @param buffer    : The handle of the packet buffer to be released
 *  @param direction : Indicates transmit/receive direction that the packet buffer has
 *                     been used for. This might be needed if tx/rx pools are separate.
 *
 */
whd_result_t whd_buffer_release(whd_driver_t whd_driver, whd_buffer_t buffer, whd_buffer_dir_t direction);

/** Retrieves the current pointer of a packet buffer
 *
 *  Implemented in the port layer interface which is specific to the
 *  buffering scheme in use.
 *  Since packet buffers usually need to be created with space at the
 *  front for additional headers, this function allows WHD to get
 *  the current 'front' location pointer.
 *
 *  @param buffer : The handle of the packet buffer whose pointer is to be retrieved
 *
 *  @return       : The packet buffer's current pointer.
 */
uint8_t *whd_buffer_get_current_piece_data_pointer(whd_driver_t whd_driver, whd_buffer_t buffer);

/** Retrieves the size of a packet buffer
 *
 *  Implemented in the port layer interface which is specific to the
 *  buffering scheme in use.
 *  Since packet buffers usually need to be created with space at the
 *  front for additional headers, the memory block used to contain a packet buffer
 *  will often be larger than the current size of the packet buffer data.
 *  This function allows WHD to retrieve the current size of a packet buffer's data.
 *
 *  @param buffer : The handle of the packet buffer whose size is to be retrieved
 *
 *  @return       :  The size of the packet buffer.
 */
uint16_t whd_buffer_get_current_piece_size(whd_driver_t whd_driver, whd_buffer_t buffer);

/** Sets the current size of a WHD packet
 *
 *
 *  Implemented in the port layer interface which is specific to the
 *  buffering scheme in use.
 *  This function sets the current length of a WHD packet buffer
 *
 *  @param buffer : The packet to be modified
 *  @param size   : The new size of the packet buffer
 *
 *  @return       : WHD_SUCCESS or error code
 */
whd_result_t whd_buffer_set_size(whd_driver_t whd_driver, whd_buffer_t buffer, uint16_t size);

/** Moves the current pointer of a packet buffer
 *
 *  Implemented in the port layer interface which is specific to the buffering scheme in use.
 *
 *  Since packet buffers usually need to be created with space at the front for additional headers,
 *  this function allows WHD to move  the current 'front' location pointer so that it has space to
 *  add headers to transmit packets, and so that the network stack does not see the internal WHD
 *  headers on received packets.
 *
 *  @param buffer            : A pointer to the handle of the current packet buffer for which the
 *                             current pointer will be moved. On return this may contain a pointer
 *                             to a newly allocated packet buffer which has been daisy chained to
 *                             the front of the given one. This would be the case if the given packet
 *                             buffer  didn't have enough space at the front.
 *  @param add_remove_amount : This is the number of bytes to move the current pointer of the packet
 *                             buffer - a negative value increases the space for headers at the front
 *                             of the packet, a positive value decreases the space.
 *
 *  @return                  : WHD_SUCCESS or error code
 */
whd_result_t whd_buffer_add_remove_at_front(whd_driver_t whd_driver, whd_buffer_t *buffer, int32_t add_remove_amount);
#ifdef __cplusplus
} /*extern "C" */
#endif

#endif /* ifndef _WHD_INTERNAL_BUFFER_API_H_ */

