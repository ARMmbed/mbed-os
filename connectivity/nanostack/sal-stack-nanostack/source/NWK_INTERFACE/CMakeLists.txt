# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Include
)

target_sources(mbed-os
    PRIVATE
        protocol_core.c
        protocol_core_sleep.c
        protocol_stats.c
        protocol_timer.c
)
