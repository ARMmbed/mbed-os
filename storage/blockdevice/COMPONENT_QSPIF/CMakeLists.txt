# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-storage-qspif
    INTERFACE
        include
        include/QSPIF
)

target_sources(mbed-storage-qspif
    INTERFACE
        source/QSPIFBlockDevice.cpp
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    if (NOT BUILD_GREENTEA_TESTS)
        add_subdirectory(UNITTESTS)
    endif()
endif()
