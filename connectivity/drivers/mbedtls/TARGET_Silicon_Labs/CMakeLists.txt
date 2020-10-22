# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-mbedtls
    INTERFACE
        .
)

target_sources(mbed-mbedtls
    INTERFACE
        aes_aes.c
        crypto_aes.c
        crypto_ecp.c
        crypto_management.c
        crypto_sha.c
)
