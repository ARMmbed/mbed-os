# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-cellular-gemalto-cinterion INTERFACE)

target_include_directories(mbed-os-cellular-gemalto-cinterion
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-cellular-gemalto-cinterion
    INTERFACE
        GEMALTO_CINTERION.cpp
        GEMALTO_CINTERION_CellularContext.cpp
        GEMALTO_CINTERION_CellularInformation.cpp
        GEMALTO_CINTERION_CellularStack.cpp
)

target_link_libraries(mbed-os-cellular-gemalto-cinterion INTERFACE mbed-os-cellular)
