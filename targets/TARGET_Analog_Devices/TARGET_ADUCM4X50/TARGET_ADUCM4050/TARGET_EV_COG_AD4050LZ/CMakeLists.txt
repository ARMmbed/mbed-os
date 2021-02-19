# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-ev-cog-ad4050lz INTERFACE)

target_include_directories(mbed-ev-cog-ad4050lz
    INTERFACE
        .
        device
    )

target_sources(mbed-ev-cog-ad4050lz
    INTERFACE
        device/system_ADuCM4050.c
        device/startup_ADuCM4050.c
)

target_link_libraries(mbed-ev-cog-ad4050lz INTERFACE mbed-aducm4050 mbed-flash-cmsis-algo)
