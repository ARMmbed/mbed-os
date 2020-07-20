# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        stm-s2lp-rf-driver
        source
)

target_sources(mbed-os
    PRIVATE
        source/NanostackRfPhys2lp.cpp
        source/at24mac_s2lp.cpp
        source/rf_configuration.c
)
