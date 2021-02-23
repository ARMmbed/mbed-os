# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# This connects interface Mbed OS CMake target with object target
function(mbed_attach_object_lib_with_interface_lib name)
    set_property(TARGET ${name} PROPERTY INTERFACE_SOURCES $<TARGET_OBJECTS:${name}-obj>)

    foreach(options COMPILE_DEFINITIONS COMPILE_FEATURES COMPILE_OPTIONS INCLUDE_DIRECTORIES LINK_LIBRARIES)
        set_target_properties(${name}-obj PROPERTIES ${options} $<TARGET_PROPERTY:${name},INTERFACE_${options}>)
    endforeach()
endfunction()
