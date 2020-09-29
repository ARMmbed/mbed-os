# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-mbedtls-samsung INTERFACE)

target_include_directories(mbed-os-mbedtls-samsung
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/sha
)

target_sources(mbed-os-mbedtls-samsung
    INTERFACE
        sha/sha256_alt.c
        sha/sha512_alt.c
)

target_link_libraries(mbed-os-mbedtls-samsung INTERFACE mbed-os-mbedtls)
