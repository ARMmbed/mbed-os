# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(COMPONENT_DATAFLASH EXCLUDE_FROM_ALL)
add_subdirectory(COMPONENT_FLASHIAP EXCLUDE_FROM_ALL)
add_subdirectory(COMPONENT_I2CEE EXCLUDE_FROM_ALL)
add_subdirectory(COMPONENT_QSPIF EXCLUDE_FROM_ALL)
add_subdirectory(COMPONENT_OSPIF EXCLUDE_FROM_ALL)
add_subdirectory(COMPONENT_SD EXCLUDE_FROM_ALL)
add_subdirectory(COMPONENT_SPIF EXCLUDE_FROM_ALL)

add_library(mbed-storage-blockdevice INTERFACE)

target_include_directories(mbed-storage-blockdevice
    INTERFACE
        .
        include
        include/blockdevice
        include/blockdevice/internal
)

target_sources(mbed-storage-blockdevice
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
        source/SFDP.cpp
        source/SlicingBlockDevice.cpp
)
