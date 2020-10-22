# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-802.15.4-rf
    INTERFACE
        stm-s2lp-rf-driver
        source
)

target_sources(mbed-802.15.4-rf
    INTERFACE
        source/NanostackRfPhys2lp.cpp
        source/at24mac_s2lp.cpp
        source/rf_configuration.c
)
