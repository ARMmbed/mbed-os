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

#ifndef EMAC_UTIL_H
#define EMAC_UTIL_H

#define SECOND_TO_US            1000000
#define SECOND_TO_MS            1000
#define MS_TO_US                1000

extern const unsigned char eth_mac_broadcast_addr[];

// Trace flags
#define TRACE_NONE              0x00
#define TRACE_ETH_FRAMES        0x01
#define TRACE_SUCCESS           0x02
#define TRACE_FAILURE           0x04
#define TRACE_SEND              0x08
// Sets trace level
#define SET_TRACE_LEVEL(level)  emac_if_set_trace_level(level)

// Message validation flags
#define BROADCAST               0x01
#define RESPONSE_RECEIVED       0x02
#define INVALID_LENGTH          0x04
#define INVALID_DATA            0x08
#define PRINTED                 0x10
// Validates outgoing messages for replies, sets error flags on error
#define VALIDATE_OUTGOING_MESSAGES   emac_if_validate_outgoing_msg();
// Resets message data
#define RESET_OUTGOING_MSG_DATA      emac_if_reset_outgoing_msg()

// General error flags
#define TEST_FAILED             0x01
#define MSG_VALID_ERROR         0x02
#define OUT_OF_MSG_DATA         0x04
#define NO_FREE_MEM_BUF         0x08
#define NO_RESPONSE             0x10

#define ERROR_FLAGS              emac_if_get_error_flags()
#define RESET_ALL_ERROR_FLAGS    emac_if_reset_all_error_flags()
#define PRINT_ERROR_FLAGS        emac_if_print_error_flags()
#define SET_ERROR_FLAGS(flags)   emac_if_set_error_flags(flags)
#define RESET_ERROR_FLAGS(flags) emac_if_reset_error_flags(flags)

#define ETH_FRAME_HEADER_LEN    28
#define ETH_FRAME_MIN_LEN       60 + 4
#define ETH_FRAME_PADD_LEN      60
#define ETH_MAC_ADDR_LEN        6

#define TIMEOUT                 1
#define INPUT                   2

#define ETH_MAX_LEN            (14 + emac_if_get_mtu_size())

// Starts test loop. Calls callback function on interval defined by the timeout
// and when input message is received.
#define START_TEST_LOOP(cb_function, timeout) worker_loop_start(cb_function, timeout)

// Ends test loop
#define END_TEST_LOOP          { worker_loop_end(); return; }

// Checks if echo server MAC address is known
#define ECHO_SERVER_ADDRESS_KNOWN  emac_if_count_echo_server_addr()

int emac_if_find_outgoing_msg(int receipt_number);
void emac_if_free_outgoing_msg(int index);
int emac_if_count_outgoing_msg(void);
void emac_if_reset_outgoing_msg(void);
int emac_if_add_outgoing_msg(int length);
void emac_if_timeout_outgoing_msg(void);
void emac_if_validate_outgoing_msg(void);
void emac_if_set_outgoing_msg_receipt_num(int index, int receipt_number);
void emac_if_set_outgoing_msg_flags(int index, int flags);

void emac_if_add_echo_server_addr(unsigned char *addr);
int emac_if_count_echo_server_addr(void);
unsigned char *emac_if_get_echo_server_addr(int index);

void emac_if_set_error_flags(unsigned int error_flags_value);
unsigned int emac_if_get_error_flags(void);
void emac_if_reset_all_error_flags(void);
void emac_if_reset_error_flags(unsigned int error_flags_value);
void emac_if_print_error_flags(void);

void emac_if_set_trace_level(char trace_level_value);
char emac_if_get_trace_level();

void emac_if_trace_to_ascii_hex_dump(const char *prefix, int len, char *data);

unsigned char *emac_if_get_own_addr(void);

int emac_if_get_mtu_size();
void emac_if_set_mtu_size(int mtu_size);

extern void emac_if_link_input_cb(void *buf);
extern void emac_if_link_state_change_cb(bool up);

void emac_if_set_all_multicast(bool all);
void emac_if_add_multicast_group(uint8_t *address);

void emac_if_set_output_memory(bool memory);
void emac_if_set_input_memory(bool memory);
void emac_if_check_memory(bool output);
void emac_if_set_memory(bool memory);

void worker_loop_init(void);
void worker_loop_start(void (*test_step_cb_fnc)(int opt), int timeout);
void worker_loop_end(void);
void worker_loop_link_up_wait(void);

#endif /* EMAC_UTIL_H */
