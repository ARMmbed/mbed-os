# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-nuvoton INTERFACE)

add_subdirectory(TARGET_M480 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_NUC472 EXCLUDE_FROM_ALL)

target_include_directories(mbed-emac-nuvoton
    INTERFACE
        .
)

target_sources(mbed-emac-nuvoton
    INTERFACE
        numaker_emac.cpp
)
