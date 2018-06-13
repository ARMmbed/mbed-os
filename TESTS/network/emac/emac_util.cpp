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

#include "mbed.h"

#include "EMAC.h"
#include "EMACMemoryManager.h"
#include "emac_TestMemoryManager.h"

#include "emac_tests.h"
#include "emac_initialize.h"
#include "emac_util.h"
#include "emac_membuf.h"
#include "emac_ctp.h"

using namespace utest::v1;

/* For LPC boards define the memory bank ourselves to give us section placement
   control */
#ifndef ETHMEM_SECTION
#if defined(TARGET_LPC4088) || defined(TARGET_LPC4088_DM)
#  if defined (__ICCARM__)
#     define ETHMEM_SECTION
#  elif defined(TOOLCHAIN_GCC_CR)
#     define ETHMEM_SECTION __attribute__((section(".data.$RamPeriph32")))
#  else
#     define ETHMEM_SECTION __attribute__((section("AHBSRAM0"),aligned))
#  endif
#elif defined(TARGET_LPC1768) || defined(TARGET_LPC1769)
#  if defined (__ICCARM__)
#     define ETHMEM_SECTION
#  elif defined(TOOLCHAIN_GCC_CR)
#     define ETHMEM_SECTION __attribute__((section(".data.$RamPeriph32")))
#  else
#     define ETHMEM_SECTION __attribute__((section("AHBSRAM1"),aligned))
#  endif
#endif
#endif

#ifndef ETHMEM_SECTION
#define ETHMEM_SECTION
#endif

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

// MTU size
static int eth_mtu_size = 0;

// Event queue
static rtos::Semaphore worker_loop_semaphore;
static rtos::Semaphore link_status_semaphore;

#if defined (__ICCARM__)
#pragma location = ".ethusbram"
#endif
ETHMEM_SECTION static EventQueue worker_loop_event_queue(20 * EVENTS_EVENT_SIZE);

static void worker_loop_event_cb(int event);
static Event<void(int)> worker_loop_event(&worker_loop_event_queue, worker_loop_event_cb);
static void link_input_event_cb(void *buf);
static Event<void(void *)> link_input_event(&worker_loop_event_queue, link_input_event_cb);

// Found echo server addresses
static unsigned char eth_mac_echo_server_addr[ECHO_SERVER_COUNT][ETH_MAC_ADDR_LEN];
static int etc_mac_echo_server_free_index = 0;

static bool output_memory = true;
static bool input_memory = true;

static void (*current_test_step_cb_fnc)(int opt);

// Outgoing messages
static outgoing_msg_t outgoing_msgs[OUTGOING_MSG_COUNT];

static unsigned int trace_level = 0;
static unsigned int error_flags = 0;
static unsigned int no_response_cnt = 0;
static bool link_up = false;

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

            if (!(outgoing_msgs[i].flags & RESPONSE_RECEIVED)) {
                count++;
            }
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

bool emac_if_update_reply_to_outgoing_msg(int receipt_number, int length, int invalid_data_index)
{
    int32_t outgoing_msg_index = emac_if_find_outgoing_msg(receipt_number);

    if (outgoing_msg_index >= 0) {
        outgoing_msgs[outgoing_msg_index].flags |= RESPONSE_RECEIVED;

        if (outgoing_msgs[outgoing_msg_index].length < ETH_FRAME_MIN_LEN) {
            /* If length of the sent message is smaller than Ethernet minimum frame length, validates against
               minimum frame length or sent length (in case frame has been converted to be longer than minimum
               length does not validate length)  */
            if (length != ETH_FRAME_MIN_LEN && length != ETH_FRAME_PADD_LEN && outgoing_msgs[outgoing_msg_index].length != length && length < ETH_FRAME_MIN_LEN) {
                outgoing_msgs[outgoing_msg_index].flags |= INVALID_LENGHT;
            }
        } else {
            if (outgoing_msgs[outgoing_msg_index].length != length) {
                outgoing_msgs[outgoing_msg_index].flags |= INVALID_LENGHT;
            }
        }
        if (invalid_data_index && invalid_data_index < outgoing_msgs[outgoing_msg_index].length) {
            outgoing_msgs[outgoing_msg_index].flags |= INVALID_DATA;
        }
        return true;
    } else {
        return false;
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

void emac_if_reset_error_flags(unsigned int error_flags_value)
{
    error_flags &= ~error_flags_value;
    if (error_flags_value & NO_RESPONSE) {
        no_response_cnt = 0;
    }
}

void emac_if_reset_all_error_flags(void)
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

char emac_if_get_trace_level(void)
{
    return trace_level;
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

void emac_if_set_all_multicast(bool all)
{
    emac_if_get()->set_all_multicast(all);
}

void emac_if_add_multicast_group(uint8_t *address)
{
    emac_if_get()->add_multicast_group(address);
}

void emac_if_set_output_memory(bool memory)
{
    output_memory = memory;
}

void emac_if_set_input_memory(bool memory)
{
    input_memory = memory;

    emac_if_set_memory(memory);
}

void emac_if_check_memory(bool output)
{
    if (output) {
        emac_if_set_memory(output_memory);
    } else {
        emac_if_set_memory(input_memory);
    }
}

void emac_if_set_memory(bool memory)
{
    static bool memory_value = true;
    if (memory_value != memory ) {
        memory_value = memory;
        EmacTestMemoryManager *mem_mngr = emac_m_mngr_get();
        mem_mngr->set_memory_available(memory);
    }
}

void emac_if_link_state_change_cb(bool up)
{
    if (up) {
        worker_loop_event.post(LINK_UP);
    } else {
        worker_loop_event.post(LINK_DOWN);
    }
}

void emac_if_link_input_cb(void *buf)
{
    link_input_event.post(buf);
}

static void link_input_event_cb(void *buf)
{
    int length = emac_m_mngr_get()->get_total_len(buf);

    if (length >= ETH_FRAME_HEADER_LEN) {
        // Ethernet input frame
        unsigned char eth_input_frame_data[ETH_FRAME_HEADER_LEN];
        memset(eth_input_frame_data, 0, ETH_FRAME_HEADER_LEN);

        int invalid_data_index = emac_if_memory_buffer_read(buf, eth_input_frame_data);

        if (eth_input_frame_data[12] == 0x90 && eth_input_frame_data[13] == 0x00) {
            unsigned char eth_output_frame_data[ETH_FRAME_HEADER_LEN];
            int receipt_number;

            ctp_function function = emac_if_ctp_header_handle(eth_input_frame_data, eth_output_frame_data, emac_if_get_hw_addr(), &receipt_number);

            if (function == CTP_REPLY) {
                // If reply has valid receipt number
                if (emac_if_update_reply_to_outgoing_msg(receipt_number, length, invalid_data_index)) {
                    // Checks received messages for errors
                    emac_if_validate_outgoing_msg();
                    // Removes not replied retry entries if any
                    emac_if_reset_outgoing_msg();
                    // Removes retry entries no response flags
                    emac_if_reset_error_flags(NO_RESPONSE);
                    // Calls test loop
                    worker_loop_event_queue.call(current_test_step_cb_fnc, INPUT);
                }
#if MBED_CONF_APP_ECHO_SERVER
            // Echoes only if configured as echo server
            } else if (function == CTP_FORWARD) {
                emac_if_memory_buffer_write(buf, eth_output_frame_data, false);
                emac_if_get()->link_out(buf);
                buf = 0;
#endif
            }

            emac_if_add_echo_server_addr(&eth_input_frame_data[6]);

            if (trace_level & TRACE_ETH_FRAMES) {
                 printf("INP> LEN %i\r\n\r\n", length);
                 const char trace_type[] = "INP>";
                 emac_if_trace_to_ascii_hex_dump(trace_type, ETH_FRAME_HEADER_LEN, eth_input_frame_data);
            }
        }
    }

    if (buf) {
        emac_m_mngr_get()->free(buf);
    }
}


#if defined (__ICCARM__)
#pragma location = ".ethusbram"
#endif
ETHMEM_SECTION static unsigned char thread_stack[2048];

void worker_loop(void);

void worker_loop_init(void)
{
    static rtos::Thread worker_loop_thread(osPriorityNormal, 2048, thread_stack);

    static bool init_done = false;

    if (!init_done) {
        if (worker_loop_thread.get_state() == Thread::Deleted) {
            worker_loop_thread.start(mbed::callback(&worker_loop));
        }
        init_done = true;
    }
}

void worker_loop_start(void (*test_step_cb_fnc)(int opt), int timeout)
{
    current_test_step_cb_fnc = test_step_cb_fnc;

    int test_step_cb_timer = worker_loop_event_queue.call_every(timeout, test_step_cb_fnc, TIMEOUT);
    int timeout_outgoing_msg_timer = worker_loop_event_queue.call_every(1000, emac_if_timeout_outgoing_msg);

    int validate_outgoing_msg_timer = 0;
    if (timeout > 500) {
        // For long test step callback timeouts validates messages also between callback timeouts
        validate_outgoing_msg_timer = worker_loop_event_queue.call_every(200, emac_if_validate_outgoing_msg);
    }

#if MBED_CONF_APP_ECHO_SERVER
    worker_loop_semaphore.wait();
#else
    worker_loop_semaphore.wait(600 * SECOND_TO_MS);
#endif

    worker_loop_event_queue.cancel(test_step_cb_timer);
    worker_loop_event_queue.cancel(timeout_outgoing_msg_timer);
    if (validate_outgoing_msg_timer) {
        worker_loop_event_queue.cancel(validate_outgoing_msg_timer);
    }

    osDelay(1000);
}

static void worker_loop_event_cb(int event)
{
    if (event == LINK_UP) {
        link_up = true;
        link_status_semaphore.release();
    }

    if (event == LINK_DOWN) {
        link_up = false;
    }
}

void worker_loop_link_up_wait(void)
{
    if (!link_up) {
        link_status_semaphore.wait();
    }
}

void worker_loop_end(void)
{
    worker_loop_semaphore.release();
}

void worker_loop(void)
{
    worker_loop_event_queue.dispatch_forever();
}

unsigned char *emac_if_get_own_addr(void)
{
    return (emac_if_get_hw_addr());
}

int emac_if_get_mtu_size()
{
    return eth_mtu_size;
}

void emac_if_set_mtu_size(int mtu_size)
{
    eth_mtu_size = mtu_size;
}

#endif
