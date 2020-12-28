# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(tech)
add_subdirectory(transceiver)

target_include_directories(mbed-nfc
    INTERFACE
        .
        ./ndef
        ./platform
)

target_sources(mbed-nfc
    INTERFACE
        ndef/ndef.c

        platform/nfc_scheduler.c
        platform/nfc_transport.c
)
