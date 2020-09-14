# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# This is the boilerplate for Mbed OS

cmake_minimum_required(VERSION 3.18.2 FATAL_ERROR)

# Using relative paths behavior
if(POLICY CMP0076)
    cmake_policy(SET CMP0076 NEW)
endif()

include(${MBED_CONFIG_PATH}/mbed_config.cmake)

# Set default toolchain file
if(NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${MBED_ROOT}/tools/cmake/toolchain.cmake" CACHE INTERNAL "")
endif()

# Toolchain setup
include(${MBED_ROOT}/tools/cmake/toolchains/${MBED_TOOLCHAIN}.cmake)
enable_language(C CXX ASM)

include(${MBED_ROOT}/tools/cmake/core.cmake)
include(${MBED_ROOT}/tools/cmake/util.cmake)
include(${MBED_ROOT}/tools/cmake/profile.cmake)

# Create Mbed OS library
add_library(mbed-os OBJECT)

mbed_set_cpu_core_options(mbed-os ${MBED_TOOLCHAIN})
mbed_set_toolchain_options(mbed-os)
mbed_set_language_standard(mbed-os)
mbed_set_profile_options(mbed-os ${MBED_TOOLCHAIN})

set_target_properties(mbed-os
    PROPERTIES
        MBED_TARGET_LABELS "${MBED_TARGET_LABELS}"
)

target_compile_definitions(mbed-os
    PUBLIC
        ${MBED_TARGET_DEFINITIONS}
        ${MBED_CONFIG_DEFINITIONS}
)

# Include mbed.h and config from generate folder
target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Default build
add_subdirectory(cmsis)
add_subdirectory(components)
add_subdirectory(connectivity)
add_subdirectory(drivers)
add_subdirectory(events)
add_subdirectory(features)
add_subdirectory(hal)
add_subdirectory(platform)
add_subdirectory(rtos)
add_subdirectory(storage)
add_subdirectory(targets)


#
# Configures the application
#
function(mbed_configure_app_target target)
    mbed_set_language_standard(${target})
endfunction()

#
# Specifies linker script used for linking `target`.
#
function(mbed_set_mbed_target_linker_script target)
    get_property(mbed_target_linker_script GLOBAL PROPERTY MBED_TARGET_LINKER_FILE)
    if(MBED_TOOLCHAIN STREQUAL "GCC_ARM")
        mbed_generate_gcc_options_for_linker(${target} _linker_preprocess_definitions _linker_preprocess_options)
        set(CMAKE_PRE_BUILD_COMMAND
            COMMAND "arm-none-eabi-cpp" -E -P
                ${_linker_preprocess_options} ${_linker_preprocess_definitions}
                ${mbed_target_linker_script} -o ${CMAKE_BINARY_DIR}/${target}.link_script.ld

            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            BYPRODUCTS "${CMAKE_BINARY_DIR}/${target}.link_script.ld"
        )
    elseif(MBED_TOOLCHAIN STREQUAL "ARM")
        set(CMAKE_PRE_BUILD_COMMAND COMMAND "")
        target_link_options(mbed-os
            PUBLIC
                "--scatter=${mbed_target_linker_script}"
        )
    endif()
    add_custom_command(
        TARGET
            ${target}
        PRE_LINK
            ${CMAKE_PRE_BUILD_COMMAND}
        COMMENT
            "Link line:"
        VERBATIM
    )
endfunction()


#
# Converts output file of `target` to binary file and to Intel HEX file.
#
function(mbed_generate_bin_hex target)
    get_property(elf_to_bin GLOBAL PROPERTY ELF2BIN)
    # TODO: @mbed-os-tools This post-build commands should get details from target + profile.
    if(MBED_TOOLCHAIN STREQUAL "GCC_ARM")
        set(CMAKE_POST_BUILD_COMMAND
            COMMAND ${elf_to_bin} -O binary $<TARGET_FILE:${target}> $<TARGET_FILE:${target}>.bin
            COMMAND ${CMAKE_COMMAND} -E echo "-- built: $<TARGET_FILE:${target}>.bin"
            COMMAND ${elf_to_bin} -O ihex $<TARGET_FILE:${target}> $<TARGET_FILE:${target}>.hex
            COMMAND ${CMAKE_COMMAND} -E echo "-- built: $<TARGET_FILE:${target}>.hex"
        )
    elseif(MBED_TOOLCHAIN STREQUAL "ARM")
        get_property(mbed_studio_arm_compiler GLOBAL PROPERTY MBED_STUDIO_ARM_COMPILER)
        set(CMAKE_POST_BUILD_COMMAND
            COMMAND ${elf_to_bin} ${mbed_studio_arm_compiler} --bin  -o $<TARGET_FILE:${target}>.bin $<TARGET_FILE:${target}>
            COMMAND ${CMAKE_COMMAND} -E echo "-- built: $<TARGET_FILE:${target}>.bin"
            COMMAND ${elf_to_bin} ${mbed_studio_arm_compiler} --i32combined  -o $<TARGET_FILE:${target}>.hex $<TARGET_FILE:${target}>
            COMMAND ${CMAKE_COMMAND} -E echo "-- built: $<TARGET_FILE:${target}>.hex"
        )
    endif()
    add_custom_command(
        TARGET
            ${target}
        POST_BUILD
            ${CMAKE_POST_BUILD_COMMAND}
        COMMENT
            "executable:"
        VERBATIM
    )
endfunction()
