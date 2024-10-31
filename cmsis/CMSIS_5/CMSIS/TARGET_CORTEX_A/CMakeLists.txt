# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cmsis-cortex-a ${CMAKE_LIBRARY_TYPE})

target_include_directories(mbed-cmsis-cortex-a
    ${CMAKE_LIBRARY_TYPE}
        Include
)

target_sources(mbed-cmsis-cortex-a
    ${CMAKE_LIBRARY_TYPE}
        Source/irq_ctrl_gic.c
)
