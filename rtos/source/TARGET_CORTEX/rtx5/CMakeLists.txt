# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(RTX)

target_include_directories(mbed-os
    PUBLIC
        Include
)

target_sources(mbed-os
    PRIVATE
        Source/os_systick.c
        Source/os_tick_ptim.c
)
