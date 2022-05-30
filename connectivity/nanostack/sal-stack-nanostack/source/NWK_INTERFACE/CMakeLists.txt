# Copyright (c) 2020-2021, Pelion and affiliates.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nanostack-sal_stack
    PUBLIC
        .
        ./Include
)

target_sources(mbed-nanostack-sal_stack
    PRIVATE
        protocol_core.c
        protocol_core_sleep.c
        protocol_stats.c
        protocol_timer.c
)
