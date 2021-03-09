# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# We use INTERFACE library to form the main object library otherwise we would 
# get circular deps error between ble <-> cordio
add_library(mbed-ble-cordio INTERFACE)

if("CORDIO_LL" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_CORDIO_LL)
endif()

add_subdirectory(cordio_stack)

target_include_directories(mbed-ble-cordio
    INTERFACE
        .
)
