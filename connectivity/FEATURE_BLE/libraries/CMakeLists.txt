# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-ble-cordio OBJECT)

if("CORDIO_LL" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_CORDIO_LL)
endif()

target_link_libraries(mbed-os-ble mbed-os-ble-cordio)

add_subdirectory(cordio_stack)

target_include_directories(mbed-os-ble-cordio
    PUBLIC
        .
)
