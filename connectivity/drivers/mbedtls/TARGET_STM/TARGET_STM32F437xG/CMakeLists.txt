# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-mbedtls-stm32f437xg INTERFACE)

target_include_directories(mbed-mbedtls-stm32f437xg
    INTERFACE
        .
)

target_link_libraries(mbed-mbedtls-stm32f437xg INTERFACE mbed-mbedtls-stm)

target_link_libraries(mbed-mbedtls PUBLIC mbed-mbedtls-stm32f437xg)
