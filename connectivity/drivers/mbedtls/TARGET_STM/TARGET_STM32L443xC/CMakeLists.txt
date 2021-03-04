# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-mbedtls-stm32l443xc INTERFACE)

target_include_directories(mbed-mbedtls-stm32l443xc
    INTERFACE
        .
)

target_link_libraries(mbed-mbedtls-stm32l443xc INTERFACE mbed-tls-stm)
