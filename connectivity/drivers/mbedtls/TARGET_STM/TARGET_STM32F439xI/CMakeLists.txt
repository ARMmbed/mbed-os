# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-mbedtls-stm32f439xl INTERFACE)

target_include_directories(mbed-mbedtls-stm32f439xl
    INTERFACE
        .
)

target_link_libraries(mbed-mbedtls-stm32f439xl INTERFACE mbed-tls-stm)
