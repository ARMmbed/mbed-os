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

set_property(TARGET mbed-cmsis-cortex-a PROPERTY INTERFACE_SOURCES $<TARGET_OBJECTS:mbed-cmsis-cortex-a-obj>)

foreach(options COMPILE_DEFINITIONS COMPILE_FEATURES COMPILE_OPTIONS INCLUDE_DIRECTORIES LINK_LIBRARIES)
    set_target_properties(mbed-cmsis-cortex-a-obj PROPERTIES ${options} $<TARGET_PROPERTY:mbed-cmsis-cortex-a,INTERFACE_${options}>)
endforeach()
