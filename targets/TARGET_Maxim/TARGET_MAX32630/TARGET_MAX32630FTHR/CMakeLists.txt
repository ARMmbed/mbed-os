# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-max32630fthr INTERFACE)
add_library(mbed-max32630fthr-obj OBJECT)

target_include_directories(mbed-max32630fthr 
    INTERFACE
        .
)

target_sources(mbed-max32630fthr-obj
    PRIVATE
        low_level_init.c
)

set_property(TARGET mbed-max32630fthr PROPERTY INTERFACE_SOURCES $<TARGET_OBJECTS:mbed-max32630fthr-obj>)

foreach(options COMPILE_DEFINITIONS COMPILE_FEATURES COMPILE_OPTIONS INCLUDE_DIRECTORIES LINK_LIBRARIES)
    set_target_properties(mbed-max32630fthr-obj PROPERTIES ${options} $<TARGET_PROPERTY:mbed-max32630fthr,INTERFACE_${options}>)
endforeach()

target_link_libraries(mbed-max32630fthr INTERFACE mbed-max32630)
