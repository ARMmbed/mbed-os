# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

mbed_add_cmake_directory_if_labels("FEATURE")

add_subdirectory(cellular)
add_subdirectory(drivers)
add_subdirectory(libraries)
add_subdirectory(lorawan)
add_subdirectory(lwipstack)
add_subdirectory(mbedtls)
add_subdirectory(nanostack)
add_subdirectory(netsocket)
add_subdirectory(nfc)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
