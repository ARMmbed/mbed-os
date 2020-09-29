# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-mbedtls-cypress-mxcrypto-01 OBJECT)

target_include_directories(mbed-os-mbedtls-cypress-mxcrypto-01
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

mbed_link_component(mbed-os-mbedtls-cypress-mxcrypto-01 mbed-os-mbedtls)
