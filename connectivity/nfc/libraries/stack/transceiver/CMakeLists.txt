# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nfc
    INTERFACE
        .
)

target_sources(mbed-nfc
    INTERFACE
        transceiver.c
)
