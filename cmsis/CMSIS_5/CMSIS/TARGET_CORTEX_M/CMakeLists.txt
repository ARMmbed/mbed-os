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

set_property(TARGET mbed-cmsis-cortex-m PROPERTY INTERFACE_SOURCES $<TARGET_OBJECTS:mbed-cmsis-cortex-m-obj>)

foreach(options COMPILE_DEFINITIONS COMPILE_FEATURES COMPILE_OPTIONS INCLUDE_DIRECTORIES LINK_LIBRARIES)
    set_target_properties(mbed-cmsis-cortex-m-obj PROPERTIES ${options} $<TARGET_PROPERTY:mbed-cmsis-cortex-m,INTERFACE_${options}>)
endforeach()
