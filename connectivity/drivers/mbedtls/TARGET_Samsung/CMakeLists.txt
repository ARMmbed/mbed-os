# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-mbedtls-samsung OBJECT)

target_include_directories(mbed-os-mbedtls-samsung
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/sha
)

target_sources(mbed-os-mbedtls-samsung
    PRIVATE
        sha/sha256_alt.c
        sha/sha512_alt.c
)

mbed_link_component(mbed-os-mbedtls-samsung mbed-os-mbedtls)
