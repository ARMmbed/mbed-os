/*
 * Copyright (c) 2011-2017, Arm Limited and affiliates.
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

#include "nsconfig.h"
#include "string.h"
#include "ns_types.h"
#include "nsdynmemLIB.h"
#include "Core/include/address.h"
#include "Core/include/ns_buffer.h"
#include "platform/arm_hal_interrupt.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "ip_fsc.h"
#include "buffer_dyn_stub.h"
#include "Core/include/socket.h"

buffer_dyn_stub_def buffer_dyn_stub;

uint8_t *(buffer_corrupt_check)(buffer_t *buf)
{
    return buffer_data_pointer(buf);
}

socket_t *buffer_socket_set(buffer_t *buf, socket_t *socket)
{
    return socket;
}

buffer_t *buffer_get(uint16_t size)
{
    return buffer_dyn_stub.buffer_ptr;
}

buffer_t *buffer_get_specific(uint16_t headroom, uint16_t size, uint16_t minspace)
{
    return buffer_dyn_stub.buffer_ptr;
}

buffer_t *buffer_get_minimal(uint16_t size)
{
    return buffer_dyn_stub.buffer_ptr;
}

buffer_t *buffer_headroom(buffer_t *buf, uint16_t size)
{
    return buffer_dyn_stub.buffer_ptr;
}

buffer_t *buffer_free_route(buffer_t *buf)
{
    return buffer_dyn_stub.buffer_ptr;
}

buffer_t *buffer_free(buffer_t *buf)
{
    return NULL;
}

void buffer_free_list(buffer_list_t *b)
{

}

buffer_t *buffer_turnaround(buffer_t *buf)
{
    return buffer_dyn_stub.buffer_ptr;
}

void buffer_note_predecessor(buffer_t *buf, const sockaddr_t *addr)
{

}

void buffer_data_add(buffer_t *buf, const uint8_t *data_ptr, uint16_t data_len)
{
}

buffer_t *buffer_clone(buffer_t *buf)
{
    return buffer_dyn_stub.buffer_ptr;
}

uint16_t buffer_ipv6_fcf(const buffer_t *buf, uint8_t next_header)
{
    return buffer_dyn_stub.uint16_value;
}

void buffer_copy_metadata(buffer_t *dst, buffer_t *src, bool non_clonable_to_dst)
{

}
