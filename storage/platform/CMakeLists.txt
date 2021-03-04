# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-storage
    INTERFACE
        source/PlatformStorage.cpp
)

list(APPEND mbed-storage-libs
    mbed-storage-blockdevice
    mbed-storage-filesystem
    mbed-storage-fat
    mbed-storage-littlefs
)

target_link_libraries(mbed-storage
    INTERFACE
        ${MBED_STORAGE_BLOCKDEVICE_LIBRARIES}
)
