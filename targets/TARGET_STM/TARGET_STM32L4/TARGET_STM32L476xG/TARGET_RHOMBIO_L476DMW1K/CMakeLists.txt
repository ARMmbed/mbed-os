# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-rhombio-l476dmw1k INTERFACE)

target_sources(mbed-rhombio-l476dmw1k
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-rhombio-l476dmw1k
    INTERFACE
        .
)

target_link_libraries(mbed-rhombio-l476dmw1k INTERFACE mbed-stm32l476xg)

set(MBED_STORAGE_BLOCKDEVICE_LIBRARIES
    ${MBED_STORAGE_BLOCKDEVICE_LIBRARIES}
    mbed-storage-qspif
    CACHE INTERNAL ""
)
