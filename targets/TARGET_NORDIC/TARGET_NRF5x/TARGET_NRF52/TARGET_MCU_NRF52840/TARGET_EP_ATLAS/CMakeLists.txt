# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-ep-atlas INTERFACE)

target_include_directories(mbed-ep-atlas
    INTERFACE
        .
)

target_sources(mbed-ep-atlas
    INTERFACE
        ONBOARD_TELIT_ME310.cpp
        usb_stdio.cpp
        atlas_init.c
)

target_link_libraries(mbed-ep-atlas INTERFACE mbed-mcu-nrf52840)

# Overwrite MBED_STORAGE_BLOCKDEVICE_LIBRARIES to remove mbed-storage-qspif
set(MBED_STORAGE_BLOCKDEVICE_LIBRARIES
    mbed-storage-flashiap
    mbed-storage-spif
    CACHE INTERNAL ""
)
