# Copyright (c) 2020-2023 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_APM32F4 EXCLUDE_FROM_ALL)

add_library(mbed-geehy INTERFACE)

target_include_directories(mbed-geehy
    INTERFACE
        .
)

target_link_libraries(mbed-geehy INTERFACE mbed-cmsis-cortex-m)
