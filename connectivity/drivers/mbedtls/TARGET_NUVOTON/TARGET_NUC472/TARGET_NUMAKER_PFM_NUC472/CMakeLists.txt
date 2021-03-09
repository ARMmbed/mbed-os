# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-mbedtls-numaker-pfm-nuc472)

target_include_directories(mbed-mbedtls-numaker-pfm-nuc472
    INTERFACE
        .
)

target_link_libraries(mbed-mbedtls-numaker-pfm-nuc472 INTERFACE mbed-mbedtls-nuc472)

target_link_libraries(mbed-mbedtls PRIVATE mbed-mbedtls-numaker-pfm-nuc472)

# Expose include paths for an application and components
# mbedtls_device.h should be publicly available
target_include_directories(mbed-mbedtls
    PUBLIC
        $<TARGET_PROPERTY:mbed-mbedtls-numaker-pfm-nuc472,INTERFACE_INCLUDE_DIRECTORIES>
)
