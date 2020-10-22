# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-802.15.4-rf
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-802.15.4-rf
    INTERFACE
        NanostackRfPhyEfr32.cpp
)
