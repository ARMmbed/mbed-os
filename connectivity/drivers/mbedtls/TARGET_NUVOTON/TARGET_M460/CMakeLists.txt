# Copyright (c) 2022 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-mbedtls
    INTERFACE
        .
        ./aes
        ./ecp
        ./rsa
        ./sha
        ./gcm
	./ccm
)

target_sources(mbed-mbedtls
    INTERFACE
        aes/aes_alt.c
        ecp/crypto_ecc_hw.c
        ecp/ecp_alt.c
        ecp/ecp_curves_alt.c
        ecp/ecp_helper.c
        ecp/ecp_internal_alt.c
        rsa/crypto_rsa_hw.c
        rsa/rsa_alt.c
        sha/sha1_alt.c
        sha/sha256_alt.c
        sha/sha512_alt.c
        sha/sha_alt_hw.c
        gcm/gcm_alt.c
        ccm/ccm_alt.c
)
