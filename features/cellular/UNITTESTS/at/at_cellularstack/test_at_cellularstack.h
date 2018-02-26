/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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
#ifndef TEST_AT_CELLULARSTACK_H
#define TEST_AT_CELLULARSTACK_H

class Test_AT_CellularStack
{
public:
    Test_AT_CellularStack();

    virtual ~Test_AT_CellularStack();

    void test_AT_CellularStack_constructor();

    void test_AT_CellularStack_get_ip_address();

    void test_AT_CellularStack_socket_open();

    void test_AT_CellularStack_socket_close();

    void test_AT_CellularStack_socket_bind();

    void test_AT_CellularStack_socket_listen();

    void test_AT_CellularStack_socket_connect();

    void test_AT_CellularStack_socket_accept();

    void test_AT_CellularStack_socket_send();

    void test_AT_CellularStack_socket_recv();

    void test_AT_CellularStack_socket_sendto();

    void test_AT_CellularStack_socket_recvfrom();

    void test_AT_CellularStack_socket_attach();
};

#endif // TEST_AT_CELLULARSTACK_H

