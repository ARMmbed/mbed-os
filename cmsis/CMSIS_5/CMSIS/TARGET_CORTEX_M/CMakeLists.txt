# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cmsis-cortex-m INTERFACE)
add_library(mbed-cmsis-cortex-m-obj OBJECT)

target_include_directories(mbed-cmsis-cortex-m
    INTERFACE
        Include
)

target_sources(mbed-cmsis-cortex-m-obj
    PRIVATE
        Source/mbed_tz_context.c
)

mbed_attach_object_lib_with_interface_lib(mbed-cmsis-cortex-m)
