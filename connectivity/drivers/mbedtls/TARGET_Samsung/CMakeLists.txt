# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-mbedtls
    PUBLIC
        .
        ./sha
)

target_sources(mbed-mbedtls
    PRIVATE
        sha/sha256_alt.c
        sha/sha512_alt.c
)
