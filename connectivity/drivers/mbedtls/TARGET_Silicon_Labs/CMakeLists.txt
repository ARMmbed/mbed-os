# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-mbedtls-silicon-labs INTERFACE)

target_include_directories(mbed-mbedtls-silicon-labs
    INTERFACE
        .
)

target_sources(mbed-mbedtls-silicon-labs
    INTERFACE
        aes_aes.c
        crypto_aes.c
        crypto_ecp.c
        crypto_management.c
        crypto_sha.c
)

target_link_libraries(mbed-mbedtls PUBLIC mbed-mbedtls-silicon-labs)
