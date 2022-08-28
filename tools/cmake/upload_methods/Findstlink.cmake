# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# ----------------------------------------------
# CMake finder for stlink, the open-source ST-Link package
#
#
# This module defines:
# st-util_PATH - full path to st-util executable
# st-flash_PATH - full path to st-flash executable
# stlink_FOUND - whether or not the stlink tools were found

# try to figure out where stlink may be installed.
if("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")

    # on Windows, assume that the user extracted the binaries to Program Files

    # if the host is 64 bit, there will be a Program Files (x86) folder, this covers both
    file(GLOB stlink_HINTS "C:/Program Files*/stlink*/bin")

    # if we found multiple paths, check the one with the highest version number first
    list(SORT stlink_HINTS)
    list(REVERSE stlink_HINTS)

else()
	set(stlink_HINTS "")
endif()

find_program(st-util_PATH
	NAMES st-util
	DOC "Path to the st-util executable"
	HINTS ${stlink_HINTS})

find_program(st-flash_PATH
	NAMES st-flash
	DOC "Path to the st-flash executable"
	HINTS ${stlink_HINTS})

find_package_handle_standard_args(stlink FOUND_VAR stlink_FOUND REQUIRED_VARS st-util_PATH st-flash_PATH)