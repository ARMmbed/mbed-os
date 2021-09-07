/*
 * Copyright 2021, Cypress Semiconductor Corporation (an Infineon company)
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

/** @file whd_network_types.h
 *  Prototypes of functions corresponding to Buffer and Network Interface
 *
 *  This file provides prototypes for functions which allows different functionalities related to:
 *      - Buffer Interface: Allocate and release a packet buffer, Retrieve the current pointer and size of a packet buffer, etc.
 *      - Network Interface: Called by WHD to pass received data to the network stack, to send an ethernet frame to WHD, etc.
 */
#include "whd.h"

#ifndef INC_WHD_NETWORK_TYPES_H_
#define INC_WHD_NETWORK_TYPES_H_

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
*                  Constants
******************************************************/
/** @addtogroup buffif WHD Buffer Interface API
 * Allows WHD to perform buffer related operations like, allocating, releasing, retrieving the current pointer of and size of a packet buffer.
 *  @{
 */

/**
 * Indicates transmit/receive direction that the packet buffer has
 * been used for. This is needed if tx/rx pools are separate.
 */
typedef enum
{
    WHD_NETWORK_TX, /**< Transmit direction */
    WHD_NETWORK_RX  /**< Recieve direction */
} whd_buffer_dir_t;

/**
 * Allows WHD to perform buffer related operations like, allocating, releasing, retrieving the current pointer of and size of a packet buffer.
 */
struct whd_buffer_funcs
{
    /** Allocates a packet buffer
     *
     *  Implemented in the port layer interface which is specific to the
     *  buffering scheme in use.
     *  Attempts to allocate a packet buffer of the size requested. It can do this
     *  by allocating a pre-existing packet from a pool, using a static buffer,
     *  or by dynamically allocating memory. The method of allocation does not
     *  concern WHD, however it must match the way the network stack expects packet
     *  buffers to be allocated. Usually WHD requires packet of size of WHD_LINK_MTU
     *  which includes the MTU, other other various header. Refer to whd_types.h
     *  to find the size of WHD_LINK_MTU
     *
     *  @param buffer      A pointer which receives the allocated packet buffer handle
     *  @param direction   Indicates transmit/receive direction that the packet buffer is
     *                     used for. This may be needed if tx/rx pools are separate.
     *  @param size        The number of bytes to allocate.
     *  @param timeout_ms  Maximum period to block for available buffer
     *
     *  @return            WHD_SUCCESS or error code
     *
     */
    whd_result_t (*whd_host_buffer_get)(whd_buffer_t *buffer, whd_buffer_dir_t direction, unsigned short size,
                                        unsigned long timeout_ms);

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
     *  @param buffer     The handle of the packet buffer to be released
     *  @param direction  Indicates transmit/receive direction that the packet buffer has
     *                     been used for. This might be needed if tx/rx pools are separate.
     *
     */
    void (*whd_buffer_release)(whd_buffer_t buffer, whd_buffer_dir_t direction);

    /** Retrieves the current pointer of a packet buffer
     *
     *  Implemented in the port layer interface which is specific to the
     *  buffering scheme in use.
     *  Since packet buffers usually need to be created with space at the
     *  front for additional headers, this function allows WHD to get
     *  the current 'front' location pointer.
     *
     *  @param buffer  The handle of the packet buffer whose pointer is to be retrieved
     *
     *  @return        The packet buffer's current pointer.
     */
    uint8_t *(*whd_buffer_get_current_piece_data_pointer)(whd_buffer_t buffer);

    /** Retrieves the size of a packet buffer
     *
     *  Implemented in the port layer interface which is specific to the
     *  buffering scheme in use.
     *  Since packet buffers usually need to be created with space at the
     *  front for additional headers, the memory block use to contain a packet buffer
     *  will often be larger than the current size of the packet buffer data.
     *  This function allows WHD to retrieve the current size of a packet buffer's data.
     *
     *  @param buffer   The handle of the packet buffer whose size is to be retrieved
     *
     *  @return         The size of the packet buffer.
     */
    uint16_t (*whd_buffer_get_current_piece_size)(whd_buffer_t buffer);

    /** Sets the current size of a WHD packet
     *
     *  Implemented in the port layer interface which is specific to the
     *  buffering scheme in use.
     *  This function sets the current length of a WHD packet buffer
     *
     *  @param buffer  The packet to be modified
     *  @param size    The new size of the packet buffer
     *
     *  @return        WHD_SUCCESS or error code
     */
    whd_result_t (*whd_buffer_set_size)(whd_buffer_t buffer, unsigned short size);

    /** Moves the current pointer of a packet buffer
     *
     *  Implemented in the port layer interface which is specific to the buffering scheme in use.
     *
     *  Since packet buffers usually need to be created with space at the front for additional headers,
     *  this function allows WHD to move  the current 'front' location pointer so that it has space to
     *  add headers to transmit packets, and so that the network stack does not see the internal WHD
     *  headers on received packets.
     *
     *  @param buffer             A pointer to the handle of the current packet buffer for which the
     *                            current pointer will be moved. On return this may contain a pointer
     *                            to a newly allocated packet buffer which has been daisy chained to
     *                            the front of the given packet buffer. This would be the case if the given packet
     *                            buffer  didn't have enough space at the front.
     *  @param add_remove_amount  This is the number of bytes to move the current pointer of the packet
     *                            buffer - a negative value increases the space for headers at the front
     *                            of the packet, a positive value decreases the space.
     *
     *  @return                   WHD_SUCCESS or error code
     */
    whd_result_t (*whd_buffer_add_remove_at_front)(whd_buffer_t *buffer, int32_t add_remove_amount);
};
/*  @} */

/** @addtogroup netif WHD Network Interface API
 *  Allows WHD to pass received data to the network stack, to send an ethernet frame to WHD, etc.
 *  @{
 */

/**
 * Contains functions which allows WHD to pass received data to the network stack, to send an ethernet frame to WHD, etc
 */
struct whd_netif_funcs
{
    /** Called by WHD to pass received data to the network stack
     *
     *
     *  Packets received from the Wi-Fi network by WHD are forwarded to by calling function ptr which
     *  must be implemented in the network interface. Ethernet headers
     *  are present at the start of these packet buffers.
     *
     *  This function is called asynchronously in the context of the
     *  WHD thread whenever new data has arrived.
     *  Packet buffers are allocated within WHD, and ownership is transferred
     *  to the network stack. The network stack or application is thus
     *  responsible for releasing the packet buffers.
     *  Most packet buffering systems have a pointer to the 'current point' within
     *  the packet buffer. When this function is called, the pointer points
     *  to the start of the Ethernet header. There is other inconsequential data
     *  before the Ethernet header.
     *
     *  It is preferable that the (whd_network_process_ethernet_data)() function simply puts
     *  the received packet on a queue for processing by another thread. This avoids the
     *  WHD thread being unnecessarily tied up which would delay other packets
     *  being transmitted or received.
     *
     *  @param interface  The interface on which the packet was received.
     *  @param buffer     Handle of the packet which has just been received. Responsibility for
     *                    releasing this buffer is transferred from WHD at this point.
     *
     */
    void (*whd_network_process_ethernet_data)(whd_interface_t ifp, whd_buffer_t buffer);
};

/** To send an ethernet frame to WHD (called by the Network Stack)
 *
 *  This function takes ethernet data from the network stack and queues it for transmission over the wireless network.
 *  The function can be called from any thread context as it is thread safe, however
 *  it must not be called from interrupt context since it might get blocked while waiting
 *  for a lock on the transmit queue.
 *
 *  This function returns immediately after the packet has been queued for transmit,
 *  NOT after it has been transmitted.  Packet buffers passed to the WHD
 *  are released inside the WHD once they have been transmitted.
 *
 *  @param ifp           Pointer to handle instance of whd interface
 *  @param buffer        Handle of the packet buffer to be sent.
 *
 *  @return WHD_SUCCESS or Error code
 *
 */
extern void whd_network_send_ethernet_data(whd_interface_t ifp, whd_buffer_t buffer);
/*  @} */


#ifdef __cplusplus
} /*extern "C" */
#endif
#endif /* INC_WHD_NETWORK_TYPES_H_ */

