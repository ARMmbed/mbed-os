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

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#if MBED_CONF_APP_TEST_WIFI || MBED_CONF_APP_TEST_ETHERNET

extern "C" {   // netif input
#include "tcpip.h"
}

#include "emac_api.h"
#include "emac_stack_mem.h"

#include "emac_tests.h"
#include "emac_initialize.h"
#include "emac_util.h"
#include "emac_membuf.h"
#include "emac_ctp.h"

using namespace utest::v1;

typedef struct {
    int length;
    int receipt_number;
    unsigned short flags;
    unsigned short lifetime;
} outgoing_msg_t;

#define ECHO_SERVER_COUNT       5

#define OUTGOING_MSG_COUNT      100

// Event flags
#define LINK_UP                 0x01
#define LINK_DOWN               0x02

// Hook to lwip input function
extern struct netif *netif_list;

// Broadcast address
const unsigned char eth_mac_broadcast_addr[ETH_MAC_ADDR_LEN] = {0xff,0xff,0xff,0xff,0xff,0xff};

// Event queue
static EventQueue worker_loop_event_queue;
static void worker_loop_event_cb(int event);
static Event<void(int)> worker_loop_event(&worker_loop_event_queue, worker_loop_event_cb);
static void link_input_event_cb(emac_stack_mem_chain_t *mem_chain_p);
static Event<void(emac_stack_mem_chain_t *)> link_input_event(&worker_loop_event_queue, link_input_event_cb);

// Found echo server addresses
static unsigned char eth_mac_echo_server_addr[ECHO_SERVER_COUNT][ETH_MAC_ADDR_LEN];
static int etc_mac_echo_server_free_index = 0;

// Outgoing messages
static outgoing_msg_t outgoing_msgs[OUTGOING_MSG_COUNT];

static unsigned int trace_level = 0;
static unsigned int error_flags = 0;
static unsigned int no_response_cnt = 0;

int emac_if_find_outgoing_msg(int receipt_number)
{
    for (int i = 0; i < OUTGOING_MSG_COUNT; i++) {
        if (outgoing_msgs[i].length && outgoing_msgs[i].receipt_number == receipt_number) {
            return i;
        }
    }
    return -1;
}

void emac_if_free_outgoing_msg(int index)
{
    outgoing_msgs[index].length = 0;
}

int emac_if_count_outgoing_msg(void)
{
    int count = 0;

    for (int i = 0; i < OUTGOING_MSG_COUNT; i++) {
        if (outgoing_msgs[i].length) {
            count++;
        }
    }

    return count;
}

void emac_if_reset_outgoing_msg(void)
{
    for (int i = 0; i < OUTGOING_MSG_COUNT; i++) {
        if (outgoing_msgs[i].length) {
            outgoing_msgs[i].length = 0;
        }
    }
}

int emac_if_add_outgoing_msg(int length)
{
    for (int i = 0; i < OUTGOING_MSG_COUNT; i++) {
        if (!outgoing_msgs[i].length) {
            outgoing_msgs[i].receipt_number = 0;
            outgoing_msgs[i].length = length;
            outgoing_msgs[i].flags = 0;
            outgoing_msgs[i].lifetime = 10;
            return i;
        }
    }
    return -1;
}

void emac_if_set_outgoing_msg_receipt_num(int index, int receipt_number)
{
    outgoing_msgs[index].receipt_number = receipt_number;
}

void emac_if_set_outgoing_msg_flags(int index, int flags)
{
    outgoing_msgs[index].flags |= flags;
}

void emac_if_timeout_outgoing_msg(void)
{
    for (int i = 0; i < OUTGOING_MSG_COUNT; i++) {
        if (outgoing_msgs[i].length) {
            if (outgoing_msgs[i].lifetime) {
                outgoing_msgs[i].lifetime--;
                if (outgoing_msgs[i].lifetime == 0) {
                    SET_ERROR_FLAGS(NO_RESPONSE);
                }
            }
        }
    }
}

void emac_if_validate_outgoing_msg(void)
{
    static char broadcast_resp_count = 0;

    for (int i = 0; i < OUTGOING_MSG_COUNT; i++) {
        if (outgoing_msgs[i].length) {

            if (outgoing_msgs[i].flags & RESPONSE_RECEIVED) {

                int failure = outgoing_msgs[i].flags & (INVALID_LENGHT | INVALID_DATA);

                if (failure) {
                    SET_ERROR_FLAGS(MSG_VALID_ERROR);
                }

                if (!(outgoing_msgs[i].flags & PRINTED)) {
                    if ((trace_level & TRACE_SUCCESS) || ((trace_level & TRACE_FAILURE) && failure)) {
                        printf("response: receipt number %i %s %s %s\r\n\r\n", outgoing_msgs[i].receipt_number,
                            outgoing_msgs[i].flags & INVALID_LENGHT ? "LENGTH INVALID" : "LENGTH OK",
                            outgoing_msgs[i].flags & INVALID_DATA ? "DATA INVALID" : "DATA OK",
                            outgoing_msgs[i].flags & BROADCAST ? "BROADCAST" : "UNICAST");
                        outgoing_msgs[i].flags |= PRINTED;
                    }
                }

                if (outgoing_msgs[i].flags & BROADCAST) {
                    outgoing_msgs[i].lifetime = 2;
                    broadcast_resp_count++;
                    if (broadcast_resp_count > 5) {
                        emac_if_free_outgoing_msg(i);
                    }
                } else {
                    emac_if_free_outgoing_msg(i);
                }
            }

            if (!outgoing_msgs[i].lifetime) {
                if (!(outgoing_msgs[i].flags & RESPONSE_RECEIVED) && (trace_level & TRACE_FAILURE)) {
                    printf("NO RESPONSE: receipt number %i\r\n\r\n", outgoing_msgs[i].receipt_number);
                }
                emac_if_free_outgoing_msg(i);
            }
        }
    }
}

void emac_if_update_reply_to_outgoing_msg(int receipt_number, int lenght, int invalid_data_index)
{
    int32_t outgoing_msg_index = emac_if_find_outgoing_msg(receipt_number);

    if (outgoing_msg_index >= 0) {
        outgoing_msgs[outgoing_msg_index].flags |= RESPONSE_RECEIVED;

#if MBED_CONF_APP_TEST_ETHERNET
        if (outgoing_msgs[outgoing_msg_index].length < ETH_FRAME_MIN_LEN) {
            if (lenght != ETH_FRAME_MIN_LEN) {
                outgoing_msgs[outgoing_msg_index].flags |= INVALID_LENGHT;
            }
        } else {
#endif
            if (outgoing_msgs[outgoing_msg_index].length != lenght) {
                outgoing_msgs[outgoing_msg_index].flags |= INVALID_LENGHT;
            }
#if MBED_CONF_APP_TEST_ETHERNET
        }
#endif
        if (invalid_data_index && invalid_data_index < outgoing_msgs[outgoing_msg_index].length) {
            outgoing_msgs[outgoing_msg_index].flags |= INVALID_DATA;
        }
    }
}

void emac_if_add_echo_server_addr(unsigned char *addr)
{
    if (etc_mac_echo_server_free_index == ECHO_SERVER_COUNT) {
        return;
    }

    for (int i = 0; i < etc_mac_echo_server_free_index; i++) {
        if (memcmp(&eth_mac_echo_server_addr[i][0], addr, ETH_MAC_ADDR_LEN) == 0) {
            return;
        }
    }

    memcpy(&eth_mac_echo_server_addr[etc_mac_echo_server_free_index][0], addr, ETH_MAC_ADDR_LEN);
    etc_mac_echo_server_free_index++;
}

int emac_if_count_echo_server_addr(void)
{
    return etc_mac_echo_server_free_index;
}

unsigned char *emac_if_get_echo_server_addr(int index)
{
    if (index < etc_mac_echo_server_free_index) {
        return &eth_mac_echo_server_addr[index][0];
    }

    return 0;
}

void emac_if_set_error_flags(unsigned int error_flags_value)
{
    error_flags |= error_flags_value;

    if (error_flags_value & NO_RESPONSE) {
        no_response_cnt++;
    }
}

unsigned int emac_if_get_error_flags(void)
{
    int error_flags_value = error_flags;

    // Indicate no response error only if more than three messages are lost
    if (error_flags_value & NO_RESPONSE) {
        if (no_response_cnt < 3) {
            error_flags_value &= ~NO_RESPONSE;
        }
    }

    return error_flags_value;
}

void emac_if_reset_error_flags(void)
{
    error_flags = 0;
    no_response_cnt = 0;
}

void emac_if_print_error_flags(void)
{
   int error_flags_value = emac_if_get_error_flags();

   char no_resp_message[50];
   if (error_flags_value & NO_RESPONSE) {
       snprintf(no_resp_message, 50, "no response from echo server, counter: %i", no_response_cnt);
   } else if (no_response_cnt > 0) {
       printf("no response from echo server, counter: %i\r\n\r\n", no_response_cnt);
   }

   printf("test result: %s%s%s%s%s%s\r\n\r\n",
       error_flags_value ? "Test FAILED, reason: ": "PASS",
       error_flags_value & TEST_FAILED ? "test failed ": "",
       error_flags_value & MSG_VALID_ERROR ? "message content validation error ": "",
       error_flags_value & OUT_OF_MSG_DATA ? "out of message validation data storage ": "",
       error_flags_value & NO_FREE_MEM_BUF ? "no free memory buffers ": "",
       error_flags_value & NO_RESPONSE ? no_resp_message: "");
}

void emac_if_set_trace_level(char trace_level_value)
{
    trace_level = trace_level_value;
}

void emac_if_trace_to_ascii_hex_dump(const char *prefix, int len, unsigned char *data)
{
    int line_len = 0;

    for (int i = 0; i < len; i++) {
       if ((line_len % 14) == 0) {
           if (line_len != 0) {
               printf("\r\n");
           }
           printf("%s %06x", prefix, line_len);
       }
       line_len++;
       printf(" %02x", data[i]);
    }
    printf("\r\n\r\n");
}

void emac_if_link_state_change_cb(void *data, bool up)
{
    if (up) {
        worker_loop_event.post(LINK_UP);
    } else {
        worker_loop_event.post(LINK_DOWN);
    }
}

void emac_if_link_input_cb(void *data, emac_stack_mem_chain_t *mem_chain_p)
{
    link_input_event.post(mem_chain_p);
}

static void link_input_event_cb(emac_stack_mem_chain_t *mem_chain_p)
{
    int lenght = emac_stack_mem_len(0, mem_chain_p);

    if (lenght >= ETH_FRAME_HEADER_LEN) {
        // Ethernet input frame
        unsigned char eth_input_frame_data[ETH_FRAME_HEADER_LEN];
        memset(eth_input_frame_data, 0, ETH_FRAME_HEADER_LEN);

        int invalid_data_index = emac_if_memory_buffer_read(mem_chain_p, eth_input_frame_data);

        if (eth_input_frame_data[12] == 0x90 && eth_input_frame_data[13] == 0x00) {
            unsigned char eth_output_frame_data[ETH_FRAME_HEADER_LEN];
            int receipt_number;

            ctp_function function = emac_if_ctp_header_handle(eth_input_frame_data, eth_output_frame_data, emac_if_get_hw_addr(), &receipt_number);

            if (function == CTP_REPLY) {
                emac_if_update_reply_to_outgoing_msg(receipt_number, lenght, invalid_data_index);
#if MBED_CONF_APP_ECHO_SERVER
            // Echoes only if configured as echo server
            } else if (function == CTP_FORWARD) {
                emac_if_memory_buffer_write(mem_chain_p, eth_output_frame_data, false);
                emac_if_get()->ops.link_out(emac_if_get(), mem_chain_p);
#endif
            }

            emac_if_add_echo_server_addr(&eth_input_frame_data[6]);

            emac_stack_mem_free(0, mem_chain_p);

            if (trace_level & TRACE_ETH_FRAMES) {
                 printf("LEN %i\r\n\r\n", lenght);
                 const char trace_type[] = "INP>";
                 emac_if_trace_to_ascii_hex_dump(trace_type, ETH_FRAME_HEADER_LEN, eth_input_frame_data);
            }
            return;
        }
    }

    // Forward other than CTP frames to lwip
    struct netif *netif;

    /* loop through netif's */
    netif = netif_list;
    if (netif != NULL) {
        struct pbuf *p = (struct pbuf *)mem_chain_p;

        /* pass all packets to ethernet_input, which decides what packets it supports */
        if (netif->input(p, netif) != ERR_OK) {
            emac_stack_mem_free(0, mem_chain_p);
        }
    } else {
        emac_stack_mem_free(0, mem_chain_p);
    }
}

void worker_loop_start(void (*test_step_cb_fnc)(void), int timeout)
{
    int test_step_cb_timer = worker_loop_event_queue.call_every(timeout, test_step_cb_fnc);
    int timeout_outgoing_msg_timer = worker_loop_event_queue.call_every(1000, emac_if_timeout_outgoing_msg);

#if MBED_CONF_APP_ECHO_SERVER
    worker_loop_event_queue.dispatch_forever();
#else
    worker_loop_event_queue.dispatch(600 * SECOND_TO_MS);
#endif

    worker_loop_event_queue.cancel(test_step_cb_timer);
    worker_loop_event_queue.cancel(timeout_outgoing_msg_timer);

    worker_loop_event_queue.dispatch(5);
}

static void worker_loop_event_cb(int event)
{
    if (event == LINK_UP) {
        printf("cable connected\r\n\r\n");
    }

    if (event == LINK_DOWN) {
        printf("cable disconnected\r\n\r\n");
    }
}

void worker_loop_end(void)
{
    worker_loop_event_queue.break_dispatch();
}

unsigned char *emac_if_get_own_addr(void)
{
    return (emac_if_get_hw_addr());
}

#endif
