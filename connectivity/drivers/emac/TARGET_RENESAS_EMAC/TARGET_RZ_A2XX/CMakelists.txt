# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-emac
    INTERFACE
        .
        r_ether_rza2
        r_ether_rza2/src/phy
        r_ether_rza2/src/targets/TARGET_GR_MANGO
        r_ether_rza2/src
)

target_sources(mbed-emac
    INTERFACE
        rza2_emac.cpp
        r_ether_rza2/src/r_ether_rza2.c
        r_ether_rza2/src/phy/phy.c
        r_ether_rza2/src/targets/TARGET_GR_MANGO/r_ether_setting_rza2m.c
)
