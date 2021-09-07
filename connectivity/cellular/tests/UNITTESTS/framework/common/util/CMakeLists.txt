# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(GoogleTest)

set(TEST_NAME cellular-framework-common-util-unittest)

add_executable(${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE
        ${mbed-os_SOURCE_DIR}/connectivity/cellular/source/framework/common/CellularUtil.cpp
        utiltest.cpp
)

target_link_libraries(${TEST_NAME}
    PRIVATE
        mbed-headers-netsocket
        mbed-headers-cellular
        mbed-stubs-platform
        gmock_main
)

gtest_discover_tests(${TEST_NAME} PROPERTIES LABELS "cellular")
