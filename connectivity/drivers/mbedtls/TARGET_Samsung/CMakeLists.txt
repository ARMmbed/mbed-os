# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os-mbedtls
    INTERFACE
        .
        ./sha
)

target_sources(mbed-os-mbedtls
    INTERFACE
        sha/sha256_alt.c
        sha/sha512_alt.c
)
