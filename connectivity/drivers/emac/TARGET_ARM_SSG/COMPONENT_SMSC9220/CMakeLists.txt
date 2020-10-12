# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os-emac
    INTERFACE
        .
)

target_sources(mbed-os-emac
    INTERFACE
        smsc9220_emac.cpp
)
