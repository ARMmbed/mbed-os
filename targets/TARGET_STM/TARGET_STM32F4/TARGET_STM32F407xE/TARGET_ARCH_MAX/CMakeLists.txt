# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-arch-max INTERFACE)

target_sources(mbed-arch-max
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(mbed-arch-max
    INTERFACE
        .
)

target_link_libraries(mbed-arch-max INTERFACE mbed-stm32f407xe)

set(MBED_STORAGE_BLOCKDEVICE_LIBRARIES
    ${MBED_STORAGE_BLOCKDEVICE_LIBRARIES}
    mbed-storage-sd
    CACHE INTERNAL ""
)
