# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-freescale INTERFACE)

add_subdirectory(TARGET_K64F EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_K66F EXCLUDE_FROM_ALL)

target_include_directories(mbed-emac-freescale
    INTERFACE
        .
)

target_sources(mbed-emac-freescale
    INTERFACE
        kinetis_emac.cpp
)
