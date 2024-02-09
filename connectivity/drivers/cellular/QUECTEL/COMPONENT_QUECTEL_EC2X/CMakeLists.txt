# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-cellular
    PUBLIC
        .
)

target_sources(mbed-cellular
    PRIVATE
        QUECTEL_EC2X.cpp
)

if("MTS_DRAGONFLY_L496VG" IN_LIST MBED_TARGET_LABELS)
	add_subdirectory(TARGET_MTS_DRAGONFLY_L496VG)
endif()