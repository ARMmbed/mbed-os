# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-mbedtls-mxcrypto_02 INTERFACE)

target_include_directories(mbed-mbedtls-mxcrypto_02
    INTERFACE
        .
)

target_link_libraries(mbed-mbedtls-mxcrypto_02 INTERFACE mbed-mbedtls-mxcrypto)
