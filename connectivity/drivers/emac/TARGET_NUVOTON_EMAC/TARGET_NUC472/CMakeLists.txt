# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-nuc472 INTERFACE)

target_include_directories(mbed-emac-nuc472
    INTERFACE
        .
)

target_sources(mbed-emac-nuc472
    INTERFACE
        nuc472_eth.c
)

target_link_libraries(mbed-emac-nuc472 INTERFACE mbed-emac-nuvoton)

target_link_libraries(mbed-netsocket PRIVATE mbed-emac-nuc472)
