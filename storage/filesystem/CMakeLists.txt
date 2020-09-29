# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_subdirectory(fat)
add_subdirectory(littlefs)
add_subdirectory(littlefsv2)

target_include_directories(mbed-os-storage
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/filesystem
)

target_sources(mbed-os-storage
    PRIVATE
        source/Dir.cpp
        source/File.cpp
        source/FileSystem.cpp
)

mbed_link_component(mbed-os-storage mbed-os-littlefs-v2 mbed-os)
