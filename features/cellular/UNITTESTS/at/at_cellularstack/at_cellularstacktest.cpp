/*
 * Copyright (c) 2015, Arm Limited and affiliates.
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
#include "CppUTest/TestHarness.h"
#include "test_at_cellularstack.h"

TEST_GROUP(AT_CellularStack)
{
    Test_AT_CellularStack* unit;

    void setup()
    {
        unit = new Test_AT_CellularStack();
    }

    void teardown()
    {
        delete unit;
    }
};

TEST(AT_CellularStack, Create)
{
    CHECK(unit != NULL);
}

TEST(AT_CellularStack, test_AT_CellularStack_constructor)
{
    unit->test_AT_CellularStack_constructor();
}

TEST(AT_CellularStack, test_AT_CellularStack_get_ip_address)
{
    unit->test_AT_CellularStack_get_ip_address();
}

TEST(AT_CellularStack, test_AT_CellularStack_socket_open)
{
    unit->test_AT_CellularStack_socket_open();
}

TEST(AT_CellularStack, test_AT_CellularStack_socket_close)
{
    unit->test_AT_CellularStack_socket_close();
}

TEST(AT_CellularStack, test_AT_CellularStack_socket_bind)
{
    unit->test_AT_CellularStack_socket_bind();
}

TEST(AT_CellularStack, test_AT_CellularStack_socket_listen)
{
    unit->test_AT_CellularStack_socket_listen();
}

TEST(AT_CellularStack, test_AT_CellularStack_socket_connect)
{
    unit->test_AT_CellularStack_socket_connect();
}

TEST(AT_CellularStack, test_AT_CellularStack_socket_accept)
{
    unit->test_AT_CellularStack_socket_accept();
}

TEST(AT_CellularStack, test_AT_CellularStack_socket_send)
{
    unit->test_AT_CellularStack_socket_send();
}

TEST(AT_CellularStack, test_AT_CellularStack_socket_recv)
{
    unit->test_AT_CellularStack_socket_recv();
}

TEST(AT_CellularStack, test_AT_CellularStack_socket_sendto)
{
    unit->test_AT_CellularStack_socket_sendto();
}

TEST(AT_CellularStack, test_AT_CellularStack_socket_recvfrom)
{
    unit->test_AT_CellularStack_socket_recvfrom();
}

TEST(AT_CellularStack, test_AT_CellularStack_socket_attach)
{
    unit->test_AT_CellularStack_socket_attach();
}

