# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-stubs-platform)

target_include_directories(mbed-stubs-platform
    PRIVATE
        .
)

target_sources(mbed-stubs-platform
    PRIVATE
        mbed_critical_stub.c
        mbed_atomic_stub.c
        mbed_error.c
        mbed_poll_stub.cpp
        mbed_assert_stub.cpp
        mbed_wait_api_stub.cpp
        mbed_retarget_stub.cpp
)

target_link_libraries(mbed-stubs-platform
    PRIVATE
        mbed-headers
)
