# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os-storage
    INTERFACE
        source/PlatformStorage.cpp
)

list(APPEND mbed-os-storage-libs
    mbed-os-storage-blockdevice
    mbed-os-storage-filesystem
    mbed-os-storage-fat
    mbed-os-storage-littlefs
)

if("DATAFLASH" IN_LIST MBED_TARGET_LABELS)
    list(APPEND mbed-os-storage-libs mbed-os-storage-dataflash)
endif()

if("FLASHIAP" IN_LIST MBED_TARGET_LABELS)
    list(APPEND mbed-os-storage-libs mbed-os-storage-flashiap)
endif()

if("QSPIF" IN_LIST MBED_TARGET_LABELS)
    list(APPEND mbed-os-storage-libs mbed-os-storage-qspif)
endif()

if("SD" IN_LIST MBED_TARGET_LABELS)
    list(APPEND mbed-os-storage-libs mbed-os-storage-sd)
endif()

if("SPIF" IN_LIST MBED_TARGET_LABELS)
    list(APPEND mbed-os-storage-libs mbed-os-storage-spif)
endif()

target_link_libraries(mbed-os-storage
    INTERFACE
        ${mbed-os-storage-libs}
)
