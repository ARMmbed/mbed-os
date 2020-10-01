# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-ble-cordio INTERFACE)

if("CORDIO_LL" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_CORDIO_LL)
endif()

add_subdirectory(cordio_stack)

target_include_directories(mbed-os-ble-cordio
    INTERFACE
        .
)

target_link_libraries(mbed-os-ble-cordio
    INTERFACE
        mbed-os-ble
)
