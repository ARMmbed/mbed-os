# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(TEST_NAME netsocket-socket-address-unittest)

add_executable(${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE
        ${MBED_PATH}/connectivity/netsocket/source/SocketAddress.cpp
        ${MBED_PATH}/connectivity/libraries/nanostack-libservice/source/libip4string/ip4tos.c
        ${MBED_PATH}/connectivity/libraries/nanostack-libservice/source/libip6string/ip6tos.c
        ${MBED_PATH}/connectivity/libraries/nanostack-libservice/source/libip4string/stoip4.c
        ${MBED_PATH}/connectivity/libraries/nanostack-libservice/source/libip6string/stoip6.c
        ${MBED_PATH}/connectivity/libraries/nanostack-libservice/source/libBits/common_functions.c   
        test_SocketAddress.cpp
)

target_link_libraries(${TEST_NAME}
    PRIVATE
        mbed-headers
        mbed-stubs
        mbed-stubs-headers
        gmock_main
)

add_test(NAME "${TEST_NAME}" COMMAND ${TEST_NAME})

set_tests_properties(${TEST_NAME} PROPERTIES LABELS "netsocket")
