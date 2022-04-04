# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(RTX)

target_include_directories(mbed-rtos-flags
    INTERFACE
        Include
)

target_sources(mbed-rtos-sources
    INTERFACE
        Source/os_systick.c
        Source/os_tick_ptim.c
)
