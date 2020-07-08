# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

mbed_add_cmake_directory_if_labels("TARGET")

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/aes
        ${CMAKE_CURRENT_SOURCE_DIR}/des
        ${CMAKE_CURRENT_SOURCE_DIR}/sha
)

target_sources(mbed-os
    PRIVATE
        aes/aes_alt.c

        des/des_alt.c

        sha/sha1_alt.c
        sha/sha1_alt_sw.c
        sha/sha256_alt.c
        sha/sha256_alt_sw.c
        sha/sha_alt_hw.c
)
