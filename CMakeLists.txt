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
# TODO: @mbed-os-tools We need to get this gen_config somehow set up
target_link_libraries(mbed-os PRIVATE gen_config)

# Default build
add_subdirectory(cmsis)
add_subdirectory(drivers)
add_subdirectory(events)
add_subdirectory(hal)
add_subdirectory(platform)
add_subdirectory(targets)
add_subdirectory(rtos)

add_subdirectory(features)
add_subdirectory(components)

# Add additional modules
get_property(modules GLOBAL PROPERTY MBED_OS_MODULES)

foreach(module ${modules})
    add_subdirectory(${module})
endforeach()
