# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-802.15.4-stm-s2lp INTERFACE)

target_include_directories(mbed-os-802.15.4-stm-s2lp
    INTERFACE
        stm-s2lp-rf-driver
        source
)

target_sources(mbed-os-802.15.4-stm-s2lp
    INTERFACE
        source/NanostackRfPhys2lp.cpp
        source/at24mac_s2lp.cpp
        source/rf_configuration.c
)

target_link_libraries(mbed-os-802.15.4-stm-s2lp INTERFACE mbed-os-nanostack)
