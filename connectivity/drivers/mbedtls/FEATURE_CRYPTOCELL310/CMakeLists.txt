# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-mbedtls-cryptocell310 OBJECT)

mbed_add_cmake_directory_if_labels("TARGET")
add_subdirectory(binaries)

target_include_directories(mbed-os-mbedtls-cryptocell310
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cryptocell310
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cryptocell310/internal
)

target_sources(mbed-os-mbedtls-cryptocell310
    PRIVATE
        source/aes_alt.c
        source/cc_internal.c
        source/ccm_alt.c
        source/cmac_alt.c
        source/ecdh_alt.c
        source/ecdsa_alt.c
        source/sha1_alt.c
        source/sha256_alt.c
        source/sha512_alt.c
        source/trng.c
)

mbed_link_component(mbed-os-mbedtls-cryptocell310 mbed-os-mbedtls)
