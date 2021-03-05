# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-smsc9220 INTERFACE)

target_include_directories(mbed-emac-smsc9220
    INTERFACE
        .
)

target_sources(mbed-emac-smsc9220
    INTERFACE
        smsc9220_emac.cpp
)

target_link_libraries(mbed-netsocket PRIVATE mbed-emac-smsc9220)
