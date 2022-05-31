# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-cellular
    PUBLIC
        .
)

target_sources(mbed-cellular
    PRIVATE
        GEMALTO_CINTERION.cpp
        GEMALTO_CINTERION_CellularContext.cpp
        GEMALTO_CINTERION_CellularInformation.cpp
        GEMALTO_CINTERION_CellularStack.cpp
)
