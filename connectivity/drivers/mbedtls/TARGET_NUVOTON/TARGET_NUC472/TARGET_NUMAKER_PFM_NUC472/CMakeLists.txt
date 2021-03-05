# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-mbedtls-numaker_pfm_nuc472)

target_include_directories(mbed-mbedtls-numaker_pfm_nuc472
    INTERFACE
        .
)

target_link_libraries(mbed-mbedtls-numaker_pfm_nuc472 INTERFACE mbed-mbedtls-nuc472)

target_link_libraries(mbed-mbedtls PUBLIC mbed-mbedtls-numaker_pfm_nuc472)
