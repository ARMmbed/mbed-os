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

find_program(st-util_PATH NAMES st-util DOC "Path to the st-util executable")
find_program(st-flash_PATH NAMES st-flash DOC "Path to the st-flash executable")

find_package_handle_standard_args(stlink FOUND_VAR stlink_FOUND REQUIRED_VARS st-util_PATH st-flash_PATH)