# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-mbedtls-sl OBJECT)

target_include_directories(mbed-os-mbedtls-sl
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-mbedtls-sl
    PRIVATE
        aes_aes.c
        crypto_aes.c
        crypto_ecp.c
        crypto_management.c
        crypto_sha.c
)

target_link_libraries(mbed-os-mbedtls-sl mbed-os-mbedtls)
