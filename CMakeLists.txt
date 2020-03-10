# SPDX-License-Identifier: Apache-2.0

# This is not the application cmake, only boilerplate for Mbed OS

cmake_minimum_required(VERSION 3.13)

# Using relative paths behavior
if(POLICY CMP0076)
    cmake_policy(SET CMP0076 NEW)
endif()

# Create Mbed OS library
add_library(mbed-os OBJECT)

# Include mbed.h and config from generate folder
target_include_directories(mbed-os PUBLIC .)

# Link config from generate folder
target_link_libraries(mbed-os PRIVATE gen_config)

# Baremetal build with RTOS and events
add_subdirectory(cmsis)
add_subdirectory(drivers)
add_subdirectory(events)
add_subdirectory(hal)
add_subdirectory(platform)
add_subdirectory(targets)

# TODO: this is the default, but what if baremetal is selected, this needs to be under
# the condition
# cc @mbed-os-tools Also how to get this in from tools, to be defined
add_subdirectory(rtos)
