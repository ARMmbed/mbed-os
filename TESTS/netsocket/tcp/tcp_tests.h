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
void tcpsocket_connect_to_echo_srv(TCPSocket& sock);
void tcpsocket_connect_to_discard_srv(TCPSocket& sock);

/*
 * Test cases
 */
void TCPSOCKET_CONNECT_INVALID();
void TCPSOCKET_ECHOTEST();
void TCPSOCKET_ECHOTEST_NONBLOCK();
void TCPSOCKET_ECHOTEST_BURST();
void TCPSOCKET_ECHOTEST_BURST_NONBLOCK();
void TCPSOCKET_ENDPOINT_CLOSE();
void TCPSOCKET_OPEN_CLOSE_REPEAT();
void TCPSOCKET_OPEN_LIMIT();
void TCPSOCKET_RECV_100K();
void TCPSOCKET_RECV_100K_NONBLOCK();
void TCPSOCKET_RECV_TIMEOUT();
void TCPSOCKET_SEND_REPEAT();
void TCPSOCKET_SEND_TIMEOUT();
void TCPSOCKET_THREAD_PER_SOCKET_SAFETY();

#endif //TCP_TESTS_H
