# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-mbedtls-m480 INTERFACE)

target_include_directories(mbed-mbedtls-m480
    INTERFACE
        .
        ./aes
        ./des
        ./sha
)

target_sources(mbed-mbedtls-m480
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

target_link_libraries(mbed-mbedtls PUBLIC mbed-mbedtls-m480)
