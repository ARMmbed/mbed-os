# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_RZ_A1XX EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_RZ_A2XX EXCLUDE_FROM_ALL)

add_library(mbed-renesas INTERFACE)

target_include_directories(mbed-renesas
    INTERFACE
        .
)

target_link_libraries(mbed-renesas INTERFACE mbed-cmsis-cortex-a)
