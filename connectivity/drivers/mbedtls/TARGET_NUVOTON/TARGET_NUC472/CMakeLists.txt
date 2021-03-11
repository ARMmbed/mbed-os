# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("NUMAKER_PFM_NUC472" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUMAKER_PFM_NUC472)
endif()

add_library(mbed-mbedtls-nuc472 INTERFACE)

add_subdirectory(TARGET_NUMAKER_PFM_NUC472 EXCLUDE_FROM_ALL)

target_include_directories(mbed-mbedtls-nuc472
    INTERFACE
        .
        ./aes
        ./des
        ./sha
)

target_sources(mbed-mbedtls-nuc472
    INTERFACE
        aes/aes_alt.c

        des/des_alt.c

        sha/sha1_alt.c
        sha/sha1_alt_sw.c
        sha/sha256_alt.c
        sha/sha256_alt_sw.c
        sha/sha_alt_hw.c
)
