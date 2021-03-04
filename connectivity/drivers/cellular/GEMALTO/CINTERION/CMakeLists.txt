# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cellular-gemalt-cinterion INTERFACE)

target_include_directories(mbed-cellular-gemalt-cinterion
    INTERFACE
        .
)

target_sources(mbed-cellular-gemalt-cinterion
    INTERFACE
        GEMALTO_CINTERION.cpp
        GEMALTO_CINTERION_CellularContext.cpp
        GEMALTO_CINTERION_CellularInformation.cpp
        GEMALTO_CINTERION_CellularStack.cpp
)
