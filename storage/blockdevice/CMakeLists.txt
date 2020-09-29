# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(COMPONENT_DATAFLASH)
add_subdirectory(COMPONENT_FLASHIAP)
add_subdirectory(COMPONENT_I2CEE)
add_subdirectory(COMPONENT_QSPIF)
add_subdirectory(COMPONENT_SD)
add_subdirectory(COMPONENT_SPIF)

target_include_directories(mbed-os-storage
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/blockdevice
)

target_sources(mbed-os-storage
    INTERFACE
        source/BufferedBlockDevice.cpp
        source/ChainingBlockDevice.cpp
        source/ExhaustibleBlockDevice.cpp
        source/FlashSimBlockDevice.cpp
        source/HeapBlockDevice.cpp
        source/MBRBlockDevice.cpp
        source/ObservingBlockDevice.cpp
        source/ProfilingBlockDevice.cpp
        source/ReadOnlyBlockDevice.cpp
        source/SlicingBlockDevice.cpp
)
