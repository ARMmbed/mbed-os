# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-mbedtls-cypress-mxcrypto-01 INTERFACE)

target_include_directories(mbed-os-mbedtls-cypress-mxcrypto-01
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(mbed-os-mbedtls-cypress-mxcrypto-01 INTERFACE mbed-os-mbedtls)
