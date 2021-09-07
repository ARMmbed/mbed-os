# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(GoogleTest)

set(TEST_NAME cellular-framework-common-list-unittest)

add_executable(${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE
        listtest.cpp
)

target_link_libraries(${TEST_NAME}
    PRIVATE
        mbed-headers-cellular
        gmock_main
)

gtest_discover_tests(${TEST_NAME} PROPERTIES LABELS "cellular")
