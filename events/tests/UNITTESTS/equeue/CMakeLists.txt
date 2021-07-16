# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(GoogleTest)

set(TEST_NAME equeue-unittest)

add_executable(${TEST_NAME})

target_compile_definitions(${TEST_NAME}
    PRIVATE
        EQUEUE_PLATFORM_POSIX
)

target_compile_options(${TEST_NAME}
    PRIVATE
        "-pthread"
)

target_sources(${TEST_NAME}
    PRIVATE
        ${mbed-os_SOURCE_DIR}/events/source/equeue.c
        test_equeue.cpp
)

target_link_libraries(${TEST_NAME}
    PRIVATE
        mbed-headers-platform
        mbed-headers-events
        mbed-stubs-events
        mbed-stubs-platform
        gmock_main
)

gtest_discover_tests(${TEST_NAME} PROPERTIES LABELS "equeue")
