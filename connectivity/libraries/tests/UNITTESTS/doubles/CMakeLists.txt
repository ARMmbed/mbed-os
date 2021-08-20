# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-headers-nanostack-libservice INTERFACE)

target_include_directories(mbed-headers-nanostack-libservice
    INTERFACE
        ${mbed-os_SOURCE_DIR}/connectivity/libraries/nanostack-libservice
        ${mbed-os_SOURCE_DIR}/connectivity/libraries/nanostack-libservice/mbed-client-libservice
)
add_library(mbed-stubs-nanostack-libservice)

target_sources(mbed-stubs-nanostack-libservice
    PRIVATE
        ip4tos_stub.c
        stoip4_stub.c
)

target_link_libraries(mbed-stubs-nanostack-libservice
    PRIVATE
        mbed-headers-nanostack-libservice
        gtest
)
