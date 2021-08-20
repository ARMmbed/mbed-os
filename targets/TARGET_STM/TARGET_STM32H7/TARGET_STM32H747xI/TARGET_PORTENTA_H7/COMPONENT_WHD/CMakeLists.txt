# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-portenta-whd-4343w-fw INTERFACE)
target_include_directories(mbed-portenta-whd-4343w-fw
    INTERFACE
        resources/firmware/COMPONENT_4343W_FS
)
target_sources(mbed-portenta-whd-4343w-fw 
    INTERFACE
        resources/firmware/COMPONENT_4343W_FS/4343WA1_bin.c
        resources/firmware/COMPONENT_4343W_FS/4343WA1_clm_blob.c
)

add_library(mbed-portenta-whd-4343w-nvram INTERFACE)
target_include_directories(mbed-portenta-whd-4343w-nvram INTERFACE resources/nvram)

add_library(mbed-portenta-whd INTERFACE)
target_include_directories(mbed-portenta-whd
    INTERFACE
        .
        port
        interface
)

target_sources(mbed-portenta-whd
    INTERFACE
        port/cyhal.c
        port/cyhalgpio.cpp
        port/cyhalsdio.c
        port/wiced_filesystem.cpp 
        interface/cyabs_rtos_rtxv5.c
)

