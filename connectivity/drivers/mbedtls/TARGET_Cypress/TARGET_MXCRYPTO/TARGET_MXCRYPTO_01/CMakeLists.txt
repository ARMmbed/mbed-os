# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-mbedtls-mxcrypto-01 INTERFACE)

target_include_directories(mbed-mbedtls-mxcrypto-01
    INTERFACE
        .
)

target_link_libraries(mbed-mbedtls-mxcrypto-01 INTERFACE mbed-mbedtls-mxcrypto)

target_link_libraries(mbed-mbedtls PRIVATE mbed-mbedtls-mxcrypto-01)

# Expose include paths for an application and components
# mbedtls_device.h should be publicly available
target_include_directories(mbed-mbedtls
    PUBLIC
        $<TARGET_PROPERTY:mbed-mbedtls-mxcrypto-01,INTERFACE_INCLUDE_DIRECTORIES>
)
