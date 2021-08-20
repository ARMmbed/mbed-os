# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../ CACHE INTERNAL "")
set(TEST_TARGET mbed-connectivity-netsocket-tls)

include(${MBED_PATH}/tools/cmake/mbed_greentea.cmake)

project(${TEST_TARGET})

list(
    APPEND
        TEST_SOURCE_LIST
            main.cpp
            tlssocket_simultaneous.cpp
            tlssocket_send_unconnected.cpp
            tlssocket_send_timeout.cpp
            tlssocket_send_repeat.cpp
            tlssocket_send_closed.cpp
            tlssocket_recv_timeout.cpp
            tlssocket_open_twice.cpp
            tlssocket_open_limit.cpp
            tlssocket_open_destruct.cpp
            tlssocket_no_cert.cpp
            tlssocket_handshake_invalid.cpp
            tlssocket_endpoint_close.cpp
            tlssocket_echotest.cpp
            tlssocket_echotest_burst.cpp
            tlssocket_connect_invalid.cpp       
)

mbed_greentea_add_test(
    TEST_NAME
        ${TEST_TARGET}
    TEST_SOURCES
        ${TEST_SOURCE_LIST}
    TEST_REQUIRED_LIBS
        mbed-netsocket
)
