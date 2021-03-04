# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-gd INTERFACE)

add_subdirectory(TARGET_GD32F30X EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_GD32F4XX EXCLUDE_FROM_ALL)

target_include_directories(mbed-emac-gd
    INTERFACE
        .
)

target_sources(mbed-emac-gd
    INTERFACE
        gd32xx_emac.cpp
)
