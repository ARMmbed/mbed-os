/*
 * Copyright (c) 2014-2016, Arm Limited and affiliates.
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

/**
* \file \test_libTrace\Test.c
*
* \brief Unit tests for libTrace
*/
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"

#include "unity.h"
#include "unity_improved.h"

/* Socket API*/

#include "socket_api.h"

typedef struct {
    int8_t id;
    void (*cb_ptr)(void *);
    uint8_t address[16];
    uint8_t *buffer_ptr;
    uint16_t length;
    ns_list_link_t link;
} socket_t;
typedef NS_LIST_HEAD(socket_t, link) socket_list_t;

socket_list_t sckt_list;
// Test function
int lt_test_sockets_open()
{
    return ns_list_count(&sckt_list);
}

socket_t *sim_get_socket(int8_t socket)
{
    ns_list_foreach(socket_t, cur_ptr, &sckt_list) {
        if (cur_ptr->id == socket) {
            return cur_ptr;
        }
    }
    return NULL;
}
int8_t socket_open(uint8_t protocol, uint16_t identifier, void (*passed_fptr)(void *))
{
    socket_t *sckt_ptr;
    int8_t id = 1;

    ns_list_foreach(socket_t, cur_ptr, &sckt_list) {
        if (cur_ptr->id == id) {
            id++;
            cur_ptr = ns_list_get_first(&sckt_list);
        }
    }
    sckt_ptr = ns_dyn_mem_alloc(sizeof(socket_t));
    ns_list_add_to_start(&sckt_list, sckt_ptr);
    sckt_ptr->id = id;
    sckt_ptr->cb_ptr = passed_fptr;
    printf("SIM Socket socket_open() id = %d\n", id);
    return sckt_ptr->id;
}
int8_t socket_close(int8_t socket)
{
    socket_t *sckt_ptr;
    printf("SIM Socket socket_close() id = %d\n", socket);

    sckt_ptr = sim_get_socket(socket);
    if (sckt_ptr != NULL) {
        ns_list_remove(&sckt_list, sckt_ptr);
        ns_dyn_mem_free(sckt_ptr);
    }
    return 0;
}
typedef struct {
    uint16_t socket_send_len;
    uint8_t *socket_send_ptr;
    uint8_t socket_send_address[16];
    int8_t socket_send_socket;
} socket_send_to_t;

int8_t socket_sendto(int8_t socket, ns_address_t *address, uint8_t *buffer, uint16_t length)
{
    socket_send_to_t *data_ptr = malloc(sizeof(socket_send_to_t));
    printf("SIM Socket socket_sendto() socket = %d length = %d\n", socket, length);
    data_ptr->socket_send_ptr = malloc(length);
    memcpy(data_ptr->socket_send_ptr, buffer, length);
    data_ptr->socket_send_len = length;
    data_ptr->socket_send_socket = socket;
    memcpy(data_ptr->socket_send_address, address->address, 16);
    TEST_METHOD_CALLED("socket_sendto", data_ptr);
    return 1;
}
void lt_test_socket_sendto(int8_t socket, uint8_t *address, uint8_t *buffer, uint16_t length)
{
    socket_send_to_t *data_ptr;
    data_ptr = TEST_METHOD_CHECK("socket_sendto");
    printf("lt_test_socket_sendto() parameters\n");
    printf("Address :\n");
    print_buf(data_ptr->socket_send_address, 16);
    printf("Data :\n");
    test_buf(buffer, length, data_ptr->socket_send_ptr, data_ptr->socket_send_len);
    TEST_ASSERT_EQUAL_INT_MESSAGE(socket, data_ptr->socket_send_socket, "Wrong Socket ID");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(address, data_ptr->socket_send_address, 16, "Incorrect destination address");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(buffer, data_ptr->socket_send_ptr, data_ptr->socket_send_len, "MESSAGE_DATA_FAILED");
    TEST_ASSERT_EQUAL_INT_MESSAGE(length, data_ptr->socket_send_len, "Message length wrong");
    free(data_ptr->socket_send_ptr);
    free(data_ptr);
}

void lt_test_socket_recv_data(uint8_t interface, int8_t socket, uint8_t *address, uint8_t *buffer, uint16_t length)
{
    socket_callback_t sckt_data;
    socket_t *sckt_ptr;
    sckt_ptr = sim_get_socket(socket);
    if (sckt_ptr == NULL) {
        printf("Invalid socket");
        TEST_FAIL();

        return ;
    }

    sckt_data.event_type = 0 /*SOCKET_DATA*/;
    sckt_data.socket_id = sckt_ptr->id;
    sckt_data.interface_id = interface;
    sckt_data.d_len = length;
    sckt_ptr->buffer_ptr = buffer;
    sckt_ptr->length = length;
    memcpy(sckt_ptr->address, address, 16);

    sckt_ptr->cb_ptr(&sckt_data);
}

int16_t socket_read(int8_t socket, ns_address_t *address, uint8_t *buffer, uint16_t length)
{
    printf("SIM Socket socket_read()\n");
    socket_t *sckt_ptr;
    sckt_ptr = sim_get_socket(socket);
    if (sckt_ptr == NULL) {
        printf("Socket not found\n");
        return -1;
    }
    memcpy(buffer, sckt_ptr->buffer_ptr, length);
    memcpy(address->address, sckt_ptr->address, 16);
    return length;
}
int8_t socket_setsockopt(int8_t socket, uint8_t level, uint8_t opt_name, const void *opt_value, uint16_t opt_len)
{
    printf("SIM Socket socket_setsockopt() socket = %d name = %d ", socket, opt_name);
    if (opt_name == 0xfe) {
        printf("SOCKET_INTERFACE_SELECT");
    }
    if (opt_name == 0xff) {
        printf("SOCKET_IPV6_ADDRESS_SELECT");
    }
    printf("\n");
    return 1;
}
