# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        atmel-rf-driver
        source
)

target_sources(mbed-os
    PRIVATE
        source/NanostackRfPhyAT86RF215.cpp
        source/NanostackRfPhyAtmel.cpp
        source/at24mac.cpp
)
