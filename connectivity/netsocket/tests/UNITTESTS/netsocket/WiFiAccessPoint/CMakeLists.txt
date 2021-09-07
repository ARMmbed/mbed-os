# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(GoogleTest)

set(TEST_NAME netsocket-wifi-access-point-unittest)

add_executable(${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE
        ${mbed-os_SOURCE_DIR}/connectivity/netsocket/source/WiFiAccessPoint.cpp
        test_WiFiAccessPoint.cpp
)

target_link_libraries(${TEST_NAME}
    PRIVATE
        mbed-headers-netsocket
        mbed-stubs
        mbed-stubs-headers
        gmock_main
)

gtest_discover_tests(${TEST_NAME} PROPERTIES LABELS "netsocket")
