# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../ CACHE INTERNAL "")
set(TEST_TARGET mbed-connectivity-netsocket-tcp)

include(${MBED_PATH}/tools/cmake/mbed_greentea.cmake)

project(${TEST_TARGET})

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

mbed_greentea_add_test(
    TEST_NAME
        ${TEST_TARGET}
    TEST_SOURCES
        ${TEST_SOURCE_LIST}
    TEST_REQUIRED_LIBS
        mbed-netsocket
)
