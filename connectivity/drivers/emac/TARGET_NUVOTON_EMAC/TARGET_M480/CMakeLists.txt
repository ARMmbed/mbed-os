# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-m480 INTERFACE)

target_include_directories(mbed-emac-m480
    INTERFACE
        .
)

target_sources(mbed-emac-m480
    INTERFACE
        m480_eth.c
)

target_link_libraries(mbed-emac-m480 INTERFACE mbed-emac-nuvoton)

target_link_libraries(mbed-netsocket PRIVATE mbed-emac-m480)
