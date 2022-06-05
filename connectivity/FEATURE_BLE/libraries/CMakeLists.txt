# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-ble-cordio STATIC EXCLUDE_FROM_ALL)

if("CORDIO_LL" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_CORDIO_LL)
endif()

add_subdirectory(cordio_stack)

target_include_directories(mbed-ble-cordio
    PUBLIC
        .
)

target_link_libraries(mbed-ble-cordio
    PUBLIC
        mbed-ble
)
