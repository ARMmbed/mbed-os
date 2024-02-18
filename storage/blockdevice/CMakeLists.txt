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

# Note: For the storage components, we always make them available as CMake targets if supported,
# even if the COMPONENT is not enabled.
# This is because the COMPONENT not being enabled means that they don't exist on the given dev board,
# but a user can still add one via their own circuitry.
# However, it's still correct for the directories to be named COMPONENT_xxx, because that keeps the JSON
# files inside from being parsed for default pins unless the relevant COMPONENT is enabled.
if("DEVICE_SPI=1" IN_LIST MBED_TARGET_DEFINITIONS)
    add_subdirectory(COMPONENT_DATAFLASH)
endif()

if("DEVICE_FLASH=1" IN_LIST MBED_TARGET_DEFINITIONS)
    add_subdirectory(TARGET_FLASH)
endif()

if("DEVICE_I2C=1" IN_LIST MBED_TARGET_DEFINITIONS)
    add_subdirectory(COMPONENT_I2CEE)
endif()

if("DEVICE_QSPI=1" IN_LIST MBED_TARGET_DEFINITIONS)
    add_subdirectory(COMPONENT_QSPIF)
endif()

if("DEVICE_OSPI=1" IN_LIST MBED_TARGET_DEFINITIONS)
    add_subdirectory(COMPONENT_OSPIF)
endif()

if("DEVICE_SPI=1" IN_LIST MBED_TARGET_DEFINITIONS)
    add_subdirectory(COMPONENT_SD)
endif()

if("DEVICE_SPI=1" IN_LIST MBED_TARGET_DEFINITIONS)
    add_subdirectory(COMPONENT_SPIF)
endif()

if("DEVICE_QSPI=1" IN_LIST MBED_TARGET_DEFINITIONS)
    add_subdirectory(COMPONENT_SPINAND)
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