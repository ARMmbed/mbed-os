# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NRF5x EXCLUDE_FROM_ALL)

add_library(mbed-nordic INTERFACE)

target_include_directories(mbed-nordic
    INTERFACE
        .
)

target_link_libraries(mbed-nordic INTERFACE mbed-cmsis-cortex-m)
