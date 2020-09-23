# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        Include
)

target_sources(mbed-os
    PRIVATE
        Source/irq_ctrl_gic.c
)
