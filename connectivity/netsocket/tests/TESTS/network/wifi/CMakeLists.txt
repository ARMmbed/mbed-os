# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../.. CACHE INTERNAL "")
set(TEST_TARGET mbed-connectivity-netsocket-network-wifi)

include(${MBED_PATH}/tools/cmake/mbed_greentea.cmake)

project(${TEST_TARGET})

list(
    APPEND
        TEST_SOURCE_LIST
            main.cpp
            get_interface.cpp
            get_security.cpp
            wifi-constructor.cpp
            wifi_connect.cpp
            wifi_connect_disconnect_nonblock.cpp
            wifi_connect_disconnect_repeat.cpp
            wifi_connect_nocredentials.cpp
            wifi_connect_params_channel.cpp
            wifi_connect_params_channel_fail.cpp
            wifi_connect_params_null.cpp
            wifi_connect_params_valid_secure.cpp
            wifi_connect_secure.cpp
            wifi_connect_secure_fail.cpp
            wifi_get_rssi.cpp
            wifi_scan.cpp
            wifi_scan_null.cpp
            wifi_set_channel.cpp
            wifi_set_credential.cpp
)

mbed_greentea_add_test(
    TEST_NAME
        ${TEST_TARGET}
    TEST_SOURCES
        ${TEST_SOURCE_LIST}
    TEST_REQUIRED_LIBS
        mbed-netsocket
)
