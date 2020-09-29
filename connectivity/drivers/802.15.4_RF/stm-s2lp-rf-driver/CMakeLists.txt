# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-802.15.4-stm-s2lp OBJECT)

target_include_directories(mbed-os-802.15.4-stm-s2lp
    PUBLIC
        stm-s2lp-rf-driver
        source
)

target_sources(mbed-os-802.15.4-stm-s2lp
    PRIVATE
        source/NanostackRfPhys2lp.cpp
        source/at24mac_s2lp.cpp
        source/rf_configuration.c
)

mbed_link_component(mbed-os-802.15.4-stm-s2lp mbed-os-nanostack mbed-os)
