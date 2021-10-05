# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../.. CACHE INTERNAL "")
set(TEST_TARGET mbed-connectivity-netsocket-network-emac)

include(${MBED_PATH}/tools/cmake/mbed_greentea.cmake)

project(${TEST_TARGET})

list(
    APPEND
        TEST_SOURCE_LIST
            main.cpp
            emac_TestMemoryManager.cpp
            emac_TestNetworkStack.cpp
            emac_ctp.cpp
            emac_membuf.cpp
            emac_test_broadcast.cpp
            emac_test_initialize.cpp
            emac_test_memory.cpp
            emac_test_multicast_filter.cpp
            emac_test_unicast.cpp
            emac_test_unicast_burst.cpp
            emac_test_unicast_frame_len.cpp
            emac_test_unicast_long.cpp
            emac_util.cpp
)

mbed_greentea_add_test(
    TEST_NAME
        ${TEST_TARGET}
    TEST_SOURCES
        ${TEST_SOURCE_LIST}
    TEST_REQUIRED_LIBS
        mbed-emac
)
