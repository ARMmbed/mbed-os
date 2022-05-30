# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0



if(${CMAKE_CROSSCOMPILING})
    # List of all storage libraries available, also has some source files.
    add_library(mbed-storage STATIC EXCLUDE_FROM_ALL)

    # The directories below contain optional target libraries
    add_subdirectory(blockdevice EXCLUDE_FROM_ALL)
    add_subdirectory(filesystem EXCLUDE_FROM_ALL)
    add_subdirectory(kvstore EXCLUDE_FROM_ALL)
    add_subdirectory(platform EXCLUDE_FROM_ALL)
else()
    # List of all storage libraries available.
    add_library(mbed-storage INTERFACE)

    # Add these subdirectories for the Unit test
    add_subdirectory(blockdevice)
    add_subdirectory(filesystem)
    add_subdirectory(kvstore)
endif()

target_include_directories(mbed-storage
    INTERFACE
        .
)
