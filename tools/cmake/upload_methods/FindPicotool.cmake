# Copyright (c) 2023 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# ----------------------------------------------
# CMake finder for Picotool (https://github.com/raspberrypi/picotool)
#
#
# This module defines:
# Picotool - full path to picotool executable
# Picotool_FOUND - whether or not the picotool executable was found

# Currently there's no standard install location for picotool.  We just have to search on the PATH.
find_program(Picotool NAMES picotool DOC "Path to picotool from Raspberry Pi")

find_package_handle_standard_args(Picotool REQUIRED_VARS Picotool)
