# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-storage-qspif STATIC EXCLUDE_FROM_ALL)

target_include_directories(mbed-storage-qspif
    PUBLIC
        include
        include/QSPIF
)

target_sources(mbed-storage-qspif
    PRIVATE
        source/QSPIFBlockDevice.cpp
)

target_link_libraries(mbed-storage-qspif PUBLIC mbed-storage-blockdevice)

if(MBED_OS_ENABLE_TESTS)
    if (NOT BUILD_GREENTEA_TESTS)
        add_subdirectory(UNITTESTS)
    endif()
endif()
