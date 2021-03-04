# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-mbedtls-mxcrypto_01 INTERFACE)

target_include_directories(mbed-mbedtls
    INTERFACE
        .
)

target_link_libraries(mbed-mbedtls-mxcrypto_01 INTERFACE mbed-mbedtls-mxcrypto)
