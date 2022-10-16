# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# ----------------------------------------------
# CMake finder for the Arduino variant of bossac
#
#
# This module defines:
# ArduinoBossac - full path to bossac executable
# ArduinoBossac_FOUND - whether or not the ArduinoBossac executable was found

set(ArduinoBossac_PATHS "")

# try to figure out where ArduinoBossac may be installed.
if("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")

    # on Windows, assume that the user extracted the binaries to Program Files

    # On my computer the path is C:\Users\jamie\AppData\Local\Arduino15\packages\arduino\tools\bossac\1.9.1-arduino2\bossac.exe
    file(GLOB ArduinoBossac_PATHS "$ENV{LocalAppData}/Arduino*/packages/arduino/tools/bossac/1.9.1-arduino2")
else()

    # Linux / Mac
    # Per here: https://docs.zephyrproject.org/2.7.0/boards/arm/arduino_nano_33_ble/doc/index.html
    # a possible path would be $HOME/.arduino15/packages/arduino/tools/bossac/1.9.1-arduino2/bossac
    file(GLOB ArduinoBossac_PATHS "$ENV{HOME}/.arduino*/packages/arduino/tools/bossac/1.9.1-arduino2")

endif()

# if we found multiple paths, check the one with the highest version number first
list(SORT ArduinoBossac_PATHS)
list(REVERSE ArduinoBossac_PATHS)

find_program(ArduinoBossac NAMES bossac HINTS ${ArduinoBossac_PATHS} DOC "Path to the Arduino variant of bossac")

find_package_handle_standard_args(ArduinoBossac REQUIRED_VARS ArduinoBossac)


