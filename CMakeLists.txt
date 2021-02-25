# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# This is the boilerplate for Mbed OS

cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

option(MBED_BUILD_UNITTESTS "Run unit tests only." OFF)

if(MBED_BUILD_UNITTESTS)
    project(mbed-unittests)
    enable_testing()
    
    add_subdirectory(UNITTESTS)
    add_subdirectory(${MBED_PATH}/drivers/tests/UNITTESTS)
    add_subdirectory(${MBED_PATH}/connectivity/cellular/tests/UNITTESTS)
    add_subdirectory(${MBED_PATH}/storage/blockdevice/tests/UNITTESTS)
    add_subdirectory(${MBED_PATH}/storage/kvstore/tdbstore/tests/UNITTESTS)
    add_subdirectory(${MBED_PATH}/storage/kvstore/filesystemstore/tests/UNITTESTS)
    add_subdirectory(${MBED_PATH}/platform/tests/UNITTESTS)   
    add_subdirectory(${MBED_PATH}/events/tests/UNITTESTS)
else()

include(${MBED_CONFIG_PATH}/mbed_config.cmake)
include(tools/cmake/set_linker_script.cmake)

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

mbed_set_cpu_core_definitions(mbed-core)
if(${MBED_TOOLCHAIN_FILE_USED})
    mbed_set_profile_options(mbed-core ${MBED_TOOLCHAIN})
    mbed_set_c_lib(mbed-core ${MBED_C_LIB})
    mbed_set_printf_lib(mbed-core ${MBED_PRINTF_LIB})

    target_compile_features(mbed-core
        INTERFACE
            c_std_11
            cxx_std_14
    )

endif()

target_compile_definitions(mbed-core
    INTERFACE
        ${MBED_TARGET_DEFINITIONS}
        ${MBED_CONFIG_DEFINITIONS}
)

# Add compile definitions for backward compatibility with the toolchain
# supported. New source files should instead check for __GNUC__ and __clang__
# for the GCC_ARM and ARM toolchains respectively.
if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    target_compile_definitions(mbed-core
        INTERFACE
            TOOLCHAIN_GCC_ARM
            TOOLCHAIN_GCC
    )
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    target_compile_definitions(mbed-core
        INTERFACE
            TOOLCHAIN_ARM
    )
endif()

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
add_subdirectory(drivers/usb EXCLUDE_FROM_ALL)
add_subdirectory(features EXCLUDE_FROM_ALL)
add_subdirectory(cmsis/CMSIS_5/CMSIS/RTOS2 EXCLUDE_FROM_ALL)
add_subdirectory(cmsis/device/rtos EXCLUDE_FROM_ALL)

# This is a temporary workaround to prevent the build from failing for MBED_TARGETS that
# haven't been converted to build system targets yet.
# The refactored MBED_TARGETS set the linker script and forward it to the build system as a
# usage requirement. The 'old' mechanism was to set the linker script on the top level mbed-core
# target. This was needed because MBED_TARGETS were not registered as buildsystem targets,
# preventing CMake from working its usage requirements magic and forcing us to set the linker
# script globally.
#
# Ensure the words that make up the Mbed target name are separated with a hyphen, lowercase, and with the `mbed-` prefix.
string(TOLOWER ${MBED_TARGET} MBED_TARGET_CONVERTED)
string(REPLACE "_" "-" MBED_TARGET_CONVERTED ${MBED_TARGET_CONVERTED})
string(PREPEND MBED_TARGET_CONVERTED "mbed-")

# TODO: Remove when all MBED_TARGETS have been converted to build system targets.
if(TARGET ${MBED_TARGET_CONVERTED})
    target_link_libraries(mbed-core INTERFACE ${MBED_TARGET_CONVERTED})
else()
    get_property(LINKER_SCRIPT GLOBAL PROPERTY MBED_TARGET_LINKER_FILE)
    mbed_set_linker_script(mbed-core ${LINKER_SCRIPT})
endif()

#
# Configures the application
# Note, this function will be removed in the next revisions
#
function(mbed_configure_app_target target)
    # We need to generate a "response file" to pass to the C preprocessor because of path length
    # limitations on Windows. We set the response file and bind the path to a global property here.
    # We query this global property when we set the linker script for the MBED_TARGET being built.
    #
    # TODO: Remove this and find a more idiomatic way of passing compile definitions to CPP without
    # using global properties.
    mbed_generate_options_for_linker(${target} LINKER_PREPROCESS_DEFINITIONS)
    set_property(GLOBAL PROPERTY COMPILE_DEFS_RESPONSE_FILE ${LINKER_PREPROCESS_DEFINITIONS})

    # Gcc Arm requires memap to be set with app name, equally to ARMClang
    # TODO: move this to toolchain and set properly
    if(MBED_TOOLCHAIN STREQUAL "GCC_ARM")
        message(${target})
        target_link_options(mbed-core
            INTERFACE
                "-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX}.map"
        )
    endif()
endfunction()

#
# Converts output file of `target` to binary file and to Intel HEX file.
#
function(mbed_generate_bin_hex target)
    get_property(elf_to_bin GLOBAL PROPERTY ELF2BIN)
    if(MBED_TOOLCHAIN STREQUAL "GCC_ARM")
        set(CMAKE_POST_BUILD_COMMAND
            COMMAND ${elf_to_bin} -O binary $<TARGET_FILE:${target}> ${CMAKE_CURRENT_BINARY_DIR}/${target}.bin
            COMMAND ${CMAKE_COMMAND} -E echo "-- built: ${CMAKE_CURRENT_BINARY_DIR}/${target}.bin"
            COMMAND ${elf_to_bin} -O ihex $<TARGET_FILE:${target}> ${CMAKE_CURRENT_BINARY_DIR}/${target}.hex
            COMMAND ${CMAKE_COMMAND} -E echo "-- built: ${CMAKE_CURRENT_BINARY_DIR}/${target}.hex"
        )
    elseif(MBED_TOOLCHAIN STREQUAL "ARM")
        get_property(mbed_studio_arm_compiler GLOBAL PROPERTY MBED_STUDIO_ARM_COMPILER)
        set(CMAKE_POST_BUILD_COMMAND
            COMMAND ${elf_to_bin} ${mbed_studio_arm_compiler} --bin  -o ${CMAKE_CURRENT_BINARY_DIR}/${target}.bin $<TARGET_FILE:${target}>
            COMMAND ${CMAKE_COMMAND} -E echo "-- built: ${CMAKE_CURRENT_BINARY_DIR}/${target}.bin"
            COMMAND ${elf_to_bin} ${mbed_studio_arm_compiler} --i32combined  -o ${CMAKE_CURRENT_BINARY_DIR}/${target}.hex $<TARGET_FILE:${target}>
            COMMAND ${CMAKE_COMMAND} -E echo "-- built: ${CMAKE_CURRENT_BINARY_DIR}/${target}.hex"
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

    if(TARGET mbed-post-build-bin)
        add_custom_target(mbed-post-build
            ALL
            DEPENDS
                mbed-post-build-bin
        )
    endif()
endfunction()

#
# Parse toolchain generated map file of `target` and display a readable table format.
#
function(mbed_generate_map_file target)
     add_custom_command(
         TARGET
             ${target}
         POST_BUILD
         COMMAND ${Python3_EXECUTABLE} ${MBED_PATH}/tools/memap.py -t ${MBED_TOOLCHAIN} ${CMAKE_CURRENT_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX}.map
         WORKING_DIRECTORY
             ${CMAKE_CURRENT_BINARY_DIR}
         COMMENT
             "Displaying memory map for ${target}"
)
endfunction()

#
# Validate selected application profile.
#
function(mbed_validate_application_profile target)
    get_target_property(app_link_libraries ${target} LINK_LIBRARIES)
    string(FIND "${app_link_libraries}" "mbed-baremetal" string_found_position)
    if(${string_found_position} GREATER_EQUAL 0)
        if(NOT "bare-metal" IN_LIST MBED_TARGET_SUPPORTED_APPLICATION_PROFILES)
            message(FATAL_ERROR
                "Use full profile as baremetal profile is not supported for this Mbed target")
        endif()
    elseif(NOT "full" IN_LIST MBED_TARGET_SUPPORTED_APPLICATION_PROFILES)
        message(FATAL_ERROR
            "The full profile is not supported for this Mbed target")
    endif()
endfunction()

#
# Set post build operations
#
function(mbed_set_post_build target)
    mbed_validate_application_profile(${target})
    mbed_generate_bin_hex(${target})

    if(HAVE_MEMAP_DEPS)
        mbed_generate_map_file(${target})
    endif()
endfunction()

# Ninja requires to be forced for response files
if ("${CMAKE_GENERATOR}" MATCHES "Ninja")
    # known issue ARMClang and Ninja with response files for windows
    # https://gitlab.kitware.com/cmake/cmake/-/issues/21093
    if(NOT (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows" AND CMAKE_CXX_COMPILER_ID MATCHES "ARMClang"))
        set(CMAKE_NINJA_FORCE_RESPONSE_FILE 1 CACHE INTERNAL "")
    endif()
endif()

endif()
