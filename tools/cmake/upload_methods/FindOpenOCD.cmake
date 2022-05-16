# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# ----------------------------------------------
# CMake finder for OpenOCD
#
#
# This module defines:
# OpenOCD - full path to OpenOCD executable
# OpenOCD_SCRIPT_DIR - Path containing OpenOCD scripts
# OpenOCD_FOUND - whether or not the OpenOCD executable was found

set(OpenOCD_PATHS "")

# try to figure out where OpenOCD may be installed.
if("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")

    # on Windows, assume that the user extracted the binaries to Program Files

    # if the host is 64 bit, there will be a Program Files (x86) folder, this covers both
    file(GLOB OpenOCD_PATHS "C:/Program Files*/openocd*/bin*")

    # if we found multiple paths, check the one with the highest version number first
    list(SORT OpenOCD_PATHS)
    list(REVERSE OpenOCD_PATHS)

endif()

find_program(OpenOCD NAMES openocd PATHS ${OpenOCD_PATHS} DOC "Path to the openocd executable")

# guess a script dir based on the executable path
set(OpenOCD_SCRIPT_DIR_HINTS "")
if(EXISTS ${OpenOCD})
    get_filename_component(OpenOCD_EXECUTABLE_DIR ${OpenOCD} DIRECTORY)

    # on Windows it's in ../scripts, on Linux it's in ../share/openocd/scripts
    set(OpenOCD_SCRIPT_DIR_HINTS HINTS ${OpenOCD_EXECUTABLE_DIR}/../scripts ${OpenOCD_EXECUTABLE_DIR}/../share/openocd/scripts)
endif()

find_path(OpenOCD_SCRIPT_DIR
    NAMES interface/cmsis-dap.cfg
    ${OpenOCD_SCRIPT_DIR_HINTS}
    PATHS /usr/share/openocd/scripts/
    DOC "Path to OpenOCD scripts folder.  Should contain interface/cmsis-dap.cfg.")

find_package_handle_standard_args(OpenOCD FOUND_VAR OpenOCD_FOUND REQUIRED_VARS OpenOCD OpenOCD_SCRIPT_DIR)


