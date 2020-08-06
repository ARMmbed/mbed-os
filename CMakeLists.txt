# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# This is the boilerplate for Mbed OS

cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

# Using relative paths behavior
if(POLICY CMP0076)
    cmake_policy(SET CMP0076 NEW)
endif()

# Create Mbed OS library
add_library(mbed-os OBJECT)

include(${MBED_CONFIG_PATH}/mbed_config.cmake)
include(${MBED_ROOT}/cmake/core.cmake)
include(${MBED_ROOT}/cmake/toolchain.cmake)
include(${MBED_ROOT}/cmake/profile.cmake)
include(${MBED_ROOT}/cmake/util.cmake)

set_target_properties(mbed-os
    PROPERTIES
        MBED_TARGET_LABELS "${MBED_TARGET_LABELS}"
)

target_compile_definitions(mbed-os
    PUBLIC
        ${MBED_TARGET_DEFINITIONS}
        ${MBED_CONFIG_DEFINITIONS}
)

# Specify a default build type
if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE "RelWithDebInfo"
        CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
endif()

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
# Specifies linker script used for linking `target`.
#
function(mbed_os_target_linker_script target)
    get_property(mbed_target_startup GLOBAL PROPERTY MBED_TARGET_LINKER_FILE)

    # TODO: @mbed-os-tools This pre-build commands should get details from target + profile.
    if(MBED_TOOLCHAIN STREQUAL "GCC_ARM")
        set(CMAKE_PRE_BUILD_COMMAND
            COMMAND "arm-none-eabi-cpp" -E -P
                -Wl,--gc-sections -Wl,--wrap,main -Wl,--wrap,_malloc_r -Wl,--wrap,_free_r
                -Wl,--wrap,_realloc_r -Wl,--wrap,_memalign_r -Wl,--wrap,_calloc_r
                -Wl,--wrap,exit -Wl,--wrap,atexit -Wl,-n
                -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
                -DMBED_ROM_START=0x0 -DMBED_ROM_SIZE=0x100000 -DMBED_RAM_START=0x20000000
                -DMBED_RAM_SIZE=0x30000 -DMBED_RAM1_START=0x1fff0000
                -DMBED_RAM1_SIZE=0x10000 -DMBED_BOOT_STACK_SIZE=1024
                -DXIP_ENABLE=0
                ${mbed_target_startup} -o ${CMAKE_BINARY_DIR}/${target}.link_script.ld

            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            BYPRODUCTS "${CMAKE_BINARY_DIR}/${target}.link_script.ld"
        )
    elseif(MBED_TOOLCHAIN STREQUAL "ARM")
        set(CMAKE_PRE_BUILD_COMMAND COMMAND "")
        target_link_options(mbed-os
            PUBLIC
                "--scatter=${mbed_target_startup}"
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
function(mbed_os_bin_hex target)
    get_property(elf_to_bin GLOBAL PROPERTY ELF2BIN)
    # TODO: @mbed-os-tools This post-build commands should get details from target + profile.
    if(MBED_TOOLCHAIN STREQUAL "GCC_ARM")
        set(CMAKE_POST_BUILD_COMMAND
            COMMAND ${elf_to_bin} -O binary $<TARGET_FILE:${target}> $<TARGET_FILE:${target}>.bin
            COMMAND ${CMAKE_COMMAND} -E echo "-- built: $<TARGET_FILE:${APP_TARGET}>.bin"
            COMMAND ${elf_to_bin} -O ihex $<TARGET_FILE:${target}> $<TARGET_FILE:${target}>.hex
            COMMAND ${CMAKE_COMMAND} -E echo "-- built: $<TARGET_FILE:${APP_TARGET}>.hex"
        )
    elseif(MBED_TOOLCHAIN STREQUAL "ARM")
        get_property(mbed_studio_arm_compiler GLOBAL PROPERTY MBED_STUDIO_ARM_COMPILER)
        set(CMAKE_POST_BUILD_COMMAND
            COMMAND ${elf_to_bin} ${mbed_studio_arm_compiler} --bin  -o $<TARGET_FILE:${target}>.bin $<TARGET_FILE:${target}>
            COMMAND ${CMAKE_COMMAND} -E echo "-- built: $<TARGET_FILE:${APP_TARGET}>.bin"
            COMMAND ${elf_to_bin} ${mbed_studio_arm_compiler} --i32combined  -o $<TARGET_FILE:${target}>.hex $<TARGET_FILE:${target}>
            COMMAND ${CMAKE_COMMAND} -E echo "-- built: $<TARGET_FILE:${APP_TARGET}>.hex"
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
