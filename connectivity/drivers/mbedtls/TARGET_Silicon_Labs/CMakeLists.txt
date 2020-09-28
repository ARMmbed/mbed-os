# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os-mbedtls
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-mbedtls
    PRIVATE
        aes_aes.c
        crypto_aes.c
        crypto_ecp.c
        crypto_management.c
        crypto_sha.c
)
