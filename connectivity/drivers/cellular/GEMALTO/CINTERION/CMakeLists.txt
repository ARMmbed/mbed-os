# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os-cellular
    INTERFACE
        .
)

target_sources(mbed-os-cellular
    INTERFACE
        GEMALTO_CINTERION.cpp
        GEMALTO_CINTERION_CellularContext.cpp
        GEMALTO_CINTERION_CellularInformation.cpp
        GEMALTO_CINTERION_CellularStack.cpp
)
