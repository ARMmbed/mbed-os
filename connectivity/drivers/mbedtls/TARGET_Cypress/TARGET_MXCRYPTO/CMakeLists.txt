# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("MXCRYPTO_01" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MXCRYPTO_01)
elseif("MXCRYPTO_02" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MXCRYPTO_02)
endif()

target_include_directories(mbed-mbedtls
    INTERFACE
        .
)

target_sources(mbed-mbedtls
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
