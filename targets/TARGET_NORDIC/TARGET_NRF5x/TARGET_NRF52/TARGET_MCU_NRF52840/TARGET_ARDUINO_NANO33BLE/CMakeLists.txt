# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-arduino-nano33ble INTERFACE)

target_include_directories(mbed-arduino-nano33ble
    INTERFACE
        .
)

target_link_libraries(mbed-arduino-nano33ble INTERFACE mbed-mcu-nrf52840)

# Overwrite MBED_STORAGE_BLOCKDEVICE_LIBRARIES to remove mbed-storage-qspif
set(MBED_STORAGE_BLOCKDEVICE_LIBRARIES
    mbed-storage-flashiap
    CACHE INTERNAL ""
)
