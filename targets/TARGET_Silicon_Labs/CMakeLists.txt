# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_EFM32 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_SL_RAIL EXCLUDE_FROM_ALL)

add_library(mbed-silicon-labs INTERFACE)

target_include_directories(mbed-silicon-labs
    INTERFACE
        .
)

target_link_libraries(mbed-silicon-labs INTERFACE mbed-cmsis-cortex-m)
