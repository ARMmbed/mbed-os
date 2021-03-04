# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-mbedtls-mxcrypto INTERFACE)

add_subdirectory(TARGET_MXCRYPTO_01 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MXCRYPTO_02 EXCLUDE_FROM_ALL)

target_include_directories(mbed-mbedtls-mxcrypto
    INTERFACE
        .
)

target_sources(mbed-mbedtls-mxcrypto
    INTERFACE
        aes_alt.c
        crypto_common.c
        ecdsa_alt.c
        ecp_alt.c
        ecp_curves_alt.c
        sha1_alt.c
        sha256_alt.c
        sha512_alt.c
)
