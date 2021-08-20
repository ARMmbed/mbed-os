# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-ev-cog-ad3029lz INTERFACE)

target_include_directories(mbed-ev-cog-ad3029lz
    INTERFACE
        .
        device
)

target_sources(mbed-ev-cog-ad3029lz
    INTERFACE
        device/system_ADuCM3029.c
        device/startup_ADuCM3029.c
)

target_link_libraries(mbed-ev-cog-ad3029lz INTERFACE mbed-aducm3029 mbed-flash-cmsis-algo)
