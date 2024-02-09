# Copyright (c) 2024 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-cellular
    PUBLIC
        .
)

target_sources(mbed-cellular
    PRIVATE
        TELIT_HE910.cpp
)

if("TARGET_MTS_DRAGONFLY_F411RE" IN_LIST MBED_TARGET_LABELS)
	add_subdirectory(TARGET_MTS_DRAGONFLY_F411RE)
endif()

if("TARGET_MTS_DRAGONFLY_F413RH" IN_LIST MBED_TARGET_LABELS)
	add_subdirectory(TARGET_MTS_DRAGONFLY_F413RH)
endif()