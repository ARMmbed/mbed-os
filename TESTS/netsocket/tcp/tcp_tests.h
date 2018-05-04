/*
 * Copyright (c) 2018, ARM Limited, All Rights Reserved
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

#ifndef TCP_TESTS_H
#define TCP_TESTS_H

NetworkInterface* get_interface();
void drop_bad_packets(TCPSocket& sock);
void fill_tx_buffer_ascii(char *buff, size_t len);

/*
 * Test cases
 */
void test_tcpsocket_echotest_burst();
void test_tcpsocket_echotest_burst_nonblock();
void test_tcpsocket_endpoint_close();
void test_tcpsocket_open_limit();
void test_tcpsocket_recv_100k();
void test_tcpsocket_recv_100k_nonblock();
void test_tcpsocket_recv_timeout();
void test_tcpsocket_send_timeout();
void test_tcpsocket_thread_per_socket_safety();

#endif //TCP_TESTS_H
