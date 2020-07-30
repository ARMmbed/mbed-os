# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# This is the boilerplate for Mbed OS

cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

# Using relative paths behavior
if(POLICY CMP0076)
    cmake_policy(SET CMP0076 NEW)
endif()

add_executable(${APP_TARGET})

# Create Mbed OS library
add_library(mbed-os OBJECT)

include(${MBED_CONFIG_PATH}/mbed_config.cmake)
include(${MBED_ROOT}/cmake/toolchain.cmake)
include(${MBED_ROOT}/cmake/core.cmake)
include(${MBED_ROOT}/cmake/profile.cmake)
include(${MBED_ROOT}/cmake/util.cmake)

set_target_properties(mbed-os PROPERTIES MBED_TARGET_LABELS "${MBED_TARGET_LABELS}")
target_compile_definitions(mbed-os PUBLIC ${MBED_TARGET_DEFINITIONS})
target_compile_definitions(mbed-os PUBLIC ${MBED_CONFIG_DEFINITIONS})

# Specify a default build type
if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE "RelWithDebInfo"
        CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
endif()

# Include mbed.h and config from generate folder
target_include_directories(mbed-os PUBLIC .)

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

# I have  to  leave this here as linker is processed after mbed-os added, and can't be in toolchain.cmake
# as its global symbol is empty at that stage, this needs more work
# TODO: This property + pre/post should be moved
get_property(mbed_target_startup GLOBAL PROPERTY MBED_TARGET_LINKER_FILE)

# TODO: @mbed-os-tools These pre/post build commands should get details from target + profile.
# I have  to  leave this here as linker is processed after mbed-os added, and can't be in toolchain.cmake
# as its global symbol is empty at that stage, this needs more work.
# Link the Mbed target linker file
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
            ${mbed_target_startup} -o ${CMAKE_BINARY_DIR}/${APP_TARGET}.link_script.ld

        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        BYPRODUCTS "${CMAKE_BINARY_DIR}/${APP_TARGET}.link_script.ld"
    )
    # Generate binary and hex file
    set(CMAKE_POST_BUILD_COMMAND
        COMMAND ${ELF2BIN} -O binary $<TARGET_FILE:${APP_TARGET}> $<TARGET_FILE:${APP_TARGET}>.bin
        COMMAND ${CMAKE_COMMAND} -E echo "-- built: $<TARGET_FILE:${APP_TARGET}>.bin"
        COMMAND ${ELF2BIN} -O ihex $<TARGET_FILE:${APP_TARGET}> $<TARGET_FILE:${APP_TARGET}>.hex
        COMMAND ${CMAKE_COMMAND} -E echo "-- built: $<TARGET_FILE:${APP_TARGET}>.hex"
    )
elseif(MBED_TOOLCHAIN STREQUAL "ARM")
    set(CMAKE_PRE_BUILD_COMMAND COMMAND "")
    target_link_options(mbed-os
        PUBLIC
            "--scatter=${mbed_target_startup}"
    )
    # Generate binary and hex file
    set(CMAKE_POST_BUILD_COMMAND
        COMMAND ${ELF2BIN} ${MBED_STUDIO_ARM_COMPILER} --bin  -o $<TARGET_FILE:${APP_TARGET}>.bin $<TARGET_FILE:${APP_TARGET}>
        COMMAND ${CMAKE_COMMAND} -E echo "-- built: $<TARGET_FILE:${APP_TARGET}>.bin"
        COMMAND ${ELF2BIN} ${MBED_STUDIO_ARM_COMPILER} --i32combined  -o $<TARGET_FILE:${APP_TARGET}>.hex $<TARGET_FILE:${APP_TARGET}>
        COMMAND ${CMAKE_COMMAND} -E echo "-- built: $<TARGET_FILE:${APP_TARGET}>.hex"
    )
endif()

add_custom_command(
    TARGET
        ${APP_TARGET}
    PRE_LINK
        ${CMAKE_PRE_BUILD_COMMAND}
    COMMENT
        "link line:"
    VERBATIM
)
add_custom_command(
    TARGET
        ${APP_TARGET}
    POST_BUILD
        ${CMAKE_POST_BUILD_COMMAND}
    COMMENT
        "executable:"
    VERBATIM
)
