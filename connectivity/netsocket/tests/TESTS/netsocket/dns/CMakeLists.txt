# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../ CACHE INTERNAL "")
set(TEST_TARGET mbed-connectivity-netsocket-dns)

include(${MBED_PATH}/tools/cmake/mbed_greentea.cmake)

project(${TEST_TARGET})

list(
    APPEND 
        TEST_SOURCE_LIST
            main.cpp
            synchronous_dns_multiple.cpp
            synchronous_dns_multi_ip.cpp
            synchronous_dns_invalid.cpp
            synchronous_dns.cpp
            synchronous_dns_cache.cpp
            asynchronous_dns_timeouts.cpp
            asynchronous_dns_simultaneous_repeat.cpp
            asynchronous_dns_simultaneous.cpp
            asynchronous_dns_simultaneous_cache.cpp
            asynchronous_dns_non_async_and_async.cpp
            asynchronous_dns_multi_ip.cpp
            asynchronous_dns_invalid_host.cpp
            asynchronous_dns_external_event_queue.cpp
            asynchronous_dns.cpp
            asynchronous_dns_cancel.cpp
            asynchronous_dns_cache.cpp
)

mbed_greentea_add_test(
    TEST_NAME
        ${TEST_TARGET}
    TEST_SOURCES
        ${TEST_SOURCE_LIST}
    TEST_REQUIRED_LIBS
        mbed-netsocket
)
