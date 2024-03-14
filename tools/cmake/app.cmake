# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Add our CMake list files to CMake default module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

find_program(CCACHE "ccache")
if(CCACHE)
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

# Find Python (needed to generate configurations)
include(mbed_python_interpreter)

include(mbed_generate_config_header)
include(mbed_target_functions)
include(mbed_create_distro)

# Load toolchain and mbed configuration, generating it if needed
include(mbed_generate_configuration)

# Load toolchain file
if(NOT CMAKE_TOOLCHAIN_FILE OR MBED_TOOLCHAIN_FILE_USED)
    set(MBED_TOOLCHAIN_FILE_USED TRUE CACHE INTERNAL "")
    # We want to bring CMP0123 we set in mbed_toolchain.cmake
    # to the whole Mbed OS.
    include(mbed_toolchain NO_POLICY_SCOPE)
endif()

# Specify available build profiles and add options for the selected build profile
include(mbed_profile)

enable_language(C CXX ASM)

# set executable suffix (has to be done after enabling languages)
# Note: This is nice in general, but is also required because STM32Cube will only work on files with a .elf extension
set(CMAKE_EXECUTABLE_SUFFIX .elf)

# Find the GDB server, assuming it will be in the same path as the compiler.
get_filename_component(CMAKE_COMPILER_BIN_DIR ${CMAKE_C_COMPILER} DIRECTORY)
find_program(MBED_GDB 
    NAMES arm-none-eabi-gdb gdb-multiarch
    HINTS ${CMAKE_COMPILER_BIN_DIR}
    DOC "Path to the GDB client program to use when debugging.")

if(EXISTS "${MBED_GDB}")
    set(MBED_GDB_FOUND TRUE)
else()
    message(STATUS "Mbed: Could not find arm-none-eabi-gdb or gdb-mutiarch.  Debugging will be unavailable.  Set the MBED_GDB variable to specify its path.")
    set(MBED_GDB_FOUND FALSE)
endif()

# Load upload method configuration defaults for this target.
# Loading the settings here makes sure they are set at global scope, and also makes sure that
# the user can override them by changing variable values after including app.cmake.
#
# default expected paths
set(EXPECTED_CUSTOM_UPLOAD_CFG_FILE_PATH ${CMAKE_SOURCE_DIR}/custom_targets/upload_method_cfg/${MBED_TARGET}.cmake)
set(EXPECTED_MBED_UPLOAD_CFG_FILE_PATH ${CMAKE_SOURCE_DIR}/mbed-os/targets/upload_method_cfg/${MBED_TARGET}.cmake)

# check if a custom upload config path is defined in top lvl cmake
if((DEFINED CUSTOM_UPLOAD_CFG_PATH))
    if(EXISTS ${CUSTOM_UPLOAD_CFG_PATH})
        include(${CUSTOM_UPLOAD_CFG_PATH})
        message(STATUS "Mbed: Custom upload config included from ${CUSTOM_UPLOAD_CFG_PATH}")
    else()
        message(FATAL_ERROR "Mbed: Custom upload config is defined but files was not found here - ${CUSTOM_UPLOAD_CFG_PATH}")
    endif()
	
# check if a custom upload config is present in custom_targets/YOUR_TARGET folder
elseif(EXISTS ${EXPECTED_CUSTOM_UPLOAD_CFG_FILE_PATH})
	include(${EXPECTED_CUSTOM_UPLOAD_CFG_FILE_PATH})
	message(STATUS "Mbed: Custom upload config included from ${EXPECTED_CUSTOM_UPLOAD_CFG_FILE_PATH}")
	
# check for build in upload config
elseif(EXISTS ${EXPECTED_MBED_UPLOAD_CFG_FILE_PATH})
	include(${EXPECTED_MBED_UPLOAD_CFG_FILE_PATH})
	message(STATUS "Mbed: Loading default upload method configuration from ${EXPECTED_MBED_UPLOAD_CFG_FILE_PATH}")
else()
    message(STATUS "Mbed: Target does not have any upload method configuration.  'make flash-' commands will not be available unless configured by the upper-level project.")
    set(UPLOAD_METHOD_DEFAULT "NONE")
endif()
