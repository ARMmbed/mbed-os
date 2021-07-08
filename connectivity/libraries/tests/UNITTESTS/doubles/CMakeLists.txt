# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-stubs-nanostack-libservice)

target_sources(mbed-stubs-nanostack-libservice
    PRIVATE
        ip4tos_stub.c
        stoip4_stub.c
)

target_link_libraries(mbed-stubs-nanostack-libservice
    PRIVATE
        mbed-stubs-rtos-headers
        mbed-headers
        mbed-stubs-headers
        mbed-stubs-rtos
        mbed-stubs-platform
        gtest
)
