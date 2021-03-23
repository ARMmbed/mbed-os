# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_TMPM46B EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_TMPM4G9 EXCLUDE_FROM_ALL)

add_library(mbed-toshiba INTERFACE)

target_include_directories(mbed-toshiba
    INTERFACE
        .
)

target_link_libraries(mbed-toshiba INTERFACE mbed-cmsis-cortex-m)
