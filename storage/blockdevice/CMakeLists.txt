# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(MBED_ENABLE_OS_INTERNAL_TESTS)
    if(MBED_BUILD_GREENTEA_TESTS)
        # add greentea test
    else()
        add_subdirectory(COMPONENT_QSPIF)
        add_subdirectory(tests/UNITTESTS)
    endif()
endif()

add_library(mbed-storage-blockdevice STATIC EXCLUDE_FROM_ALL)

if("DATAFLASH" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(COMPONENT_DATAFLASH)
endif()

if("FLASHIAP" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(COMPONENT_FLASHIAP)
endif()

if("I2CEE" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(COMPONENT_I2CEE)
endif()

if("QSPIF" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(COMPONENT_QSPIF)
endif()

if("OSPIF" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(COMPONENT_OSPIF)
endif()

if("SD" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(COMPONENT_SD)
endif()

if("SPIF" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(COMPONENT_SPIF)
endif()

target_include_directories(mbed-storage-blockdevice
    PUBLIC
        .
        ./include
        ./include/blockdevice
        ./include/blockdevice/internal
)

target_sources(mbed-storage-blockdevice
    PRIVATE
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

target_link_libraries(mbed-storage-blockdevice PUBLIC mbed-core-flags)