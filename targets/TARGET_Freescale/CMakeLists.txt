# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_KLXX EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MCUXpresso_MCUS EXCLUDE_FROM_ALL)

add_library(mbed-freescale INTERFACE)

target_include_directories(mbed-freescale
    INTERFACE
        .
)

target_sources(mbed-freescale
    INTERFACE
        USBPhy_Kinetis.cpp
)

target_link_libraries(mbed-freescale INTERFACE mbed-cmsis-cortex-m)
