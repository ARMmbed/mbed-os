# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-cellular
    PUBLIC
        .
)

target_sources(mbed-cellular
    PRIVATE
        SARA4_PPP.cpp
        SARA4_PPP_CellularNetwork.cpp
)

if("MTS_DRAGONFLY_L471QG" IN_LIST MBED_TARGET_LABELS)
	add_subdirectory(TARGET_MTS_DRAGONFLY_L471QG)
endif()