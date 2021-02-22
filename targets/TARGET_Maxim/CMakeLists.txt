# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0


add_subdirectory(TARGET_MAX32620C EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MAX32625 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MAX32630 EXCLUDE_FROM_ALL)

add_library(mbed-maxim INTERFACE)
add_library(mbed-maxim-obj OBJECT)

target_include_directories(mbed-maxim
    INTERFACE
        .
)

target_sources(mbed-maxim-obj
    PRIVATE
        USBPhy_Maxim.cpp
)


set_property(TARGET mbed-maxim PROPERTY INTERFACE_SOURCES $<TARGET_OBJECTS:mbed-maxim-obj>)

foreach(options COMPILE_DEFINITIONS COMPILE_FEATURES COMPILE_OPTIONS INCLUDE_DIRECTORIES LINK_LIBRARIES)
    set_target_properties(mbed-maxim-obj PROPERTIES ${options} $<TARGET_PROPERTY:mbed-maxim,INTERFACE_${options}>)
endforeach()

target_link_libraries(mbed-maxim INTERFACE mbed-cmsis-cortex-m)
