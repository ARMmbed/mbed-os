# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nfc
    PUBLIC
        ./iso7816
        ./isodep
        ./type4
)

target_sources(mbed-nfc
    PRIVATE
        iso7816/iso7816.c
        iso7816/iso7816_app.c

        isodep/isodep_target.c

        type4/type4_target.c
)
