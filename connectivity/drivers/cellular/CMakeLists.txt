# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

mbed_add_cmake_directory_if_labels("COMPONENT")

add_subdirectory(Altair)
add_subdirectory(GEMALTO)
add_subdirectory(GENERIC)
add_subdirectory(MultiTech)
add_subdirectory(QUECTEL)
add_subdirectory(RiotMicro)
add_subdirectory(TELIT)
add_subdirectory(UBLOX)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
