/*
 * Copyright (c) 2018 ARM. All rights reserved.
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

