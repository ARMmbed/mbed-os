# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# This is the boilerplate for Mbed OS

cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

# Using relative paths behavior
if(POLICY CMP0076)
    cmake_policy(SET CMP0076 NEW)
endif()

include(${MBED_ROOT}/tools/cmake/core.cmake)
include(${MBED_ROOT}/tools/cmake/profile.cmake)

add_library(mbed-core INTERFACE)

add_library(mbed-os INTERFACE)

target_link_libraries(mbed-os
        INTERFACE
            mbed-rtos
            mbed-core
)

add_library(mbed-baremetal INTERFACE)

target_link_libraries(mbed-baremetal
    INTERFACE
        mbed-core
)

# Validate selected C library type
# The C library type selected has to match the library that the target can support
if(${MBED_C_LIB} STREQUAL "small")
    if(NOT "small" IN_LIST MBED_TARGET_SUPPORTED_C_LIBS)
        if("std" IN_LIST MBED_TARGET_SUPPORTED_C_LIBS)
            message(WARNING
                "We noticed that target.c_lib is set to `${MBED_C_LIB}`."
                " As the ${MBED_TARGET} target does not support a small C library for the ${MBED_TOOLCHAIN} toolchain,"
                " we are using the standard C library instead."
            )
            set(MBED_C_LIB "std" CACHE STRING "")
        endif() 
    endif()
elseif(NOT ${MBED_C_LIB} IN_LIST MBED_TARGET_SUPPORTED_C_LIBS)
    message(FATAL_ERROR
        "Invalid `target.c_lib` ('${MBED_C_LIB}') for '${MBED_TARGET}' target."
        "\nPossible value(s): ${MBED_TARGET_SUPPORTED_C_LIBS}"
    )
endif()

# Validate selected printf library
set(MBED_PRINTF_LIB_TYPES std minimal-printf)
if(NOT ${MBED_PRINTF_LIB} IN_LIST MBED_PRINTF_LIB_TYPES)
    message(FATAL_ERROR
        "Invalid printf library type '${MBED_PRINTF_LIB}'. Possible values:\n ${MBED_PRINTF_LIB_TYPES}"
    )
endif()

mbed_set_cpu_core_options(mbed-core ${MBED_TOOLCHAIN})
mbed_set_toolchain_options(mbed-core)
mbed_set_c_lib(mbed-core ${MBED_C_LIB})
mbed_set_printf_lib(mbed-core ${MBED_PRINTF_LIB})
mbed_set_profile_options(mbed-core ${MBED_TOOLCHAIN})

set(MBED_TARGET_LABELS
         ${MBED_TARGET_LABELS} CACHE INTERNAL ""
)

target_compile_definitions(mbed-core
    INTERFACE
        ${MBED_TARGET_DEFINITIONS}
        ${MBED_CONFIG_DEFINITIONS}
)

# Include mbed.h and config from generate folder
target_include_directories(mbed-core
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# These targets are made visible here so their source files which
# are spread in different directories can be referenced and can be linked against
# by libraries that depend on them.
# TODO CMake: Should the source files be moved?
add_library(mbed-device_key INTERFACE)
add_library(mbed-rtos INTERFACE)

add_subdirectory(cmsis)
add_subdirectory(drivers)
add_subdirectory(hal)
add_subdirectory(platform)
add_subdirectory(rtos)
add_subdirectory(targets)

# The directories below contain optional target libraries
add_subdirectory(events EXCLUDE_FROM_ALL)
add_subdirectory(connectivity EXCLUDE_FROM_ALL)
add_subdirectory(storage EXCLUDE_FROM_ALL)
add_subdirectory(drivers/device_key EXCLUDE_FROM_ALL)
add_subdirectory(drivers/source/usb EXCLUDE_FROM_ALL)
add_subdirectory(features EXCLUDE_FROM_ALL)
add_subdirectory(platform/FEATURE_EXPERIMENTAL_API EXCLUDE_FROM_ALL)
add_subdirectory(cmsis/CMSIS_5/CMSIS/RTOS2 EXCLUDE_FROM_ALL)
add_subdirectory(cmsis/device/rtos EXCLUDE_FROM_ALL)

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
        mbed_generate_gcc_options_for_linker(${target} _linker_preprocess_definitions)
        set(CMAKE_PRE_BUILD_COMMAND
            COMMAND "arm-none-eabi-cpp" ${_linker_preprocess_definitions} -x assembler-with-cpp -E -Wp,-P
                ${mbed_target_linker_script} -o ${CMAKE_BINARY_DIR}/${target}.link_script.ld

            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            BYPRODUCTS "${CMAKE_BINARY_DIR}/${target}.link_script.ld"
        )
    elseif(MBED_TOOLCHAIN STREQUAL "ARM")
        set(CMAKE_PRE_BUILD_COMMAND COMMAND "")
        target_link_options(mbed-core
            INTERFACE
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
    if(MBED_TOOLCHAIN STREQUAL "GCC_ARM")
        set(CMAKE_POST_BUILD_COMMAND
            COMMAND ${elf_to_bin} -O binary $<TARGET_FILE:${target}> ${CMAKE_BINARY_DIR}/${target}.bin
            COMMAND ${CMAKE_COMMAND} -E echo "-- built: ${CMAKE_BINARY_DIR}/${target}.bin"
            COMMAND ${elf_to_bin} -O ihex $<TARGET_FILE:${target}> ${CMAKE_BINARY_DIR}/${target}.hex
            COMMAND ${CMAKE_COMMAND} -E echo "-- built: ${CMAKE_BINARY_DIR}/${target}.hex"
        )
    elseif(MBED_TOOLCHAIN STREQUAL "ARM")
        get_property(mbed_studio_arm_compiler GLOBAL PROPERTY MBED_STUDIO_ARM_COMPILER)
        set(CMAKE_POST_BUILD_COMMAND
            COMMAND ${elf_to_bin} ${mbed_studio_arm_compiler} --bin  -o ${CMAKE_BINARY_DIR}/${target}.bin $<TARGET_FILE:${target}>
            COMMAND ${CMAKE_COMMAND} -E echo "-- built: ${CMAKE_BINARY_DIR}/${target}.bin"
            COMMAND ${elf_to_bin} ${mbed_studio_arm_compiler} --i32combined  -o ${CMAKE_BINARY_DIR}/${target}.hex $<TARGET_FILE:${target}>
            COMMAND ${CMAKE_COMMAND} -E echo "-- built: ${CMAKE_BINARY_DIR}/${target}.hex"
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

#
# Parse toolchain generated map file of `target` and display a readable table format.
#
function(mbed_generate_map_file target)
     find_package (Python3)
     add_custom_command(
         TARGET
             ${target}
         POST_BUILD
             COMMAND ${Python3_EXECUTABLE} ${MBED_ROOT}/tools/memap.py -t ${MBED_TOOLCHAIN} -d 4 ${CMAKE_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX}.map
         WORKING_DIRECTORY
             ${CMAKE_BINARY_DIR}
         COMMENT
             "Displaying memory map for ${target}"
)
endfunction()

#
# Generate executables
#
function(mbed_generate_executable target)
    mbed_generate_bin_hex(${target})
    mbed_generate_map_file(${target})
endfunction()

# Ninja requires to be forced for response files
if ("${CMAKE_GENERATOR}" MATCHES "Ninja")
    # known issue ARMClang and Ninja with response files for windows
    # https://gitlab.kitware.com/cmake/cmake/-/issues/21093
    if(NOT (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows" AND CMAKE_CXX_COMPILER_ID MATCHES "ARMClang"))
        set(CMAKE_NINJA_FORCE_RESPONSE_FILE 1 CACHE INTERNAL "")
    endif()
endif()
