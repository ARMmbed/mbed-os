# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-mbedtls-cypress-mxcrypto-02 OBJECT)

target_include_directories(mbed-os-mbedtls-cypress-mxcrypto-02
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(mbed-os-mbedtls-cypress-mxcrypto-02 mbed-os-mbedtls)
