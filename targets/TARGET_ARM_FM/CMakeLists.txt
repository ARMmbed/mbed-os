# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-arm-fm INTERFACE)

add_subdirectory(TARGET_FVP_MPS2 EXCLUDE_FROM_ALL)

target_include_directories(mbed-arm-fm
    INTERFACE
        .
)

target_link_libraries(mbed-arm-fm INTERFACE mbed-cmsis-cortex-m)
