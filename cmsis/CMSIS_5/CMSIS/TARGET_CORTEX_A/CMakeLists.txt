# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-core
    INTERFACE
        Include
)

target_sources(mbed-core
    INTERFACE
        Source/irq_ctrl_gic.c
)
