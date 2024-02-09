# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-cellular
    PUBLIC
        .
)

target_sources(mbed-cellular
    PRIVATE
        QUECTEL_UG96.cpp
        QUECTEL_UG96_CellularContext.cpp
)

if("TARGET_WIO_3G" IN_LIST MBED_TARGET_LABELS)
	add_subdirectory(TARGET_WIO_3G)
endif()