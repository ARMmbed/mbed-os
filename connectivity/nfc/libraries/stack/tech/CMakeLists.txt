# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nfc
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/iso7816
        ${CMAKE_CURRENT_SOURCE_DIR}/isodep
        ${CMAKE_CURRENT_SOURCE_DIR}/type4
)

target_sources(mbed-nfc
    INTERFACE
        iso7816/iso7816.c
        iso7816/iso7816_app.c

        isodep/isodep_target.c

        type4/type4_target.c
)
