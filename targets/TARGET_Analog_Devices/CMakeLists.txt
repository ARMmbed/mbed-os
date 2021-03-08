# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_ADUCM302X EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_ADUCM4X50 EXCLUDE_FROM_ALL)

add_library(mbed-analog-devices INTERFACE)

target_include_directories(mbed-analog-devices
    INTERFACE
        .
)

target_link_libraries(mbed-analog-devices INTERFACE mbed-cmsis-cortex-m)
