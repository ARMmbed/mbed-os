# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../ CACHE INTERNAL "")
set(TEST_TARGET mbed-connectivity-netsocket-nidd)

include(${MBED_PATH}/tools/cmake/mbed_greentea.cmake)

project(${TEST_TARGET})

list(
    APPEND
        TEST_SOURCE_LIST
            main.cpp
            niddsocket_send_timeout.cpp
            niddsocket_send_repeat.cpp
            niddsocket_send_invalid.cpp
            niddsocket_recv_timeout.cpp
            niddsocket_open_twice.cpp
            niddsocket_open_limit.cpp
            niddsocket_open_destruct.cpp
            niddsocket_open_close_repeat.cpp
            niddsocket_echotest.cpp
            niddsocket_disconnect.cpp
            niddsocket_connect.cpp       
)

mbed_greentea_add_test(
    TEST_NAME
        ${TEST_TARGET}
    TEST_SOURCES
        ${TEST_SOURCE_LIST}
    TEST_REQUIRED_LIBS
        mbed-netsocket
)
