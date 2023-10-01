# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

list(
    APPEND
        TEST_SOURCE_LIST
            main.cpp
            tcpsocket_thread_per_socket_safety.cpp
            tcpsocket_setsockopt_keepalive_valid.cpp
            tcpsocket_send_timeout.cpp
            tcpsocket_send_repeat.cpp
            tcpsocket_recv_timeout.cpp
            tcpsocket_open_twice.cpp
            tcpsocket_open_limit.cpp
            tcpsocket_open_destruct.cpp
            tcpsocket_open_close_repeat.cpp
            tcpsocket_endpoint_close.cpp
            tcpsocket_echotest.cpp
            tcpsocket_echotest_burst.cpp
            tcpsocket_connect_invalid.cpp
            tcpsocket_bind_wrong_type.cpp
            tcpsocket_bind_unopened.cpp
            tcpsocket_bind_port_fail.cpp
            tcpsocket_bind_port.cpp
            tcpsocket_bind_address_invalid.cpp
            tcpsocket_bind_address.cpp       
            tcpsocket_recv_100k.cpp       	
)

if(MBED_GREENTEA_TEST_BAREMETAL)
    set(TEST_SKIPPED "This test requires an RTOS!")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-connectivity-netsocket-tcp
    TEST_SOURCES
        ${TEST_SOURCE_LIST}
    TEST_REQUIRED_LIBS
        mbed-netsocket
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
