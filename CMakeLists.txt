# SPDX-License-Identifier: Apache-2.0

# This is not the application cmake, only boilerplate for Mbed OS

cmake_minimum_required(VERSION 3.13)

# Using relative paths behavior
if(POLICY CMP0076)
    cmake_policy(SET CMP0076 NEW)
endif()

# Create mbed_os library
add_library(mbed_os OBJECT)

# Include mbed.h and config from generate folder
target_include_directories(mbed_os PUBLIC .)

# Link config from generate folder
target_link_libraries(mbed_os PRIVATE gen_config)

# Baremetal build with RTOS and events
add_subdirectory(cmsis)
add_subdirectory(drivers)
add_subdirectory(events)
add_subdirectory(hal)
add_subdirectory(platform)
add_subdirectory(targets)
add_subdirectory(rtos)

# Selectable components
# TODO: use subdirectory_ifdef or similar own function based on config

# add_subdirectory(components)
# add_subdirectory(features/mbedtls)
# add_subdirectory(features/cellular)
# add_subdirectory(features/frameworks)
# add_subdirectory(features/lorawan)
# add_subdirectory(features/lwipstack)
# add_subdirectory(features/nanostack)
# add_subdirectory(features/netsocket)
# add_subdirectory(features/nfc)
# add_subdirectory(features/storage)
# add_subdirectory(features/device_key)
