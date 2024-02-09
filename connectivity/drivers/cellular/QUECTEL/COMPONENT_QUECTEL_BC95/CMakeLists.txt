# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-cellular
    PUBLIC
        .
)

target_sources(mbed-cellular
    PRIVATE
        QUECTEL_BC95.cpp
        QUECTEL_BC95_CellularContext.cpp
        QUECTEL_BC95_CellularInformation.cpp
        QUECTEL_BC95_CellularNetwork.cpp
        QUECTEL_BC95_CellularStack.cpp
)

if("TARGET_ADV_WISE_1570" IN_LIST MBED_TARGET_LABELS)
	add_subdirectory(TARGET_ADV_WISE_1570)
endif()