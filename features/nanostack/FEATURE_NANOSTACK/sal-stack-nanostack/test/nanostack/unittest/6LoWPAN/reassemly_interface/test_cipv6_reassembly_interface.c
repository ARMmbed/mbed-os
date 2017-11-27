/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
#include <string.h>
#include "6LoWPAN/Fragmentation/cipv6_fragmenter.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "MAC/rf_driver_storage.h"
#include "ns_error_types.h"
#include "mle.h"
#include "mlme.h"
#include "mac_api.h"

#include "address_stub.h"
#include "mle_stub.h"
#include "protocol_core_stub.h"
#include "mac_helper_stub.h"
#include "socket_stub.h"
#include "mesh_stub.h"
#include "rpl_data_stub.h"
#include "nsdynmemLIB_stub.h"
#include "buffer_dyn_stub.h"
#include "common_functions_stub.h"


bool test_reassembly_interface_init()
{

    if (-2 != reassembly_interface_init(0, 0, 5) ) {
        return false;
    }

    if (-2 != reassembly_interface_init(0, 8, 0) ) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 0;

    if (-1 != reassembly_interface_init(0, 8, 5) ) {
        return false;
    }
    nsdynmemlib_stub.returnCounter = 1;
    if (-1 != reassembly_interface_init(0, 8, 5) ) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    if (0 != reassembly_interface_init(0, 8, 5) ) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    if (0 != reassembly_interface_init(0, 8, 5) ) {
        return false;
    }

    reassembly_interface_free(0);
    return true;
}

bool test_reassembly_interface_free()
{
    if (-1 != reassembly_interface_free(0) ) {
        return false;
    }
    nsdynmemlib_stub.returnCounter = 2;
    reassembly_interface_init(0, 8, 5);

    if (-1 != reassembly_interface_free(1) ) {
        return false;
    }

    if (0 != reassembly_interface_free(0) ) {
        return false;
    }

    return true;
}

static buffer_t * test_fragment_first_96(buffer_t *buf, uint8_t index)
{
    common_functions_stub.readuint16_from_queue = 2;
    common_functions_stub.uint16_value_queue[1] = 232;
    common_functions_stub.uint16_value_queue[0] = 100 + index;
    buf->buf_ptr = 0;
    buf->buf_end = 100;

    uint8_t *ptr = buf->buf + buf->buf_ptr;
    *ptr = 0xc0; //first sequency
    return buf;

}


static buffer_t * test_fragment_last_96(buffer_t *buf, uint8_t index)
{
    common_functions_stub.readuint16_from_queue = 2;
    common_functions_stub.uint16_value_queue[1] = 232;
    common_functions_stub.uint16_value_queue[0] = 100 + index;
    buf->buf_ptr = 0;
    buf->buf_end = 101;

    uint8_t *ptr = buf->buf + buf->buf_ptr;
    *ptr = 0x20; //sub sequency
    ptr += 4;
    *ptr = (96 + 40) / 8; //Set offset
    return buf;

}

static buffer_t *test_buffer_get(uint16_t size)
{
    buffer_t *buf = malloc(sizeof (buffer_t) + size);
    memset(buf, 0, sizeof(buffer_t));
    return buf;

}



bool test_reassembly_interface_reset()
{

    if (-1 != reassembly_interface_reset(0) ) {
        return false;
    }


    nsdynmemlib_stub.returnCounter = 2;
    reassembly_interface_init(0, 8, 5);
    buffer_t *reassembly_buf = test_buffer_get(127);
    buffer_t *test_buf = test_buffer_get(1500);
    buffer_dyn_stub.buffer_ptr = test_buf;
    reassembly_buf = test_fragment_first_96(reassembly_buf, 0);

    cipv6_frag_reassembly(0, reassembly_buf);
    reassembly_buf = test_fragment_first_96(reassembly_buf, 1);
    cipv6_frag_reassembly(0, reassembly_buf);

    if (0 != reassembly_interface_reset(0) ) {
        return false;
    }
    reassembly_interface_free(0);
    free(test_buf);
    free(reassembly_buf);
    return true;
}


bool test_cipv6_frag_reassembly()
{


    buffer_t *reassembly_buf = test_buffer_get(127);
    buffer_t *test_buf = test_buffer_get(1500);
    buffer_t *test_buf2 = test_buffer_get(1500);
    buffer_t *test_buf3 = test_buffer_get(1500);
    buffer_t *test_buf4 = test_buffer_get(1500);
    buffer_t *test_buf5 = test_buffer_get(1500);
    buffer_t *test_buf6 = test_buffer_get(1500);
    buffer_t *test_buf7 = test_buffer_get(1500);
    buffer_t *test_buf8 = test_buffer_get(1500);
    buffer_t *test_buf9 = test_buffer_get(1500);


    if (cipv6_frag_reassembly(0, reassembly_buf) ) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    reassembly_interface_init(0, 8, 5);
    common_functions_stub.readuint16_from_queue = 0;
    common_functions_stub.uint16_value = 0;


    //Test Size zero drop

    if (cipv6_frag_reassembly(0, reassembly_buf) ) {
        return false;
    }
    reassembly_buf->buf_end = 96;
    //Test buffer allocation fail
    buffer_dyn_stub.buffer_ptr = NULL;
    common_functions_stub.readuint16_from_queue = 2;
    common_functions_stub.uint16_value_queue[1] = 200;
    common_functions_stub.uint16_value_queue[0] = 100;

    uint8_t *ptr = reassembly_buf->buf + reassembly_buf->buf_ptr;

    *ptr = 0xc0; //First fragment

    if (cipv6_frag_reassembly(0, reassembly_buf) ) {
        return false;
    }

    //Test frag out of range
    address_stub.uint8_value = 2;
    reassembly_buf->buf_ptr = 0;
    reassembly_buf->buf_end = 96;
    //Test buffer allocation fail
    common_functions_stub.readuint16_from_queue = 2;
    common_functions_stub.uint16_value_queue[1] = 160;
    common_functions_stub.uint16_value_queue[0] = 100;
    buffer_dyn_stub.buffer_ptr = test_buf;
    if (cipv6_frag_reassembly(0, reassembly_buf) ) {
        return false;
    }

    reassembly_buf->buf_ptr = 0;
    reassembly_buf->buf_end = 96;
    ptr = reassembly_buf->buf + reassembly_buf->buf_ptr;
    *ptr = 0x20; //sub sequency
    ptr += 4;
    *ptr = 96 / 8; //Set offset
    common_functions_stub.readuint16_from_queue = 2;
    common_functions_stub.uint16_value_queue[1] = 160;
    common_functions_stub.uint16_value_queue[0] = 100;
    if (cipv6_frag_reassembly(0, reassembly_buf) ) {
        return false;
    }
    reassembly_interface_reset(0);


    //Test succesfully fragment push
    reassembly_buf = test_fragment_first_96(reassembly_buf, 0);
    buffer_dyn_stub.buffer_ptr = test_buf;
    if (cipv6_frag_reassembly(0, reassembly_buf) ) {
        return false;
    }
    reassembly_buf = test_fragment_last_96(reassembly_buf, 0);

    buffer_t *reassemled_buffer = cipv6_frag_reassembly(0, reassembly_buf);
    if (!reassemled_buffer || (reassemled_buffer->buf_end - reassemled_buffer->buf_ptr) != 192) {
        return false;
    }

    //Receive inversr order

    reassembly_buf = test_fragment_last_96(reassembly_buf, 0);
    if (cipv6_frag_reassembly(0, reassembly_buf) ) {
        return false;
    }

    reassembly_buf = test_fragment_first_96(reassembly_buf, 0);
    reassemled_buffer = cipv6_frag_reassembly(0, reassembly_buf);
    if (!reassemled_buffer || (reassemled_buffer->buf_end - reassemled_buffer->buf_ptr) != 192) {
        return false;
    }

    //Receive 2., 3. & 1.
    common_functions_stub.uint16_value_queue[1] = 328;
    common_functions_stub.uint16_value_queue[0] = 100;

    reassembly_buf->buf_ptr = 0;
    reassembly_buf->buf_end = 101;
    ptr = reassembly_buf->buf + reassembly_buf->buf_ptr;
    *ptr = 0x20; //sub sequency
    ptr += 4;
    *ptr = (96 + 40) / 8; //Set offset
    common_functions_stub.readuint16_from_queue = 2;

    if (cipv6_frag_reassembly(0, reassembly_buf) ) {
        return false;
    }

    reassembly_buf->buf_ptr = 0;
    reassembly_buf->buf_end = 101;
    ptr = reassembly_buf->buf + reassembly_buf->buf_ptr;
    *ptr = 0x20; //sub sequency
    ptr += 4;
    *ptr = (96 + 40 + 96) / 8; //Set offset
    common_functions_stub.readuint16_from_queue = 2;
    if (cipv6_frag_reassembly(0, reassembly_buf) ) {
        return false;
    }


    reassembly_buf->buf_ptr = 0;
    reassembly_buf->buf_end = 100;
    ptr = reassembly_buf->buf + reassembly_buf->buf_ptr;
    *ptr = 0xc0; //sub sequency

    common_functions_stub.readuint16_from_queue = 2;
    reassemled_buffer = cipv6_frag_reassembly(0, reassembly_buf);
    if (!reassemled_buffer || (reassemled_buffer->buf_end - reassemled_buffer->buf_ptr) != 192 + 96) {
        return false;
    }

    //First , last, middle 1., 3. 2.
    reassembly_buf->buf_ptr = 0;
    reassembly_buf->buf_end = 100;
    ptr = reassembly_buf->buf + reassembly_buf->buf_ptr;
    *ptr = 0xc0; //sub sequency

    common_functions_stub.readuint16_from_queue = 2;

    if (cipv6_frag_reassembly(0, reassembly_buf) ) {
        return false;
    }
    reassembly_buf->buf_ptr = 0;
    reassembly_buf->buf_end = 101;
    ptr = reassembly_buf->buf + reassembly_buf->buf_ptr;
    *ptr = 0x20; //sub sequency Last
    ptr += 4;
    *ptr = (96 + 40 + 96) / 8; //Set offset
    common_functions_stub.readuint16_from_queue = 2;
    if (cipv6_frag_reassembly(0, reassembly_buf) ) {
        return false;
    }

    reassembly_buf->buf_ptr = 0;
    reassembly_buf->buf_end = 101;
    ptr = reassembly_buf->buf + reassembly_buf->buf_ptr;
    *ptr = 0x20; //sub sequency
    ptr += 4;
    *ptr = (96 + 40) / 8; //Set offset
    common_functions_stub.readuint16_from_queue = 2;

    reassemled_buffer = cipv6_frag_reassembly(0, reassembly_buf);
    if (!reassemled_buffer || (reassemled_buffer->buf_end - reassemled_buffer->buf_ptr) != 192 + 96) {
        return false;
    }


    //3., 2., 1.
    reassembly_buf->buf_ptr = 0;
    reassembly_buf->buf_end = 101;
    ptr = reassembly_buf->buf + reassembly_buf->buf_ptr;
    *ptr = 0x20; //sub sequency Last
    ptr += 4;
    *ptr = (96 + 40 + 96) / 8; //Set offset
    common_functions_stub.readuint16_from_queue = 2;
    if (cipv6_frag_reassembly(0, reassembly_buf) ) {
        return false;
    }

    reassembly_buf->buf_ptr = 0;
    reassembly_buf->buf_end = 101;
    ptr = reassembly_buf->buf + reassembly_buf->buf_ptr;
    *ptr = 0x20; //sub sequency
    ptr += 4;
    *ptr = (96 + 40) / 8; //Set offset
    common_functions_stub.readuint16_from_queue = 2;
    if (cipv6_frag_reassembly(0, reassembly_buf) ) {
        return false;
    }
    reassembly_buf->buf_ptr = 0;
    reassembly_buf->buf_end = 100;
    ptr = reassembly_buf->buf + reassembly_buf->buf_ptr;
    *ptr = 0xc0; //sub sequency
    common_functions_stub.readuint16_from_queue = 2;
    reassemled_buffer = cipv6_frag_reassembly(0, reassembly_buf);
    if (!reassemled_buffer || (reassemled_buffer->buf_end - reassemled_buffer->buf_ptr) != 192 + 96) {
        return false;
    }


    //Test 2., 1. and 3.
    reassembly_buf->buf_ptr = 0;
    reassembly_buf->buf_end = 101;
    ptr = reassembly_buf->buf + reassembly_buf->buf_ptr;
    *ptr = 0x20; //sub sequency
    ptr += 4;
    *ptr = (96 + 40) / 8; //Set offset
    common_functions_stub.readuint16_from_queue = 2;
    if (cipv6_frag_reassembly(0, reassembly_buf) ) {
        return false;
    }

    reassembly_buf->buf_ptr = 0;
    reassembly_buf->buf_end = 100;
    ptr = reassembly_buf->buf + reassembly_buf->buf_ptr;
    *ptr = 0xc0; //first sequency
    common_functions_stub.readuint16_from_queue = 2;


    if (cipv6_frag_reassembly(0, reassembly_buf) ) {
        return false;
    }

    reassembly_buf->buf_ptr = 0;
    reassembly_buf->buf_end = 101;
    ptr = reassembly_buf->buf + reassembly_buf->buf_ptr;
    *ptr = 0x20; //sub sequency
    ptr += 4;
    *ptr = (96 + 40 + 96) / 8; //Set offset
    common_functions_stub.readuint16_from_queue = 2;
    reassemled_buffer = cipv6_frag_reassembly(0, reassembly_buf);
    if (!reassemled_buffer || (reassemled_buffer->buf_end - reassemled_buffer->buf_ptr) != 192 + 96) {
        return false;
    }

    //Test overlap last
    common_functions_stub.uint16_value_queue[1] = 328;
    common_functions_stub.uint16_value_queue[0] = 101;
    reassembly_buf->buf_ptr = 0;
    reassembly_buf->buf_end = 101;
    ptr = reassembly_buf->buf + reassembly_buf->buf_ptr;
    *ptr = 0x20; //sub sequency
    ptr += 4;
    *ptr = (96 + 40) / 8; //Set offset
    common_functions_stub.readuint16_from_queue = 2;
    if (cipv6_frag_reassembly(0, reassembly_buf) ) {
        return false;
    }

    reassembly_buf->buf_ptr = 0;
    reassembly_buf->buf_end = 100;
    ptr = reassembly_buf->buf + reassembly_buf->buf_ptr;
    *ptr = 0xc0; //first sequency
    common_functions_stub.readuint16_from_queue = 2;


    if (cipv6_frag_reassembly(0, reassembly_buf) ) {
        return false;
    }

    reassembly_buf->buf_ptr = 0;
    reassembly_buf->buf_end = 102;
    ptr = reassembly_buf->buf + reassembly_buf->buf_ptr;
    *ptr = 0x20; //sub sequency
    ptr += 4;
    *ptr = (96 + 40 + 96) / 8; //Set offset
    common_functions_stub.readuint16_from_queue = 2;
    if (cipv6_frag_reassembly(0, reassembly_buf)) {
        return false;
    }

    //Overflow first fragment
    common_functions_stub.uint16_value_queue[1] = 328;
    common_functions_stub.uint16_value_queue[0] = 102;
    reassembly_buf->buf_ptr = 0;
    reassembly_buf->buf_end = 101;
    ptr = reassembly_buf->buf + reassembly_buf->buf_ptr;
    *ptr = 0x20; //sub sequency Last
    ptr += 4;
    *ptr = (96 + 40 + 96) / 8; //Set offset
    common_functions_stub.readuint16_from_queue = 2;
    if (cipv6_frag_reassembly(0, reassembly_buf) ) {
        return false;
    }

    reassembly_buf->buf_ptr = 0;
    reassembly_buf->buf_end = 101;
    ptr = reassembly_buf->buf + reassembly_buf->buf_ptr;
    *ptr = 0x20; //sub sequency
    ptr += 4;
    *ptr = (96 + 40) / 8; //Set offset
    common_functions_stub.readuint16_from_queue = 2;
    if (cipv6_frag_reassembly(0, reassembly_buf) ) {
        return false;
    }
    reassembly_buf->buf_ptr = 0;
    reassembly_buf->buf_end = 101;
    ptr = reassembly_buf->buf + reassembly_buf->buf_ptr;
    *ptr = 0xc0; //sub sequency
    common_functions_stub.readuint16_from_queue = 2;
    if (cipv6_frag_reassembly(0, reassembly_buf) ) {
        return false;
    }

    //Overflow middle of
    common_functions_stub.uint16_value_queue[1] = 328;
    common_functions_stub.uint16_value_queue[0] = 103;

    reassembly_buf->buf_ptr = 0;
    reassembly_buf->buf_end = 100;
    ptr = reassembly_buf->buf + reassembly_buf->buf_ptr;
    *ptr = 0xc0; //first sequency
    common_functions_stub.readuint16_from_queue = 2;

    if (cipv6_frag_reassembly(0, reassembly_buf) ) {
        return false;
    }

    reassembly_buf->buf_ptr = 0;
    reassembly_buf->buf_end = 101;
    ptr = reassembly_buf->buf + reassembly_buf->buf_ptr;
    *ptr = 0x20; //sub sequency
    ptr += 4;
    *ptr = (96 + 40 + 96) / 8; //Set offset
    common_functions_stub.readuint16_from_queue = 2;
    if (cipv6_frag_reassembly(0, reassembly_buf) ) {
        return false;
    }

    reassembly_buf->buf_ptr = 0;
    reassembly_buf->buf_end = 102;
    ptr = reassembly_buf->buf + reassembly_buf->buf_ptr;
    *ptr = 0x20; //sub sequency
    ptr += 4;
    *ptr = (96 + 40) / 8; //Set offset
    common_functions_stub.readuint16_from_queue = 2;
    if (cipv6_frag_reassembly(0, reassembly_buf) ) {
        return false;
    }

    reassembly_interface_reset(0);

    //Test reassembly overflow start 9 tx proces which 1 should fail and 8 other success
    memset(test_buf, 0, sizeof(buffer_t));
    reassembly_buf = test_fragment_first_96(reassembly_buf, 0);
    buffer_dyn_stub.buffer_ptr = test_buf;
    cipv6_frag_reassembly(0, reassembly_buf);

    buffer_dyn_stub.buffer_ptr = test_buf2;
    reassembly_buf = test_fragment_first_96(reassembly_buf, 1);
    cipv6_frag_reassembly(0, reassembly_buf);

    buffer_dyn_stub.buffer_ptr = test_buf3;
    reassembly_buf = test_fragment_first_96(reassembly_buf, 2);
    cipv6_frag_reassembly(0, reassembly_buf);

    buffer_dyn_stub.buffer_ptr = test_buf4;
    reassembly_buf = test_fragment_first_96(reassembly_buf, 3);
    cipv6_frag_reassembly(0, reassembly_buf);

    buffer_dyn_stub.buffer_ptr = test_buf5;
    reassembly_buf = test_fragment_first_96(reassembly_buf, 4);
    cipv6_frag_reassembly(0, reassembly_buf);

    buffer_dyn_stub.buffer_ptr = test_buf6;
    reassembly_buf = test_fragment_first_96(reassembly_buf, 5);
    cipv6_frag_reassembly(0, reassembly_buf);

    buffer_dyn_stub.buffer_ptr = test_buf7;
    reassembly_buf = test_fragment_first_96(reassembly_buf, 6);
    cipv6_frag_reassembly(0, reassembly_buf);

    buffer_dyn_stub.buffer_ptr = test_buf8;
    reassembly_buf = test_fragment_first_96(reassembly_buf, 7);
    cipv6_frag_reassembly(0, reassembly_buf);

    buffer_dyn_stub.buffer_ptr = test_buf9;
    reassembly_buf = test_fragment_first_96(reassembly_buf, 8);
    cipv6_frag_reassembly(0, reassembly_buf);

    //Start testing
    for (uint8_t i= 0; i< 9;i++) {
        reassembly_buf = test_fragment_last_96(reassembly_buf, i);
        buffer_t *fully_buffer = cipv6_frag_reassembly(0, reassembly_buf);
        if (i == 8) {
            if (fully_buffer) {
                return false;
            }

        } else {

            if (!fully_buffer || (fully_buffer->buf_end - fully_buffer->buf_ptr) != 192) {
                return false;
            }
        }
    }

    reassembly_interface_free(0);
    free(test_buf);
    free(test_buf2);
    free(test_buf3);
    free(test_buf4);
    free(test_buf5);
    free(test_buf6);
    free(test_buf7);
    free(test_buf8);
    free(test_buf9);
    free(reassembly_buf);
    common_functions_stub.readuint16_from_queue = 0;
    return true;
}

bool test_cipv6_frag_timer()
{
    buffer_t *reassembly_buf = test_buffer_get(127);
    buffer_t *test_buf = test_buffer_get(1500);
    buffer_t *test_buf2 = test_buffer_get(1500);

    reassembly_buf->dst_sa.addr_type = ADDR_802_15_4_SHORT;


    reassembly_buf = test_fragment_first_96(reassembly_buf, 0);


    buffer_dyn_stub.buffer_ptr = test_buf;


    nsdynmemlib_stub.returnCounter = 2;
    reassembly_interface_init(0, 8, 5);

    address_stub.uint8_value = 2;
    cipv6_frag_reassembly(0, reassembly_buf);

    cipv6_frag_timer(1);
    reassembly_buf = test_fragment_first_96(reassembly_buf, 1);
    buffer_dyn_stub.buffer_ptr = test_buf2;
    cipv6_frag_reassembly(0, reassembly_buf);


    //Push 2 buffer to queue and time out them
    buffer_dyn_stub.buffer_ptr = test_buf;
    cipv6_frag_timer(4);
    buffer_dyn_stub.buffer_ptr = test_buf2;
    socket_stub.buffer_ptr = test_buf2;
    cipv6_frag_timer(4);
    reassembly_interface_free(0);
    free(reassembly_buf);
    free(test_buf);
    free(test_buf2);
    return true;
}


