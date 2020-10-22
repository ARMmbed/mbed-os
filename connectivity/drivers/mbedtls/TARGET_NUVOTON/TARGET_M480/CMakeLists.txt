# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-mbedtls
    INTERFACE
        .
        ./aes
        ./des
        ./sha
)

target_sources(mbed-mbedtls
    INTERFACE
        aes/aes_alt.c

        des/des_alt.c

        ecp/ecp_internal_alt.c

        sha/sha1_alt.c
        sha/sha1_alt_sw.c
        sha/sha256_alt.c
        sha/sha256_alt_sw.c
        sha/sha512_alt.c
        sha/sha512_alt_sw.c
        sha/sha_alt_hw.c
)
