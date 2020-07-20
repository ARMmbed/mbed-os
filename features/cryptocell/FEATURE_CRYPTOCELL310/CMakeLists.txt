# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

mbed_add_cmake_directory_if_labels("TARGET")
add_subdirectory(binaries)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(mbed-os
    PRIVATE
        aes_alt.c
        cc_internal.c
        ccm_alt.c
        cmac_alt.c
        ecdh_alt.c
        ecdsa_alt.c
        sha1_alt.c
        sha256_alt.c
        sha512_alt.c
        trng.c
)
