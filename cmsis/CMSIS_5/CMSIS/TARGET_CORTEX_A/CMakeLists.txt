# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cmsis-cortex-a INTERFACE)
add_library(mbed-cmsis-cortex-a-obj OBJECT)

target_include_directories(mbed-cmsis-cortex-a
    INTERFACE
        Include
)

target_sources(mbed-cmsis-cortex-a-obj
    PRIVATE
        Source/irq_ctrl_gic.c
)

mbed_attach_object_lib_with_interface_lib(mbed-cmsis-cortex-a)
